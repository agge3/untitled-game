/**
 * @project untitled game
 * @copyright See LICENSE in the top level directory
 * @file main.cpp
 * @purpose A fun and untitled game
 * @author agge3
 */

#include "app.h"

#include <stdexcept>
#include <iostream>

int main()
{
    // replace atof with std::stod for cmake
    //const double input_value = std::stod(argv[1]);
    // cmake tutorial, print exe name and version num
    //if (argc < 2) {
        // report version
        //std::cout << argv[0] << " Version " << game_VERSION_MAJOR << "."
        //    << game_VERSION_MINOR << std::endl;
        //std::cout << "Usage: " << argv[0] << " number" << std::endl;
        //return 1;
    //}
    try {
        Application app;
        app.run();
    } catch (std::exception& e) {
        std::cerr << "\nexception: " << e.what() << std::endl;
    }
}
