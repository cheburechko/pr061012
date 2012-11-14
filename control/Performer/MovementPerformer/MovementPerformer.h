/*
    Copyright (c) 2012, pr061012 Team.
    See the LICENSE file for copying permission.
*/

#ifndef MOVEMENT_PERFORMER_H__
#define MOVEMENT_PERFORMER_H__

#include "../Performer.h"
#include "../../../model/World/Indexator/Indexator.h"

/**
 * @class MovementPerformer
 * @brief Performs a move if it is possible
 */
class MovementPerformer : public Performer
{
public:
    
    //*************************************************************************
    //  CONSTRUCTOR/DESTRUCTOR
    //*************************************************************************

    /**
     * @brief Constructor for a particular world
     * @param world_size size of the world
     * @param indexator indexator
     */
    MovementPerformer(const double world_size, Indexator* indexator);

    /**
     * @brief Destructor
     */
    ~MovementPerformer();

    //*************************************************************************
    //  PERFORM
    //*************************************************************************
    
    /**
     * @brief Check if an object can move and move it
     * @param action a request for moving
     */
    void perform(Action& action);

private:

    //*************************************************************************
    //  ATTRIBUTES
    //*************************************************************************
    
    /// Size of the world
    const double world_size;
    
    /// The reference to worlds indexator
    Indexator* indexator;
};

#endif