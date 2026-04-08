#pragma once
#include <cstdint>
#include "Types.h"

namespace ElevatorSim {

// Contract for a building occupant.  State transitions are driven by the
// Simulation orchestrator so the person is a pure data/state model.
class IPerson {
public:
    virtual ~IPerson() = default;

    virtual int         getFloor()            const = 0;
    virtual PersonState getState()            const = 0;
    virtual int         getDestinationFloor() const = 0;
    virtual Direction   getTravelDirection()  const = 0;

    // Called by Simulation when a new one-way trip is initiated.
    virtual void requestTrip(int destinationFloor, uint64_t tick) = 0;

    // Called by Simulation when the elevator doors open at the person's floor.
    virtual void board(uint64_t tick) = 0;

    // Called by Simulation when the elevator doors open at the destination.
    virtual void exit(int floor, uint64_t tick) = 0;

    // The most recently completed TripRecord (valid after exit() returns).
    virtual const TripRecord& getLastTripRecord() const = 0;
};

} // namespace ElevatorSim
