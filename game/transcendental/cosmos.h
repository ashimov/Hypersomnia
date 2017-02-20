#pragma once
#include <thread>
#include "game/build_settings.h"
#include "augs/misc/streams.h"
#include "game/transcendental/cosmic_entropy.h"

#include "augs/entity_system/operations_on_all_components_mixin.h"
#include "augs/entity_system/storage_for_systems.h"

#include "game/transcendental/types_specification/all_components_declaration.h"
#include "game/transcendental/types_specification/all_messages_declaration.h"
#include "game/transcendental/types_specification/all_systems_declaration.h"

#include "game/systems_temporary/dynamic_tree_system.h"
#include "game/systems_temporary/physics_system.h"
#include "game/systems_temporary/processing_lists_system.h"

#include "augs/misc/delta.h"
#include "augs/misc/easier_handle_getters_mixin.h"

#include "game/transcendental/entity_id.h"
#include "game/detail/inventory_slot_id.h"
#include "game/transcendental/entity_handle_declaration.h"
#include "game/transcendental/data_living_one_step.h"
#include "game/detail/inventory_slot_handle_declaration.h"
#include "game/detail/item_slot_transfer_request_declaration.h"
#include "game/simulation_settings/all_simulation_settings.h"
#include "game/transcendental/cosmic_profiler.h"
#include "augs/build_settings/setting_empty_bases.h"

#include "game/detail/inventory_slot_handle.h"
#include "game/detail/item_slot_transfer_request.h"

class cosmic_delta;
struct data_living_one_step;

