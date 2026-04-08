#pragma once

namespace ElevatorSim {

// Represents the physical constraints of the building.
// Phase 1: single shaft, fixed floor count.
// Future phases: multiple shafts, zone restrictions, lobby logic, etc.
class IBuilding {
public:
    virtual ~IBuilding() = default;

    virtual int getNumFloors()   const = 0;
    virtual int getGroundFloor() const = 0;
    virtual int getTopFloor()    const = 0;
};

} // namespace ElevatorSim
