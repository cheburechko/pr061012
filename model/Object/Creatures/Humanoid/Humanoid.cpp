/*
    Copyright (c) 2012, pr061012 Team.
    See the LICENSE file for copying permission.
*/

#include <armadillo>
#include <cassert>
#include <cstdlib>
#include <sstream>
#include <string>

#include "../../../../common/BasicDefines.h"
#include "../../../../common/Math/Random.h"
#include "../../../../common/Math/DoubleComparison.h"
#include "../../Resource/Resource.h"
#include "HumanoidValueMap.h"
#include "Humanoid.h"

#define  HALF                        50
#define  HIGH_LEVEL                  80
#define  START_LEVEL_NEED_IN_HOUSE   70
#define  SMALL_LEVEL                 20
#define  TINY_LEVEL                  10
#define  A_BIT_MORE_THAN_HALF        60

// TODO:
//  Errors
//  Searching for resourse - mass
// Messages

using namespace std;
//******************************************************************************
// CONSTRUCTOR/DESTRUCTOR.
//******************************************************************************

uint Humanoid::CURRENT_HUM_ID = 0;

Humanoid::Humanoid(const DecisionMaker& dmaker) :
    Creature(HUMANOID, dmaker),
    hum_id(CURRENT_HUM_ID++),
    inventory(new ObjectHeap),
    capacity(getForce() / 10 + 5),
    free_space(capacity)
{
    int max_age = Random::int_range(HUM_AGE_MIN, HUM_AGE_MAX);

    // Initialize some inhereted things.
    max_decr_sleep_step = HUM_DECR_SLEEP_STEPS;
    this -> setMaxAge(max_age);
    this -> setAge(0);
    this -> setShapeSize(SZ_HUM_DIAM);
    this -> setShapeType(SHP_HUMANOID);
    this -> setViewArea(Shape(Vector(), SHP_HUM_VIEW_TYPE, SZ_HUM_VIEW_DIAM));
    this -> setReachArea(Shape(Vector(), SHP_HUMANOID,
                               SZ_HUM_DIAM * SZ_REACH_AREA_COEF));
    this -> setWeight(WGHT_HUMANOID);
    this -> setNormalSpeed(SPD_HUM);

    // Set danger level
    this -> setDangerLevel(DNGR_HUMANOID);

    // Create visual_memory
    visual_memory = new ObjectHeap();

    // TODO: Randomly initialize humanoid's name.
    name = "Name";

    // Randomly initialize some values.
    max_sociability = Random::int_range(HUM_SOCIABILITY_MIN, HUM_SOCIABILITY_MAX);
    laziness        = Random::int_range(HUM_LAZINESS_MIN,    HUM_LAZINESS_MAX);
    bravery         = Random::int_range(HUM_BRAVERY_MIN,     HUM_BRAVERY_MAX);

    // Initialize other values.
    sociability    = 0;  // BAD 100 - max_sociability;
    need_in_points = 0;  //100;
    need_in_house  = START_LEVEL_NEED_IN_HOUSE;

    //Initialize of matrix of attr.
    attrs(ATTR_HUNGER,0)         = 100 * getHunger() / getMaxHunger();
    attrs(ATTR_SLEEPINESS,0)     = 100 * getSleepiness() / getMaxSleepiness();
    attrs(ATTR_NEED_IN_HOUSE,0)  = need_in_house;
    attrs(ATTR_NEED_IN_POINTS,0) = 0; // need_in_points;
    attrs(ATTR_LAZINESS,0)       = laziness;
    attrs(ATTR_HEALTH,0)         = 100 * (100 - getHealth()) / getMaxHealth();
    attrs(ATTR_COMMUNICATION,0)  = 0; // 100 * sociability / max_sociability;
    attrs(ATTR_DANGER,0)         = danger;
    attrs(ATTR_NEED_IN_DESC,0)   = 0; // need_in_descendants;

    // Initialize home
    home = nullptr;

    // Initialize steps
    decr_endur_step = 0;

    this -> detailed_act  = SLEEP_ON_THE_GROUND;
}

Humanoid::~Humanoid()
{
    delete inventory;
    delete visual_memory;
}

//******************************************************************************
// ID.
//******************************************************************************

uint Humanoid::getHumanoidID() const
{
    return this -> hum_id;
}

//******************************************************************************
// TYPE NAME.
//******************************************************************************

std::string Humanoid::getTypeName() const
{
    return "humanoid";
}

