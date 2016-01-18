#include "physics_component.h"
#include <Box2D\Box2D.h>

#include "graphics/renderer.h"
#include "fixtures_component.h"

namespace components {
	physics& physics::get_owner_body(augs::entity_id id) {
		auto* physics = id->find<components::physics>();
		if (physics) return *physics;

		return id->get<components::fixtures>().get_body_entity()->get<components::physics>();
	}
	
	augs::entity_id physics::get_owner_body_entity(augs::entity_id id) {
		auto* physics = id->find<components::physics>();
		if (physics) return id;

		return id->get<components::fixtures>().get_body_entity();
	}

	bool physics::is_physical(augs::entity_id id) {
		return id->find<components::fixtures>() || id->find<components::physics>();
	}

	void physics::set_velocity(vec2 pixels) {
		body->SetLinearVelocity(pixels * PIXELS_TO_METERSf);
	}	
	
	void physics::set_linear_damping(float damping) {
		body->SetLinearDamping(damping);
	}

	void physics::set_linear_damping_vec(vec2 pixels) {
		body->SetLinearDampingVec(pixels);
	}

	void physics::apply_force(vec2 pixels) {
		apply_force(pixels, vec2(0,0), true);
	}
	
	void physics::apply_force(vec2 pixels, vec2 center_offset, bool wake) {
		vec2 force = pixels * PIXELS_TO_METERSf;
		vec2 location = body->GetWorldCenter() + (center_offset * PIXELS_TO_METERSf);

		body->ApplyForce(force, location, wake);

		if (renderer::get_current().debug_drawing && force.non_zero()) {
			auto& lines = renderer::get_current().logic_lines;
			lines.draw_green(location * METERS_TO_PIXELSf + force * METERS_TO_PIXELSf, location * METERS_TO_PIXELSf);
		}
	}

	void physics::apply_impulse(vec2 pixels) {
		apply_impulse(pixels, vec2(0, 0), true);
	}

	void physics::apply_impulse(vec2 pixels, vec2 center_offset, bool wake) {
		vec2 force = pixels * PIXELS_TO_METERSf;
		vec2 location = body->GetWorldCenter() + (center_offset * PIXELS_TO_METERSf);

		body->ApplyLinearImpulse(force, location, true);

		if (renderer::get_current().debug_drawing && force.non_zero()) {
			auto& lines = renderer::get_current().logic_lines;
			lines.draw_green(location * METERS_TO_PIXELSf + force * METERS_TO_PIXELSf, location * METERS_TO_PIXELSf);
		}
	}

	float physics::get_mass() {
		return body->GetMass();
	}

	vec2 physics::get_position() {
		return METERS_TO_PIXELSf * body->GetPosition();
	}

	vec2 physics::velocity() {
		return vec2(body->GetLinearVelocity()) * METERS_TO_PIXELSf;
	}

	vec2 physics::get_world_center() {
		return METERS_TO_PIXELSf * body->GetWorldCenter();
	}

	entity_id physics::get_owner_friction_ground() {
		if (owner_friction_grounds.empty()) return entity_id();

		return owner_friction_grounds[0];
	}

}