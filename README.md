# AirBooker - Flight Reservation System

A full-stack, console-based Flight Reservation System in C++ with a modular, loosely-coupled architecture.

## Project Structure

```
flightreservationsystem/
├── Makefile                    # Build configuration
├── README.md                   # This file
├── src/
│   ├── main.cpp               # Application entry point
│   ├── core/
│   │   └── models.h           # Core data structures (Flight, Booking)
│   ├── dal/                   # Data Access Layer
│   │   ├── IDatabaseManager.h # Abstract database interface
│   │   ├── SqliteDatabaseManager.h
│   │   └── SqliteDatabaseManager.cpp
│   ├── bll/                   # Business Logic Layer
│   │   ├── ReservationService.h
│   │   └── ReservationService.cpp
│   ├── ui/                    # User Interface Layer
│   │   ├── ConsoleUI.h
│   │   └── ConsoleUI.cpp
│   └── utils/                 # Utility Functions
│       ├── helpers.h
│       └── helpers.cpp
```

## Architecture Overview

This project follows a **3-Layer Architecture** with **Dependency Injection**:

### 1. Data Access Layer (DAL)
- **`IDatabaseManager.h`**: Abstract interface defining database operations
- **`SqliteDatabaseManager.h/.cpp`**: Concrete SQLite implementation
- Provides database independence through interface abstraction

### 2. Business Logic Layer (BLL)
- **`ReservationService.h/.cpp`**: Core business logic and transaction management
- Completely decoupled from UI and database implementation
- Handles complex operations like booking with seat validation

### 3. User Interface Layer (UI)
- **`ConsoleUI.h/.cpp`**: Console-based user interface
- Only interacts with the business logic layer
- No direct database access

## Features

### Admin Functions
- Add new flights with details (flight number, origin, destination, departure time, seats, price)
- View all scheduled flights

### Passenger Functions
- Search available flights by origin and destination
- Book flights with passenger information
- View personal bookings by email
- Cancel bookings

### Technical Features
- **SQLite Database**: Persistent data storage
- **Transaction Management**: ACID compliance for booking operations
- **Input Validation**: Robust error handling
- **Modular Design**: Easy to extend and maintain

## Prerequisites

- C++17 compatible compiler (GCC 7+, Clang 5+, MSVC 2017+)
- SQLite3 development libraries
- Make (for building)

## Installation

### Ubuntu/Debian
```bash
sudo apt-get update
sudo apt-get install -y g++ sqlite3 libsqlite3-dev make
```

### macOS
```bash
brew install sqlite3
```

### Windows (MSYS2)
```bash
pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-sqlite3 make
```

## Building and Running

1. **Clone or download the project**
2. **Navigate to the project directory**
   ```bash
   cd flightreservationsystem
   ```

3. **Build the project**
   ```bash
   make
   ```

4. **Run the application**
   ```bash
   ./flight_system
   ```

## Usage

### Main Menu
- **1. Admin Menu**: For airline staff to manage flights
- **2. Passenger Menu**: For passengers to book and manage reservations
- **3. Exit**: Close the application

### Admin Menu
- **Add New Flight**: Create new flight schedules
- **View All Flights**: Display all scheduled flights

### Passenger Menu
- **Search & Book a Flight**: Find and reserve seats
- **View My Bookings**: Check existing reservations
- **Cancel a Booking**: Cancel existing reservations

## Database Schema

### Flights Table
```sql
CREATE TABLE Flights (
    ID INTEGER PRIMARY KEY AUTOINCREMENT,
    FlightNumber TEXT NOT NULL UNIQUE,
    Origin TEXT NOT NULL,
    Destination TEXT NOT NULL,
    DepartureTime TEXT NOT NULL,
    TotalSeats INTEGER NOT NULL,
    AvailableSeats INTEGER NOT NULL,
    Price REAL NOT NULL
);
```

### Bookings Table
```sql
CREATE TABLE Bookings (
    ID INTEGER PRIMARY KEY AUTOINCREMENT,
    FlightID INTEGER NOT NULL,
    PassengerName TEXT NOT NULL,
    PassengerEmail TEXT NOT NULL,
    FOREIGN KEY(FlightID) REFERENCES Flights(ID)
);
```

## Design Patterns Used

1. **Dependency Injection**: Service layer accepts database interface
2. **Strategy Pattern**: Database implementation can be swapped
3. **Repository Pattern**: Data access abstraction
4. **Service Layer Pattern**: Business logic encapsulation
5. **MVC-like Structure**: Separation of concerns

## Extensibility

The modular design makes it easy to:

- **Add new database backends**: Implement `IDatabaseManager` interface
- **Add new UI types**: Create new UI classes that use `ReservationService`
- **Add new business logic**: Extend `ReservationService` with new methods
- **Add new data models**: Extend `models.h` with new structures

## Error Handling

- **Database errors**: Caught and reported with meaningful messages
- **Input validation**: Robust checking of user input
- **Transaction rollback**: Automatic rollback on booking failures
- **Exception safety**: RAII principles for resource management

## Contributing

1. Follow the existing code structure and naming conventions
2. Add appropriate error handling for new features
3. Update the Makefile if adding new source files
4. Test thoroughly before submitting changes

## License

This project is provided as educational material for learning C++ and software architecture principles. 