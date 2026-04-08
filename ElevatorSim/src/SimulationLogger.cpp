#include "SimulationLogger.h"
#include <iostream>
#include <iomanip>
#include <cmath>

namespace ElevatorSim {

SimulationLogger::SimulationLogger(bool verbose)
    : verbose(verbose), totalFloorsMoved(0)
{}

void SimulationLogger::logEvent(uint64_t tick, const std::string& message)
{
    if (verbose) {
        std::cout << "[Tick " << std::setw(4) << tick << "] " << message << "\n";
    }
}

void SimulationLogger::recordTrip(const TripRecord& record)
{
    trips.push_back(record);
}

void SimulationLogger::recordElevatorMove(int fromFloor, int toFloor)
{
    totalFloorsMoved += std::abs(toFloor - fromFloor);
}

void SimulationLogger::printSummary() const
{
    struct Stats {
        int      count     = 0;
        uint64_t totalWait = 0;
        uint64_t totalRide = 0;
    };
    Stats upStats, downStats;

    std::cout << "\n";
    std::cout << "==============================================\n";
    std::cout << "           SIMULATION TRIP LOG               \n";
    std::cout << "==============================================\n";

    for (size_t i = 0; i < trips.size(); ++i) {
        const TripRecord& t = trips[i];
        std::cout
            << "  Trip " << std::setw(2) << (i + 1) << ": "
            << std::setw(4) << directionToString(t.direction)
            << "  Floor " << std::setw(2) << t.originFloor
            << " -> " << std::setw(2) << t.destinationFloor
            << "  | Wait: " << std::setw(3) << t.waitTime()  << " ticks"
            << "  | Ride: " << std::setw(3) << t.rideTime()  << " ticks"
            << "  | Total: " << std::setw(3) << t.totalTime() << " ticks\n";

        if (t.direction == Direction::UP) {
            ++upStats.count;
            upStats.totalWait += t.waitTime();
            upStats.totalRide += t.rideTime();
        } else if (t.direction == Direction::DOWN) {
            ++downStats.count;
            downStats.totalWait += t.waitTime();
            downStats.totalRide += t.rideTime();
        }
    }

    std::cout << "\n==============================================\n";
    std::cout << "              AGGREGATE METRICS               \n";
    std::cout << "==============================================\n";

    auto printDirectionStats = [](const char* label, const Stats& s) {
        std::cout << "\n  " << label << " trips (" << s.count << " total):\n";
        if (s.count > 0) {
            std::cout << std::fixed << std::setprecision(2);
            std::cout << "    Avg wait time : "
                      << static_cast<double>(s.totalWait) / s.count << " ticks\n";
            std::cout << "    Avg ride time : "
                      << static_cast<double>(s.totalRide) / s.count << " ticks\n";
            std::cout << "    Avg total time: "
                      << static_cast<double>(s.totalWait + s.totalRide) / s.count << " ticks\n";
        }
    };

    printDirectionStats("Upward  ", upStats);
    printDirectionStats("Downward", downStats);

    std::cout << "\n  Elevator travel:\n";
    std::cout << "    Total floors moved: " << totalFloorsMoved << "\n";

    if (trips.size() >= 2) {
        uint64_t spanTicks = trips.back().exitTick - trips.front().requestTick;
        if (spanTicks > 0) {
            std::cout << std::fixed << std::setprecision(2);
            std::cout << "    Simulation span : " << spanTicks << " ticks\n";
            std::cout << "    Floors per tick : "
                      << static_cast<double>(totalFloorsMoved) / spanTicks << "\n";
        }
    }

    std::cout << "==============================================\n";
}

} // namespace ElevatorSim
