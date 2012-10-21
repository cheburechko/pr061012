/*
    Copyright (c) 2012, pr061012 Team.
    See the LICENSE file for copying permission.
*/

#ifndef HUMANOID_H
#define HUMANOID_H

#include "../Creature.h"

/**
 * @class Humanoid
 * @brief
 */
class Humanoid : virtual public Creature
{
public:
    /**
     * @brief Random constructor (creates Humanoid with really random
     *        attributes).
     * @param dmaker    humanoid's decision maker
     */
    Humanoid(const DecisionMaker & dmaker);

	/**
     * @brief Destructor
	 */
    ~Humanoid();

    /**
     * @brief Makes a next step.
     */
    void step();
};

#endif // HUMANOID_H
