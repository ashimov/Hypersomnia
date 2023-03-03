#pragma once
#include "test_scenes/test_id_to_pool_id.h"

enum class test_scene_image_id {
	// GEN INTROSPECTOR enum class test_scene_image_id
	BLANK = 0,

	BLANK_2X2,

	CRATE,

	TRUCK_INSIDE,
	TRUCK_FRONT,

	JMIX114,

	TEST_CROSSHAIR,

	SMOKE_1,
	SMOKE_2,
	SMOKE_3,
	SMOKE_4,
	SMOKE_5,
	SMOKE_6,

	PIXEL_THUNDER_1,
	PIXEL_THUNDER_2,
	PIXEL_THUNDER_3,
	PIXEL_THUNDER_4,
	PIXEL_THUNDER_5,

	ASSAULT_RIFLE,
	BILMER2000,
	KEK9,
	SN69,
	AO44,
	PRO90,
	LEWSII,
	ELON_HRL,
	BLUNAZ,
	BLUNAZ_SHOT_1,

	FLOWER_PINK_1,
	FLOWER_CYAN_1,
	CONSOLE_LIGHT_1,
	BAKA47_SHOT_1,

	DATUM_GUN,
	DATUM_GUN_SHOT_1,

	CALICO,
	CALICO_SHOT_1,

	WARX_FQ12,

	SOIL,
	FLOOR,
	WATER_ROOM_FLOOR,

	BAKA47_MAGAZINE,
	BILMER2000_MAGAZINE,
	KEK9_MAGAZINE,
	SN69_MAGAZINE,
	AO44_MAGAZINE,
	CALICO_MAGAZINE,
	WARX_FQ12_MAGAZINE,
	PRO90_MAGAZINE,
	LEWSII_MAGAZINE,
	ROUND_TRACE,
	ELECTRIC_MISSILE,
	PINK_CHARGE,
	PINK_SHELL,
	CYAN_CHARGE,
	CYAN_SHELL,
	RED_CHARGE,
	RED_SHELL,

	GREEN_CHARGE,
	GREEN_SHELL,

	ORANGE_CHARGE,
	ORANGE_SHELL,
	ORANGE_ROUND,

	STEEL_CHARGE,
	STEEL_SHELL,
	STEEL_ROUND,

	SHOTGUN_RED_CHARGE,
	SHOTGUN_RED_ROUND,
	SHOTGUN_RED_SHELL,

	SKULL_ROCKET,
	SKULL_ROCKET_FLYING,
	SKULL_ROCKET_SHELL,

	STEEL_ROUND_REMNANT_1,
	STEEL_ROUND_REMNANT_2,
	STEEL_ROUND_REMNANT_3,

	METROPOLIS_BACKPACK,
	RESISTANCE_BACKPACK,

	HAVE_A_PLEASANT,
	AWAKENING,
	METROPOLIS,

	BRICK_WALL,
	ROAD,
	ROAD_FRONT_DIRT,

	CAST_BLINK_1,

	SILVER_TROUSERS_1,
	SILVER_TROUSERS_STRAFE_1,

	METROPOLIS_TORSO_BARE_WALK_SHOT_1,
	METROPOLIS_TORSO_KNIFE_WALK_1,
	METROPOLIS_TORSO_KNIFE_PRIM_1,
	METROPOLIS_TORSO_KNIFE_PRIM_RETURN_1,
	METROPOLIS_TORSO_KNIFE_SECD_1,
	METROPOLIS_TORSO_PISTOL_WALK_1,
	METROPOLIS_TORSO_PISTOL_SHOT_1,
	METROPOLIS_TORSO_PISTOL_PTM_1,
	METROPOLIS_TORSO_RIFLE_WALK_1,
	METROPOLIS_TORSO_RIFLE_SHOT_1,
	METROPOLIS_TORSO_RIFLE_PTM_1,
	METROPOLIS_TORSO_RIFLE_GTM_1,
	METROPOLIS_TORSO_HEAVY_WALK_1,
	METROPOLIS_TORSO_HEAVY_SHOT_1,
	METROPOLIS_TORSO_HEAVY_GTM_1,
	METROPOLIS_TORSO_AKIMBO_WALK_SHOT_1,

	FURY_THROWER,
	ELECTRIC_RAPIER,
	CYAN_SCYTHE,
	POSEIDON,
	YELLOW_DAGGER,

	RESISTANCE_TORSO_BARE_WALK_SHOT_1,
	RESISTANCE_TORSO_KNIFE_WALK_1,
	RESISTANCE_TORSO_KNIFE_PRIM_1,
	RESISTANCE_TORSO_KNIFE_PRIM_RETURN_1,
	RESISTANCE_TORSO_KNIFE_SECD_1,
	RESISTANCE_TORSO_PISTOL_WALK_1,
	RESISTANCE_TORSO_PISTOL_SHOT_1,
	RESISTANCE_TORSO_PISTOL_PTM_1,
	RESISTANCE_TORSO_RIFLE_WALK_1,
	RESISTANCE_TORSO_RIFLE_SHOT_1,
	RESISTANCE_TORSO_RIFLE_PTM_1,
	RESISTANCE_TORSO_RIFLE_GTM_1,
	RESISTANCE_TORSO_HEAVY_WALK_1,
	RESISTANCE_TORSO_HEAVY_SHOT_1,
	RESISTANCE_TORSO_HEAVY_GTM_1,
	RESISTANCE_TORSO_AKIMBO_WALK_SHOT_1,

