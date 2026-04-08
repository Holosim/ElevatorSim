#pragma once
#include "interfaces/IPerson.h"

namespace ElevatorSim {

// Phase-1 person: passive state model driven entirely by the Simulation.
class Person final : public IPerson {
public:
    explicit Person(int startFloor);

    int         getFloor()            const override;
    PersonState getState()            const override;
    int         getDestinationFloor() const override;
    Direction   getTravelDirection()  const override;

    void requestTrip(int destinationFloor, uint64_t tick) override;
    void board      (uint64_t tick)                       override;
    void exit       (int floor, uint64_t tick)            override;

    const TripRecord& getLastTripRecord() const override;

private:
    int         currentFloor;
    int         destinationFloor;
    PersonState state;
    TripRecord  activeTrip;
    TripRecord  lastTrip;
};

} // namespace ElevatorSim
