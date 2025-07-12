#include "SqliteDatabaseManager.h"
#include <iostream>
#include <stdexcept>

SqliteDatabaseManager::SqliteDatabaseManager(const std::string& db_name) : db(nullptr), dbName(db_name) {
    if (sqlite3_open(dbName.c_str(), &db)) {
        std::string errorMsg = "Can't open database: " + std::string(sqlite3_errmsg(db));
        sqlite3_close(db);
        throw std::runtime_error(errorMsg);
    }
}

SqliteDatabaseManager::~SqliteDatabaseManager() {
    if (db) {
        sqlite3_close(db);
    }
}

bool SqliteDatabaseManager::execute(const std::string& sql) {
    char* zErrMsg = nullptr;
    int rc = sqlite3_exec(db, sql.c_str(), nullptr, 0, &zErrMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << zErrMsg << std::endl;
        sqlite3_free(zErrMsg);
        return false;
    }
    return true;
}

void SqliteDatabaseManager::initialize() {
    std::string createFlightsTable =
        "CREATE TABLE IF NOT EXISTS Flights ("
        "ID INTEGER PRIMARY KEY AUTOINCREMENT, "
        "FlightNumber TEXT NOT NULL UNIQUE, "
        "Origin TEXT NOT NULL, "
        "Destination TEXT NOT NULL, "
        "DepartureTime TEXT NOT NULL, "
        "TotalSeats INTEGER NOT NULL, "
        "AvailableSeats INTEGER NOT NULL, "
        "Price REAL NOT NULL);";

    std::string createBookingsTable =
        "CREATE TABLE IF NOT EXISTS Bookings ("
        "ID INTEGER PRIMARY KEY AUTOINCREMENT, "
        "FlightID INTEGER NOT NULL, "
        "PassengerName TEXT NOT NULL, "
        "PassengerEmail TEXT NOT NULL, "
        "FOREIGN KEY(FlightID) REFERENCES Flights(ID));";

    if (!execute(createFlightsTable) || !execute(createBookingsTable)) {
        throw std::runtime_error("Failed to create database tables.");
    }
}

bool SqliteDatabaseManager::addFlight(const Flight& flight) {
    sqlite3_stmt* stmt;
    const char* sql = "INSERT INTO Flights (FlightNumber, Origin, Destination, DepartureTime, TotalSeats, AvailableSeats, Price) VALUES (?, ?, ?, ?, ?, ?, ?);";
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) return false;

    sqlite3_bind_text(stmt, 1, flight.flightNumber.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, flight.origin.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, flight.destination.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, flight.departureTime.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 5, flight.totalSeats);
    sqlite3_bind_int(stmt, 6, flight.availableSeats);
    sqlite3_bind_double(stmt, 7, flight.price);

    bool success = (sqlite3_step(stmt) == SQLITE_DONE);
    sqlite3_finalize(stmt);
    return success;
}

std::vector<Flight> SqliteDatabaseManager::searchFlights(const std::string& origin, const std::string& destination) {
    std::vector<Flight> flights;
    sqlite3_stmt* stmt;
    const char* sql = "SELECT * FROM Flights WHERE Origin = ? AND Destination = ? AND AvailableSeats > 0;";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) return flights;
    sqlite3_bind_text(stmt, 1, origin.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, destination.c_str(), -1, SQLITE_STATIC);
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        flights.emplace_back(Flight{
            sqlite3_column_int(stmt, 0),
            (const char*)sqlite3_column_text(stmt, 1),
            (const char*)sqlite3_column_text(stmt, 2),
            (const char*)sqlite3_column_text(stmt, 3),
            (const char*)sqlite3_column_text(stmt, 4),
            sqlite3_column_int(stmt, 5),
            sqlite3_column_int(stmt, 6),
            sqlite3_column_double(stmt, 7)
        });
    }
    sqlite3_finalize(stmt);
    return flights;
}

std::optional<Flight> SqliteDatabaseManager::getFlightById(int flightId) {
    sqlite3_stmt* stmt;
    const char* sql = "SELECT * FROM Flights WHERE ID = ?;";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) return std::nullopt;
    sqlite3_bind_int(stmt, 1, flightId);
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        Flight flight = {
            sqlite3_column_int(stmt, 0),
            (const char*)sqlite3_column_text(stmt, 1),
            (const char*)sqlite3_column_text(stmt, 2),
            (const char*)sqlite3_column_text(stmt, 3),
            (const char*)sqlite3_column_text(stmt, 4),
            sqlite3_column_int(stmt, 5),
            sqlite3_column_int(stmt, 6),
            sqlite3_column_double(stmt, 7)
        };
        sqlite3_finalize(stmt);
        return flight;
    }
    sqlite3_finalize(stmt);
    return std::nullopt;
}

