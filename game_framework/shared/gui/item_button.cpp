#include "item_button.h"
#include "gui/stroke.h"

#include "game_framework/shared/inventory_slot.h"
#include "game_framework/shared/inventory_utils.h"
#include "game_framework/components/gui_element_component.h"

item_button::item_button(rects::xywh<float> rc) : rect(rc) {
	clip = false;
	scrollable = false;
	focusable = false;
}

void item_button::draw_triangles(draw_info in) {
	draw_children(in);

	if (is_inventory_root())
		return;

	rgba inside_col = cyan;
	rgba border_col = cyan;

	inside_col.a = 20;
	border_col.a = 220;
	
	if (detector.is_hovered || detector.current_appearance == decltype(detector)::appearance::pushed) {
		inside_col.a = 40;
		border_col.a = 255;
	}

	draw_stretched_texture(in, augs::gui::material(assets::texture_id::BLANK, inside_col));
	augs::gui::solid_stroke stroke;
	stroke.set_material(augs::gui::material(assets::texture_id::BLANK, border_col));
	stroke.draw(in.v, *this);
}

bool item_button::is_inventory_root() {
	return item == gui_element_entity;
}

void item_button::perform_logic_step(augs::gui::gui_world& gr) {
	rect::perform_logic_step(gr);
	vec2i parent_position;

	//if (slot_id.container_entity->find<components::item>()) {
	//
	//}
	//
	//auto gridded_offset = user_drag_offset;
	//gridded_offset /= 10;
	//gridded_offset *= 10;
	//
	//rc.set_position(parent_position + slot_relative_pos + gridded_offset);
}

void item_button::consume_gui_event(event_info info) {
	detector.update_appearance(info);

	if (info == rect::gui_event::ldrag) {
		//user_drag_offset = (info.owner.state.mouse.pos - slot_relative_pos - info.owner.ldrag_relative_anchor);
	}
}


item_button& get_meta(augs::entity_id id) {
	return get_owning_transfer_capability(id)->get<components::gui_element>().item_metadata[id];
}
