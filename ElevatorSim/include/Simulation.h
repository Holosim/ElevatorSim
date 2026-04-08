#pragma once
#include <memory>
#include "Types.h"
#include "interfaces/IElevatorObserver.h"

namespace ElevatorSim {

// Forward-declare interface types so concrete headers stay out of this
// header.  The destructor is declared here and defined in Simulation.cpp
// where all complete types are visible (required for unique_ptr deletion).
class ISimulationClock;
class IElevator;
class IPerson;
class IBuilding;
class ISimulationLogger;

// Top-level orchestrator.
//
// Responsibilities:
//   - Owns all subsystem instances via interfaces (composition).
//   - Runs the discrete-time update loop.
//   - Implements IElevatorObserver to react to elevator state changes.
//   - Decides when to schedule new trips and when to terminate.
//
// The Simulation never depends on concrete types — only on interfaces —
// making each subsystem independently replaceable.
class Simulation final : public IElevatorObserver {
public:
    Simulation(
        const SimConfig&                  config,
        std::unique_ptr<ISimulationClock> clock,
        std::unique_ptr<IElevator>        elevator,
        std::unique_ptr<IPerson>          person,
        std::unique_ptr<IBuilding>        building,
        std::unique_ptr<ISimulationLogger> logger
    );
    ~Simulation();  // defined in Simulation.cpp

    void run();

    // IElevatorObserver — called from within elevator->update()
    void onElevatorArrivedAt(int floor)               override;
    void onDoorsOpened      (int floor)               override;
    void onDoorsClosed      (int floor)               override;
    void onElevatorMoved    (int fromFloor, int toFloor) override;

private:
    SimConfig                          config;
    std::unique_ptr<ISimulationClock>  clock;
    std::unique_ptr<IElevator>         elevator;
    std::unique_ptr<IPerson>           person;
    std::unique_ptr<IBuilding>         building;
    std::unique_ptr<ISimulationLogger> logger;
    int                                tripsCompleted;

    void scheduleNextTrip();
    int  getNextDestination() const;
};

} // namespace ElevatorSim
