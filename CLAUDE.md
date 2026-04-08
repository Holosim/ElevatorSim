# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Building

**Visual Studio 2022 (primary):** Open `ElevatorSim.sln`, select Debug or Release / x64, press Ctrl+Shift+B.

**MSBuild (CLI):**
```
msbuild ElevatorSim.sln /p:Configuration=Debug /p:Platform=x64
msbuild ElevatorSim.sln /p:Configuration=Release /p:Platform=x64
```

Output binary: `x64/Debug/ElevatorSim.exe` or `x64/Release/ElevatorSim.exe`

**Run:**
```
x64/Debug/ElevatorSim.exe
```

There is no test framework in place yet. Simulation parameters are hardcoded in `src/main.cpp` via `SimConfig`.

## Architecture

Everything lives in the `ElevatorSim` namespace. The project is designed around **interface-first composition**: `Simulation` owns all subsystems exclusively through abstract interfaces, so any subsystem can be replaced without touching the orchestrator.

### Layers

**`Types.h`** — shared vocabulary: `Direction`, `ElevatorState`, `PersonState`, `SimConfig`, `TripRecord`. No dependencies; include freely.

**`include/interfaces/`** — pure abstract contracts:
- `ISimulationClock` — tick counter (getTick / advance)
- `IElevator` — floor movement, stop queue, observer registration, per-tick update
- `IElevatorObserver` — callbacks fired synchronously from inside `elevator->update()`
- `IPerson` — passenger state model; state transitions are externally driven
- `IBuilding` — floor range metadata
- `ISimulationLogger` — event log, trip recording, summary output

**Concrete implementations** in `include/` + `src/`: `Elevator`, `Person`, `Building`, `SimulationClock`, `SimulationLogger`.

**`Simulation`** — the top-level orchestrator. Owns all subsystems via `unique_ptr` to their interfaces. Implements `IElevatorObserver` to react to elevator state changes. Runs the discrete-time loop: each iteration calls `elevator->update(tick)`, which may synchronously fire observer callbacks back into `Simulation`.

### Elevator state machine

```
IDLE ──addStop──> MOVING ──arrived──> DOORS_OPEN ──timerExpired──> IDLE
IDLE ──addStop(same floor)──────────> DOORS_OPEN
```
Door timer is measured in ticks (`doorOpenDuration` in `SimConfig`). The FIFO stop queue is a `std::queue<int>` inside `Elevator`.

### Trip lifecycle

1. `Simulation::scheduleNextTrip()` calls `person->requestTrip()` and adds two stops to the elevator (pickup floor, then destination floor).
2. `onDoorsOpened()` — if the person is `WAITING` at this floor, they board; if `RIDING` and this is their destination, they exit. On exit, `TripRecord` is logged.
3. `onDoorsClosed()` — if the person is back to `IDLE` and trips remain, the next trip is scheduled immediately.

### Note on `floorsPerTick`

`SimConfig::floorsPerTick` is defined but `Elevator`'s constructor does not accept it — the concrete `Elevator` currently moves exactly one floor per tick. Wiring `floorsPerTick` into `Elevator` is a pending Phase 2 item.

## Conventions

- C++17, MSVC toolset v143, x64 only. No precompiled headers.
- Include path root is `ElevatorSim/include/`; headers use flat names (`#include "Elevator.h"`) or relative paths for interfaces (`#include "interfaces/IElevator.h"`).
- `Simulation.h` forward-declares all interface types to keep concrete headers out of it; the destructor is defined in `Simulation.cpp` where complete types are visible (required for `unique_ptr` deletion of incomplete types).
- `main.cpp` is the composition root — all wiring happens there.

## Git

GitHub repo: https://github.com/Holosim/ElevatorSim  
Remote: `origin` → `https://github.com/Holosim/ElevatorSim.git`, branch `master`.

**After every meaningful unit of work, commit and push.** This includes completing a feature, fixing a bug, refactoring, or any change that leaves the code in a coherent state. Never batch up multiple unrelated changes into one commit.

Commit message format:
- Subject line: short imperative summary (e.g. `Wire floorsPerTick into Elevator constructor`)
- Body (when needed): explain *what* changed and *why*, not just *how*

```
git add <specific files>
git commit -m "Subject line

Optional body explaining motivation or context."
git push
```

This ensures there is always a recoverable snapshot on GitHub and makes it easy to revert any change.
