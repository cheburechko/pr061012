/*
    Copyright (c) 2012, pr061012 Team.
    See the LICENSE file for copying permission.
*/

#ifndef DECISION_MAKER_H
#define DECISION_MAKER_H

#include <armadillo>
#include <fstream>
#include <vector>
#include <cstdlib>

#include "../../common/BasicTypes.h"

/**
 * @class DecisionMaker
 * @brief Makes a prediction about next creature's action.
 */
class DecisionMaker
{
public:

    //**************************************************************************
    // CONSTRUCTOR/DESTRUCTOR.
    //**************************************************************************

    /**
     * @brief Constructor.
     */
    DecisionMaker(CreatureType type);

    /**
     * @brief Destructor.
     */
    ~DecisionMaker();

    //**************************************************************************
    // PREDICTION.
    //**************************************************************************

    /**
     * @brief  Makes a creature's decision about next action.
     * @param  attrs    vector of creature's attributes
     * @return int      next action
     */
    CreatureAction makeDecision(arma::mat attrs) const;
    /**
     * @brief  Calculate the difference between current decision and other actions.
     * @param  attrs            vector of creature's attributes
     * @param  current_decision current decision of creature
     * @return bool             true if we need to change our decision
     */
    bool isDecisionActual(arma::mat attrs, CreatureAction current_decision) const;

    //**************************************************************************
    // DEBUG
    //**************************************************************************
    /**
     * @brief  Gets matrix of actions
     * @return mat  matrix of actions
     */
    arma::mat getActMatrix(arma::mat attrs) const;

private:
    /// @brief Matrix for decision making. In the nutshell, it's a SM or
    ///        no-hidden-layer NN matrix.
    arma:: mat theta;

};

#endif // DECISION_MAKER_H

