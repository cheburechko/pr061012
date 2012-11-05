#ifndef E_PARAM_ARRAY_BAD_KEY_H
#define E_PARAM_ARRAY_BAD_KEY_H

#include <exception>

/**
 * @brief ParamArray exception -- unknown (bad) key was used.
 */
class EParamArrayBadKey : public std::exception
{
    virtual const char * what() const throw();
};

#endif // E_PARAM_ARRAY_BAD_KEY_H