//******************************************************************************
// ACTIONS.
//******************************************************************************

std::vector <Action>* Humanoid::getActions()
{
    // Memorising objects around.
    visualMemorize();

    // Checking: is steps greater than 0? And decrease them.
    if (this -> desc_steps)
    {
        this -> desc_steps--;
    }

    // Updates parametr.
//    if(desc_steps == 0)
//        updateNeedInDesc();
    updateCommonAttrs();

    // FIXME: Workaround! Need to find bug origin and fix it.
    if (steps_to_choose_place > HUM_CPFH_STEPS_MAX)
    {
        steps_to_choose_place = HUM_CPFH_STEPS_MAX;
    }

    // Update Need_in_house
    if (this -> home != nullptr)
    {
        if
        (
            100 * home -> getHealthPoints() / home -> getMaxHealthPoints()
            > START_LEVEL_NEED_IN_HOUSE
        )
        {
            need_in_house = 100 * home -> getHealthPoints()
            / home -> getMaxHealthPoints();
        }
        if (home -> getCompleteness())
        {
            this -> need_in_house = 0;

        }
        attrs(ATTR_NEED_IN_HOUSE,0) = need_in_house;
    }

    // Update attrs
    if (current_action == EAT)
    {
        if (getHunger() < TINY_LEVEL)
        {
            attrs(ATTR_HUNGER, 0) = getHunger();
            current_action = NONE;
        }
    }
    if (current_action== SLEEP)
    {
        if (getSleepiness() == 0)
        {
            attrs(ATTR_SLEEPINESS, 0) = 0;
            current_action = NONE;
        }
    }

    // Force him to sleep if he really want it
    if (getSleepiness() == getMaxSleepiness())
    {
        current_action = SLEEP;
        detailed_act   = SLEEP_ON_THE_GROUND;
    }

    // check: is humanoid's home ok?
    if (home && home -> isDestroyed())
    {
        home = nullptr;
        need_in_house = START_LEVEL_NEED_IN_HOUSE;
    }
    if (need_in_house == 0 && home -> getHealthPoints() != home -> getMaxHealthPoints())
    {
        if (100 * home -> getHealthPoints() / home -> getMaxHealthPoints() < START_LEVEL_NEED_IN_HOUSE)
        {
            need_in_house = START_LEVEL_NEED_IN_HOUSE;
        }
        else
        {
            need_in_house = 100 * home -> getHealthPoints() / home -> getMaxHealthPoints();
        }
    }
    // Boundary case. Maybe meteor shower killed our victim?
    if (aim && aim -> isDestroyed())
    {
        resetAim();
        if (detailed_act == FIGHT)
        {
            current_action = NONE;
        }
    }

    // Get current errors
    current_errors.clear();
    current_errors = errorProcess();

    // Store the result of last action and clear actions
    clearActions();
    cleanInventory();

    // If decision is not actual humanoid makes new decision.
    if (!brains.isDecisionActual(attrs, current_action))
    {
        current_action   = NONE;
        detailed_act     = SLEEP_ON_THE_GROUND;
    }

    // Make new decision and set aim and direction.
    if (current_action == NONE)
    {
        current_action = brains.makeDecision(attrs);
        resetAim();
        detailed_act = chooseAction(current_action);
    }

    // Get messages
    messageProcess();

    //**************************************************************************
    // DETAILED DECISION : RELAX_AT_HOME
    // First of all, we set direction (to home). If direction is set
    // HUMANOID goes to home. If he is in home his health and endurance will
    // increase.
    //**************************************************************************

    if (detailed_act == RELAX_AT_HOME)
    {
        if (aim == nullptr)
        {
            setAim(home);
        }

        if (!getReachArea().hitTest(aim -> getShape()))
        {
            go(SLOW_SPEED);
        }
        else
        {
            if (free_space < HUM_CRIT_SPACE * 2)
            {
                putInvInHome();
            }
            relax();
        }
    }

    //**************************************************************************
    // DETAILED DECISION : TAKE_FOOD_FROM_INVENTORY
    // Humanoid searching for resource_food in inventory. After that he just
    // eat it.
    //**************************************************************************

    if (detailed_act == TAKE_FOOD_FROM_INVENTORY)
    {
        chooseFood();
        if (aim)
        {
            Action act(EAT_OBJ, this);
            act.addParticipant(aim);
            this -> actions.push_back(act);
        }
        current_action = NONE;
    }

    //**************************************************************************
    // DETAILED DECISION : HUNT
    // First of all, he searching for victim in objects around. If he has
    // founded his victim, he just hunts. If other case he goes in random
    // way serching for victim.
    //**************************************************************************

    if (detailed_act == HUNT)
    {
        findVictim();
        if (aim == nullptr)
        {
            if (100 * getHunger() / getMaxHunger() > A_BIT_MORE_THAN_HALF)
            {
                findNearestRes(GRASS);
                if (aim != nullptr)
                {
                    detailed_act = FIND_FOOD;
                }
                else
                {
                    go(SLOW_SPEED);
                }
            }
            else
            {
                go(SLOW_SPEED);
            }
        }
        else
        {
            hunt();

            if (aim -> isDestroyed())
            {
                eat();
            }
        }
    }

    //**************************************************************************
    // DETAILED DECISION : FIND_FOOD
    // Searching for food inside humanoid visual memory. If it is founded he
    // eat it. In other case he just shuffles on the street and explores enviro-
    // ment.
    //**************************************************************************

    if (detailed_act == FIND_FOOD)
    {
        if ((visual_memory != nullptr) && (aim == nullptr))
        {
            findNearestRes(BERRIES);
            if (!aim)
            {
                findNearestRes(GRASS);
            }
        }
        if (aim == nullptr)
        {
            if (100 * getHunger() / getMaxHunger() > A_BIT_MORE_THAN_HALF)
            {
                findVictim();
                if (aim != nullptr)
                {
                    detailed_act = HUNT;
                }
                else
                {
                    go(SLOW_SPEED);
                }
            }
            else
            {
                go(SLOW_SPEED);
            }
        }
        else
        {
            if (!this -> getShape().hitTest(aim -> getShape()))
            {
                go(SLOW_SPEED);
            }
            else
            {
                eat();
            }
        }
    }

    //**************************************************************************
    // DETAILED DECISION : SLEEP_AT_HOME
    // First of all, humanoid comes home. After that we decrease his sleepiness
    // and increase endurance. We do it one time in HUM_DECR_SLEEP_STEPS steps.
    //**************************************************************************

    if (detailed_act == SLEEP_AT_HOME)
    {
        if (aim == nullptr)
        {
            setAim(home);
        }

        if (!getReachArea().hitTest(aim -> getShape()))
        {
            go(SLOW_SPEED);
        }
        else
        {
            sleep();
            if (getSleepiness() == 0)
            {
                current_action = NONE;
            }
        }

    }

    //**************************************************************************
    // DETAILED DECISION : SLEEP_ON_THE_GROUND | OK
    // Humanoid just increases endurance and decreases sleepiness
    //**************************************************************************

    if (detailed_act == SLEEP_ON_THE_GROUND)
    {
        sleep();
        if (getSleepiness() == 0)
        {
            current_action = NONE;
        }
    }

    //**************************************************************************
    // DETAILED DECISION : BUILD_HOUSE
    // Humanoid just asking controller for permission to increase health
    // of building.
    //**************************************************************************

    if (detailed_act == BUILD_HOUSE)
    {
        if (home)
        {
            if (aim == nullptr)
            {
                setAim(home);
            }
            if (!getReachArea().hitTest(aim -> getShape()))
            {
                go(SLOW_SPEED);
            }
            else
            {
                Action act(REGENERATE_OBJ, this);
                act.addParticipant(home);
                act.addParam("object_index", 0);
                this -> actions.push_back(act);
                current_action = NONE;
            }
        }
    }

    //**************************************************************************
    // DETAILED DECISION : MINE_RESOURSE | OK
    // If we don't choose which resource humanoid want to mine, we check
    // his visual memory. After that humanoid come to this resource. If he
    // did not find any resource in his memory, he will just shuffle on the
    // street and memorize new object.
    //**************************************************************************

    if (detailed_act == MINE_RESOURSES)
    {
        if ((visual_memory != nullptr) && aim == nullptr)
        {
            findNearestRes(TREE);
        }
        if (aim != nullptr && aim -> isDestroyed())
        {
            resetAim();
        }
        if (aim == nullptr)
        {
             go(SLOW_SPEED);
        }
        else
        {
            if (!getReachArea().hitTest(aim -> getShape()))
            {
                go(SLOW_SPEED);
            }
            else
            {
                Action act(MINE_OBJ, this);
                act.addParticipant(aim);
                act.addParam("res_index", 0);
                this -> actions.push_back(act);
                // if we got some res in inventory, we are checking - is it all
                // what we need to take?
                Object* obj = isResInInventory(TREE);
                if (obj)
                {
                    if
                    (
                        obj -> getHealthPoints() >
                        calculateNecessResAmount() * REG_BUILDING_COEF
                    )
                    {
                        if (home != nullptr && current_action == BUILD)
                        {
                            detailed_act = BUILD_HOUSE;
                            setAim(home);
                        }
                        else
                        {
                            current_action = NONE;
                        }
                    }
                }
             }
        }
    }

    //**************************************************************************
    // DETAILED DECISION : FIGHT
    //**************************************************************************

    if (detailed_act == FIGHT)
    {
        if (aim && !aim -> isDestroyed())
        {
            if (!getReachArea().hitTest(aim -> getShape()))
            {
                go(FAST_SPEED);
                this -> sociability++;
            }
            else
            {
                fight();
            }
        }
        else
        {
            resetAim();
            current_action = NONE;
        }
    }

    //**************************************************************************
    // DETAILED DECISION : RUN_FROM_DANGER
    // Humanoid chooses direction to escape. After thart he run, if he doesn't
    // tired. In other case he just goes.
    // Endurance is to make him go then he tired.
    //**************************************************************************

    if (detailed_act == RUN_FROM_DANGER)
    {
        chooseDirectionToEscape();
        if (getEndurance() > getMaxEndurance() / 2)
        {
            if (decr_endur_step == 0)
            {
                 decr_sleep_step = HUM_DECR_ENDUR_STEPS;
            }
            go(FAST_SPEED);
        }
        else
        {
            go(SLOW_SPEED);
        }
        direction_is_set = true;
    }

    //**************************************************************************
    // DETAILED DECISION : CHOOSE_PLACE_FOR_HOME
    // FIXME
    // Better way to choose place for home
    //**************************************************************************

    if (detailed_act == CHOOSE_PLACE_FOR_HOME)
    {
        if (steps_to_choose_place == 0)
        {
            // Okay. Stop examination of surroundings and choosing best place
            // for home.
            if (aimless)
            {
                chooseBestPlace();
            }

            if (!aimless)
            {
                // Checking distance to the best place point.
                double distance = getCoords().getDistance(current_goal_point);
                if (DoubleComparison::isLess(distance, SZ_HUM_DIAM))
                {
                    // Did we try to create building here or not?
                    bool errors = false;
                    for (uint i = 0; i < current_errors.size(); i++)
                    {
                        if (current_errors[i] == NO_PLACE_TO_PLACE_OBJ_ON)
                        {
                            errors = true;
                        }
                    }

                    // Choosing another place if there were errors.
                    if (errors)
                    {
                        //Vector old_best_place = current_goal_point;
                        //chooseBestPlace();

                        // FIXME: NO, GOD, NO, I REALLY NEED TO FIND BETTER
                        //        FIX.
                        // If best place didn't change, forcing humanoid to
                        // roam around one more time.
                        //if (old_best_place == current_goal_point)
                        //{
                            angle = Random::double_num(2 * M_PI);
                            steps_to_choose_place = Random::int_range(HUM_CPFH_STEPS_MIN, HUM_CPFH_STEPS_MAX);
                        //}
                    }
                    // No errors: trying to create building.
                    else
                    {
                        Action act(CREATE_OBJ, this);
                        act.addParam<ObjectType>("obj_type", BUILDING);
                        act.addParam<Vector>("obj_center", current_goal_point);
                        // TODO: Ugly. Humanoid need to pick max_space and max_health values
                        //       more accurate.
                        act.addParam<uint>("building_max_space",
                                       Random::int_range(BLD_MAX_SPACE_MIN, BLD_MAX_SPACE_MAX));
                        act.addParam<uint>("building_max_health",
                                       Random::int_range(BLD_MAX_HEALTH_MIN, BLD_MAX_HEALTH_MAX));
                        this -> actions.push_back(act);
                        current_action = NONE;
                    }
                }
                else
                {
                    go(SLOW_SPEED);
                }
            }
        }
        else
        {
            steps_to_choose_place--;
            go(SLOW_SPEED);
        }
    }

    return &actions;
}

