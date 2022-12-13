#include "SoftEngineCore/Application.hpp"

#include "SoftEngineCore/Input.hpp"
#include <imgui/imgui.h>

#include <iostream>
#include <memory>


class SoftEngineEditor : public SoftEngine::Application 
{
	double m_initial_mouse_pos_x = 0.0;
	double m_initial_mouse_pos_y = 0.0;

	virtual void on_update() override 
	{
		glm::vec3 movement_delta = { 0.0f, 0.0f, 0.0f };
		glm::vec3 rotation_delta = { 0.0f, 0.0f, 0.0f };
		if (SoftEngine::Input::is_key_pressed(SoftEngine::KeyCode::KEY_W))
		{
			movement_delta.x += 0.01f;
		}
		if (SoftEngine::Input::is_key_pressed(SoftEngine::KeyCode::KEY_S))
		{
			movement_delta.x -= 0.01f;
		}
		if (SoftEngine::Input::is_key_pressed(SoftEngine::KeyCode::KEY_A))
		{
			movement_delta.y -= 0.01f;
		}
		if (SoftEngine::Input::is_key_pressed(SoftEngine::KeyCode::KEY_D))
		{
			movement_delta.y += 0.01f;
		}
		if (SoftEngine::Input::is_key_pressed(SoftEngine::KeyCode::KEY_Q))
		{
			movement_delta.z -= 0.01f;
		}
		if (SoftEngine::Input::is_key_pressed(SoftEngine::KeyCode::KEY_E))
		{
			movement_delta.z += 0.01f;
		}
		if (SoftEngine::Input::is_key_pressed(SoftEngine::KeyCode::KEY_UP))
		{
			rotation_delta.y -= 0.5f;
		}
		if (SoftEngine::Input::is_key_pressed(SoftEngine::KeyCode::KEY_DOWN))
		{
			rotation_delta.y += 0.5f;
		}
		if (SoftEngine::Input::is_key_pressed(SoftEngine::KeyCode::KEY_RIGHT))
		{
			rotation_delta.z -= 0.5f;
		}
		if (SoftEngine::Input::is_key_pressed(SoftEngine::KeyCode::KEY_LEFT))
		{
			rotation_delta.z += 0.5f;
		}
		if (SoftEngine::Input::is_key_pressed(SoftEngine::KeyCode::KEY_P))
		{
			rotation_delta.x -= 0.5f;
		}
		if (SoftEngine::Input::is_key_pressed(SoftEngine::KeyCode::KEY_O))
		{
			rotation_delta.x += 0.5f;
		}

		if (SoftEngine::Input::is_mouse_button_pressed(SoftEngine::MouseButton::MOUSE_BUTTON_RIGHT))
		{
			glm::vec2 current_cursor_position = get_current_cursor_position();
			if (SoftEngine::Input::is_mouse_button_pressed(SoftEngine::MouseButton::MOUSE_BUTTON_LEFT))
			{
				camera.move_right(static_cast<float>(-(m_initial_mouse_pos_x - current_cursor_position.x)) / 100.f);
				camera.move_up(static_cast<float>((m_initial_mouse_pos_y - current_cursor_position.y)) / 100.f);
			}
			else
			{
				rotation_delta.z += static_cast<float>(m_initial_mouse_pos_x - current_cursor_position.x) / 5.f;
				rotation_delta.y -= static_cast<float>(m_initial_mouse_pos_y - current_cursor_position.y) / 5.f;
			}
			m_initial_mouse_pos_x = current_cursor_position.x;
			m_initial_mouse_pos_y = current_cursor_position.y;
		}

		camera.add_movement_rotation(movement_delta, rotation_delta);
	}

	virtual void on_mouse_button_event(const SoftEngine::MouseButton button_code, 
									   const double x_pos,
									   const double y_pos, 
									   const bool pressed) override
	{
		m_initial_mouse_pos_x = x_pos;
		m_initial_mouse_pos_y = y_pos;
	}

	virtual void on_ui_draw() override
	{
		camera_position[0] = camera.get_camera_position().x;
		camera_position[1] = camera.get_camera_position().y;
		camera_position[2] = camera.get_camera_position().z;
		camera_rotation[0] = camera.get_camera_rotation().x;
		camera_rotation[1] = camera.get_camera_rotation().y;
		camera_rotation[2] = camera.get_camera_rotation().z;

		ImGui::Begin("Editor");
		if (ImGui::SliderFloat3("camera position", camera_position, -10.f, 10.f))
		{
			camera.set_position(glm::vec3(camera_position[0], camera_position[1], camera_position[2]));
		}
		if (ImGui::SliderFloat3("camera rotation", camera_rotation, 0.f, 360.f))
		{
			camera.set_rotation(glm::vec3(camera_rotation[0], camera_rotation[1], camera_rotation[2]));
		}
		ImGui::Checkbox("pespective camera", &b_perspective_camera);
		ImGui::End();
	}

};

int main() {
	auto pSoftEngineEditor = std::make_unique<SoftEngineEditor>();

	int returnCode = pSoftEngineEditor->start(1024, 768, "SoftEngine Editor");

	return returnCode;
}