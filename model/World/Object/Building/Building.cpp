/*
    Copyright (c) 2012, pr061012 Team.
    See the LICENSE file for copying permission.
*/

#include <sstream>

#include "Building.h"
#include "../../../../common/Math/Random.h"
#include "../../../../common/BasicDefines.h"

//******************************************************************************
// CONSTRUCTOR/DESTRUCTOR.
//******************************************************************************

Building::Building(uint max_space, uint max_health) :
    Object(BUILDING, false),
    contents(new ObjectHeap),
    health(1),
    max_health(max_health),
    free_space(max_space),
    max_space(max_space),
    completeness(false)
{
    this -> setDangerLevel(DNGR_BUILDING);
    this -> setShapeType(SHP_BUILDING);
    this -> setShapeSize(Random::double_range(SZ_BUILDING_SIDE_MIN, SZ_BUILDING_SIDE_MAX));
}

Building::~Building()
{
    delete contents;
}

//******************************************************************************
// INHERETED THINGS.
//******************************************************************************

std::vector<Action>* Building::getActions()
{
    this -> actions.clear();
    return &(this -> actions);
}

void Building::receiveMessage(Message message)
{
}

std::string Building::printObjectInfo()
{
    std::string output = Object::printObjectInfo();

    std::stringstream ss;

    // TODO: Add contents printing.
    ss << "Is complete\t\t" << (completeness ? "yes" : "no") << std::endl <<
          "Free space\t\t" << free_space << "/" << max_space << std::endl;

    return output + ss.str();
}

uint Building::getHealthPoints() const
{
    return this -> health;
}

uint Building::getMaxHealthPoints() const
{
    return this -> max_health;
}

std::string Building::getTypeName() const
{
    return "building";
}

//******************************************************************************
// HEALTH MANIPULATION.
//******************************************************************************

void Building::decreaseHealth(uint delta)
{
    if (this -> health > delta)
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
    if (this -> health + delta < this -> max_health)
    {
        this -> health += delta;
    }
    else
    {
        this -> health = this -> max_health;
    }

    if (this -> health == this -> max_health)
    {
        this -> completeness = true;
        this -> makeSolid();
    }
}

void Building::damage(uint delta)
{
    this -> decreaseHealth(delta);
}

void Building::heal(uint delta)
{
    this -> increaseHealth(delta);
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