//******************************************************************************
// MESSAGES
//******************************************************************************

// Receiving of messages. Used in controller.
void Humanoid::receiveMessage(Message message)
{
    this -> msgs.push_back(message);
}

// Processing of messages
void Humanoid::messageProcess()
{
    for (uint i = 0; i < msgs.size(); i++)
    {
        if (msgs[i].getType() == UNDER_ATTACK)
        {
            this -> current_action = ESCAPE;
            if
            (
                // Humanoid is strong or bravery enough.
                (
                    (getForce() > HALF && bravery > HALF) ||
                    (getForce() > HIGH_LEVEL) || (bravery > HIGH_LEVEL)
                ) &&
                // Aggressor is a creature (cannot fight with somebody/something
                // else).
                msgs[i].getReason() -> getType() == CREATURE
            )
            {
                detailed_act = FIGHT;
            }
            else
            {
                detailed_act = RUN_FROM_DANGER;
            }
            setAim(aim);
        }
    }
}

//******************************************************************************
// ACTION'S ERRORS
// Processing of action's errors.
//******************************************************************************

std::vector<ActionError> Humanoid::errorProcess()
{
    std::vector<ActionError> error;
    for (uint i=0; i < actions.size(); i++)
    {
        if (actions[i].isSucceeded())
        {
            error.push_back(NO_ERROR);
            break;
        }
        error.push_back(actions[i].getError());
    }
    return error;

}

