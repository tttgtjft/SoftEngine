#pragma once

#include "Keys.hpp"

namespace SoftEngine {

	class Input
	{
	public:
		static bool is_key_pressed(const KeyCode key_code);
		static void press_key(const KeyCode key_code);
		static void release_key(const KeyCode key_code);
		 
		static bool is_mouse_button_pressed(const MouseButton mouse_button);
		static void press_mouse_button(const MouseButton mouse_button);
		static void release_mouse_button(const MouseButton mouse_button);

	private:
		static bool m_keys_pressed[];
		static bool m_mouse_button_pressed[];
	};

}