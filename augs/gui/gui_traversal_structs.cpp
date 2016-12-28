#include "gui_traversal_structs.h"

namespace augs {
	namespace gui {
		draw_info::draw_info(vertex_triangle_buffer& v) : v(v) {}
		event_traversal_flags::event_traversal_flags(const augs::window::event::change change) : change(change) {}
		event_info::event_info(const gui_event msg, const int scroll_amount, vec2i total_dragged_amount) : msg(msg), scroll_amount(scroll_amount), total_dragged_amount(total_dragged_amount) {}

		event_info::operator gui_event() const {
			return msg;
		}
	}
}