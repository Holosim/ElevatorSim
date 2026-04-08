#include <memory>
#include <iostream>
#include "Types.h"
#include "SimulationClock.h"
#include "Elevator.h"
#include "Person.h"
#include "Building.h"
#include "SimulationLogger.h"
#include "Simulation.h"

int main()
{
    using namespace ElevatorSim;

    // -----------------------------------------------------------------------
    // Configure the simulation
    // -----------------------------------------------------------------------
    SimConfig config;
    config.numFloors        = 10;   // floors 1..10
    config.startFloor       = 1;    // elevator and person both start on floor 1
    config.floorsPerTick    = 1;    // elevator moves 1 floor per tick
    config.doorOpenDuration = 3;    // doors stay open 3 ticks
    config.numTrips         = 6;    // 3 up + 3 down
    config.verboseLogging   = true;

    // -----------------------------------------------------------------------
    // Compose subsystems through their interfaces
    // -----------------------------------------------------------------------
    auto clock    = std::make_unique<SimulationClock>();
    auto elevator = std::make_unique<Elevator>(config.startFloor, config.doorOpenDuration);
    auto person   = std::make_unique<Person>(config.startFloor);
    auto building = std::make_unique<Building>(config.numFloors);
    auto logger   = std::make_unique<SimulationLogger>(config.verboseLogging);

    Simulation simulationThread(
        config,
        std::move(clock),
        std::move(elevator),
        std::move(person),
        std::move(building),
        std::move(logger)
    );

    // -----------------------------------------------------------------------
    // Run
    // -----------------------------------------------------------------------
    simulationThread.run();
    return 0;
}
