#include "spell_data.h"
#include "game/transcendental/cosmos.h"
#include "game/transcendental/entity_handle.h"
#include "game/components/sentience_component.h"
#include "game/components/render_component.h"
#include "game/components/wandering_pixels_component.h"
#include "game/components/damage_component.h"
#include "game/components/sound_response_component.h"
#include "game/messages/create_particle_effect.h"
#include "game/systems_stateless/particles_existence_system.h"
#include "game/systems_stateless/sound_existence_system.h"

#include "game/ingredients/ingredients.h"
#include "game/assets/sound_response_id.h"

#include "game/detail/entity_scripts.h"
#include "game/enums/filters.h"

spell_data get_spell_data(const spell_type spell) {
	spell_data d;
	
	switch (spell) {
	case spell_type::HASTE:
		d.cooldown_ms = 5000;
		d.incantation = L"treximo";
		d.perk_seconds = 33;
		d.personal_electricity_required = 60;
		break;

	case spell_type::FURY_OF_THE_AEONS:
		d.cooldown_ms = 2000;
		d.incantation = L"mania aiones";
		d.personal_electricity_required = 100;
		break;

	case spell_type::ELECTRIC_TRIAD:
		d.cooldown_ms = 3000;
		d.incantation = L"energeia triada";
		d.personal_electricity_required = 120;
		break;

	case spell_type::ULTIMATE_WRATH_OF_THE_AEONS:
		d.cooldown_ms = 2000;
		d.casting_time_ms = 3000;
		d.incantation = L"megalyteri aiones via";
		d.personal_electricity_required = 260;
		break;

	case spell_type::ELECTRIC_SHIELD:
		d.cooldown_ms = 5000;
		d.incantation = L"energeia aspida";
		d.perk_seconds = 60;
		d.personal_electricity_required = 50;
		break;
	
	default: 
		LOG("Unknown spell: %x", static_cast<int>(spell)); 
		break;
	}

	return std::move(d);
}

spell_appearance get_spell_appearance(const spell_type spell) {
	spell_appearance a;

	const rgba turqoise_spell_color = turquoise;
	const rgba blue_spell_border = cyan; //{ 0, 128, 209, 255 };
	const rgba green_spell_color = { 0, 200, 0, 255 };

	switch (spell) {
	case spell_type::HASTE:
		a.icon = assets::texture_id::SPELL_HASTE_ICON;
		a.border_col = green_spell_color;
		break;

	case spell_type::FURY_OF_THE_AEONS:
		a.icon = assets::texture_id::SPELL_FURY_OF_THE_AEONS_ICON;
		a.border_col = blue_spell_border;
		break;

	case spell_type::ELECTRIC_TRIAD:
		a.icon = assets::texture_id::SPELL_ELECTRIC_TRIAD_ICON;
		a.border_col = blue_spell_border;
		break;

	case spell_type::ULTIMATE_WRATH_OF_THE_AEONS:
		a.icon = assets::texture_id::SPELL_ULTIMATE_WRATH_OF_THE_AEONS_ICON;
		a.border_col = blue_spell_border;
		break;

	case spell_type::ELECTRIC_SHIELD:
		a.icon = assets::texture_id::SPELL_ELECTRIC_SHIELD_ICON;
		a.border_col = turqoise_spell_color;
		break;
	
	default: 
		LOG("Unknown spell: %x", static_cast<int>(spell)); 
		break;
	}

	return std::move(a);
}

std::wstring describe_spell(
	const const_entity_handle subject,
	const spell_type spell
) {
	const auto& sentience = subject.get<components::sentience>();
	const auto spell_data = get_spell_data(spell);

	const auto properties = typesafe_sprintf(
		L"Incantation: [color=yellow]%x[/color]\nPE to cast: [color=vscyan]%x[/color]\nCooldown: [color=vscyan]%x[/color]",
		std::wstring(spell_data.incantation), 
		spell_data.personal_electricity_required, 
		spell_data.cooldown_ms
	);

	std::wstring d;

	switch (spell) {
	case spell_type::HASTE:
		d = typesafe_sprintf(L"[color=green]Haste[/color]\n%x\n[color=vsdarkgray]Increases movement speed for %x seconds.[/color]", properties, spell_data.perk_seconds);
		break;

	case spell_type::FURY_OF_THE_AEONS:
		d = typesafe_sprintf(L"[color=cyan]Fury of the Aeons[/color]\n%x\n[color=vsdarkgray]Causes instant damage around the caster.[/color]", properties);
		break;

	case spell_type::ELECTRIC_TRIAD:
		d = typesafe_sprintf(L"[color=cyan]Electric Triad[/color]\n%x\n[color=vsdarkgray]Spawns three electric missiles\nhoming towards hostile entities.[/color]", properties);
		break;

	case spell_type::ULTIMATE_WRATH_OF_THE_AEONS:
		d = typesafe_sprintf(L"[color=cyan]Ultimate Wrath of the Aeons[/color]\n%x\n[color=vsdarkgray]Causes massive damage around the caster.\nRequires delay to initiate.[/color]", properties);
		break;

	case spell_type::ELECTRIC_SHIELD:
		d = typesafe_sprintf(L"[color=turquoise]Electric Shield[/color]\n%x\n[color=vsdarkgray]For %x seconds, damage is absorbed\nby [/color][color=cyan]Personal Electricity[/color][color=vsdarkgray] instead of [/color][color=red]Health[/color][color=vsdarkgray].[/color]", properties, spell_data.perk_seconds);
		break;

	default: 
		LOG("Unknown spell: %x", static_cast<int>(spell)); 
		break;
	}

	return std::move(d);
}

