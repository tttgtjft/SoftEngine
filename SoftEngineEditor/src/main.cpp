#include "SoftEngineCore/Application.hpp"

#include "SoftEngineCore/Input.hpp"
#include <imgui/imgui.h>
#include <imgui_internal.h>

#include <iostream>
#include <memory>


class SoftEngineEditor : public SoftEngine::Application 
{
	double m_initial_mouse_pos_x = 0.0;
	double m_initial_mouse_pos_y = 0.0;
	float camera_position[3] = { 0.0f, 0.0f, 1.0f };
	float camera_rotation[3] = { 0.0f, 0.0f, 0.0f };
	float camera_fov = 60.f;
	float camera_near_plane = 0.1f;
	float camera_far_plane = 100.f;
	bool b_perspective_camera = true;

	virtual void on_update() override 
	{
		glm::vec3 movement_delta = { 0.0f, 0.0f, 0.0f };
		glm::vec3 rotation_delta = { 0.0f, 0.0f, 0.0f };
		if (SoftEngine::Input::is_key_pressed(SoftEngine::KeyCode::KEY_W))
		{
			movement_delta.x += 0.1f;
		}
		if (SoftEngine::Input::is_key_pressed(SoftEngine::KeyCode::KEY_S))
		{
			movement_delta.x -= 0.1f;
		}
		if (SoftEngine::Input::is_key_pressed(SoftEngine::KeyCode::KEY_A))
		{
			movement_delta.y -= 0.1f;
		}
		if (SoftEngine::Input::is_key_pressed(SoftEngine::KeyCode::KEY_D))
		{
			movement_delta.y += 0.1f;
		}
		if (SoftEngine::Input::is_key_pressed(SoftEngine::KeyCode::KEY_Q))
		{
			movement_delta.z -= 0.1f;
		}
		if (SoftEngine::Input::is_key_pressed(SoftEngine::KeyCode::KEY_E))
		{
			movement_delta.z += 0.1f;
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

	void setup_dockspace_menu()
	{
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode | ImGuiDockNodeFlags_NoWindowMenuButton;
		static ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		window_flags |= ImGuiWindowFlags_NoBackground;

		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->WorkPos);
		ImGui::SetNextWindowSize(viewport->WorkSize);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.f));
		ImGui::Begin("DockSpace", nullptr, window_flags);
		ImGui::PopStyleVar(3);
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("FIle"))
			{
				if (ImGui::MenuItem("New scene...", NULL))
				{

				}
				if (ImGui::MenuItem("Open scene...", NULL))
				{

				}
				if (ImGui::MenuItem("Save scene...", NULL))
				{

				}
				ImGui::Separator();
				if (ImGui::MenuItem("Exit", NULL))
				{
					close();
				}
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		ImGui::End();
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
		setup_dockspace_menu();
		camera_position[0] = camera.get_position().x;
		camera_position[1] = camera.get_position().y;
		camera_position[2] = camera.get_position().z;
		camera_rotation[0] = camera.get_rotation().x;
		camera_rotation[1] = camera.get_rotation().y;
		camera_rotation[2] = camera.get_rotation().z;

		camera_fov = camera.get_field_of_view();
		camera_near_plane = camera.get_near_clip_plane();
		camera_far_plane = camera.get_far_clip_plane();

		ImGui::Begin("Editor");
		ImGui::SliderFloat3("Light source position", light_source_position, -10.f, 10.f);
		ImGui::ColorEdit3("Light source color", light_source_color);
		ImGui::SliderFloat("Ambient factor", &ambient_factor, 0.f, 1.f);
		ImGui::SliderFloat("Diffuse factor", &diffuse_factor, 0.f, 1.f);
		ImGui::SliderFloat("Specular factor", &specular_factor, 0.f, 1.f);
		ImGui::SliderFloat("Shininess", &shininess, 1.f, 128.f);
		if (ImGui::SliderFloat3("Camera position", camera_position, -10.f, 10.f))
		{
			camera.set_position(glm::vec3(camera_position[0], camera_position[1], camera_position[2]));
		}
		if (ImGui::SliderFloat3("Camera rotation", camera_rotation, 0.f, 360.f))
		{
			camera.set_rotation(glm::vec3(camera_rotation[0], camera_rotation[1], camera_rotation[2]));
		}
		if (ImGui::SliderFloat("Camera FOV", &camera_fov, 1.f, 120.f))
		{
			camera.set_field_of_view(camera_fov);
		}
		if (ImGui::SliderFloat("Camera near clip plane", &camera_near_plane, 0.1f, 10.f))
		{
			camera.set_near_clip_plane(camera_near_plane);
		}	
		if (ImGui::SliderFloat("Camera far clip plane", &camera_far_plane, 1.f, 100.f))
		{
			camera.set_far_clip_plane(camera_far_plane);
		}
		if (ImGui::Checkbox("Pespective camera", &b_perspective_camera))
		{
			camera.set_projection_mode(b_perspective_camera ? SoftEngine::Camera::ProjectionMode::Perspective : SoftEngine::Camera::ProjectionMode::Orthographic);
		}
		ImGui::End();
	}

};

int main() {
	auto pSoftEngineEditor = std::make_unique<SoftEngineEditor>();

	int returnCode = pSoftEngineEditor->start(1024, 768, "SoftEngine Editor");

	return returnCode;
}