//******************************************************************************
// UPDATES
// We change attrs of our hum
//******************************************************************************

void Humanoid::updateNeedInDesc()
{
    this -> need_in_descendants += HUM_DELTA_NEED_IN_DESC;
    this -> attrs(ATTR_NEED_IN_DESC,0) = need_in_descendants;
    this -> desc_steps = CREAT_DESC_STEPS;
}

//TODO Make separate update for sciability
    // this -> sociability += HUM_DELTA_SOC;
    // this -> attrs(ATTR_COMMUNICATION,0)     = 100 * sociability / max_sociability;

//******************************************************************************
// CHOOSE ACTION
// Fuтction gets current action (decision) and called other function to make
// detailed decision.
//******************************************************************************

DetailedHumAction Humanoid::chooseAction(CreatureAction action)
{
    DetailedHumAction result_act = SLEEP_ON_THE_GROUND;

    // Draft of father processing
    switch(action)
    {
        case EAT:               result_act = chooseWayToEat();      break;
        case SLEEP:             result_act = chooseWayToSleep();    break;
        case COMMUNICATE:                                           break;
        case RELAX:             result_act = chooseWayToRelax();    break;
        case WORK:                                                  break;
        case REALIZE_DREAM:                                         break;
        case ESCAPE:            result_act = chooseWayToEscape();   break;
        case BUILD:             result_act = chooseWayToBuild();    break;
        case REPRODUCE:                                             break;
        default:                                                    break;
    }

    return result_act;
}

