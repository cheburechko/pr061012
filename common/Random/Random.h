/*
    Copyright (c) 2012, pr061012 Team.
    See the COPYING file for copying permission.
*/

#ifndef RANDOM_H
#define RANDOM_H

#include <cstdlib>

/**
 * @brief Interface for getting pseudorandom numbers.
 *
 *        *NOTE*: As you can see this interface doesn't use function
 *                overloading. This is done in order to prevent bugs such like
 *                when you want get double random number from 10 to 20 range
 *                and accidently use range(10, 20) instead of needed
 *                range(10.0, 20.0).
 */
class Random
{
public:
    /**
     * @brief Returns random number from range [0, top_boundary - 1].
     */
    static int int_num(int top_boundary)
    {
        if(top_boundary == 0)
        {
            return 0;
        }

        return rand() % top_boundary;
    }

    /**
     * @brief Returns random double number from range [0, top_boundary - 1].
     */
    static double double_num(double top_boundary)
    {
        // 1E-5 was chosen by fairly really random. I guarantee that.
        if(top_boundary <= 1E-5)
        {
            return 0;
        }

        top_boundary *= 1000;
        return (rand() % ((int) top_boundary)) / 1000.0;
    }

    /**
     * @brief Returns random number from range [low_boundary, top_boundary].
     */
    static int int_range(int low_boundary, int top_boundary)
    {
        return Random::int_num(top_boundary - low_boundary) + low_boundary;
    }

    /**
     * @brief Returns random double number from range [low_boundary,
     *        top_boundary].
     */
    static double double_range(double low_boundary, double top_boundary)
    {
        return Random::double_num(top_boundary - low_boundary) + low_boundary;
    }
};

#endif // RANDOM_H
