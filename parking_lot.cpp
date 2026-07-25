#include <bits/stdc++.h>
using namespace std;

// ---------- Enum ----------
enum class VehicleType {
    BIKE,
    CAR,
    TRUCK
};

string vehicleTypeToString(VehicleType type) {
    switch (type) {
        case VehicleType::BIKE:  return "BIKE";
        case VehicleType::CAR:   return "CAR";
        case VehicleType::TRUCK: return "TRUCK";
    }
    return "UNKNOWN";
}

// ---------- Vehicle (Abstract) ----------
class Vehicle {
protected:
    string registrationNo;
    VehicleType type;

public:
    Vehicle(string regNo, VehicleType type) {
        this->registrationNo = regNo;
        this->type = type;
    }

    virtual ~Vehicle() {}

    string getRegistrationNo() { return registrationNo; }
    VehicleType getType() { return type; }

    virtual void displayInfo() {
        cout << "Vehicle [" << registrationNo << "] Type: "
             << vehicleTypeToString(type) << endl;
    }
};

class Bike : public Vehicle {
public:
    Bike(string regNo) : Vehicle(regNo, VehicleType::BIKE) {}
};

class Car : public Vehicle {
public:
    Car(string regNo) : Vehicle(regNo, VehicleType::CAR) {}
};

class Truck : public Vehicle {
public:
    Truck(string regNo) : Vehicle(regNo, VehicleType::TRUCK) {}
};

// ---------- ParkingSpot (Abstract) ----------
class ParkingSpot {
protected:
    int spotId;
    bool occupied;
    Vehicle *vehicle;
    VehicleType type;

public:
    ParkingSpot(int spotId, VehicleType type) {
        this->spotId = spotId;
        this->type = type;
        this->occupied = false;
        this->vehicle = nullptr;
    }

    virtual ~ParkingSpot() {}

    bool parkVehicle(Vehicle *v) {
        if (occupied) return false;
        vehicle = v;
        occupied = true;
        return true;
    }

    bool removeVehicle() {
        if (!occupied) return false;
        vehicle = nullptr;
        occupied = false;
        return true;
    }

    bool isOccupied() { return occupied; }
    VehicleType getType() { return type; }
    int getSpotId() { return spotId; }
    Vehicle* getVehicle() { return vehicle; }
};

class BikeSpot : public ParkingSpot {
public:
    BikeSpot(int spotId) : ParkingSpot(spotId, VehicleType::BIKE) {}
};

class CarSpot : public ParkingSpot {
public:
    CarSpot(int spotId) : ParkingSpot(spotId, VehicleType::CAR) {}
};

class TruckSpot : public ParkingSpot {
public:
    TruckSpot(int spotId) : ParkingSpot(spotId, VehicleType::TRUCK) {}
};

// ---------- ParkingSystem ----------
class ParkingSystem {
private:
    vector<BikeSpot*> bikeSpots;
    vector<CarSpot*> carSpots;
    vector<TruckSpot*> truckSpots;
    unordered_map<string, ParkingSpot*> vehicleMap; // regNo -> spot

public:
    ParkingSystem(int numBike, int numCar, int numTruck) {
        for (int i = 0; i < numBike; i++)
            bikeSpots.push_back(new BikeSpot(i + 1));
        for (int i = 0; i < numCar; i++)
            carSpots.push_back(new CarSpot(i + 1));
        for (int i = 0; i < numTruck; i++)
            truckSpots.push_back(new TruckSpot(i + 1));
    }

    ParkingSpot* findSpot(Vehicle *v) {
        VehicleType type = v->getType();
        if (type == VehicleType::BIKE) {
            for (auto *s : bikeSpots) if (!s->isOccupied()) return s;
        } else if (type == VehicleType::CAR) {
            for (auto *s : carSpots) if (!s->isOccupied()) return s;
        } else if (type == VehicleType::TRUCK) {
            for (auto *s : truckSpots) if (!s->isOccupied()) return s;
        }
        return nullptr;
    }

    bool parkVehicle(Vehicle *v) {
        if (vehicleMap.count(v->getRegistrationNo())) {
            cout << "Vehicle " << v->getRegistrationNo() << " is already parked.\n";
            return false;
        }

        ParkingSpot *spot = findSpot(v);
        if (!spot) {
            cout << "No available spot for " << vehicleTypeToString(v->getType()) << endl;
            return false;
        }

        spot->parkVehicle(v);
        vehicleMap[v->getRegistrationNo()] = spot;
        cout << v->getRegistrationNo() << " parked at "
             << vehicleTypeToString(spot->getType()) << " spot #" << spot->getSpotId() << endl;
        return true;
    }

    bool removeVehicle(string regNo) {
        if (!vehicleMap.count(regNo)) {
            cout << "Vehicle " << regNo << " not found.\n";
            return false;
        }
        ParkingSpot *spot = vehicleMap[regNo];
        spot->removeVehicle();
        vehicleMap.erase(regNo);
        cout << regNo << " removed from "
             << vehicleTypeToString(spot->getType()) << " spot #" << spot->getSpotId() << endl;
        return true;
    }

    void displayAvailableSpots() {
        int bikeFree = 0, carFree = 0, truckFree = 0;
        for (auto *s : bikeSpots) if (!s->isOccupied()) bikeFree++;
        for (auto *s : carSpots) if (!s->isOccupied()) carFree++;
        for (auto *s : truckSpots) if (!s->isOccupied()) truckFree++;

        cout << "Available -> Bike: " << bikeFree
             << " Car: " << carFree
             << " Truck: " << truckFree << endl;
    }

    void displayOccupiedSpots() {
        cout << "Occupied spots:\n";
        for (auto &entry : vehicleMap) {
            ParkingSpot *spot = entry.second;
            cout << "  " << entry.first << " -> "
                 << vehicleTypeToString(spot->getType())
                 << " spot #" << spot->getSpotId() << endl;
        }
    }

    ~ParkingSystem() {
        for (auto *s : bikeSpots) delete s;
        for (auto *s : carSpots) delete s;
        for (auto *s : truckSpots) delete s;
    }
};

// ---------- main ----------
int main() {
    ParkingSystem system(2, 2, 1); // 2 bike spots, 2 car spots, 1 truck spot

    Vehicle *b1 = new Bike("BIKE-001");
    Vehicle *c1 = new Car("CAR-001");
    Vehicle *c2 = new Car("CAR-002");
    Vehicle *c3 = new Car("CAR-003"); // should fail, no car spots left
    Vehicle *t1 = new Truck("TRUCK-001");

    system.parkVehicle(b1);
    system.parkVehicle(c1);
    system.parkVehicle(c2);
    system.parkVehicle(c3);
    system.parkVehicle(t1);

    system.displayAvailableSpots();
    system.displayOccupiedSpots();

    system.removeVehicle("CAR-001");
    system.parkVehicle(c3); // now succeeds

    system.displayAvailableSpots();
    system.displayOccupiedSpots();

    delete b1; delete c1; delete c2; delete c3; delete t1;
    return 0;
}