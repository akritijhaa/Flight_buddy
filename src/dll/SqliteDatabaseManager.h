#ifndef SQLITE_DATABASE_MANAGER_H
#define SQLITE_DATABASE_MANAGER_H

#include "IDatabaseManager.h"
#include <sqlite3.h>

// Concrete implementation of the IDatabaseManager interface for SQLite.
class SqliteDatabaseManager : public IDatabaseManager {
public:
    SqliteDatabaseManager(const std::string& db_name);
    ~SqliteDatabaseManager() override;

    void initialize() override;

    // Flight Management
    bool addFlight(const Flight& flight) override;
    std::vector<Flight> searchFlights(const std::string& origin, const std::string& destination) override;
    std::optional<Flight> getFlightById(int flightId) override;
    std::vector<Flight> getAllFlights() override;
    bool updateFlightSeatCount(int flightId, int change) override;

    // Booking Management
    std::optional<int> addBooking(int flightId, const std::string& passengerName, const std::string& passengerEmail) override;
    std::optional<Booking> getBookingById(int bookingId) override;
    bool deleteBooking(int bookingId) override;
    std::vector<Booking> getBookingsForPassenger(const std::string& passengerEmail) override;
    
    // Transaction Management
    bool beginTransaction() override;
    bool commitTransaction() override;
    bool rollbackTransaction() override;

private:
    sqlite3* db;
    std::string dbName;
    bool execute(const std::string& sql);
};

#endif // SQLITE_DATABASE_MANAGER_H 