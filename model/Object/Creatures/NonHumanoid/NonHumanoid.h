/*
    Copyright (c) 2012, pr061012 Team.
    See the LICENSE file for copying permission.
*/

#ifndef NON_HUMANOID_H
#define NON_HUMANOID_H

#include "../Creature.h"

/**
 * @class NonHumanoid
 * @brief NonHumanoid is silly creature, has only few goals in its life: food
 *        and sleep.
 */
class NonHumanoid : public Creature
{
public:
    //**************************************************************************
    // CONSTRUCTOR/DESTRUCTOR.
    //**************************************************************************

    /**
     * @brief Random constructor (creates NonHumanoid with really random
     *        attributes).
     * @param type      non-humanoid's type
     * @param dmaker    non-humanoid's decision maker
     */
    NonHumanoid(NonHumanoidType type, const DecisionMaker& dmaker);

    /**
     * @brief Destructor
     */
    virtual ~NonHumanoid();

    //**************************************************************************
    // INHERETED THINGS.
    //**************************************************************************

    std::string getTypeName() const;
    std::string printObjectInfo(bool full) const;

    //**************************************************************************
    // NON-HUMANOID TYPE.
    //**************************************************************************

    /**
     * @brief  Gets non-humanoid's type.
     * @return non-humanoid's type
     */
    NonHumanoidType getSubsubtype() const;

    //**************************************************************************
    // NON-HUMANOID ACTIONS.
    //**************************************************************************

    std::vector <Action>* getActions();
    void receiveMessage(Message message);

private:
    //**************************************************************************
    // UPDATES.
    //**************************************************************************

    /**
     * @brief Update need in descendants.
     */
    void updateNeedInDesc();

    //**************************************************************************
    // AUXILIARY FUNCTIONS.
    //**************************************************************************

    /**
     * @brief  Searches for victim.
     * @return the nearest victim
     */
    void findVictim();

    /**
     * @brief  Searches for food around NonHumanoid.
     * @return the nearest food
     */
    void findFood();

    //**************************************************************************
    // PRIVATE ATTRIBUTES.
    //**************************************************************************

    /// Non-humanoid's type.
    NonHumanoidType subsubtype;

    /// Steps for current roam direction.
    uint roam_steps;

    /// Array with messages.
    std::vector <Message> messages;
};

#endif // NON_HUMANOID_H
