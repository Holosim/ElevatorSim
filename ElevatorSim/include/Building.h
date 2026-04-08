#pragma once
#include "interfaces/IBuilding.h"

namespace ElevatorSim {

// Phase-1 building: fixed floor count, single shaft, ground floor = 1.
class Building final : public IBuilding {
public:
    explicit Building(int numFloors);

    int getNumFloors()   const override;
    int getGroundFloor() const override;
    int getTopFloor()    const override;

private:
    int numFloors;
};

} // namespace ElevatorSim
