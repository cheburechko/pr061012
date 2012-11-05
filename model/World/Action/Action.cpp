/*
    Copyright (c) 2012, pr061012 Team.
    See the LICENSE file for copying permission.
*/

#include "Action.h"

//******************************************************************************
// CONSTRUCTOR/DESTRUCTOR
//******************************************************************************

Action::Action(const ActionType type, Object * const actor) :
    type(type),
    actor(actor)
{

}

Action::~Action()
{

}

//******************************************************************************
// STATE'S CHECKERS
//******************************************************************************

ActionState Action::getState() const
{
    return this -> state;
}

bool Action::isPending() const
{
    return (this -> state == PENDING);
}

bool Action::isFailed() const
{
    return (this -> state == FAILED);
}

bool Action::isSucceeded() const
{
    return (this -> state == SUCCEED);
}

//**************************************************************************
// STATE'S CHANGERS.
//**************************************************************************

void Action::markAsFailed()
{
    this -> state = FAILED;
}

void Action::markAsSucceeded()
{
    this -> state = SUCCEED;
}

//******************************************************************************
// ACCESSING PARAMS/PARTICIPANTS.
//******************************************************************************

void Action::addParticipant(Object * obj)
{
    this -> participants.push_back(obj);
}

void Action::addParam(std::string param, int value)
{
    this -> params.addKey(param, value);
}

int Action::getParam(std::string param) const
{
    return this -> params.getValue(param);
}

Object * const Action::getActor() const
{
    return actor;
}

ActionType Action::getType() const
{
    return type;
}


