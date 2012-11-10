/*
    Copyright (c) 2012, pr061012 Team.
    See the LICENSE file for copying permission.
*/

/**
 * @file BasicFuncs.h
 */
 
#ifndef BASIC_FUNC_H
#define BASIC_FUNC_H

/**
 * @brief Returns random number from range [low_boundary, top_boundary].
 */
static inline int randFromRange(int low_boundary, int top_boundary)
{
    return rand() % (top_boundary - low_boundary) + low_boundary + 1;
}

/**
 * @brief Returns random double number.
 */
static inline double doubleRand(int top_boundary)
{
    top_boundary *= 100;
    return (rand() % top_boundary + 1) / 100.0;
}
#endif // BASIC_FUNC_H
