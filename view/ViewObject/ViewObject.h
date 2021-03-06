/*
    Copyright (c) 2012, pr061012 Team.
    See the LICENSE file for copying permission.
*/

#ifndef VIEW_OBJECT_H
#define VIEW_OBJECT_H

#include "../../common/BasicTypes.h"

class ViewObject
{
public:
    //**************************************************************************
    // CONSTRUCTOR/DESTRUCTOR.
    //**************************************************************************

    /**
     * @brief Constructor
     * @param newtype type of the created object
     * @param newx X coordinate of the created object relative to screen center
     * @param newy Y coordinate of the created object relative to screen center
     */
    ViewObject(ObjectType newtype, double newx, double newy);

    /**
     * @brief Destructor
     */
    ~ViewObject();

private:
    /// Type of rendered object
    ObjectType type;

    /// X and Y coordinates of an object, relative to screen center
    double x;
    double y;

    /// This variable tells what animation to render
    // ObjectState cur_state;

public:
    //**************************************************************************
    // ACCESSORS.
    //**************************************************************************

    /**
     * @brief Set the type of current object.
     * @param new_var the new value of type
     */
    void setType(ObjectType new_var);

    /**
     * @brief Get the type of current object.
     * @return the value of type
     */
    ObjectType getType();

    /**
     * @brief Get the x coordinate.
     * @return the value of x
     */
    double getX();

    /**
     * @brief Set x coordinate of an object.
     * @param new_var the new value of x
     */
    void setX(ObjectType new_var);

    /**
     * @brief Get the y coordinate.
     * @return the value of y
     */
    double getY();

    /**
     * @brief Set y coordinate of an object.
     * @param new_var the new value of y
     */
    void setY(ObjectType new_var);

};

#endif // VIEW_OBJECT_H
