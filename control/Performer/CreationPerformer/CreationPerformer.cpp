/*
    Copyright (c) 2012, pr061012 Team.
    See the LICENSE file for copying permission.
*/

#include "CreationPerformer.h"
#include "../../../common/BasicTypes.h"
#include "../../../common/BasicDefines.h"
#include "../../../model/World/Object/Creatures/Creature.h"
#include "../../../model/World/Object/Resource/Resource.h"
#include "../../../common/Random/Random.h"

CreationPerformer::CreationPerformer(World* world):
    indexator(world -> getIndexator()),
    world(world),
    factory(world -> getObjectFactory())
{

}

CreationPerformer::~CreationPerformer()
{

}

void CreationPerformer::perform(Action& action)
{
    // Get initial value.
    Object* actor = action.getActor();
    Object* new_object;
    ObjectType type = actor -> getType();
    double size = actor -> getShape().getSize();

    ObjectType obj_type = action.getParam<ObjectType>("obj_type");
    Vector new_center(Random::double_range(2*size, 4*size), Random::double_range(2*size, 4*size));
    ParamArray param;

    // Check of actor type.
    if ( type == RESOURCE || type == CREATURE )
    {
        switch (obj_type)
        {
            case CREATURE:
            {
                // Create creature.
                new_object = createCreature(action, param);

                // Set coord new_object and check it.
                new_object -> setCoords(actor -> getCoords() + new_center);
                if (checkCoord(actor -> getShape()))
                {
                    // If all is OK, add new_object in world.
                    world -> addObject(true, new_object);

                    action.markAsSucceeded();
                }
                else
                {
                    action.markAsFailed();
                }
            }
            break;

            case RESOURCE:
            {
                if (type == RESOURCE)
                {
                    // Create new resource.
                    new_object = createResource(action, param);


                    dynamic_cast<Resource*>(new_object) -> makePickable();
                    // If all is OK, add new_object in world.
                    world -> addObject(false, new_object);

                    // Increase actor amount.
                    static_cast<Resource*>(actor) -> increaseAmount(1);

                    action.markAsSucceeded();
                }
            }
            break;
/*
            case TOOL:
            {
                new_object = createTool(action, param);
                world -> addObject(false, new_object);

                action.markAsSucceeded();
            }
            break;
*/
            case BUILDING:
            {
                // Create new resource.
                new_object = createBuilding(action, param);

                // Set coord new_object and add its in world.
                new_object -> setCoords(actor -> getCoords());
                world -> addObject(true, new_object);

                action.markAsSucceeded();
            }
            break;

            default:
                action.markAsFailed();
            break;
        }
    }
    else
    {
        action.markAsFailed();
    }
}

bool CreationPerformer::checkCoord(Shape shape)
{
    bool ret = false;

    // Get obstacles
    ObjectHeap obstacles = indexator -> getAreaContents(shape);

    //Check amount creature and resource in shape
    if (!obstacles.getTypeAmount(CREATURE) &&
        !obstacles.getTypeAmount(RESOURCE))
    {
        ret = true;
    }

    return ret;
}

Object* CreationPerformer::createBuilding(Action& action, ParamArray& param)
{
    uint max_health = action.getParam<uint>("building_max_health");
    uint max_space = action.getParam<uint>("building_max_space");

    param.addKey<uint>("max_health",max_health);
    param.addKey<uint>("max_space",max_space);

    return factory -> createObject(BUILDING, param);
}

Object* CreationPerformer::createCreature(Action& action, ParamArray& param)
{
    CreatureType creat_type = action.getParam<CreatureType>("creat_type");

    param.addKey<CreatureType>("creat_type", creat_type);

    return factory -> createObject(CREATURE, param);
}

Object* CreationPerformer::createResource(Action& action, ParamArray& param)
{
    ResourceType res_type = action.getParam<ResourceType>("res_type");
    uint res_amount = action.getParam<uint>("res_amount");

    param.addKey<ResourceType>("res_type", res_type);
    param.addKey<uint>("res_amount", res_amount);

    return factory -> createObject(RESOURCE, param);
}

Object* CreationPerformer::createTool(Action& action, ParamArray& param)
{
    ToolType tool_type = action.getParam<ToolType>("tool_type");
    ResourceType mat_type = action.getParam<ResourceType>("mat_type");
    uint tool_str = action.getParam<uint>("tool_str");

    param.addKey<ToolType>("tool_type", tool_type);
    param.addKey<ResourceType>("mat_type", mat_type);
    param.addKey<uint>("tool_str",tool_str);

    return factory -> createObject(TOOL, param);
}