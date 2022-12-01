#include <SoftEngineCore/Input.hpp>

namespace SoftEngine {

	bool Input::m_keys_pressed[static_cast<size_t>(KeyCode::KEY_LAST)] = {};

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

}