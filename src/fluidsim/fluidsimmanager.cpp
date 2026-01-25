#include "fluidsimmanager.hpp"
#include "fluidsim.hpp"
#include "fluidsimtypes.hpp"
#include "../../raylib/src/raylib.h"
#include <vector>
#include <limits>
#include <tuple>

std::vector<std::tuple<FluidSim, float>> activesims;

void AddFluidSim(FluidSimType type, float duration) {
    activesims.push_back(std::make_tuple(FluidSim(type), duration));
}

void UpdateFluidSims() {
    float deltaTime = GetFrameTime();
    for (auto i = activesims.begin(); i != activesims.end(); i++) {
        std::tuple<FluidSim, float> tuple = *i;
        auto [sim, durationRemaining] = tuple;
        //FluidSim sim, float durationRemaining

        if ((durationRemaining -= deltaTime) <= 0) {
            activesims.erase(i);
            i--;
            continue;
        }
        sim.UpdateSim();
    }
}
void DrawFluidSims() {
    for (auto i = activesims.begin(); i != activesims.end(); i++) {
        std::tuple<FluidSim, float> tuple = *i;
        auto [sim, durationRemaining] = tuple;

        sim.DrawSim();
    }
}


void InitializeFluidSims() {
    AddFluidSim(type2, std::numeric_limits<float>::infinity());
}