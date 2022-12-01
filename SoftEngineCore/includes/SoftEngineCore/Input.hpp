#pragma once

#include "Keys.hpp"

namespace SoftEngine {

	class Input
	{
	public:
		static bool is_key_pressed(const KeyCode key_code);
		static void press_key(const KeyCode key_code);
		static void release_key(const KeyCode key_code);
	private:
		static bool m_keys_pressed[static_cast<size_t>(KeyCode::KEY_LAST)];
	};

}