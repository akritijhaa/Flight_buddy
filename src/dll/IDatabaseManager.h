#ifndef IDATABASE_MANAGER_H
#define IDATABASE_MANAGER_H

#include <vector>
#include <string>
#include <optional>
#include "../core/models.h"

// Abstract interface for database operations.
// This allows the business logic layer to be independent of the specific database used.
class IDatabaseManager {
public:
    virtual ~IDatabaseManager() = default;

    virtual void initialize() = 0;

    // Flight Management
    virtual bool addFlight(const Flight& flight) = 0;
    virtual std::vector<Flight> searchFlights(const std::string& origin, const std::string& destination) = 0;
    virtual std::optional<Flight> getFlightById(int flightId) = 0;
    virtual std::vector<Flight> getAllFlights() = 0;
    virtual bool updateFlightSeatCount(int flightId, int change) = 0;

    // Booking Management
    virtual std::optional<int> addBooking(int flightId, const std::string& passengerName, const std::string& passengerEmail) = 0;
    virtual std::optional<Booking> getBookingById(int bookingId) = 0;
    virtual bool deleteBooking(int bookingId) = 0;
    virtual std::vector<Booking> getBookingsForPassenger(const std::string& passengerEmail) = 0;

    // Transaction Management
    virtual bool beginTransaction() = 0;
    virtual bool commitTransaction() = 0;
    virtual bool rollbackTransaction() = 0;
};

#endif // IDATABASE_MANAGER_H 