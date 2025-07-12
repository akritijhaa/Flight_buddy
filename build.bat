@echo off
echo Building Flight Reservation System...

REM Check if g++ is available
where g++ >nul 2>nul
if %errorlevel% neq 0 (
    echo Error: g++ compiler not found. Please install MinGW or MSYS2.
    echo For MSYS2: pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-sqlite3
    pause
    exit /b 1
)

REM Create object files
echo Compiling source files...
g++ -std=c++17 -Wall -Wextra -I./src -I./src/core -I./src/dal -I./src/bll -I./src/ui -I./src/utils -c src/main.cpp -o src/main.o
g++ -std=c++17 -Wall -Wextra -I./src -I./src/core -I./src/dal -I./src/bll -I./src/ui -I./src/utils -c src/dal/SqliteDatabaseManager.cpp -o src/dal/SqliteDatabaseManager.o
g++ -std=c++17 -Wall -Wextra -I./src -I./src/core -I./src/dal -I./src/bll -I./src/ui -I./src/utils -c src/bll/ReservationService.cpp -o src/bll/ReservationService.o
g++ -std=c++17 -Wall -Wextra -I./src -I./src/core -I./src/dal -I./src/bll -I./src/ui -I./src/utils -c src/ui/ConsoleUI.cpp -o src/ui/ConsoleUI.o
g++ -std=c++17 -Wall -Wextra -I./src -I./src/core -I./src/dal -I./src/bll -I./src/ui -I./src/utils -c src/utils/helpers.cpp -o src/utils/helpers.o

REM Link the executable
echo Linking executable...
g++ -std=c++17 -Wall -Wextra -I./src -I./src/core -I./src/dal -I./src/bll -I./src/ui -I./src/utils -o flight_system.exe src/main.o src/dal/SqliteDatabaseManager.o src/bll/ReservationService.o src/ui/ConsoleUI.o src/utils/helpers.o -lsqlite3

if %errorlevel% equ 0 (
    echo Build successful! Run flight_system.exe to start the application.
) else (
    echo Build failed! Please check the error messages above.
)

pause 