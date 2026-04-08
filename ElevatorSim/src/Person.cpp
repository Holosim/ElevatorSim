#include "Person.h"

namespace ElevatorSim {

Person::Person(int startFloor)
    : currentFloor(startFloor),
      destinationFloor(startFloor),
      state(PersonState::IDLE)
{}

int         Person::getFloor()            const { return currentFloor;      }
PersonState Person::getState()            const { return state;             }
int         Person::getDestinationFloor() const { return destinationFloor;  }

Direction Person::getTravelDirection() const
{
    if (destinationFloor > currentFloor) return Direction::UP;
    if (destinationFloor < currentFloor) return Direction::DOWN;
    return Direction::NONE;
}

const TripRecord& Person::getLastTripRecord() const
{
    return lastTrip;
}

void Person::requestTrip(int dest, uint64_t tick)
{
    destinationFloor = dest;
    state            = PersonState::WAITING;

    activeTrip                  = TripRecord{};
    activeTrip.requestTick      = tick;
    activeTrip.originFloor      = currentFloor;
    activeTrip.destinationFloor = dest;
    activeTrip.direction        = getTravelDirection();
}

void Person::board(uint64_t tick)
{
    state                = PersonState::RIDING;
    activeTrip.boardTick = tick;
}

void Person::exit(int floor, uint64_t tick)
{
    currentFloor        = floor;
    state               = PersonState::IDLE;
    activeTrip.exitTick = tick;
    lastTrip            = activeTrip;
}

} // namespace ElevatorSim
