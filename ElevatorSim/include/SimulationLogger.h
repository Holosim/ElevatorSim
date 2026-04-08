#pragma once
#include <vector>
#include "interfaces/ISimulationLogger.h"

namespace ElevatorSim {

// Console-based logger that accumulates TripRecords and prints a
// formatted efficiency report at the end of the simulation.
class SimulationLogger final : public ISimulationLogger {
public:
    explicit SimulationLogger(bool verbose = true);

    void logEvent        (uint64_t tick, const std::string& message) override;
    void recordTrip      (const TripRecord& record)                  override;
    void recordElevatorMove(int fromFloor, int toFloor)              override;
    void printSummary    () const                                     override;

private:
    bool                    verbose;
    std::vector<TripRecord> trips;
    int                     totalFloorsMoved;
};

} // namespace ElevatorSim
