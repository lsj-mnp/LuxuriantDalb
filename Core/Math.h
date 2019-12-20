#pragma once

#include <cstdlib>

static int GetRandom(int Min, int Max)
{
	return rand() % (Max - Min + 1) + Min;
}

static float GetRandomFloat(int Min, int Max)
{
	return static_cast<float>(GetRandom(Min, Max));
}
