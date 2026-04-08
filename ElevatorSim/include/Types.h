#pragma once
#include <cstdint>
#include <string>

namespace ElevatorSim {

// ---------------------------------------------------------------------------
// Core enumerations
// ---------------------------------------------------------------------------

enum class Direction {
    UP,
    DOWN,
    NONE
};

enum class ElevatorState {
    IDLE,
    MOVING,
    DOORS_OPEN
};

enum class PersonState {
    IDLE,
    WAITING,   // waiting on a floor for the elevator
    RIDING     // inside the elevator
};

// ---------------------------------------------------------------------------
// Simulation configuration
// ---------------------------------------------------------------------------

struct SimConfig {
    int numFloors       = 10;
    int startFloor      = 1;
    int floorsPerTick   = 1;   // floors the elevator moves per tick
    int doorOpenDuration = 3;  // ticks the doors stay open
    int numTrips        = 6;   // total one-way trips to simulate
    bool verboseLogging = true;
};

// ---------------------------------------------------------------------------
// Immutable record produced after each one-way trip completes
// ---------------------------------------------------------------------------

struct TripRecord {
    uint64_t  requestTick      = 0;
    uint64_t  boardTick        = 0;
    uint64_t  exitTick         = 0;
    int       originFloor      = 0;
    int       destinationFloor = 0;
    Direction direction        = Direction::NONE;

    uint64_t waitTime()  const { return boardTick - requestTick; }
    uint64_t rideTime()  const { return exitTick  - boardTick;   }
    uint64_t totalTime() const { return exitTick  - requestTick; }
};

// ---------------------------------------------------------------------------
// Helper
// ---------------------------------------------------------------------------

inline std::string directionToString(Direction d)
{
    switch (d) {
        case Direction::UP:   return "UP";
        case Direction::DOWN: return "DOWN";
        default:              return "NONE";
    }
}

} // namespace ElevatorSim