void do_spell_callback(
	const spell_type spell,
	const entity_handle subject,
	components::sentience& sentience,
	const augs::stepped_timestamp when_casted,
	const augs::stepped_timestamp now
) {
	const auto spell_data = get_spell_data(spell);
	auto& cosm = subject.get_cosmos();
	const auto dt = cosm.get_fixed_delta();
	const auto transform = subject.logic_transform();
	const auto appearance = get_spell_appearance(spell);
	
	const auto ignite_sparkles = [&]() {
		messages::create_particle_effect burst;
		burst.subject = subject;
		burst.place_of_birth = transform;
		burst.input.effect = assets::particle_effect_id::CAST_SPARKLES;
		burst.input.modifier.colorize = appearance.border_col;

		particles_existence_system().create_particle_effect_entity(cosm, burst).add_standard_components();
	};

	const auto standard_sparkles_sound = [&]() {
		components::sound_existence::effect_input in;
		in.delete_entity_after_effect_lifetime = true;
		in.direct_listener = subject;
		in.effect = assets::sound_buffer_id::CAST_SUCCESSFUL;

		sound_existence_system().create_sound_effect_entity(cosm, in, transform, entity_id()).add_standard_components();
	};

	switch (spell) {
	case spell_type::HASTE: 
		sentience.haste.set_for_duration(spell_data.perk_seconds * 1000, now); 
		ignite_sparkles();
		standard_sparkles_sound();

		break;

	case spell_type::ELECTRIC_SHIELD: 
		sentience.electric_shield.set_for_duration(spell_data.perk_seconds * 1000, now); 
		ignite_sparkles();
		standard_sparkles_sound();

		break;

	case spell_type::FURY_OF_THE_AEONS:
		ignite_sparkles();
		standard_sparkles_sound();

		break;

	case spell_type::ULTIMATE_WRATH_OF_THE_AEONS:
		if (now == when_casted) {
			ignite_sparkles();
			standard_sparkles_sound();
		}

		break;

	case spell_type::ELECTRIC_TRIAD:
		ignite_sparkles();
		standard_sparkles_sound();

		{
			const auto hostiles = get_closest_hostiles(
				subject,
				subject,
				800,
				filters::bullet()
			);

			for (size_t i = 0; i < hostiles.size() && i < 3; ++i) {
				const auto next_hostile = cosm[hostiles[i]];

				const auto round_definition = cosm.create_entity("round_definition");
				auto new_energy_ball_transform = transform;
				new_energy_ball_transform.rotation = (next_hostile.logic_transform().pos - transform.pos).degrees();

				auto& s = ingredients::sprite(round_definition, new_energy_ball_transform, assets::texture_id::ENERGY_BALL, cyan, render_layer::FLYING_BULLETS);
				ingredients::bullet_round_physics(round_definition);

				{
					auto& response = round_definition += components::particle_effect_response{ assets::particle_effect_response_id::ELECTRIC_PROJECTILE_RESPONSE };
					response.modifier.colorize = cyan;
				}

				{
					auto& response = round_definition += components::sound_response();
					response.response = assets::sound_response_id::ELECTRIC_PROJECTILE_RESPONSE;
				}

				auto& damage = round_definition += components::damage();
				damage.homing_towards_hostile_strength = 1.0f;
				damage.amount = 42;
				damage.sender = subject;
				damage.particular_homing_target = next_hostile;

				round_definition.get<components::physics>().set_velocity(vec2().set_from_degrees(new_energy_ball_transform.rotation).set_length(2000));

				round_definition.add_standard_components();
			}
		}

		break;

	default: 
		LOG("Unknown spell: %x", static_cast<int>(spell)); 
		break;
	}
}

