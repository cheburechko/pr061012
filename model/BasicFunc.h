/*
    Copyright (c) 2012, pr061012 Team.
    See the LICENSE file for copying permission.
*/

#ifndef BASIC_FUNC_H
#define BASIC_FUNC_H

/**
 * @brief The generator of random value
 */
static inline int randFromRange(int low_boundary, int top_boundary)
{
    return rand() % (top_boundary - low_boundary) + low_boundary + 1;
}

#endif // BASIC_FUNC_H