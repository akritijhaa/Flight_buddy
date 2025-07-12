# Makefile for the Flight Reservation System

# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I./src -I./src/core -I./src/dal -I./src/bll -I./src/ui -I./src/utils
LDFLAGS = -lsqlite3

# Source files
SRCS = src/main.cpp \
       src/dal/SqliteDatabaseManager.cpp \
       src/bll/ReservationService.cpp \
       src/ui/ConsoleUI.cpp \
       src/utils/helpers.cpp

# Object files
OBJS = $(SRCS:.cpp=.o)

# Executable name
TARGET = flight_system

# Default target
all: $(TARGET)

# Link the executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS)

# Compile source files to object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up build files
clean:
	rm -f $(OBJS) $(TARGET)

# Install dependencies (for Ubuntu/Debian)
install-deps:
	sudo apt-get update
	sudo apt-get install -y g++ sqlite3 libsqlite3-dev

# Install dependencies (for macOS)
install-deps-mac:
	brew install sqlite3

# Install dependencies (for Windows with MSYS2)
install-deps-windows:
	pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-sqlite3

.PHONY: all clean install-deps install-deps-mac install-deps-windows 