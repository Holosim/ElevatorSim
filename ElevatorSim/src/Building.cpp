#include "Building.h"

namespace ElevatorSim {

Building::Building(int numFloors) : numFloors(numFloors) {}

int Building::getNumFloors()   const { return numFloors; }
int Building::getGroundFloor() const { return 1; }
int Building::getTopFloor()    const { return numFloors; }

} // namespace ElevatorSim
