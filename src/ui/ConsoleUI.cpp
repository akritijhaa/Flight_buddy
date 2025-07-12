#include "ConsoleUI.h"
#include "../utils/helpers.h"
#include "../core/models.h"
#include <iostream>
#include <iomanip>

ConsoleUI::ConsoleUI(ReservationService& service) : service(service) {}

void ConsoleUI::run() {
    while (true) {
        showMainMenu();
        int choice = getIntegerInput("Enter your choice: ");
        switch (choice) {
            case 1: showAdminMenu(); break;
            case 2: showPassengerMenu(); break;
            case 3: return;
            default: std::cout << "Invalid choice. Please try again.\n"; pressEnterToContinue();
        }
    }
}

void ConsoleUI::showMainMenu() {
    clearScreen();
    std::cout << "========================================\n"
              << "     Welcome to AirBooker System\n"
              << "========================================\n"
              << "1. Admin Menu\n"
              << "2. Passenger Menu\n"
              << "3. Exit\n"
              << "----------------------------------------\n";
}

void ConsoleUI::showAdminMenu() {
    while (true) {
        clearScreen();
        std::cout << "========================================\n"
                  << "              Admin Menu\n"
                  << "========================================\n"
                  << "1. Add New Flight\n"
                  << "2. View All Flights\n"
                  << "3. Back to Main Menu\n"
                  << "----------------------------------------\n";
        int choice = getIntegerInput("Enter your choice: ");
        switch (choice) {
            case 1: addFlight(); break;
            case 2: viewAllFlights(); break;
            case 3: return;
            default: std::cout << "Invalid choice.\n"; pressEnterToContinue();
        }
    }
}

void ConsoleUI::showPassengerMenu() {
    while (true) {
        clearScreen();
        std::cout << "========================================\n"
                  << "            Passenger Menu\n"
                  << "========================================\n"
                  << "1. Search & Book a Flight\n"
                  << "2. View My Bookings\n"
                  << "3. Cancel a Booking\n"
                  << "4. Back to Main Menu\n"
                  << "----------------------------------------\n";
        int choice = getIntegerInput("Enter your choice: ");
        switch (choice) {
            case 1: searchAndBookFlight(); break;
            case 2: viewMyBookings(); break;
            case 3: cancelBooking(); break;
            case 4: return;
            default: std::cout << "Invalid choice.\n"; pressEnterToContinue();
        }
    }
}

void ConsoleUI::addFlight() {
    clearScreen();
    std::cout << "--- Add New Flight ---\n";
    Flight f;
    
    std::cout << "Enter Flight Number (e.g., AI404): ";
    std::getline(std::cin, f.flightNumber);
    
    std::cout << "Enter Origin (e.g., Delhi): ";
    std::getline(std::cin, f.origin);
    
    std::cout << "Enter Destination (e.g., Mumbai): ";
    std::getline(std::cin, f.destination);
    
    std::cout << "Enter Departure Time (YYYY-MM-DD HH:MM): ";
    std::getline(std::cin, f.departureTime);
    
    f.totalSeats = getIntegerInput("Enter Total Seats: ");
    f.availableSeats = f.totalSeats;
    
    std::cout << "Enter Price: ";
    std::cin >> f.price;
    clearInputBuffer();

    if (service.addNewFlight(f)) {
        std::cout << "\nFlight added successfully!\n";
    } else {
        std::cout << "\nFailed to add flight. Flight number might already exist.\n";
    }
    pressEnterToContinue();
}

void ConsoleUI::viewAllFlights() {
    clearScreen();
    std::cout << "--- All Scheduled Flights ---\n";
    displayFlights(service.getAllFlights());
    pressEnterToContinue();
}

