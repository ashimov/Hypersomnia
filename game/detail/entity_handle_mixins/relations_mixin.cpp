#include "relations_mixin.h"
#include "game/detail/inventory/inventory_slot_id.h"
#include "game/detail/inventory/inventory_slot_handle.h"
#include "game/transcendental/entity_handle.h"
#include "game/transcendental/cosmos.h"
#include "game/components/substance_component.h"
#include "game/components/guid_component.h"
#include "game/components/child_component.h"
#include "game/components/sub_entities_component.h"
#include "game/components/physical_relations_component.h"
#include "augs/templates/container_templates.h"

template <class D>
void relations_mixin<false, D>::make_child(const entity_id ch_id, const sub_entity_name optional_name) const {
	auto& self = *static_cast<const D*>(this);
	auto& cosmos = self.get_cosmos();

	auto ch = cosmos[ch_id];

	if (ch.alive()) {
		ch.child_component().parent = self;
	}
}

template <class D>
components::child& relations_mixin<false, D>::child_component() const {
	auto& self = *static_cast<const D*>(this);

	if (!self.has<components::child>())
		self.add(components::child());

	return self.get<components::child>();
}

template <class D>
components::sub_entities& relations_mixin<false, D>::sub_entities_component() const {
	auto& self = *static_cast<const D*>(this);

	if (!self.has<components::sub_entities>())
		self.add(components::sub_entities());

	return self.get<components::sub_entities>();
}

template <class D>
components::physical_relations& relations_mixin<false, D>::physical_relations_component() const {
	auto& self = *static_cast<const D*>(this);

	if (!self.has<components::physical_relations>())
		self.add(components::physical_relations());

	return self.get<components::physical_relations>();
}

template <class D>
void relations_mixin<false, D>::make_cloned_sub_entities_recursive(const entity_id from) const {
	auto& self = *static_cast<const D*>(this);
	auto& cosmos = self.get_cosmos();
	auto from_rels = cosmos[from].get_sub_entities_component();

	for (const auto& id : from_rels.other_sub_entities)
		add_sub_entity(cosmos.clone_entity(id));

	for (const auto& id : from_rels.sub_entities_by_name)
		map_sub_entity(id.first, cosmos.clone_entity(id.second));
}

template <class D>
void relations_mixin<false, D>::set_owner_body(const entity_id owner_id) const {
	auto& self = *static_cast<const D*>(this);

	auto& cosmos = self.get_cosmos();
	auto new_owner = cosmos[owner_id];
	auto this_id = self.get_id();

	auto former_owner = cosmos[self.get_physical_relations_component().owner_body];

	if (former_owner.alive()) {
		remove_element(former_owner.physical_relations_component().fixture_entities, this_id);
		cosmos.partial_resubstantiation<physics_system>(former_owner);
	}

	self.physical_relations_component().owner_body = new_owner;

	if (new_owner.alive()) {
		remove_element(new_owner.physical_relations_component().fixture_entities, this_id);
		new_owner.physical_relations_component().fixture_entities.push_back(this_id);
		cosmos.partial_resubstantiation<physics_system>(new_owner);
	}
	else {
		cosmos.partial_resubstantiation<physics_system>(self);
	}
}

template <class D>
void relations_mixin<false, D>::add_sub_entity(const entity_id p, const sub_entity_name optional_name = sub_entity_name::INVALID) const {
	make_child(p, optional_name);
	sub_entities_component().other_sub_entities.push_back(p);
}

template <class D>
void relations_mixin<false, D>::map_sub_entity(const sub_entity_name n, const entity_id p) const {
	make_child(p, n);
	sub_entities_component().sub_entities_by_name[n] = p;
}

template <bool C, class D>
typename basic_relations_mixin<C, D>::inventory_slot_handle_type basic_relations_mixin<C, D>::operator[](const slot_function func) const {
	auto& self = *static_cast<const D*>(this);
	return inventory_slot_handle_type(self.owner, inventory_slot_id(func, self.raw_id));
}

template <bool C, class D>
D basic_relations_mixin<C, D>::operator[](sub_entity_name child) const {
	auto& self = *static_cast<const D*>(this);
	const auto& subs = get_sub_entities_component().sub_entities_by_name;

	const auto found = subs.find(child);
	
	if (found == subs.end()) {
		return self.get_cosmos()[entity_id()];
	}

	return self.get_cosmos()[(*found).second];
}

template <bool C, class D>
D basic_relations_mixin<C, D>::get_owner_body() const {
	auto& self = *static_cast<const D*>(this);
	return self.get_cosmos()[get_physical_relations_component().owner_body];
}

template <bool C, class D>
std::vector<D> basic_relations_mixin<C, D>::get_fixture_entities() const {
	auto& self = *static_cast<const D*>(this);
	return self.get_cosmos()[get_physical_relations_component().fixture_entities];
}

#if COSMOS_TRACKS_GUIDS
template <bool C, class D>
unsigned basic_relations_mixin<C, D>::get_guid() const {
	auto& self = *static_cast<const D*>(this);
	return self.get<components::guid>().value;
}
#endif

template <bool C, class D>
D basic_relations_mixin<C, D>::get_parent() const {
	auto& self = *static_cast<const D*>(this);
	entity_id parent = get_child_component().parent;
	return self.get_cosmos()[parent];
}


template <bool C, class D>
const components::child& basic_relations_mixin<C, D>::get_child_component() const {
	static thread_local const components::child original;
	
	const auto& self = *static_cast<const D*>(this);
	
	if (self.has<components::child>())
		return self.get<components::child>();

	return original;
}

template <bool C, class D>
const components::sub_entities& basic_relations_mixin<C, D>::get_sub_entities_component() const {
	static thread_local const components::sub_entities original;
	
	const auto& self = *static_cast<const D*>(this);

	if (self.has<components::sub_entities>())
		return self.get<components::sub_entities>();

	return original;
}

template <bool C, class D>
const components::physical_relations& basic_relations_mixin<C, D>::get_physical_relations_component() const {
	static thread_local const components::physical_relations original;
	
	const auto& self = *static_cast<const D*>(this);

	if (self.has<components::physical_relations>())
		return self.get<components::physical_relations>();

	return original;
}

template class basic_relations_mixin<false, basic_entity_handle<false>>;
template class basic_relations_mixin<true, basic_entity_handle<true>>;
template class relations_mixin<false, basic_entity_handle<false>>;
template class relations_mixin<true, basic_entity_handle<true>>;
