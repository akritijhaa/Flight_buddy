#include "ReservationService.h"
#include <iostream>

ReservationService::ReservationService(std::unique_ptr<IDatabaseManager> dbManager)
    : db(std::move(dbManager)) {}

bool ReservationService::addNewFlight(const Flight& flight) {
    return db->addFlight(flight);
}

std::vector<Flight> ReservationService::getAllFlights() {
    return db->getAllFlights();
}

std::vector<Flight> ReservationService::findAvailableFlights(const std::string& origin, const std::string& destination) {
    return db->searchFlights(origin, destination);
}

std::optional<int> ReservationService::bookFlight(int flightId, const std::string& passengerName, const std::string& passengerEmail) {
    // Transactional logic is now in the service layer, where it belongs.
    if (!db->beginTransaction()) return std::nullopt;

    // 1. Check for available seats
    auto flightOpt = db->getFlightById(flightId);
    if (!flightOpt || flightOpt->availableSeats <= 0) {
        std::cerr << "Booking failed: No available seats or flight not found." << std::endl;
        db->rollbackTransaction();
        return std::nullopt;
    }

    // 2. Create the booking record
    auto bookingIdOpt = db->addBooking(flightId, passengerName, passengerEmail);
    if (!bookingIdOpt) {
        db->rollbackTransaction();
        return std::nullopt;
    }
    
    // 3. Update the flight's seat count
    if (!db->updateFlightSeatCount(flightId, -1)) {
        db->rollbackTransaction();
        return std::nullopt;
    }

    if (!db->commitTransaction()) return std::nullopt;

    return bookingIdOpt;
}

bool ReservationService::cancelBooking(int bookingId) {
    if (!db->beginTransaction()) return false;

    // 1. Get booking to find the flight ID
    auto bookingOpt = db->getBookingById(bookingId);
    if (!bookingOpt) {
        std::cerr << "Cancellation failed: Booking ID not found." << std::endl;
        db->rollbackTransaction();
        return false;
    }

    // 2. Delete the booking
    if (!db->deleteBooking(bookingId)) {
        db->rollbackTransaction();
        return false;
    }

    // 3. Increment the flight's seat count
    if (!db->updateFlightSeatCount(bookingOpt->flightId, +1)) {
        db->rollbackTransaction();
        return false;
    }

    return db->commitTransaction();
}

std::vector<Booking> ReservationService::findMyBookings(const std::string& passengerEmail) {
    return db->getBookingsForPassenger(passengerEmail);
} 