std::vector<Flight> SqliteDatabaseManager::getAllFlights() {
    std::vector<Flight> flights;
    sqlite3_stmt* stmt;
    const char* sql = "SELECT * FROM Flights;";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) return flights;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        flights.emplace_back(Flight{
            sqlite3_column_int(stmt, 0),
            (const char*)sqlite3_column_text(stmt, 1),
            (const char*)sqlite3_column_text(stmt, 2),
            (const char*)sqlite3_column_text(stmt, 3),
            (const char*)sqlite3_column_text(stmt, 4),
            sqlite3_column_int(stmt, 5),
            sqlite3_column_int(stmt, 6),
            sqlite3_column_double(stmt, 7)
        });
    }
    sqlite3_finalize(stmt);
    return flights;
}

bool SqliteDatabaseManager::updateFlightSeatCount(int flightId, int change) {
    sqlite3_stmt* stmt;
    const char* sql = "UPDATE Flights SET AvailableSeats = AvailableSeats + ? WHERE ID = ?;";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) return false;
    sqlite3_bind_int(stmt, 1, change);
    sqlite3_bind_int(stmt, 2, flightId);
    bool success = (sqlite3_step(stmt) == SQLITE_DONE);
    sqlite3_finalize(stmt);
    return success;
}

std::optional<int> SqliteDatabaseManager::addBooking(int flightId, const std::string& passengerName, const std::string& passengerEmail) {
    sqlite3_stmt* stmt;
    const char* sql = "INSERT INTO Bookings (FlightID, PassengerName, PassengerEmail) VALUES (?, ?, ?);";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) return std::nullopt;
    
    sqlite3_bind_int(stmt, 1, flightId);
    sqlite3_bind_text(stmt, 2, passengerName.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, passengerEmail.c_str(), -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        sqlite3_finalize(stmt);
        return std::nullopt;
    }
    int bookingId = sqlite3_last_insert_rowid(db);
    sqlite3_finalize(stmt);
    return bookingId;
}

std::optional<Booking> SqliteDatabaseManager::getBookingById(int bookingId) {
    sqlite3_stmt* stmt;
    const char* sql = "SELECT b.ID, b.FlightID, b.PassengerName, b.PassengerEmail, f.FlightNumber, f.Origin, f.Destination, f.DepartureTime FROM Bookings b JOIN Flights f ON b.FlightID = f.ID WHERE b.ID = ?;";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) return std::nullopt;
    sqlite3_bind_int(stmt, 1, bookingId);
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        Booking booking = {
            sqlite3_column_int(stmt, 0),
            sqlite3_column_int(stmt, 1),
            (const char*)sqlite3_column_text(stmt, 2),
            (const char*)sqlite3_column_text(stmt, 3),
            (const char*)sqlite3_column_text(stmt, 4),
            (const char*)sqlite3_column_text(stmt, 5),
            (const char*)sqlite3_column_text(stmt, 6),
            (const char*)sqlite3_column_text(stmt, 7)
        };
        sqlite3_finalize(stmt);
        return booking;
    }
    sqlite3_finalize(stmt);
    return std::nullopt;
}

bool SqliteDatabaseManager::deleteBooking(int bookingId) {
    sqlite3_stmt* stmt;
    const char* sql = "DELETE FROM Bookings WHERE ID = ?;";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) return false;
    sqlite3_bind_int(stmt, 1, bookingId);
    bool success = (sqlite3_step(stmt) == SQLITE_DONE);
    sqlite3_finalize(stmt);
    return success;
}

std::vector<Booking> SqliteDatabaseManager::getBookingsForPassenger(const std::string& passengerEmail) {
    std::vector<Booking> bookings;
    sqlite3_stmt* stmt;
    const char* sql = "SELECT b.ID, b.FlightID, b.PassengerName, b.PassengerEmail, f.FlightNumber, f.Origin, f.Destination, f.DepartureTime FROM Bookings b JOIN Flights f ON b.FlightID = f.ID WHERE b.PassengerEmail = ?;";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) return bookings;
    sqlite3_bind_text(stmt, 1, passengerEmail.c_str(), -1, SQLITE_STATIC);
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        bookings.emplace_back(Booking{
            sqlite3_column_int(stmt, 0),
            sqlite3_column_int(stmt, 1),
            (const char*)sqlite3_column_text(stmt, 2),
            (const char*)sqlite3_column_text(stmt, 3),
            (const char*)sqlite3_column_text(stmt, 4),
            (const char*)sqlite3_column_text(stmt, 5),
            (const char*)sqlite3_column_text(stmt, 6),
            (const char*)sqlite3_column_text(stmt, 7)
        });
    }
    sqlite3_finalize(stmt);
    return bookings;
}

bool SqliteDatabaseManager::beginTransaction() {
    return execute("BEGIN TRANSACTION;");
}

bool SqliteDatabaseManager::commitTransaction() {
    return execute("COMMIT;");
}

bool SqliteDatabaseManager::rollbackTransaction() {
    return execute("ROLLBACK;");
} 