class EMPTY_BASES cosmos : 
	private put_all_components_into<augs::operations_on_all_components_mixin, cosmos>::type, 
	public augs::easier_handle_getters_mixin<cosmos>
{
	void advance_deterministic_schemata_and_queue_destructions(const logic_step step_state);
	void perform_deletions(const logic_step);

#if COSMOS_TRACKS_GUIDS
	friend class cosmic_delta;

	template<class T>
	friend void transform_component_guids_to_ids(T&, const cosmos&);

	std::map<entity_guid, entity_id> guid_map_for_transport;
	std::vector<std::string> entity_debug_names;

	void delete_debug_name(const entity_id);

	void assign_next_guid(const entity_handle);
	void clear_guid(const entity_handle);
	entity_guid get_guid(const const_entity_handle) const;
public:
	void remap_guids();
private:
	entity_handle create_entity_with_specific_guid(
		const std::string& debug_name, 
		const entity_guid specific_guid
	);
#endif

	void destroy_substance_completely();
	void create_substance_completely();

	entity_handle allocate_new_entity();
public:
	storage_for_all_systems_temporary systems_temporary;

	mutable cosmic_profiler profiler;
	augs::stream reserved_memory_for_serialization;

	class significant_state {
	public:
		class metadata {
			friend class cosmos;

			augs::delta delta;
			unsigned total_steps_passed = 0;

#if COSMOS_TRACKS_GUIDS
			entity_guid next_entity_guid = 1;
#endif
		public:
			all_simulation_settings settings;

		} meta;

		aggregate_pool_type pool_for_aggregates;
		component_pools_type pools_for_components;

		bool operator==(const significant_state&) const;
		bool operator!=(const significant_state&) const;

	} significant;

	template <class Archive>
	void serialize(Archive& ar) {
		ar(CEREAL_NVP(significant));
		complete_resubstantiation();
	}

	cosmos(const unsigned reserved_entities = 0);
	cosmos(const cosmos&);

	cosmos& operator=(const cosmos&);
	cosmos& operator=(const significant_state&);

	bool operator==(const cosmos&) const;
	bool operator!=(const cosmos&) const;

	template<class Pre, class Post>
	void advance_deterministic_schemata(const cosmic_entropy& input, Pre pre_solve, Post post_solve) {
		data_living_one_step queues;
		logic_step step(*this, input, queues);

		pre_solve(step);
		advance_deterministic_schemata_and_queue_destructions(step);
		post_solve(const_logic_step(step));
		
		perform_deletions(step);
	}

	void advance_deterministic_schemata(const cosmic_entropy& input);

	void reserve_storage_for_entities(const size_t);

	entity_handle create_entity(const std::string& debug_name);
	entity_handle clone_entity(const entity_id);
	void delete_entity(const entity_id);

	void refresh_for_new_significant_state();

	void complete_resubstantiation();
	void complete_resubstantiation(const const_entity_handle);
	void destroy_substance_for_entity(const const_entity_handle);
	void create_substance_for_entity(const const_entity_handle);
	
	const std::string& get_debug_name(entity_id) const;
	void set_debug_name(const entity_id, const std::string& new_debug_name);

	template<class System>
	void partial_resubstantiation(const entity_handle handle) {
		auto& sys = systems_temporary.get<System>();

		sys.destruct(handle);

		if (handle.has<components::substance>()) {
			sys.construct(handle);
		}
	}

#if COSMOS_TRACKS_GUIDS
	entity_handle get_entity_by_guid(const entity_guid);
	const_entity_handle get_entity_by_guid(const entity_guid) const;
	bool entity_exists_with_guid(const entity_guid) const;
#endif

	template <class T>
	decltype(auto) get_handle(const T t) {
		return t;
	}

	template <class T>
	decltype(auto) get_handle(const T t) const {
		return t;
	}

	entity_handle get_handle(const entity_id);
	const_entity_handle get_handle(const entity_id) const;
	entity_handle get_handle(const unversioned_entity_id);
	const_entity_handle get_handle(const unversioned_entity_id) const;
	inventory_slot_handle get_handle(const inventory_slot_id);
	const_inventory_slot_handle get_handle(const inventory_slot_id) const;
	item_slot_transfer_request get_handle(const item_slot_transfer_request_data);
	const_item_slot_transfer_request get_handle(const item_slot_transfer_request_data) const;
#if COSMOS_TRACKS_GUIDS
	inventory_slot_handle get_handle(const basic_inventory_slot_id<entity_guid>);
	const_inventory_slot_handle get_handle(const basic_inventory_slot_id<entity_guid>) const;
	item_slot_transfer_request get_handle(const basic_item_slot_transfer_request_data<entity_guid>);
	const_item_slot_transfer_request get_handle(const basic_item_slot_transfer_request_data<entity_guid>) const;

	basic_inventory_slot_id<entity_guid> guidize(const inventory_slot_id) const;
	basic_item_slot_transfer_request_data<entity_guid> guidize(const item_slot_transfer_request_data) const;
#endif

	randomization get_rng_for(const entity_id) const;
	size_t get_rng_seed_for(const entity_id) const;

	std::vector<entity_handle> get(const processing_subjects);
	std::vector<const_entity_handle> get(const processing_subjects) const;

	size_t entities_count() const;
	size_t get_maximum_entities() const;
	std::wstring summary() const;

	float get_total_time_passed_in_seconds(const float view_interpolation_ratio) const;
	float get_total_time_passed_in_seconds() const;
	unsigned get_total_steps_passed() const;

	augs::stepped_timestamp get_timestamp() const;

	const augs::delta& get_fixed_delta() const;
	void set_fixed_delta(const augs::delta&);
	void set_fixed_delta(const unsigned steps_per_second);

	/* saving procedure is not const due to possible resubstantiation of the universe */
	void save_to_file(const std::string&);
	bool load_from_file(const std::string&);

	template <class D>
	void for_each_entity_id(D pred) {
		get_aggregate_pool().for_each_id(pred);
	}

	template <class D>
	void for_each_entity_id(D pred) const {
		get_aggregate_pool().for_each_id(pred);
	}

	auto& get_aggregate_pool() {
		return significant.pool_for_aggregates;
	}

	const auto& get_aggregate_pool() const {
		return significant.pool_for_aggregates;
	}

	template<class T>
	auto& get_component_pool() {
		return std::get<augs::pool<T>>(significant.pools_for_components);
	}

	template<class T>
	const auto& get_component_pool() const {
		return std::get<augs::pool<T>>(significant.pools_for_components);
	}
};

