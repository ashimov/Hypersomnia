#pragma once
#include "stdafx.h"
#include "bindings.h"

#include "../resources/scriptable_info.h"

namespace bindings {
	luabind::scope _script() {
		return
			luabind::class_<resources::lua_state_wrapper>("lua_state_wrapper"),

			luabind::class_<script>("script")
			.def(luabind::constructor<resources::lua_state_wrapper&>())
			.def("associate_filename", (void (script::*)(const std::string&))&script::associate_filename)
			.def("call", &script::call);
	}
}