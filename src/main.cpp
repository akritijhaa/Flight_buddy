/*
================================================================================
            AirBooker - Flight Reservation System (Modular Design)
================================================================================
* Description: A full-stack, console-based Flight Reservation System in C++
* refactored into a loosely-coupled, modular architecture.
* Author: Gemini
* Language: C++17
* Database: SQLite3
*
* --- PROJECT STRUCTURE ---
* /
* |- Makefile                 - For compiling the project
* |- main.cpp                 - Application entry point
* |
* |- core/
* |   |- models.h             - Core data structures (Flight, Booking)
* |
* |- dal/
* |   |- IDatabaseManager.h     - Abstract interface for the database
* |   |- SqliteDatabaseManager.h- Concrete SQLite implementation
* |   |- SqliteDatabaseManager.cpp
* |
* |- bll/
* |   |- ReservationService.h   - Business logic layer
* |   |- ReservationService.cpp
* |
* |- ui/
* |   |- ConsoleUI.h            - Presentation layer
* |   |- ConsoleUI.cpp
* |
* |- utils/
* |- helpers.h              - Utility functions
* |- helpers.cpp
*
* --- HOW TO COMPILE AND RUN ---
* 1. Save each file block below into its corresponding file and directory.
* 2. Run `make` in the root directory to compile.
* 3. Run the executable: `./flight_system`
*
================================================================================
*/

#include "dal/SqliteDatabaseManager.h"
#include "bll/ReservationService.h"
#include "ui/ConsoleUI.h"
#include <iostream>
#include <memory>

int main() {
    try {
        // 1. Create the concrete Data Access Layer object.
        auto dbManager = std::make_unique<SqliteDatabaseManager>("flights.db");
        dbManager->initialize();

        // 2. Create the Business Logic Layer, injecting the DAL.
        ReservationService service(std::move(dbManager));

        // 3. Create the UI Layer, injecting the BLL.
        ConsoleUI ui(service);

        // 4. Run the application.
        ui.run();

    } catch (const std::exception& e) {
        std::cerr << "A critical error occurred: " << e.what() << std::endl;
        return 1;
    }

    std::cout << "Thank you for using AirBooker. Goodbye!\n";
    return 0;
}
