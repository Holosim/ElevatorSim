#pragma once
#include "interfaces/ISimulationClock.h"

namespace ElevatorSim {

// Deterministic integer-tick clock.  One tick represents one simulation
// time step (e.g. one second).  Header-only — no separate .cpp needed.
class SimulationClock final : public ISimulationClock {
public:
    SimulationClock() : currentTick(0) {}

    uint64_t getTick() const override { return currentTick; }
    void     advance() override       { ++currentTick; }

private:
    uint64_t currentTick;
};

} // namespace ElevatorSim
