/*
    Copyright (c) 2012, pr061012 Team.
    See the LICENSE file for copying permission.
*/

#include "Building.h"

//******************************************************************************
// CONSTRUCTOR/DESTRUCTOR.
//******************************************************************************

Building::Building(uint max_space, uint max_health) :
    Object(BUILDING),
    contents(new ObjectHeap),
    health(0),
    max_health(max_health),
    max_space(max_space),
    completeness(false)
{
}

Building::~Building()
{
    delete contents;
}

//******************************************************************************
// ACTIONS ACCESSING.
//******************************************************************************

std::vector <Action> * Building::getActions()
{
    this -> actions.clear();
    return &(this -> actions);
}

//******************************************************************************
// HEALTH MANIPULATION.
//******************************************************************************

void Building::decreaseHealth(uint delta)
{
    if(this -> health > delta)
    {
        this -> health -= delta;
    }
    else
    {
        this -> health = 0;
    }
}

void Building::increaseHealth(uint delta)
{
    if(this -> health + delta < this -> max_health)
    {
        this -> health += delta;
    }
    else
    {
        this -> health = this -> max_health;
    }
}

//******************************************************************************
// CONTENTS MANIPULATION.
//******************************************************************************

bool Building::putInside(Object * object)
{
    return this -> contents -> push(object);
}

bool Building::takeOut(Object * object)
{
    return this -> contents -> remove(object);
}

//******************************************************************************
// REPAIRING.
//******************************************************************************

void Building::repair(uint delta)
{
    this -> increaseHealth(delta);

    if(this -> health == this -> max_health)
    {
        this -> completeness = true;
    }
}

//******************************************************************************
// CONTENTS AND SPACE ACCESSORS.
//******************************************************************************

ObjectHeap * Building::getContents()
{
    return this -> contents;
}

uint Building::getFreeSpace()
{
    return this -> free_space;
}

uint Building::getMaxSpace()
{
    return this -> max_space;
}

bool Building::getCompleteness()
{
    return this -> completeness;
}
