#pragma once
#include "augs/misc/constant_size_vector.h"
#include "augs/misc/enum/enum_array.h"

#include "game/container_sizes.h"
#include "game/transcendental/component_synchronizer.h"

namespace invariants {
	struct shape_circle {
		// GEN INTROSPECTOR struct invariants::shape_circle
		real32 radius = 0.f;
		// END GEN INTROSPECTOR

		auto get_radius() const {
			return radius;
		}
	};
}
