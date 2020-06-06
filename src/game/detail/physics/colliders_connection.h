#pragma once
#include "game/cosmos/entity_id.h"
#include "game/components/transform_component.h"
#include "augs/pad_bytes.h"

struct colliders_connection {
	// GEN INTROSPECTOR struct colliders_connection
	entity_id owner;
	transformr shape_offset;
	bool flip_vertically = false;
	pad_bytes<3> pad;
	// END GEN INTROSPECTOR

	static auto none() {
		return colliders_connection();
	}

	bool operator==(const colliders_connection& b) const {
		return owner == b.owner && shape_offset == b.shape_offset;
	}

	bool operator!=(const colliders_connection& b) const {
		return !operator==(b);
	}
};
