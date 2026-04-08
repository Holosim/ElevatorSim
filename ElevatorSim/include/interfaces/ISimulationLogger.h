#pragma once
#include <string>
#include <cstdint>
#include "Types.h"

namespace ElevatorSim {

// Separates observability concerns from simulation logic.
// Implementations can write to console, CSV, in-memory structures, etc.
class ISimulationLogger {
public:
    virtual ~ISimulationLogger() = default;

    // Human-readable timestamped event (verbose output).
    virtual void logEvent(uint64_t tick, const std::string& message) = 0;

    // Record a completed one-way trip for metric aggregation.
    virtual void recordTrip(const TripRecord& record) = 0;

    // Accumulate elevator travel distance (called once per floor moved).
    virtual void recordElevatorMove(int fromFloor, int toFloor) = 0;

    // Print aggregated statistics to stdout.
    virtual void printSummary() const = 0;
};

} // namespace ElevatorSim
