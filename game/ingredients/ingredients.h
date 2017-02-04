#pragma once
#include "game/transcendental/entity_handle.h"
#include "augs/math/vec2.h"

#include "game/assets/texture_id.h"
#include "game/assets/animation_response_id.h"
#include "game/components/render_component.h"

#include "augs/graphics/pixel.h"
#include "game/transcendental/entity_handle.h"

class cosmos;
#include "game/transcendental/step_declaration.h"

namespace components {
	struct item;
	struct sprite;
}

namespace ingredients {
	void camera(entity_handle, int w, int h);

	components::item& make_item(entity_handle);
	
	components::sprite& sprite(entity_handle, components::transform pos, assets::texture_id = assets::texture_id::BLANK, rgba col = rgba(255, 255, 255, 255), render_layer = render_layer::GROUND);
	components::sprite& sprite_scalled(entity_handle, components::transform pos, vec2i size = vec2i(), assets::texture_id = assets::texture_id::BLANK, rgba col = rgba(255, 255, 255, 255), render_layer = render_layer::GROUND);
	
	void bullet_round_physics(entity_handle);
	void see_through_dynamic_body(entity_handle);
	void shell_dynamic_body(entity_handle);
	void standard_dynamic_body(entity_handle, const bool destructible = false);
	void standard_static_body(entity_handle);

	void wsad_character_physics(entity_handle);
	void wsad_character_legs(entity_handle legs, entity_handle player);
	void wsad_character(entity_handle, entity_handle crosshair_entity, const assets::animation_response_id torso_set = assets::animation_response_id::TORSO_SET);
	void wsad_character_corpse(entity_handle);

	void make_always_visible(entity_handle);

	void character_inventory(entity_handle);
	void backpack(entity_handle);

	void default_gun_container(entity_handle, const float mag_rotation = -90.f);
	void default_gun(entity_handle);

	void standard_pathfinding_capability(entity_handle);
	void soldier_intelligence(entity_handle);
}

namespace prefabs {
	entity_handle create_car(cosmos&, const components::transform&);
	entity_handle create_motorcycle(cosmos&, const components::transform&);

	entity_handle create_sample_suppressor(cosmos& world, vec2 pos);

	entity_handle create_sample_magazine(const logic_step, components::transform pos, std::string space = "0.30", entity_id charge_inside = entity_id());
	entity_handle create_small_magazine(const logic_step, components::transform pos, std::string space = "0.30", entity_id charge_inside = entity_id());
	entity_handle create_sample_rifle(const logic_step, vec2 pos, entity_id load_mag = entity_id());
	entity_handle create_sample_bilmer2000(const logic_step, vec2 pos, entity_id load_mag = entity_id());
	entity_handle create_pistol(const logic_step, vec2 pos, entity_id load_mag = entity_id());
	entity_handle create_kek9(const logic_step, vec2 pos, entity_id load_mag = entity_id());
	entity_handle create_submachine(const logic_step, vec2 pos, entity_id load_mag = entity_id());

	entity_handle create_amplifier_arm(
		cosmos&, 
		vec2 pos 
	);

	entity_handle create_pink_charge(cosmos&, vec2 pos, int charges = 23);
	entity_handle create_cyan_charge(cosmos&, vec2 pos, int charges = 30);
	entity_handle create_green_charge(cosmos& world, vec2 pos, int charges = 23);

	entity_handle create_sample_backpack(cosmos&, vec2 pos);

	entity_handle create_character_crosshair(cosmos&, const vec2i screen_size);
	
	entity_handle create_character(
		cosmos&, 
		const components::transform pos, 
		const vec2i screen_size, 
		const std::string name = "character_unnamed", 
		const assets::animation_response_id torso_set = assets::animation_response_id::TORSO_SET
	);
	
	entity_handle create_crate(cosmos&, const components::transform pos, const vec2 size = vec2());
	entity_handle create_brick_wall(cosmos&, const components::transform pos, const vec2 size = vec2());

	entity_handle create_cyan_urban_machete(cosmos&, const vec2 pos);
}