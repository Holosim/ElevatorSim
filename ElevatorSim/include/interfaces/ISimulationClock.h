#pragma once
#include <cstdint>

namespace ElevatorSim {

// Abstracts discrete-time advancement so the clock implementation can be
// swapped out (real-time, accelerated, deterministic fixed-step, etc.)
class ISimulationClock {
public:
    virtual ~ISimulationClock() = default;

    virtual uint64_t getTick() const = 0;
    virtual void     advance()       = 0;
};

} // namespace ElevatorSim
