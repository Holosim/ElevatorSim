#pragma once
#include <queue>
#include "interfaces/IElevator.h"

namespace ElevatorSim {

// Phase-1 elevator: single car, FIFO stop queue, fixed speed.
//
// State machine:
//   IDLE ──addStop──> MOVING ──arrivedAtTarget──> DOORS_OPEN
//   IDLE ──addStop(sameFloor)──────────────────> DOORS_OPEN
//   DOORS_OPEN ──timerExpired──> IDLE (pop stop, notify observer)
class Elevator final : public IElevator {
public:
    explicit Elevator(int startFloor, int doorOpenDuration);

    int           getFloor()     const override;
    ElevatorState getState()     const override;
    Direction     getDirection() const override;
    bool          hasStops()     const override;

    void addStop    (int floor)                    override;
    void setObserver(IElevatorObserver* observer)  override;
    void update     (uint64_t tick)                override;

private:
    int                  currentFloor;
    int                  targetFloor;
    int                  doorOpenDuration;
    uint64_t             doorsOpenedAt;
    ElevatorState        state;
    Direction            direction;
    std::queue<int>      stopQueue;
    IElevatorObserver*   observer;

    void openDoors      (uint64_t tick);
    void processNextStop(uint64_t tick);
};

} // namespace ElevatorSim
