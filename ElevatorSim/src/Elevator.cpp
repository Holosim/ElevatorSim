#include "Elevator.h"

namespace ElevatorSim {

Elevator::Elevator(int startFloor, int doorOpenDuration)
    : currentFloor(startFloor),
      targetFloor(startFloor),
      doorOpenDuration(doorOpenDuration),
      doorsOpenedAt(0),
      state(ElevatorState::IDLE),
      direction(Direction::NONE),
      observer(nullptr)
{}

int           Elevator::getFloor()     const { return currentFloor; }
ElevatorState Elevator::getState()     const { return state; }
Direction     Elevator::getDirection() const { return direction; }
bool          Elevator::hasStops()     const { return !stopQueue.empty(); }

void Elevator::addStop(int floor)
{
    stopQueue.push(floor);
}

void Elevator::setObserver(IElevatorObserver* obs)
{
    observer = obs;
}

void Elevator::update(uint64_t tick)
{
    switch (state)
    {
        case ElevatorState::IDLE:
            processNextStop(tick);
            break;

        case ElevatorState::MOVING:
        {
            int prevFloor = currentFloor;
            if (direction == Direction::UP)  ++currentFloor;
            else                             --currentFloor;

            if (observer) observer->onElevatorMoved(prevFloor, currentFloor);

            if (currentFloor == targetFloor) {
                if (observer) observer->onElevatorArrivedAt(currentFloor);
                openDoors(tick);
            }
            break;
        }

        case ElevatorState::DOORS_OPEN:
            if (tick - doorsOpenedAt >= static_cast<uint64_t>(doorOpenDuration)) {
                state = ElevatorState::IDLE;
                if (!stopQueue.empty()) stopQueue.pop();
                if (observer) observer->onDoorsClosed(currentFloor);
            }
            break;
    }
}

// ---------------------------------------------------------------------------
// Private helpers
// ---------------------------------------------------------------------------

void Elevator::openDoors(uint64_t tick)
{
    state        = ElevatorState::DOORS_OPEN;
    doorsOpenedAt = tick;
    direction    = Direction::NONE;
    if (observer) observer->onDoorsOpened(currentFloor);
}

void Elevator::processNextStop(uint64_t tick)
{
    if (stopQueue.empty()) return;

    targetFloor = stopQueue.front();

    if (targetFloor == currentFloor) {
        if (observer) observer->onElevatorArrivedAt(currentFloor);
        openDoors(tick);
    } else {
        direction = (targetFloor > currentFloor) ? Direction::UP : Direction::DOWN;
        state     = ElevatorState::MOVING;
    }
}

} // namespace ElevatorSim
