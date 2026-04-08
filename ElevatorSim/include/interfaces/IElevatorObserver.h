#pragma once

namespace ElevatorSim {

// Observer interface for elevator state-change events.
// The Simulation implements this to react to elevator transitions without
// tightly coupling the elevator to any specific orchestrator.
class IElevatorObserver {
public:
    virtual ~IElevatorObserver() = default;

    virtual void onElevatorArrivedAt(int floor)               = 0;
    virtual void onDoorsOpened      (int floor)               = 0;
    virtual void onDoorsClosed      (int floor)               = 0;
    virtual void onElevatorMoved    (int fromFloor, int toFloor) = 0;
};

} // namespace ElevatorSim
