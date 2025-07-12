#ifndef RESERVATION_SERVICE_H
#define RESERVATION_SERVICE_H

#include "../dal/IDatabaseManager.h"
#include <memory>

// Business Logic Layer: Handles the core application logic.
// It is completely decoupled from the UI and the concrete database implementation.
class ReservationService {
public:
    // Uses dependency injection to accept any class that implements IDatabaseManager.
    ReservationService(std::unique_ptr<IDatabaseManager> dbManager);

    // Admin services
    bool addNewFlight(const Flight& flight);
    std::vector<Flight> getAllFlights();
    
    // Passenger services
    std::vector<Flight> findAvailableFlights(const std::string& origin, const std::string& destination);
    std::optional<int> bookFlight(int flightId, const std::string& passengerName, const std::string& passengerEmail);
    bool cancelBooking(int bookingId);
    std::vector<Booking> findMyBookings(const std::string& passengerEmail);

private:
    std::unique_ptr<IDatabaseManager> db;
};

#endif // RESERVATION_SERVICE_H 