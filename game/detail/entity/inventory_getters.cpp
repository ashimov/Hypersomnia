#include "game/entity_handle.h"
#include "game/detail/inventory_slot_id.h"
#include "game/detail/inventory_slot_handle.h"
#include "game/components/item_component.h"
#include "game/components/item_slot_transfers_component.h"
#include "game/cosmos.h"

template <bool C>
basic_entity_handle<C> basic_entity_handle<C>::get_owning_transfer_capability() const {
	auto& cosmos = get_cosmos();

	if (dead())
		return cosmos[entity_id()];

	auto* maybe_transfer_capability = find<components::item_slot_transfers>();

	if (maybe_transfer_capability)
		return *this;

	auto* maybe_item = find<components::item>();

	if (!maybe_item || cosmos[maybe_item->current_slot].dead())
		return cosmos[entity_id()];

	return cosmos[maybe_item->current_slot].get_container().get_owning_transfer_capability();
}

template <bool C>
typename basic_entity_handle<C>::inventory_slot_handle_type basic_entity_handle<C>::first_free_hand() const {
	auto& cosmos = get_cosmos();

	auto maybe_primary = operator[](slot_function::PRIMARY_HAND);
	auto maybe_secondary = operator[](slot_function::SECONDARY_HAND);

	if (maybe_primary.is_empty_slot())
		return maybe_primary;

	if (maybe_secondary.is_empty_slot())
		return maybe_secondary;

	return cosmos[inventory_slot_id()];
}

template <bool C>
typename basic_entity_handle<C>::inventory_slot_handle_type basic_entity_handle<C>::determine_hand_holstering_slot(basic_entity_handle searched_root_container) const {
	auto& item_entity = *this;
	auto& cosmos = item_entity.get_cosmos();

	ensure(item_entity.alive());
	ensure(searched_root_container.alive());

	auto maybe_shoulder = searched_root_container[slot_function::SHOULDER_SLOT];

	if (maybe_shoulder.alive()) {
		if (maybe_shoulder.can_contain(item_entity))
			return maybe_shoulder;
		else if (maybe_shoulder->items_inside.size() > 0) {
			auto maybe_item_deposit = maybe_shoulder.get_items_inside()[0][slot_function::ITEM_DEPOSIT];

			if (maybe_item_deposit.alive() && maybe_item_deposit.can_contain(item_entity))
				return maybe_item_deposit;
		}
	}
	else {
		auto maybe_armor = searched_root_container[slot_function::TORSO_ARMOR_SLOT];

		if (maybe_armor.alive())
			if (maybe_armor.can_contain(item_entity))
				return maybe_armor;
	}

	return cosmos[inventory_slot_id()];
}

template <bool C>
typename basic_entity_handle<C>::inventory_slot_handle_type basic_entity_handle<C>::determine_pickup_target_slot_in(basic_entity_handle searched_root_container) const {
	auto& item_entity = *this;
	ensure(item_entity.alive());
	ensure(searched_root_container.alive());
	auto& cosmos = item_entity.get_cosmos();

	auto hidden_slot = item_entity.determine_hand_holstering_slot(searched_root_container);;

	if (cosmos[hidden_slot].alive())
		return hidden_slot;

	if (searched_root_container[slot_function::PRIMARY_HAND].can_contain(item_entity))
		return searched_root_container[slot_function::PRIMARY_HAND];

	if (searched_root_container[slot_function::SECONDARY_HAND].can_contain(item_entity))
		return searched_root_container[slot_function::SECONDARY_HAND];

	return cosmos[inventory_slot_id()];
}

template <bool C>
typename basic_entity_handle<C>::inventory_slot_handle_type basic_entity_handle<C>::map_primary_action_to_secondary_hand_if_primary_empty(int is_action_secondary) const {
	auto& root_container = *this;

	auto primary = root_container[slot_function::PRIMARY_HAND];
	auto secondary = root_container[slot_function::SECONDARY_HAND];

	if (primary.is_empty_slot())
		return secondary;
	else
		return is_action_secondary ? secondary : primary;
}

// explicit instantiation
template class basic_entity_handle <false>;
template class basic_entity_handle <true>;