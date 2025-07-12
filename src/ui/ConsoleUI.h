#ifndef CONSOLE_UI_H
#define CONSOLE_UI_H

#include "../bll/ReservationService.h"

// The UI layer. It knows nothing about the database.
// It only interacts with the ReservationService.
class ConsoleUI {
public:
    ConsoleUI(ReservationService& service);
    void run();

private:
    ReservationService& service;

    // Menus
    void showMainMenu();
    void showAdminMenu();
    void showPassengerMenu();

    // Admin Actions
    void addFlight();
    void viewAllFlights();

    // Passenger Actions
    void searchAndBookFlight();
    void viewMyBookings();
    void cancelBooking();

    // Display Helpers
    void displayFlights(const std::vector<Flight>& flights);
    void displayBookings(const std::vector<Booking>& bookings);
};

#endif // CONSOLE_UI_H 