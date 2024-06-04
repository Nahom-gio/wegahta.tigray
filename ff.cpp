#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

const int MAX_FLIGHTS = 10;
const int MAX_PASSENGERS = 100;
const int MAX_USERS = 50;
const int MAX_BOOKINGS_PER_USER = 10;
// Flight Information
struct Flight {
    std::string flightNumber;
    std::string destination;
    int totalSeats;
    int bookedSeats;
};

// Passenger Information
struct Passenger {
    std::string name;
    std::string flightNumber;
    int age;
    std::string gender;
    std::string contactNumber;
};



Flight flights[MAX_FLIGHTS];
Passenger passengers[MAX_PASSENGERS];
int flightCount = 0;
int passengerCount = 0;

void loadFlightsFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Error: Unable to open file " << filename << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line) && flightCount < MAX_FLIGHTS) {
        std::istringstream iss(line);
        Flight flight;
        if (!(iss >> flight.flightNumber >> flight.destination >> flight.totalSeats)) {
            std::cerr << "Error: Invalid format in file" << std::endl;
            continue;
        }
        flight.bookedSeats = 0;
        flights[flightCount++] = flight;
    }
    file.close();
}

void savePassengersToFile(const std::string& filename) {
    std::ofstream file(filename);
    if (!file) {
        std::cerr << "Error: Unable to open file " << filename << std::endl;
        return;
    }

    for (int i = 0; i < passengerCount; ++i) {
        file << passengers[i].name << " "
             << passengers[i].flightNumber << " "
             << passengers[i].age << " "
             << passengers[i].gender << " "
             << passengers[i].contactNumber << std::endl;
    }
    file.close();
}

void displayAvailableFlights() {
    for (int i = 0; i < flightCount; ++i) {
        std::cout << "Flight Number: " << flights[i].flightNumber
                  << ", Destination: " << flights[i].destination
                  << ", Available Seats: " << flights[i].totalSeats - flights[i].bookedSeats << std::endl;
    }
}

void bookSeat() {
    if (passengerCount >= MAX_PASSENGERS) {
        std::cout << "Cannot add more passengers, maximum limit reached." << std::endl;
        return;
    }

    std::string flightNumber;
    std::cout << "Enter flight number to book: ";
    std::cin >> flightNumber;

    bool flightFound = false;
    for (int i = 0; i < flightCount; ++i) {
        if (flights[i].flightNumber == flightNumber) {
            flightFound = true;
            if (flights[i].bookedSeats < flights[i].totalSeats) {
                std::cout << "Enter passenger name: ";
                std::cin >> passengers[passengerCount].name;
                std::cout << "Enter passenger age: ";
                std::cin >> passengers[passengerCount].age;
                std::cout << "Enter passenger gender: ";
                std::cin >> passengers[passengerCount].gender;
                std::cout << "Enter passenger phone number: ";
                std::cin >> passengers[passengerCount].contactNumber;
                passengers[passengerCount].flightNumber = flightNumber;

                flights[i].bookedSeats++;
                passengerCount++;
                std::cout << "Booking successful for " << passengers[passengerCount - 1].name << std::endl;
                savePassengersToFile("passengers.txt");
            } else {
                std::cout << "No available seats on flight " << flightNumber << std::endl;
            }
            break;
        }
    }

    if (!flightFound) {
        std::cout << "Flight not found." << std::endl;
    }
}

void displayPassengers() {
    for (int i = 0; i < passengerCount; ++i) {
        std::cout << "Passenger Name: " << passengers[i].name
                  << ", Flight Number: " << passengers[i].flightNumber
                  << ", Age: " << passengers[i].age
                  << ", Gender: " << passengers[i].gender
                  << ", phone Number: " << passengers[i].contactNumber << std::endl;
    }
}

void loadPassengersFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Error: Unable to open file " << filename << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line) && passengerCount < MAX_PASSENGERS) {
        std::istringstream iss(line);
        Passenger passenger;
        if (!(iss >> passenger.name >> passenger.flightNumber >> passenger.age >> passenger.gender >> passenger.contactNumber)) {
            std::cerr << "Error: Invalid format in file" << std::endl;
            continue;
        }
        passengers[passengerCount++] = passenger;
    }
    file.close();
}

void modifyBooking() {
    std::string name;
    std::cout << "Enter the name of the passenger to modify: ";
    std::cin >> name;

    bool passengerFound = false;
    for (int i = 0; i < passengerCount; ++i) {
        if (passengers[i].name == name) {
            passengerFound = true;
            std::cout << "Enter new flight number: ";
            std::cin >> passengers[i].flightNumber;
            std::cout << "Enter new age: ";
            std::cin >> passengers[i].age;
            std::cout << "Enter new gender: ";
            std::cin >> passengers[i].gender;
            std::cout << "Enter new phone number: ";
            std::cin >> passengers[i].contactNumber;

            std::cout << "Modification successful for " << passengers[i].name << std::endl;
            savePassengersToFile("passengers.txt");
            break;
        }
    }

    if (!passengerFound) {
        std::cout << "Passenger not found." << std::endl;
    }
}

void cancelBooking() {
    std::string name;
    std::cout << "Enter the name of the passenger to cancel: ";
    std::cin >> name;

    bool passengerFound = false;
    for (int i = 0; i < passengerCount; ++i) {
        if (passengers[i].name == name) {
            passengerFound = true;
            std::string flightNumber = passengers[i].flightNumber;

            for (int j = i; j < passengerCount - 1; ++j) {
                passengers[j] = passengers[j + 1];
            }
            passengerCount--;

            for (int j = 0; j < flightCount; ++j) {
                if (flights[j].flightNumber == flightNumber) {
                    flights[j].bookedSeats--;
                    break;
                }
            }

            std::cout << "Cancellation successful for " << name << std::endl;
            savePassengersToFile("passengers.txt");
            break;
        }
    }

    if (!passengerFound) {
        std::cout << "Passenger not found." << std::endl;
    }
}

int main() {
    loadFlightsFromFile("flights.txt");
    loadPassengersFromFile("passengers.txt");

    int choice;
    do {
        std::cout << "\nFlight Reservation System\n";
        std::cout << "1. Display Available Flights\n";
        std::cout << "2. Book Seat\n";
        std::cout << "3. Display Passengers\n";
        std::cout << "4. Modify Booking\n";
        std::cout << "5. Cancel Booking\n";
        std::cout << "6. Exit\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        switch (choice) {
            case 1:
                displayAvailableFlights();
                break;
            case 2:
                bookSeat();
                break;
            case 3:
                displayPassengers();
                break;
            case 4:
                modifyBooking();
                break;
            case 5:
                cancelBooking();
                break;
            case 6:
                std::cout << "Exiting..." << std::endl;
                break;
            default:
                std::cout << "Invalid choice. Please try again." << std::endl;
        }
    } while (choice != 6);

    return 0;
}