void ConsoleUI::searchAndBookFlight() {
    clearScreen();
    std::cout << "--- Search & Book a Flight ---\n";
    
    std::string origin, destination;
    std::cout << "Enter Origin: ";
    std::getline(std::cin, origin);
    
    std::cout << "Enter Destination: ";
    std::getline(std::cin, destination);
    
    auto flights = service.findAvailableFlights(origin, destination);
    if (flights.empty()) {
        std::cout << "\nNo available flights found for this route.\n";
        pressEnterToContinue();
        return;
    }
    
    displayFlights(flights);
    
    int flightId = getIntegerInput("\nEnter the ID of the flight to book (0 to cancel): ");
    if (flightId == 0) return;
    
    std::string name, email;
    std::cout << "Enter your full name: ";
    std::getline(std::cin, name);
    
    std::cout << "Enter your email address: ";
    std::getline(std::cin, email);
    
    auto bookingId = service.bookFlight(flightId, name, email);
    if (bookingId) {
        std::cout << "\nBooking successful! Your Booking ID is: " << *bookingId << "\n";
    } else {
        std::cout << "\nBooking failed. The flight may be full or does not exist.\n";
    }
    pressEnterToContinue();
}

void ConsoleUI::viewMyBookings() {
    clearScreen();
    std::cout << "--- View My Bookings ---\n";
    
    std::string email;
    std::cout << "Enter your email address: ";
    std::getline(std::cin, email);
    
    displayBookings(service.findMyBookings(email));
    pressEnterToContinue();
}

void ConsoleUI::cancelBooking() {
    clearScreen();
    std::cout << "--- Cancel a Booking ---\n";
    
    int bookingId = getIntegerInput("Enter the Booking ID to cancel: ");
    
    std::cout << "Are you sure you want to cancel booking ID " << bookingId << "? (y/n): ";
    char confirm;
    std::cin >> confirm;
    clearInputBuffer();
    
    if (confirm == 'y' || confirm == 'Y') {
        if (service.cancelBooking(bookingId)) {
            std::cout << "\nBooking successfully cancelled.\n";
        } else {
            std::cout << "\nCancellation failed. Check the Booking ID.\n";
        }
    } else {
        std::cout << "\nCancellation aborted.\n";
    }
    pressEnterToContinue();
}

void ConsoleUI::displayFlights(const std::vector<Flight>& flights) {
    if (flights.empty()) {
        std::cout << "\nNo flights to display.\n";
        return;
    }
    
    std::cout << "\n" << std::left 
              << std::setw(5) << "ID" 
              << std::setw(12) << "Flight #" 
              << std::setw(15) << "Origin" 
              << std::setw(15) << "Destination" 
              << std::setw(20) << "Departure" 
              << std::setw(10) << "Seats" 
              << std::setw(10) << "Price ($)" 
              << "\n" << std::string(87, '-') << "\n";
    
    for (const auto& f : flights) {
        std::cout << std::left 
                  << std::setw(5) << f.id 
                  << std::setw(12) << f.flightNumber 
                  << std::setw(15) << f.origin 
                  << std::setw(15) << f.destination 
                  << std::setw(20) << f.departureTime 
                  << std::setw(10) << f.availableSeats 
                  << std::fixed << std::setprecision(2) << std::setw(10) << f.price 
                  << "\n";
    }
}

void ConsoleUI::displayBookings(const std::vector<Booking>& bookings) {
    if (bookings.empty()) {
        std::cout << "\nNo bookings found.\n";
        return;
    }
    
    std::cout << "\n" << std::left 
              << std::setw(10) << "BookingID" 
              << std::setw(12) << "Flight #" 
              << std::setw(15) << "Origin" 
              << std::setw(15) << "Destination" 
              << std::setw(20) << "Departure" 
              << std::setw(25) << "Passenger" 
              << "\n" << std::string(97, '-') << "\n";
    
    for (const auto& b : bookings) {
        std::cout << std::left 
                  << std::setw(10) << b.id 
                  << std::setw(12) << b.flightNumber 
                  << std::setw(15) << b.origin 
                  << std::setw(15) << b.destination 
                  << std::setw(20) << b.departureTime 
                  << std::setw(25) << b.passengerName 
                  << "\n";
    }
} 