#pragma once
#include <cstdint>
#include "Types.h"
#include "IElevatorObserver.h"

namespace ElevatorSim {

// Contract for an elevator car.  All policy decisions (dispatch order,
// door timing, movement speed) live in the concrete implementation so
// they can be varied independently in future phases.
class IElevator {
public:
    virtual ~IElevator() = default;

    virtual int           getFloor()     const = 0;
    virtual ElevatorState getState()     const = 0;
    virtual Direction     getDirection() const = 0;
    virtual bool          hasStops()     const = 0;

    // Enqueue a floor stop.  Multiple stops may be added; they are
    // served in FIFO order in Phase 1.
    virtual void addStop(int floor) = 0;

    // Register the single observer that receives state-change events.
    virtual void setObserver(IElevatorObserver* observer) = 0;

    // Advance elevator state by one simulation tick.
    virtual void update(uint64_t tick) = 0;
};

} // namespace ElevatorSim
