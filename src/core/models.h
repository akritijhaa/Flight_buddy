#ifndef MODELS_H
#define MODELS_H

#include <string>

// Plain data structure for a Flight.
struct Flight {
    int id;
    std::string flightNumber;
    std::string origin;
    std::string destination;
    std::string departureTime;
    int totalSeats;
    int availableSeats;
    double price;
};

// Plain data structure for a Booking.
struct Booking {
    int id;
    int flightId;
    std::string passengerName;
    std::string passengerEmail;
    // Denormalized data for easy display
    std::string flightNumber;
    std::string origin;
    std::string destination;
    std::string departureTime;
};

#endif // MODELS_H 