//******************************************************************************
// CHOOSE_WAY_TO_RELAX
// We just check: is his home exsist at all? If it doesn't exist... So, he is
// going to sleep on the ground.
// But they relax too much. So, sometimes they searching for food or resource.
//******************************************************************************

DetailedHumAction Humanoid::chooseWayToRelax()
{
    if
    (
        (laziness < SMALL_LEVEL && 100 * getHealth()
        / getMaxHealth() > HIGH_LEVEL)
    )
    {
        if (free_space <= 2 * HUM_CRIT_SPACE)
        {
            return RELAX_AT_HOME;
        }
        uint a = Random::int_num(2);
        switch (a)
        {
            case 0: return MINE_RESOURSES; break;
            case 1:
                if (bravery > A_BIT_MORE_THAN_HALF)
                {
                    return HUNT;
                }
                else
                {
                    return FIND_FOOD;
                }
                break;
            default: return FIND_FOOD;
        }
    }
    else
    {
        if(home != nullptr)
        {
            return RELAX_AT_HOME;
        }
    }
        return SLEEP_ON_THE_GROUND;
}

//******************************************************************************
// CHOOSE_WAY_TO_BUILD
// We just check: is his home exsist at all? If it doesn't exist he is going to
// choose place for home. After that he will mine reciurse and build.
//******************************************************************************

DetailedHumAction Humanoid::chooseWayToBuild()
{
    if (this -> home == 0)
    {
        return CHOOSE_PLACE_FOR_HOME;
    }
    else
    {
        if (isResInInventory(TREE))
        {
            setAim(home);
            return BUILD_HOUSE;
        }
        return MINE_RESOURSES;
    }
}

