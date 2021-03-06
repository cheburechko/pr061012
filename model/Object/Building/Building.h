/*
    Copyright (c) 2012, pr061012 Team.
    See the LICENSE file for copying permission.
*/

#ifndef BUILDING_H
#define BUILDING_H

#include "../Object.h"
#include "../../ObjectHeap/ObjectHeap.h"

/**
 * @class Building
 * @brief Building class contains information about building such as objects
 *        inside (including creatures).
 */
class Building : public Object
{
public:
    //**************************************************************************
    // CONSTRUCTOR/DESTRUCTOR.
    //**************************************************************************

    /**
     * @brief Constructor.
     */
    Building(uint max_space, uint max_health);

    /**
     * @brief Destructor.
     */
    virtual ~Building();

    //**************************************************************************
    // INHERETED THINGS.
    //**************************************************************************

    std::string printObjectInfo(bool full) const;
    std::string getTypeName() const;

    //**************************************************************************
    // HEALTH MANIPULATION.
    //**************************************************************************

    uint damage(uint delta);
    uint heal(uint delta);

    uint getHealthPoints() const;
    uint getMaxHealthPoints() const;

    //**************************************************************************
    // CONTENTS MANIPULATION.
    //**************************************************************************

    /**
     * @brief  Puts object inside building.
     * @param  object   object to put inside
     * @return success of the operation
     */
    bool putInside(Object* object);

    /**
     * @brief  Takes out object from building.
     * @param  object   object to take out
     * @return success of the operation
     */
    bool takeOut(Object * object);

    //**************************************************************************
    // CONTENTS AND SPACE ACCESSORS.
    //**************************************************************************

    /**
     * @brief  Get the value of contents.
     * @return the value of contents
     */
    ObjectHeap * getContents();

    /**
     * @brief  Get the value of free_space.
     * @return the value of free_space
     */
    uint getFreeSpace();

    /**
     * @brief  Get the value of max_space.
     * @return the value of max_space
     */
    uint getMaxSpace();

    /**
     * @brief  Get the value of isFinished.
     * @return the value of isFinished
     */
    bool getCompleteness();

private:
    /// Building contents.
    ObjectHeap* contents;

    /// Building's health.
    uint health;
    /// Building's maximum health.
    uint max_health;

    /// Free space.
    uint free_space;
    /// Maximum available space.
    uint max_space;

    /// @brief Completeness of the building. As building is founded, it gets 0
    ///        health. Building proccess is equivalent to repairing proccess.
    ///        After first repairing (as health becomes equals to max_health)
    ///        completeness value changes to true and building becomes complete.
    bool completeness;
};

#endif // BUILDING_H
