/*
    Copyright (c) 2012, pr061012 Team.
    See the LICENSE file for copying permission.
*/

#include "DroppingPerformer.h"
#include "../../../model/World/Object/Creatures/Creature.h"
#include <string>
#include <cmath>

//*****************************************************************************
// FOR TEST ONLY
//*****************************************************************************
#ifndef CREAT_SPEED_FAST_VALUE
#define CREAT_SPEED_FAST_VALUE 10
#endif

#ifndef CREAT_SPEED_SLOW_VALUE
#define CREAT_SPEED_SLOW_VALUE 5
#endif

// TODO
// Improve movement algo

DroppingPerformer::DroppingPerformer(World * world, ObjectHeap * buffer) :
    Performer(world),
    buffer(buffer)
{
}

DroppingPerformer::~DroppingPerformer()
{
}

void DroppingPerformer::perform(Action& action)
{
    // Get needed data
    Object * actor = action.getActor();
    ObjectType actor_type = actor -> getType();

    // check if actor can go
    if (actor_type != CREATURE)
    {
        action.markAsFailed();
        action.setError(OBJ_CANT_DROP);
        return;
    }

    // continue getting data
    Vector dest = actor -> getCoords();
    std::vector<Object*> participants = action.getParticipants();
    ObjectHeap * inventory = dynamic_cast<Creature *>(actor) -> getInventory();

    // initalize flags
    bool success = false;
    bool errors = false;

    // go through all objects in participants and check if
    // they lie in inventory
    for (std::vector<Object*>::iterator j = participants.begin(); 
         j != participants.end(); j++) 
    {
        ObjectHeap::iterator object = inventory -> find(*j);
        if (object == inventory -> end())
        {
            errors = true;
            action.setError(OBJ_IS_NOT_IN_HEAP);
        }
        else 
        {
            success = true;

            // make object visible
            buffer -> push(*j);

            // place it on the ground
            inventory -> remove(*j);
            (*j) -> setCoords(actor -> getCoords());
        }
    }

    if (!success)
    {
        action.markAsFailed();
    }
    else if (success && errors) 
    {
        action.markAsSucceededWithErrors();
    }
    else 
    {
        action.markAsSucceeded();
    }
}
