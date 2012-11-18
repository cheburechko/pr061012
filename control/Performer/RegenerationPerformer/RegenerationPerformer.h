/*
    Copyright (c) 2012, pr061012 Team.
    See the LICENSE file for copying permission.
*/

#ifndef REGENERATION_PERFORMER_H
#define REGENERATION_PERFORMER_H

#include "../Performer.h"
#include "../../../model/World/Indexator/Indexator.h"

/**
 * @class RegenerationPerformer
 * @brief Performs a regeneration if it is possible
 */
class RegenerationPerformer : public Performer
{
public:
    
    //*************************************************************************
    //  CONSTRUCTOR/DESTRUCTOR
    //*************************************************************************

    /**
     * @brief Constructor
     */
    RegenerationPerformer(Indexator& indexator);

    /**
     * @brief Destructor
     */
    ~RegenerationPerformer();

    //*************************************************************************
    //  PERFORM
    //*************************************************************************
    
    /**
     * @brief Check if
     * @param action a request for
     */
    void perform(Action& action);

private:

    //*************************************************************************
    //  ATTRIBUTES
    //*************************************************************************

    Indexator& indexator;
};

#endif
