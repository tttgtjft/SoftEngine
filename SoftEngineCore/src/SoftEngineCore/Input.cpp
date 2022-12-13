#include <SoftEngineCore/Input.hpp>

namespace SoftEngine {

	bool Input::m_keys_pressed[static_cast<size_t>(KeyCode::KEY_LAST) + 1] = {};
	bool Input::m_mouse_button_pressed[static_cast<size_t>(MouseButton::MOUSE_BUTTON_LAST) + 1] = {};

	bool Input::is_key_pressed(const KeyCode key_code)
	{
		return m_keys_pressed[static_cast<size_t>(key_code)];
	}

	void Input::press_key(const KeyCode key_code)
	{
		m_keys_pressed[static_cast<size_t>(key_code)] = true;
	}

	void Input::release_key(const KeyCode key_code)
	{
		m_keys_pressed[static_cast<size_t>(key_code)] = false;
	}

	bool Input::is_mouse_button_pressed(const MouseButton mouse_button)
	{
		return m_mouse_button_pressed[static_cast<size_t>(mouse_button)];
	}

	void Input::press_mouse_button(const MouseButton mouse_button)
	{
		m_mouse_button_pressed[static_cast<size_t>(mouse_button)] = true;
	}

	void Input::release_mouse_button(const MouseButton mouse_button)
	{
		m_mouse_button_pressed[static_cast<size_t>(mouse_button)] = false;
	}

}