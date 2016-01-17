#pragma once
#include "entity_system/component.h"
#include "transform_component.h"

#include "math/vec2.h"

namespace components {
	struct render : public augs::component {
		vec2 last_screen_pos;
		bool was_drawn = false;

		transform previous_transform;
		transform saved_actual_transform;
		
		vec2 interpolation_direction() {
			return saved_actual_transform.pos - previous_transform.pos;
		}

		bool interpolate = true;

		enum mask_type {
			WORLD,
		};

		enum render_layer {
			CROSSHAIR,
			GUI,
			HAT,
			CHARACTER,
			HELD_ITEM,
			DYNAMIC_BODY,
			DROPPED_ITEM,
			CAR_WHEEL,
			CAR_INTERIOR,
			GROUND,
			BOTTOM,
		};

		render_layer layer = render_layer::BOTTOM;
		unsigned mask = mask_type::WORLD;

		bool flip_horizontally = false;
		bool flip_vertically = false;
		bool absolute_transform = false;

		int rendering_proxy;
	};
}