//******************************************************************************
// CHOOSE_WAY_TO_EAT
// If he is bravery guy he will hunt. In other case, he will search food. But if
// he already has foon in inventory - he just takes it.
//******************************************************************************

DetailedHumAction Humanoid::chooseWayToEat()
{
    if (isResInInventory(GRASS) || isResInInventory(MEAT) || isResInInventory(BERRIES))
    {
        return TAKE_FOOD_FROM_INVENTORY;
    }


    if
    (
        ((getForce() > HALF && bravery > HALF) || (getForce() > HIGH_LEVEL)
        || (bravery > HIGH_LEVEL))
    )
    {
        return HUNT;
    }
    else
    {
        return FIND_FOOD;
    }
}

//******************************************************************************
// CHOOSE_WAY_TO_SLLEP
// The best way to sleep is sleeping at home. But ih humanoid doesn't have home
// or he is too far from it he will sleep in the ground.
//******************************************************************************

DetailedHumAction Humanoid::chooseWayToSleep()
{
    if (this -> home != nullptr)
    {
            return SLEEP_AT_HOME;
    }
    return SLEEP_ON_THE_GROUND;

}

//******************************************************************************
// CHOOSE_WAY_TO_ESCAPE
// Indeed this is case when he just escapes dangerous place. He would figt if
// somebody tries to attack him or when he hunts.
//******************************************************************************

DetailedHumAction Humanoid::chooseWayToEscape()
{
    if (detailed_act == FIGHT)
        return FIGHT;
    return RUN_FROM_DANGER;
}

// Puts new object in humanoid's memory
void Humanoid::visualMemorize()
{

    ObjectHeap::const_iterator iter;
    for(
        iter = objects_around.begin(BUILDING);
        iter != objects_around.end(BUILDING); iter++
       )
    {
        this -> visual_memory -> push(*iter);
    }
    for(
        iter = objects_around.begin(RESOURCE);
        iter != objects_around.end(RESOURCE); iter++
       )
    {
         this -> visual_memory -> push(*iter);
    }
}

// Just setter
void Humanoid::setHome(Building *home)
{
    this -> home = home;
}

//******************************************************************************
// INHERETED THINGS.
//******************************************************************************

std::string Humanoid::printObjectInfo(bool full) const
{
    std::stringstream ss;
    ss << Creature::printObjectInfo(full);

    ss << insertSpaces("Detailed action");
    switch (detailed_act)
    {
        case HUNT:                     ss << "hunt";                     break;
        case TAKE_FOOD_FROM_INVENTORY: ss << "take food from inventory"; break;
        case FIND_FOOD:                ss << "find food";                break;
        case RELAX_AT_HOME:            ss << "relax at home";            break;
        case SLEEP_AT_HOME:            ss << "sleep at home";            break;
        case SLEEP_ON_THE_GROUND:      ss << "sleep on the ground";      break;
        case MINE_RESOURSES:           ss << "mine resource";            break;
        case BUILD_HOUSE:              ss << "build house";              break;
        case CHOOSE_PLACE_FOR_HOME:    ss << "choose place for home";    break;
        case FIGHT:                    ss << "fight";                    break;
        case RUN_FROM_DANGER:          ss << "run from danger";          break;
        default:                       ss << "unknown";                  break;
    }
    ss << std::endl;

    ss << insertSpaces("Sociability")                << sociability << "/" << max_sociability << std::endl <<
          insertSpaces("Bravery")                    << bravery << std::endl <<
          insertSpaces("Laziness")                   << laziness << std::endl;

    if (full)
    {
        ss << insertSpaces("Need in house")          << need_in_house << std::endl <<
              insertSpaces("Need in points")         << need_in_points << std::endl;
    }

    ss << insertSpaces("Home ID")                    << (home == nullptr ? "none" : std::to_string(home -> getObjectID())) << std::endl;

    if (full)
    {
        ss << insertSpaces("Inventory")              << std::endl << inventory -> printIDs();
        ss << insertSpaces("Visual memory")          << std::endl << visual_memory -> printIDs() <<
              insertSpaces("Steps for choose place") << steps_to_choose_place << std::endl;
    }
    if (full)
    {
        ss << insertSpaces("FreeSpace/Capacity")     << free_space << '/' << capacity << std::endl;
    }

    return ss.str();
}

//******************************************************************************
// HOME CREATION.
//******************************************************************************

