#include "physics_system.h"

#include "game/entity_id.h"
#include "game/cosmos.h"

#include "game/components/item_component.h"
#include "game/components/driver_component.h"
#include "game/components/fixtures_component.h"

#include "game/messages/collision_message.h"
#include "game/messages/queue_destruction.h"
#include "game/messages/new_entity_message.h"
#include "game/messages/will_soon_be_deleted.h"

#include "game/cosmos.h"
#include "game/step.h"
#include "game/entity_handle.h"

double METERS_TO_PIXELS = 100.0;
double PIXELS_TO_METERS = 1.0 / METERS_TO_PIXELS;
float METERS_TO_PIXELSf = 100.f;
float PIXELS_TO_METERSf = 1.0f / METERS_TO_PIXELSf;

physics_system::physics_system(cosmos& parent_cosmos) : parent_cosmos(parent_cosmos),
b2world(b2Vec2(0.f, 0.f)), ray_casts_since_last_step(0) {
	b2world.SetAllowSleeping(false);
	b2world.SetAutoClearForces(false);
	enable_listener(true);
}

void physics_system::enable_listener(bool flag) {
	b2world.SetContactListener(flag ? &listener : nullptr);
}

void physics_system::step_and_set_new_transforms(step_state& step) {
	listener.cosmos_ptr = &parent_cosmos;
	listener.step_ptr = &step;

	int32 velocityIterations = 8;
	int32 positionIterations = 3;

	for (b2Body* b = b2world.GetBodyList(); b != nullptr; b = b->GetNext()) {
		if (b->GetType() == b2_staticBody) continue;

		auto& physics = parent_cosmos[b->GetUserData()].get<components::physics>();
		physics.measured_carried_mass = 0.f;

		b2Vec2 vel(b->GetLinearVelocity());
		float32 speed = vel.Normalize();
		float32 angular_speed = b->GetAngularVelocity();

		//if (physics.air_resistance > 0.f) {
		//	auto force_dir = physics.get_mass() * -vel;
		//	auto force_components = (physics.air_resistance  * speed * speed);
		//
		//	//if (speed > 1.0)
		//	//	force_components += (0.5f * sqrt(std::abs(speed)));
		//	
		//	physics.body->ApplyForce(force_components * force_dir, physics.body->GetWorldCenter(), true);
		//}

		auto angular_resistance = physics.angular_air_resistance;
		if (angular_resistance < 0.f) angular_resistance = physics.air_resistance;
		
		if (angular_resistance > 0.f) {
			//physics.body->ApplyTorque((angular_resistance * sqrt(sqrt(angular_speed * angular_speed)) + 0.2 * angular_speed * angular_speed)* -sgn(angular_speed) * b->GetInertia(), true);
			physics.black_detail.body->ApplyTorque((angular_resistance * angular_speed * angular_speed )* -sgn(angular_speed) * b->GetInertia(), true);
		}

		if (physics.enable_angle_motor) {
			float next_angle = b->GetAngle() + b->GetAngularVelocity() / static_cast<float>(parent_cosmos.delta.get_steps_per_second());
			
			auto target_orientation = vec2().set_from_degrees(physics.target_angle);
			auto next_orientation = vec2().set_from_radians(next_angle);

			float total_rotation = target_orientation.radians_between(next_orientation);

			if (target_orientation.cross(next_orientation) > 0)
				total_rotation *= -1;

			float desired_angular_velocity = total_rotation / static_cast<float>(parent_cosmos.delta.in_seconds());
			float impulse = b->GetInertia() * desired_angular_velocity;// disregard time factor
			b->ApplyAngularImpulse(impulse * physics.angle_motor_force_multiplier, true);
		}
	}

	listener.after_step_callbacks.clear();

	ray_casts_since_last_step = 0;
	b2world.Step(static_cast<float32>(parent_cosmos.delta.in_seconds()), velocityIterations, positionIterations);
	b2world.ClearForces();
	
	for (auto& c : listener.after_step_callbacks)
		c();

	for (b2Body* b = b2world.GetBodyList(); b != nullptr; b = b->GetNext()) {
		if (b->GetType() == b2_staticBody) continue;
		auto entity = parent_cosmos[b->GetUserData()];
		auto& physics = entity.get<components::physics>();
		
		recurential_friction_handler(b->GetUserData(), physics.get_owner_friction_ground());
	}

	set_transforms_from_body_transforms();
}

void physics_system::set_transforms_from_body_transforms() {
	for (b2Body* b = b2world.GetBodyList(); b != nullptr; b = b->GetNext()) {
		if (b->GetType() == b2_staticBody) continue;
		auto entity = parent_cosmos[b->GetUserData()];

		auto& transform = entity.get<components::transform>();
		auto& physics = entity.get<components::physics>();

		auto body_pos = METERS_TO_PIXELSf * b->GetPosition();
		auto body_angle = b->GetAngle() * RAD_TO_DEG;

		for (auto& ff : physics.black_detail.fixture_entities) {
			auto fe = parent_cosmos[ff];

			auto& fixtures = fe.get<components::fixtures>();
			auto total_offset = fixtures.get_total_offset();

			auto& transform = fe.get<components::transform>();
			transform.pos = body_pos;

			if (!b->IsFixedRotation())
				transform.rotation = body_angle;

			transform.pos += total_offset.pos;
			transform.rotation += total_offset.rotation;

			transform.pos.rotate(body_angle, body_pos);
		}

		transform.pos = body_pos;

		if (!b->IsFixedRotation())
			transform.rotation = body_angle;
	}
}

void physics_system::react_to_destroyed_entities(step_state& step) {
	auto& events = step.messages.get_queue<messages::will_soon_be_deleted>();

	for (auto& it : events) {
		auto e = parent_cosmos[it.subject];

		auto* maybe_physics = e.find<components::physics>();
		auto* maybe_fixtures = e.find<components::fixtures>();

		if (maybe_physics)
			maybe_physics->destroy_body();

		if (maybe_fixtures)
			maybe_fixtures->destroy_fixtures();
	}
}

void physics_system::react_to_new_entities(step_state& step) {
	auto& events = step.messages.get_queue<messages::new_entity_message>();

	for (auto& it : events) {
		auto e = parent_cosmos[it.subject];

		auto* maybe_physics = e.find<components::physics>();
		auto* maybe_fixtures = e.find<components::fixtures>();

		if (maybe_physics) {
			maybe_physics->black_detail.body_owner = e;
			maybe_physics->black_detail.parent_system = this;

			if (maybe_physics->should_body_exist_now()) {
				maybe_physics->build_body();
			}
		}

		if (maybe_fixtures) {
			if (parent_cosmos[maybe_fixtures->get_owner_body()].dead()) {
				maybe_fixtures->set_owner_body(e);
			}

			maybe_fixtures->black_detail.all_fixtures_owner = e;
			maybe_fixtures->black_detail.parent_system = this;

			if (maybe_fixtures->should_fixtures_exist_now()) {
				maybe_fixtures->build_fixtures();
			}
		}
	}
}