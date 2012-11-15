#include <iostream>
#include <ctime>
#include <random>
#include <cstdlib>

#include "model/BasicDefines.h"
#include "model/World/World.h"
#include "control/Controller/Controller.h"
#include "view/View.h"
#include "common/Log/Log.h"
#include "common/Exceptions/EInvalidResPath.h"

/// Period for world updating.
const int PERIOD = CLOCKS_PER_SEC / TM_TICKS_PER_SECOND;

int main()
{
    // Error flag.
    bool error = false;

    // Initializing random.
    srand(time(NULL));

    // Running game.
    try
    {
        // Creating World, View and Controller.
        World world(rand(), SZ_WORLD_HSIDE);
        Controller control(&world);
        View view(world);

        int t0 = clock();
        int counter = 0;

        do
        {
            if(clock() - t0 > PERIOD)
            {
                t0 += PERIOD;
            }

            if(counter++ == 10)
            {
                control.step();
                counter = 0;
            }

            view.redraw();

        } while(view.continues());
    }
    catch(EInvalidResPath& exc)
    {
        error = true;
        Log::ERROR("Failed to load game resource '" + exc.getResPath() +
                   "'. Terminating.");
    }
    catch(std::bad_alloc& exc)
    {
        error = true;
        Log::ERROR("Failed to allocate memory. Terminating.");
    }

    if(error)
    {
        std::cout << "An error occurred during game execution. Look for LOG " <<
                     "file for details." << std::endl;
    }

    return 0;
}