namespace augs {
	template<class A>
	bool read_object(A& ar, cosmos::significant_state& significant) {
		return 
			read_object(ar, significant.meta)
			&& read_object(ar, significant.pools_for_components)
			&& read_object(ar, significant.pool_for_aggregates);
	}

	template<class A>
	void write_object(A& ar, const cosmos::significant_state& significant) {
		write_object(ar, significant.meta);
		write_object(ar, significant.pools_for_components);
		write_object(ar, significant.pool_for_aggregates);
	}
}

#if COSMOS_TRACKS_GUIDS
inline entity_handle cosmos::get_entity_by_guid(const entity_guid guid) {
	return get_handle(guid_map_for_transport.at(guid));
}

inline const_entity_handle cosmos::get_entity_by_guid(const entity_guid guid) const {
	return get_handle(guid_map_for_transport.at(guid));
}

inline bool cosmos::entity_exists_with_guid(const entity_guid guid) const {
	return guid_map_for_transport.find(guid) != guid_map_for_transport.end();
}

inline entity_guid cosmos::get_guid(const const_entity_handle handle) const {
	return handle.get_guid();
}
#endif

inline entity_handle cosmos::get_handle(const entity_id id) {
	return { *this, id };
}

inline const_entity_handle cosmos::get_handle(const entity_id id) const {
	return { *this, id };
}

inline entity_handle cosmos::get_handle(const unversioned_entity_id id) {
	return { *this, get_aggregate_pool().make_versioned(id) };
}

inline const_entity_handle cosmos::get_handle(const unversioned_entity_id id) const {
	return { *this, get_aggregate_pool().make_versioned(id) };
}

inline inventory_slot_handle cosmos::get_handle(const inventory_slot_id id) {
	return { *this, id };
}

inline const_inventory_slot_handle cosmos::get_handle(const inventory_slot_id id) const {
	return { *this, id };
}

inline item_slot_transfer_request cosmos::get_handle(const item_slot_transfer_request_data data) {
	return {
		get_handle(data.item),
		get_handle(data.target_slot),
		data.specified_quantity,
		data.force_immediate_mount
	};
}

inline const_item_slot_transfer_request cosmos::get_handle(const item_slot_transfer_request_data data) const {
	return {
		get_handle(data.item),
		get_handle(data.target_slot),
		data.specified_quantity,
		data.force_immediate_mount
	};
}

#if COSMOS_TRACKS_GUIDS

inline inventory_slot_handle cosmos::get_handle(const basic_inventory_slot_id<entity_guid> id) {
	return {
		*this,
		{
			id.type,
			get_entity_by_guid(id.container_entity)
		}
	};
}

inline const_inventory_slot_handle cosmos::get_handle(const basic_inventory_slot_id<entity_guid> id) const {
	return {
		*this,
		{
			id.type,
			get_entity_by_guid(id.container_entity)
		}
	};
}

inline item_slot_transfer_request cosmos::get_handle(const basic_item_slot_transfer_request_data<entity_guid> data) {
	return {
		get_handle(get_entity_by_guid(data.item)),
		get_handle(data.target_slot),
		data.specified_quantity,
		data.force_immediate_mount
	};
}

inline const_item_slot_transfer_request cosmos::get_handle(const basic_item_slot_transfer_request_data<entity_guid> data) const {
	return {
		get_handle(get_entity_by_guid(data.item)),
		get_handle(data.target_slot),
		data.specified_quantity,
		data.force_immediate_mount
	};
}

inline basic_inventory_slot_id<entity_guid> cosmos::guidize(const inventory_slot_id id) const {
	return { id.type, get_guid(get_handle(id.container_entity)) };
}

inline basic_item_slot_transfer_request_data<entity_guid> cosmos::guidize(const item_slot_transfer_request_data data) const {
	return { get_guid(get_handle(data.item)), guidize(data.target_slot), data.specified_quantity, data.force_immediate_mount };
}

#endif

inline randomization cosmos::get_rng_for(const entity_id id) const {
	return{ get_rng_seed_for(id) };
}

inline size_t cosmos::entities_count() const {
	return significant.pool_for_aggregates.size();
}

inline size_t cosmos::get_maximum_entities() const {
	return significant.pool_for_aggregates.capacity();
}