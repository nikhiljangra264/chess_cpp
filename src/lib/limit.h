#pragma once

#include <stdint.h>

constexpr uint16_t MAX_DEPTH = 5;

static struct
{
	uint16_t max_depth = MAX_DEPTH;
	uint16_t max_nodes = -1;
}limit;