void Humanoid::chooseBestPlace()
{
    HumanoidValueMap map(visual_memory, Creature::world_size, Creature::world_size, 0.3);
    Vector c = map.getBestPlace();

    // Sad but true.
    if (c == Vector(-1, -1))
    {
        angle = Random::double_num(2 * M_PI);
        steps_to_choose_place = Random::int_range(HUM_CPFH_STEPS_MIN, HUM_CPFH_STEPS_MAX);
    }
    else
    {
        setAim(c);
    }
}

//******************************************************************************
// INVENTORY
//******************************************************************************

// Adds object to inventory.
bool Humanoid::addToInventory(Object *obj)
{
    uint weight = obj -> getWeight();
    // Resources should be stacked together
    if (obj -> getType() == RESOURCE)
    {
        uint amount = obj -> getHealthPoints();
        // Check if we have enough free space
        if (weight > free_space)
        {
            return false;
        }
        free_space -= weight;

        ResourceType subtype = dynamic_cast<Resource*>(obj) -> getSubtype();
        for (ObjectHeap::iterator i = inventory -> begin(RESOURCE);
             i != inventory -> end(RESOURCE) && obj -> getHealthPoints(); i++)
        {
            if (dynamic_cast<Resource*>(*i) -> getSubtype() == subtype)
            {
                dynamic_cast<Resource*>(*i) -> increaseMaxAmount(obj -> getHealthPoints());
                obj -> damage((*i) -> heal(obj -> getHealthPoints()));
            }
        }

        // Push the rest.
        if (obj -> getHealthPoints())
        {
            this -> inventory -> push(obj);
        }
        return true;
    }
    return false;
}

// Clear inventory from destroyed objects.
void Humanoid::cleanInventory()
{
    // First place them in buffer.
    std::vector<Object*> buffer;
    free_space = capacity;
    for (ObjectHeap::iterator i = inventory -> begin();
         i != inventory -> end(); i++)
    {
        if ((*i) -> isDestroyed())
        {
            buffer.push_back(*i);
        }
        else
        {
            free_space -= (*i) -> getWeight();
        }
    }

    // Then remove them from inventory.
    for (uint i = 0; i < buffer.size(); i++)
    {
        inventory -> remove(buffer[i]);
    }
}

// Remove object from inventory.
void Humanoid::removeFromInventory(Object * obj)
{
    inventory -> remove(obj);
    free_space += obj -> getWeight();
}

ObjectHeap * Humanoid::getInventory()
{
    return this -> inventory;
}

//******************************************************************************
// DEBUG
//******************************************************************************

uint Humanoid::getBravery() const
{
    return this -> bravery;
}

uint Humanoid::getNeedInHouse() const
{
    return this -> need_in_house;
}

Building* Humanoid::getHome() const
{
    return this -> home;
}

ObjectHeap* Humanoid::getVisMem() const
{
    return this -> visual_memory;
}

void Humanoid::setDetailedAction(DetailedHumAction detailed_act)
{
    this -> detailed_act = detailed_act;
}

// returns current decision of humanoid
uint Humanoid::getCurrentDetailedAct() const
{
    return detailed_act;
}

// gets free space
uint Humanoid::getFreeSpace()
{
    return this -> free_space;
}

// gets free space
uint Humanoid::getCapacity()
{
    return this -> capacity;
}

//******************************************************************************
// Searching
// Different funcs of searching to reduce code
//******************************************************************************

// Searching for res in vis mem
void Humanoid::findNearestRes(ResourceType type)
{
    ObjectHeap::const_iterator iter;
    double distance = SZ_WORLD_VSIDE;
    for
    (
        iter = visual_memory -> begin(RESOURCE);
        iter != visual_memory -> end(RESOURCE); iter++
    )
    {
        // skip all destroyed objects
        if ((*iter) -> isDestroyed())
        {
            continue;
        }
        Resource* res = dynamic_cast<Resource*>(*iter);
        if (res -> getSubtype()  == type)
        {
            Vector coords;
            coords = res -> getCoords();
            if (distance > coords.getDistance(this -> getCoords()))
            {
                setAim(res);
                distance = coords.getDistance(this -> getCoords());
            }
        }
    }
}

// searching for victim in obj around
void Humanoid::findVictim()
{
    double min_dist = SZ_WORLD_VSIDE;
    ObjectHeap::const_iterator iter;
    for
    (
        iter = objects_around.begin(CREATURE);
        iter != objects_around.end(CREATURE); iter++
    )
    {
        // FIXME You don't use any of Creature's feature, why you make dynamic_cast?
        Creature* creat = dynamic_cast<Creature*>(*iter);
        if
        (
            this -> getCoords().getDistance(creat -> getCoords()) < min_dist
            && this -> getDangerLevel() > creat -> getDangerLevel()
        )
        {
            setAim(creat);
            min_dist = this -> getCoords().getDistance(aim -> getCoords());
        }
    }
}