	BIG_BUBBLE_1,
	MEDIUM_BUBBLE_1,
	SMALL_BUBBLE_LB_1,
	SMALL_BUBBLE_LT_1,
	SMALL_BUBBLE_RB_1,
	SMALL_BUBBLE_RT_1,

	YELLOW_FISH_1,
	DARKBLUE_FISH_1,
	CYANVIOLET_FISH_1,
	JELLYFISH_1,
	DRAGON_FISH_1,

	BUTTERFLY_1,
	CICADA_1,
	MOTA_1,

	AQUARIUM_BOTTOM_LAMP_LIGHT,
	AQUARIUM_BOTTOM_LAMP_BODY,

	AQUARIUM_HALOGEN_1_LIGHT,
	AQUARIUM_HALOGEN_1_BODY,

	AQUARIUM_SAND_EDGE,
	AQUARIUM_SAND_CORNER,

	AQUARIUM_SAND_1,
	AQUARIUM_SAND_2,

	AQUARIUM_GLASS_START,
	AQUARIUM_GLASS,

	LAB_WALL_SMOOTH_END,
	LAB_WALL,
	LAB_WALL_A2,

	LAB_WALL_CORNER_CUT,
	LAB_WALL_CORNER_SQUARE,

	WATER_SURFACE_1,
	PINK_CORAL_1,

	DUNE_BIG,
	DUNE_SMALL,

	METROPOLIS_HEAD,
	RESISTANCE_HEAD,

	TRUCK_ENGINE,

	HEALTH_ICON,
	PERSONAL_ELECTRICITY_ICON,
	CONSCIOUSNESS_ICON,

	AMPLIFIER_ARM,

	SPELL_HASTE_ICON,
	SPELL_ELECTRIC_SHIELD_ICON,
	SPELL_ELECTRIC_TRIAD_ICON,
	SPELL_FURY_OF_THE_AEONS_ICON,
	SPELL_ULTIMATE_WRATH_OF_THE_AEONS_ICON,
	SPELL_EXALTATION_ICON,
	SPELL_ECHOES_OF_THE_HIGHER_REALMS_ICON,

	PERK_HASTE_ICON,
	PERK_ELECTRIC_SHIELD_ICON,

	ELECTRIC_ARMOR,

	FORCE_GRENADE,
	PED_GRENADE,
	INTERFERENCE_GRENADE,
	FLASHBANG,

	FORCE_GRENADE_RELEASED,
	PED_GRENADE_RELEASED,
	INTERFERENCE_GRENADE_RELEASED,
	FLASHBANG_RELEASED,

	FORCE_ROCKET,

	BOMB_1,
	BOMB_ARMED_1,
	BOMB_DEFUSED,

	FAN,

	METROPOLIS_LOGO,
	RESISTANCE_LOGO,
	METROPOLIS_SQUARE_LOGO,
	RESISTANCE_SQUARE_LOGO,
	ATLANTIS_LOGO,

	DEFUSE_KIT,

	BOMB_ICON,
	DEATH_ICON,
	DEFUSE_KIT_ICON,
	NO_AMMO_ICON,
	UNCONSCIOUS_ICON,
	DATUM_GUN_MAGAZINE,

	STANDARD_PERSONAL_DEPOSIT,

	MONEY_ICON,

	SNACKBAR,
	SNACKBAR_CAPTION,

	GRADOBICIE,
	GRADOBICIE_CHARGE,
	GRADOBICIE_SHELL,
	GRADOBICIE_ROUND,

	BAKA47,

	BULWARK,
	BULWARK_SHOT_1,
	BULWARK_MAGAZINE,

	ZAMIEC,
	ZAMIEC_SHOT_1,
	ZAMIEC_MAGAZINE,

	ASSAULT_RATTLE_1,

	SZTURM,
	SZTURM_MAGAZINE,

	MINI_KNIFE,

	DEAGLE,
	DEAGLE_SHOT_1,

	DEAGLE_MAGAZINE,

	DEAGLE_CHARGE,
	DEAGLE_ROUND,
	DEAGLE_SHELL,

	HPSR,
	HPSR_MAGAZINE,
	HPSR_ROUND,
	HPSR_SHELL,

	BULLDUP2000,
	BULLDUP2000_MAGAZINE,
	BULLDUP2000_ROUND,
	BULLDUP2000_SHELL,

	GALILEA,
	GALILEA_MAGAZINE,
	GALILEA_ROUND,
	GALILEA_SHELL,

	HUNTER,
	HUNTER_MAGAZINE,

	CYBERSPRAY,
	CYBERSPRAY_MAGAZINE,

	COVERT,
	COVERT_SHOT_1,
	COVERT_MAGAZINE,

	BROKEN_SHIELD_ICON,

	METROPOLIS_HEAD_ICON,
	METROPOLIS_BROKEN_HEAD_ICON,

	RESISTANCE_HEAD_ICON,
	RESISTANCE_BROKEN_HEAD_ICON,

	HEADSHOT_ICON,

	TRIANGLE_COLLIDER,

	COUNT
	// END GEN INTROSPECTOR
};

inline auto to_image_id(const test_scene_image_id id) {
	return to_pool_id<assets::image_id>(id);
}
