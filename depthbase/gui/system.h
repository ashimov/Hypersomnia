#pragma once
#include <vector>
#include "../../math/math.h"
#include "../../event.h"
#include "../../window/timer.h"
#include "rect.h"
/* window - defined as the rect that fetches events to only itself 

todo:
w textboxie poprawic przeciaganie zaznaczen double/triple clickow
WYDZIELIC W SYSTEMIE OUTPUT GUI_RECT I BACKEND QUAD'OWY
struct {
gui_rect caret;
vector<gui_rect> characters;
vector<gui_rect> selections;
}
i tak dalej
pros:
- kazdy draw_proc generuje elementy do wyrysowania wedlug dowolnej kolejnosci => prostszy kod
cons:
- wincyj pamieci zuzytej w kazdym rect'cie

z bboxami ma byc tak jak jest bo zauwaz ze bboxa rozpychaja tylko pierwsze child recty a nie ich wlasne bboxy
BBOX JEST GLOBALNY!!! twuj stary jest globalny, teraz sa lokalne


*/

namespace db {
	namespace graphics {
		extern window::fpstimer fps;
		namespace gui {
			struct font;
			using namespace math;
			extern io::input::texture* null_texture;
			namespace text {
				struct formatted_char {
					font* font_used;
					wchar_t c;
					unsigned char r, g, b, a;
					void set(wchar_t, font* = 0, const pixel_32& = pixel_32());
					void set(font* = 0, const pixel_32& = pixel_32());
				};

				struct style {
					font* f;
					pixel_32 color;
					style(font* = nullptr, pixel_32 = pixel_32());
					style(const formatted_char&);
					operator formatted_char();
				};

				typedef std::basic_string<formatted_char> fstr;
			}

			class system {
				bool 
					/* own_copy indicates whether the clipboard_change event comes from manual "copy_clipboard" or from external source */
					own_copy, 
					own_clip;
			public:
				event::state& events;
				text::fstr clipboard;

				system(event::state& subscribe_events);

				void change_clipboard();
				void copy_clipboard(text::fstr&);
				bool fetch_clipboard, is_clipboard_own();
			};

			class group {
				rect* focus;
			public:
				struct {
					material mat;
					rect_wh size;
					point pos;
					rect* subject;
					float speed_mult;
				} middlescroll;
				
				system& owner;
				rect *lholded, *rholded;

				rect root;
				std::vector<quad> quad_array;

				group(system& owner);

				void set_focus(rect*);
				rect* get_focus() const;

				void update_rectangles();
				void poll_events      ();
				void call_updaters    ();
				void update_array     ();
				void default_update   ();
				void draw_gl_fixed();
			};
				
			
			namespace text {
				extern fstr format(const std::wstring&, style);
				extern void format(const std::wstring&, style, fstr&);
				extern void format(const wchar_t*, style, fstr&);
				extern fstr format(const wchar_t*, style);
			}

			extern void paste_clipboard(text::fstr& out, text::formatted_char = text::formatted_char());
		}
	}
	namespace misc {
		extern std::wstring wstr(const graphics::gui::text::fstr& f);
		template <class T>
		T wnum(const graphics::gui::text::fstr& f) {
			std::wistringstream ss(wstr(f));
			T v;
			ss >> v;
			return v;
		}
		
	}
}

