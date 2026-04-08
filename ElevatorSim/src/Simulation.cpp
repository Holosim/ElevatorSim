#include "Simulation.h"
#include "interfaces/ISimulationClock.h"
#include "interfaces/IElevator.h"
#include "interfaces/IPerson.h"
#include "interfaces/IBuilding.h"
#include "interfaces/ISimulationLogger.h"
#include <iostream>
#include <string>

namespace ElevatorSim {

Simulation::Simulation(
    const SimConfig&                   config,
    std::unique_ptr<ISimulationClock>  clock,
    std::unique_ptr<IElevator>         elevator,
    std::unique_ptr<IPerson>           person,
    std::unique_ptr<IBuilding>         building,
    std::unique_ptr<ISimulationLogger> logger)
    : config(config),
      clock(std::move(clock)),
      elevator(std::move(elevator)),
      person(std::move(person)),
      building(std::move(building)),
      logger(std::move(logger)),
      tripsCompleted(0)
{
    this->elevator->setObserver(this);
}

Simulation::~Simulation() = default;

// ---------------------------------------------------------------------------
// Main loop
// ---------------------------------------------------------------------------

void Simulation::run()
{
    std::cout << "==============================================\n";
    std::cout << "         ELEVATOR SIMULATION — PHASE 1       \n";
    std::cout << "==============================================\n";
    std::cout << "  Building : " << building->getNumFloors() << " floors"
              << " (ground=" << building->getGroundFloor()
              << ", top=" << building->getTopFloor() << ")\n";
    std::cout << "  Trips    : " << config.numTrips << "\n";
    std::cout << "  Door dur : " << config.doorOpenDuration << " ticks\n";
    std::cout << "  Speed    : " << config.floorsPerTick << " floor(s)/tick\n";
    std::cout << "==============================================\n\n";

    if (config.numTrips > 0) {
        scheduleNextTrip();
    }

    constexpr uint64_t MAX_TICKS = 1'000'000;

    while (tripsCompleted < config.numTrips
           || person->getState()   != PersonState::IDLE
           || elevator->hasStops())
    {
        uint64_t tick = clock->getTick();

        if (tick >= MAX_TICKS) {
            std::cerr << "[ERROR] Simulation exceeded " << MAX_TICKS
                      << " ticks — aborting.\n";
            break;
        }

        elevator->update(tick);  // may invoke observer callbacks synchronously
        clock->advance();
    }

    logger->printSummary();
    std::cout << "\nSimulation ended at tick " << clock->getTick() << ".\n";
}

// ---------------------------------------------------------------------------
// Trip scheduling
// ---------------------------------------------------------------------------

void Simulation::scheduleNextTrip()
{
    int      dest = getNextDestination();
    int      from = person->getFloor();
    uint64_t tick = clock->getTick();

    person->requestTrip(dest, tick);

    // Phase-1 dispatch: pick up person first, then deliver to destination.
    elevator->addStop(from);
    elevator->addStop(dest);

    logger->logEvent(tick,
        "Trip scheduled: Floor " + std::to_string(from) +
        " -> Floor " + std::to_string(dest) +
        " (" + directionToString(person->getTravelDirection()) + ")");
}

int Simulation::getNextDestination() const
{
    // Simple shuttle: alternate between ground and top floor.
    return (person->getFloor() == building->getGroundFloor())
        ? building->getTopFloor()
        : building->getGroundFloor();
}

// ---------------------------------------------------------------------------
// IElevatorObserver callbacks
// ---------------------------------------------------------------------------

void Simulation::onElevatorArrivedAt(int floor)
{
    logger->logEvent(clock->getTick(),
        "Elevator arrived at floor " + std::to_string(floor));
}

void Simulation::onDoorsOpened(int floor)
{
    uint64_t tick = clock->getTick();
    logger->logEvent(tick, "Doors OPEN  @ floor " + std::to_string(floor));

    if (person->getState() == PersonState::WAITING && person->getFloor() == floor) {
        person->board(tick);
        logger->logEvent(tick, "  -> Person BOARDED at floor " + std::to_string(floor));
    }
    else if (person->getState() == PersonState::RIDING
             && person->getDestinationFloor() == floor)
    {
        person->exit(floor, tick);
        const TripRecord& trip = person->getLastTripRecord();
        logger->recordTrip(trip);
        logger->logEvent(tick,
            "  -> Person EXITED at floor " + std::to_string(floor) +
            " | wait=" + std::to_string(trip.waitTime()) +
            " ticks | ride=" + std::to_string(trip.rideTime()) + " ticks");
        ++tripsCompleted;
    }
}

void Simulation::onDoorsClosed(int floor)
{
    uint64_t tick = clock->getTick();
    logger->logEvent(tick, "Doors CLOSE @ floor " + std::to_string(floor));

    // Schedule the next trip as soon as the doors close after the last exit.
    if (person->getState() == PersonState::IDLE && tripsCompleted < config.numTrips) {
        scheduleNextTrip();
    }
}

void Simulation::onElevatorMoved(int fromFloor, int toFloor)
{
    logger->recordElevatorMove(fromFloor, toFloor);
    logger->logEvent(clock->getTick(),
        "Elevator: " + std::to_string(fromFloor) +
        " -> " + std::to_string(toFloor));
}

} // namespace ElevatorSim
