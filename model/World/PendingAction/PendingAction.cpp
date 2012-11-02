/*
    Copyright (c) 2012, pr061012 Team.
    See the LICENSE file for copying permission.
*/

#include "PendingAction.h"

//******************************************************************************
// CONSTRUCTOR/DESTRUCTOR
//******************************************************************************

PendingAction::PendingAction(const PendingActionType type, const Object * actor) :
    type(type),
    actor(actor)
{

}

PendingAction::~PendingAction()
{

}

//******************************************************************************
// STATE'S CHECKERS
//******************************************************************************

PendingActionStateType PendingAction::getState()
{
    return this -> state;
}

bool PendingAction::isPending()
{
    return (this -> state == PENDING);
}

bool PendingAction::isFailed()
{
    return (this -> state == FAILED);
}

bool PendingAction::isSucceed()
{
    return (this -> state == SUCCEED);
}

//**************************************************************************
// STATE'S CHANGERS.
//**************************************************************************

void PendingAction::markAsFailed()
{
    this -> state = FAILED;
}

void PendingAction::markAsSucceed()
{
    this -> state = SUCCEED;
}

//******************************************************************************
// ACCESSING PARAMS/PARTICIPANTS.
//******************************************************************************

void PendingAction::addParticipant(Object * obj)
{
    this -> participants.push_back(obj);
}

void PendingAction::addParam(std::string param, int value)
{
    this -> params[param] = value;
}

int PendingAction::getParam(std::string param)
{
    const std::map <std::string, int> :: const_iterator iter = this -> params.find(param);

    if(iter == this -> params.end())
    {
        // TODO: Throw an exception.
    }

    return iter -> second;
}
