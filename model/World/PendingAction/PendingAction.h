/*
    Copyright (c) 2012, pr061012 Team.
    See the LICENSE file for copying permission.
*/

#ifndef PENDING_ACTION_H
#define PENDING_ACTION_H

#include <string>
#include <vector>
#include <map>

// NOTE: In Object.h this file is included. To prevent compiler crash we need to
//       declare Object and PendingAction first. Maybe not a good solution.
class Object;
class PendingAction;

#include "../Object/Object.h"
#include "../../BasicTypes.h"

/**
 * @brief PendingAction contains information about object's pending action.
 */
class PendingAction
{
public:
    //**************************************************************************
    // CONSTRUCTOR/DESTRUCTOR
    //**************************************************************************

    /**
     * @brief Constructor.
     * @param type          action's type
     * @param actor         action's actor
     * @param participants  action's participants
     * @param params        map action's params
     */
    PendingAction(const PendingActionType type, const Object * actor,
                  const std::vector <const Object *> participants,
                  const std::map <std::string, int> params);

    /**
     * @brief Destructor,
     */
    ~PendingAction();

    //**************************************************************************
    // STATE'S CHECKERS
    //**************************************************************************

    /**
     * @brief  Gets current state of action.
     * @return current state of action
     */
    PendingActionStateType getState();

    /**
     * @brief  Pending state checker.
     * @return true, if action is pending
     */
    bool isPending();

    /**
     * @brief  Failed state checker.
     * @return true, if action is failed
     */
    bool isFailed();

    /**
     * @brief  Succeed state checker.
     * @return true, if action is succeed
     */
    bool isSucceed();

    //**************************************************************************
    // STATE'S CHANGERS.
    //**************************************************************************

    /**
     * @brief Marks action as failed.
     */
    void markAsFailed();

    /**
     * @brief Marks action as succeed.
     */
    void markAsSucceed();

    //**************************************************************************
    // ACCESSING PARAMS.
    //**************************************************************************

    /**
     * @brief  Gets value of param.
     * @param  param    param name
     * @return value of param
     */
    // TODO: Throw an exception when param isn't specified.
    int getParam(std::string param);

private:
    /// Action type.
    const PendingActionType type;

    /// Action's actor.
    const Object * actor;

    /// Action's participants (without actor).
    const std::vector <const Object *> participants;

    /// Parameters.
    const std::map <std::string, int> params;

    /// Result type.
    PendingActionStateType state;
};

#endif // PENDINGACTION_H