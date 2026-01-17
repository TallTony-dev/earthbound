#pragma once
#include "fluidsim.hpp"
#include "fluidsimtypes.hpp"

void AddFluidSim(FluidSimType type, float duration);
void UpdateFluidSims();
void DrawFluidSims();
void InitializeFluidSims();