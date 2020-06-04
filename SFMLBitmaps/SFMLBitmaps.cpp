// SFMLBitmaps.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "BitmapsApp.h"
#include "CellularAutomaton.h"

int main()
{
    BitmapsApp app;

    //app.init();

    //app.run();

    //app.cleanup();

    // Game-of-life neighborhood
    NeighborHood nb = {
        {1,1,1},
        {1,0,1},
        {1,1,1}
    };

    // Game-of-life next state function
    NextStateFunction gol_nsf = {
        {0, 0, 0, 1, 0, 0, 0, 0, 0 },
        {0, 0, 1, 1, 0, 0, 0, 0, 0 }
    };

    CellularAutomaton gameOfLife(20, 20, nb, gol_nsf);

    gameOfLife.print();

    // Insert blinker
    gameOfLife.insert(1, 1,
        {
            {0, 1, 0},
            {0, 1, 0},
            {0, 1, 0}
        });

    gameOfLife.print();

    gameOfLife.update();

    gameOfLife.print();


    std::cout << nb[0][0] << std::endl;

    nb[0][0] = 0;

    std::cout << nb[0][0] << std::endl;


}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