// Searching for res in inventory
Object* Humanoid::isResInInventory(ResourceType type)
{
    ObjectHeap::const_iterator iter;
    for(
        iter = inventory -> begin(RESOURCE);
        iter != inventory -> end(RESOURCE); iter++
       )
    {
        Resource* res = dynamic_cast<Resource*>(*iter);
        if (res -> getSubtype() == type)
        {
            return res;
        }
    }
    return nullptr;
}

//**************************************************************************
// INVENTORY FUNC
//**************************************************************************

// If we mine resource because we build, we calculate it (how many res we need
// to complete our home?). If we just mining res for best future, we choose
// amount randomly.

uint Humanoid::calculateNecessResAmount()
{
    if (current_action == BUILD)
    {
        if (free_space <= HUM_CRIT_SPACE)
        {
            return 0;
        }
        if (home != nullptr)
        {
            // checking - can we mine necessary amount for one time?
            // if yes - take it
            // delta health = reg_bild * space/weight
            // space/weight = amount
            uint delta_health = home -> getMaxHealthPoints() - home -> getHealthPoints();
            if
            (
                delta_health * WGHT_RESOURCE <
                (free_space - HUM_CRIT_SPACE) * REG_BUILDING_COEF
            )
            {
                return delta_health / REG_BUILDING_COEF;
            }
            // if no - take as much as you can
            else
            {
                return (free_space - HUM_CRIT_SPACE) / WGHT_RESOURCE;
            }
        }
        else
        {
            detailed_act = CHOOSE_PLACE_FOR_HOME;
            steps_to_choose_place = Random::int_range(HUM_CPFH_STEPS_MIN, HUM_CPFH_STEPS_MAX);
            return 0;
        }
    }
    else
    {
        // Just random numb
        uint res_amount = Random::int_range(1, capacity / 2);
        if (res_amount * WGHT_RESOURCE <= free_space - HUM_CRIT_SPACE)
        {
            return res_amount;
        }
        else
        {
            return (free_space - HUM_CRIT_SPACE) / WGHT_RESOURCE;
        }
    }
    return 1;
}

// move inv to home
// if we have res - put them to home, but if our inventory full of eat we shoul
// also put it at home.
void Humanoid::putInvInHome()
{
    if (putHomeAux(TREE))
    {
        return ;
    }
    else
    {
        putHomeAux(MEAT);
        putHomeAux(BERRIES);
        // I dont rhink that hum should eat grass
    }
}

bool Humanoid::putHomeAux(ResourceType type)
{
    Object* obj = isResInInventory(type);
    if (obj)
    {
        bool isOk = home -> putInside(obj);
        if (isOk)
        {
            removeFromInventory(obj);
            return true;
        }
        else
        {
            Action act(DROP_OBJS, this);
            act.addParticipant(obj);
            this -> actions.push_back(act);
            return true;
        }
    }
        return false;
}

//**************************************************************************
// AUXILIARY FUNC
// It puts victim (or res) in inventory if we want to eat and make him eat it
// if he want
//**************************************************************************

void Humanoid::eat()
{
    if (current_action == EAT)
    {
        Action act(EAT_OBJ, this);
        act.addParticipant(aim);
        this -> actions.push_back(act);
        resetAim();
    }
    else
    {
        Action act(PICK_UP_OBJS, this);
        act.addParticipant(aim);
        this -> actions.push_back(act);
        resetAim();
        current_action = NONE;
    }
}

// If he wants to eat his priorities: meat / berries / crass. in other case
// vice versa
void Humanoid::chooseFood()
{
    ResourceType food[3];
    food[0] = MEAT;
    food[1] = BERRIES;
    food[2] = GRASS;
    if (100 * getHunger() / getMaxHunger() > HALF)
    {
        for (int i = 0; i < 3; i++)
        {
            aim = isResInInventory(food[i]);
            if (aim)
            {
                break;
            }
        }
    }
    else
    {
        for (int i = 1; i >= 0; i--)
        {
            aim = isResInInventory(food[i]);
            if (aim)
            {
                break;
            }
        }
    }
}
