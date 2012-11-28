/*
    Copyright (c) 2012, pr061012 Team.
    See the COPYING file for copying permission.
*/

#include "CreationPerformer.h"

#include "../../../common/Math/Random.h"
#include "../../../common/BasicTypes.h"
#include "../../../common/BasicDefines.h"

#include "../../../model/World/Object/Creatures/Creature.h"
#include "../../../model/World/Object/Creatures/Humanoid/Humanoid.h"
#include "../../../model/World/Object/Resource/Resource.h"

CreationPerformer::CreationPerformer(World* world):
    Performer(world) 
{

}

CreationPerformer::~CreationPerformer()
{

}

//TODO: Add comments.

void CreationPerformer::perform(Action& action)
{
    // Get initial values.
    Object* actor = action.getActor();
    Object* new_object;
    ObjectType type = actor -> getType();
    double size = actor -> getShape().getSize();

    ObjectType obj_type = action.getParam<ObjectType>("obj_type");
    Vector new_center(Random::double_range(size,2*size), Random::double_range(size, 2*size));
    ParamArray param;

    // Check of actor type.
    if ( type == CREATURE )
    {
        Creature* cr = dynamic_cast<Creature*>(actor);
        switch (obj_type)
        {
            case CREATURE:
            {
                // Create creature.
                new_object = createCreature(action, param);

                // Set coord new_object and check it.
                new_object -> setCoords(actor -> getCoords() + new_center);
                if (checkCoord(new_object))
                {
                    // If all is OK, add new_object in world.
                    world -> addObject(true, new_object);

                    action.markAsSucceeded();
                    return;
                }
                else
                {
                    delete new_object;
                    action.setError(NO_PLACE_TO_PLACE_OBJ_ON);
                    action.markAsFailed();
                    return;
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

                if (cr -> getSubtype() != NON_HUMANOID)
                {
                    // Create new resource.
                    new_object = createBuilding(action, param);
                    new_object -> setCoords(actor -> getCoords());

                    if (checkCoord(new_object))
                    {
                        // Set coord new_object and add its in world.
                        world -> addObject(true, new_object);

                        Building* new_home = dynamic_cast<Building*>(new_object);
                        dynamic_cast<Humanoid* >(cr) -> setHome(new_home);

                        action.markAsSucceeded();
                    }
                    else
                    {
                        delete new_object;
                        action.setError(NO_PLACE_TO_PLACE_OBJ_ON);
                        action.markAsFailed();
                    }
                    return;
                }
                else
                {
                    action.setError(OBJ_CANT_CREATE);
                    action.markAsFailed();
                    return;
                }
            break;

            default:
                action.setError(OBJ_IS_NOT_CREATABLE);
                action.markAsFailed();
                return;
            break;

        }
    }
    else
    {
        action.setError(OBJ_CANT_CREATE);
        action.markAsFailed();
        return;
    }
}

bool CreationPerformer::checkCoord(Object* new_obj)
{
    bool ret = false;

    Shape shape = new_obj -> getShape();
    Vector center = new_obj -> getCoords();
    double size =  shape.getSize();

    // Get obstacles
    ObjectType type = new_obj -> getType();
    ObjectHeap obstacles = world -> getIndexator() -> getAreaContents(shape);

    uint count_building = obstacles.getTypeAmount(BUILDING);
    uint count_creature = obstacles.getTypeAmount(CREATURE);
    uint count_resource = obstacles.getTypeAmount(RESOURCE);

    if
    (
        center.getX() - size / 2 < 0 ||
        center.getX() + size / 2 >= world -> getSize() ||
        center.getY() - size / 2 < 0 ||
        center.getY() + size / 2 >= world -> getSize()
    )
    {
        ret = false;
    }
    else if (type == BUILDING)
    {
        if
        (
            !count_building &&
            !count_resource &&
            count_creature == 1
        )
        {
            ret = true;
        }
    }
    else if (type == CREATURE)
    {
        if
        (
            !count_creature &&
            !count_resource
        )
        {
            ret = true;
        }
    }

    return ret;
}

Object* CreationPerformer::createBuilding(Action& action, ParamArray& param)
{
    uint max_health = action.getParam<uint>("building_max_health");
    uint max_space = action.getParam<uint>("building_max_space");

    param.addKey<uint>("max_health",max_health);
    param.addKey<uint>("max_space",max_space);

    return world -> getObjectFactory() -> createObject(BUILDING, param);
}

Object* CreationPerformer::createCreature(Action& action, ParamArray& param)
{
    CreatureType creat_type = action.getParam<CreatureType>("creat_type");

    param.addKey<CreatureType>("creat_type", creat_type);

    return world -> getObjectFactory() -> createObject(CREATURE, param);
}

Object* CreationPerformer::createTool(Action& action, ParamArray& param)
{
    ToolType tool_type = action.getParam<ToolType>("tool_type");
    ResourceType mat_type = action.getParam<ResourceType>("mat_type");
    uint tool_str = action.getParam<uint>("tool_str");

    param.addKey<ToolType>("tool_type", tool_type);
    param.addKey<ResourceType>("mat_type", mat_type);
    param.addKey<uint>("tool_str",tool_str);

    return world -> getObjectFactory() -> createObject(TOOL, param);
}
