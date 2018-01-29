#include "ingredients.h"
#include "game/transcendental/cosmos.h"
#include "game/transcendental/entity_handle.h"

#include "game/assets/ids/particle_effect_id.h"

#include "game/components/missile_component.h"
#include "game/components/item_component.h"
#include "game/components/melee_component.h"
#include "game/components/fixtures_component.h"
#include "game/components/explosive_component.h"
#include "game/components/shape_circle_component.h"
#include "game/components/shape_polygon_component.h"
#include "game/components/sender_component.h"
#include "game/components/hand_fuse_component.h"

#include "game/enums/filters.h"

#include "game/detail/inventory/perform_transfer.h"

namespace test_flavours {
	void populate_grenade_types(const loaded_game_image_caches& logicals, entity_flavours& flavours) {
		{
			auto& meta = get_test_flavour(flavours, test_scene_flavour::FORCE_GRENADE);

			meta.description =
				L"Throwable explosive with a three seconds delay.\nDeals damage to [color=red]Health[/color]."
			;

			{
				invariants::render render_def;
				render_def.layer = render_layer::SMALL_DYNAMIC_BODY;

				meta.set(render_def);

			}
			test_flavours::add_sprite(meta, logicals, assets::game_image_id::FORCE_GRENADE, white);
			add_shape_invariant_from_renderable(meta, logicals);
			test_flavours::add_see_through_dynamic_body(meta);

			invariants::item item;
			item.space_occupied_per_charge = to_space_units("0.6");
			meta.set(item);

			invariants::hand_fuse fuse; 
			fuse.throw_sound.id = assets::sound_buffer_id::GRENADE_THROW;
			fuse.unpin_sound.id = assets::sound_buffer_id::GRENADE_UNPIN;
			meta.set(fuse);

			invariants::explosive explosive; 

			auto& in = explosive.explosion;
			in.type = adverse_element_type::FORCE;
			in.damage = 88.f;
			in.inner_ring_color = red;
			in.outer_ring_color = orange;
			in.effective_radius = 300.f;
			in.impact_force = 550.f;
			in.sound_gain = 1.8f;
			in.sound_effect = assets::sound_buffer_id::GREAT_EXPLOSION;

			explosive.released_image_id = assets::game_image_id::FORCE_GRENADE_RELEASED;
			explosive.released_physical_material = assets::physical_material_id::GRENADE;

			meta.set(explosive);
		}

		{
			auto& meta = get_test_flavour(flavours, test_scene_flavour::INTERFERENCE_GRENADE);

			meta.description =
				L"Throwable explosive with a three seconds delay.\nDeals damage to [color=orange]Consciousness[/color].\nCauses massive aimpunch."
			;

			{
				invariants::render render_def;
				render_def.layer = render_layer::SMALL_DYNAMIC_BODY;

				meta.set(render_def);
			}
			test_flavours::add_sprite(meta, logicals, assets::game_image_id::INTERFERENCE_GRENADE, white);
			add_shape_invariant_from_renderable(meta, logicals);
			test_flavours::add_see_through_dynamic_body(meta);

			invariants::item item;
			item.space_occupied_per_charge = to_space_units("0.6");
			meta.set(item);

			invariants::hand_fuse fuse; 
			fuse.throw_sound.id = assets::sound_buffer_id::GRENADE_THROW;
			fuse.unpin_sound.id = assets::sound_buffer_id::GRENADE_UNPIN;
			meta.set(fuse);

			invariants::explosive explosive; 

			auto& in = explosive.explosion;

			in.damage = 100.f;
			in.inner_ring_color = yellow;
			in.outer_ring_color = orange;
			in.effective_radius = 450.f;
			in.impact_force = 20.f;
			in.sound_gain = 2.2f;
			in.sound_effect = assets::sound_buffer_id::INTERFERENCE_EXPLOSION;
			in.type = adverse_element_type::INTERFERENCE;

			explosive.released_image_id = assets::game_image_id::INTERFERENCE_GRENADE_RELEASED;
			explosive.released_physical_material = assets::physical_material_id::GRENADE;

			meta.set(explosive);
		}

		{
			auto& meta = get_test_flavour(flavours, test_scene_flavour::PED_GRENADE);

			meta.description =
				L"Throwable explosive with a three seconds delay.\nDrains [color=cyan]Personal Electricity[/color].\nIf the subject has [color=turquoise]Electric Shield[/color] enabled,\nthe effect is doubled."
			;

			{
				invariants::render render_def;
				render_def.layer = render_layer::SMALL_DYNAMIC_BODY;

				meta.set(render_def);
			}
			test_flavours::add_sprite(meta, logicals, assets::game_image_id::PED_GRENADE, white);
			add_shape_invariant_from_renderable(meta, logicals);
			test_flavours::add_see_through_dynamic_body(meta);

			invariants::item item;
			item.space_occupied_per_charge = to_space_units("0.6");
			meta.set(item);

			invariants::hand_fuse fuse; 
			fuse.throw_sound.id = assets::sound_buffer_id::GRENADE_THROW;
			fuse.unpin_sound.id = assets::sound_buffer_id::GRENADE_UNPIN;
			meta.set(fuse);

			invariants::explosive explosive; 

			auto& in = explosive.explosion;
			in.damage = 88.f;
			in.inner_ring_color = cyan;
			in.outer_ring_color = turquoise;
			in.effective_radius = 350.f;
			in.impact_force = 20.f;
			in.sound_gain = 2.2f;
			in.sound_effect = assets::sound_buffer_id::PED_EXPLOSION;
			in.type = adverse_element_type::PED;
			in.create_thunders_effect = true;

			explosive.released_image_id = assets::game_image_id::PED_GRENADE_RELEASED;
			explosive.released_physical_material = assets::physical_material_id::GRENADE;

			meta.set(explosive);
		}
	}
}

namespace prefabs {
	entity_handle create_force_grenade(const logic_step step, vec2 pos) {
		auto& world = step.get_cosmos();
		const auto& metas = step.get_logical_assets();
		const auto grenade_entity = create_test_scene_entity(world, test_scene_flavour::FORCE_GRENADE);

		grenade_entity.set_logic_transform(pos);
		grenade_entity.add_standard_components(step);

		return grenade_entity;
	}

	entity_handle create_ped_grenade(const logic_step step, vec2 pos) {
		auto& world = step.get_cosmos();
		const auto& metas = step.get_logical_assets();
		const auto grenade_entity = create_test_scene_entity(world, test_scene_flavour::PED_GRENADE);

		grenade_entity.set_logic_transform(pos);
		grenade_entity.add_standard_components(step);

		return grenade_entity;
	}

	entity_handle create_interference_grenade(const logic_step step, vec2 pos) {
		auto& world = step.get_cosmos();
		const auto& metas = step.get_logical_assets();
		const auto grenade_entity = create_test_scene_entity(world, test_scene_flavour::INTERFERENCE_GRENADE);
		
		grenade_entity.set_logic_transform(pos);
		grenade_entity.add_standard_components(step);

		return grenade_entity;
	}
}

