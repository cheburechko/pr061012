/*
    Copyright (c) 2012, pr061012 Team.
    See the LICENSE file for copying permission.
*/

#include "Weather.h"

#include "../../../BasicFuncs.h"
#include "../../../BasicDefines.h"

//******************************************************************************
// CONSTRUCTOR/DESTRUCTOR.
//******************************************************************************

Weather::Weather(WeatherType type, uint living_steps) :
    Object(WEATHER),
    subtype(type),
    covered_objs(new ObjectHeap)
{
    this -> setShapeType(SHP_WEATHER);
    this -> setShapeSize(randFromRange(SZ_WEATHER_DIAM_MIN, SZ_WEATHER_DIAM_MAX));

    if(living_steps == 0)
    {
        this -> steps = randFromRange(WEAT_STEPS_MIN, WEAT_STEPS_MAX);
    }
    else
    {
        this -> steps = living_steps;
    }
}

Weather::~Weather()
{
    delete covered_objs;
}

//******************************************************************************
// WEATHER ACTIONS.
//******************************************************************************

std::vector <Action> * Weather::getActions()
{
    if(this -> steps > 0)
    {
        this -> steps--;
    }

    this -> actions.clear();

    // TODO: Add actions for RAIN, CLOUDS, SNOW.

    if
    (
        this -> subtype == METEOR_SHOWER ||
        this -> subtype == STORM ||
        this -> subtype == HURRICANE ||
        this -> subtype == HAIL ||
        this -> subtype == EARTHQUAKE
    )
    {
        Action act(HARM_OBJS, this);

        // Add all objects to this action.
        ObjectHeap::const_iterator iter;
        for(iter = covered_objs -> begin(); iter != covered_objs -> end(); iter++)
        {
            act.addParticipant(*iter);
        }

        // FIXME: I tried for about 40 minutes to compile it.
        //        Unsuccesfully.
        this -> actions.push_back(act);
    }

    return &(this -> actions);
}

//******************************************************************************
// WEATHER TYPE.
//******************************************************************************

WeatherType Weather::getSubtype() const
{
    return this -> subtype;
}

//******************************************************************************
// ACCESSORS.
//******************************************************************************

ObjectHeap * Weather::getCoveredObjects()
{
    return this -> covered_objs;
}

uint Weather::getLivingSteps() const
{
    return this -> steps;
}