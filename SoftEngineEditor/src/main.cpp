#include "SoftEngineCore/Application.hpp"

#include "SoftEngineCore/Input.hpp"
#include <imgui/imgui.h>

#include <iostream>
#include <memory>


class SoftEngineEditor : public SoftEngine::Application 
{
	virtual void on_update() override 
	{
		if (SoftEngine::Input::is_key_pressed(SoftEngine::KeyCode::KEY_W))
		{
			camera_position[2] -= 0.01f;
		}
		if (SoftEngine::Input::is_key_pressed(SoftEngine::KeyCode::KEY_S))
		{
			camera_position[2] += 0.01f;
		}
		if (SoftEngine::Input::is_key_pressed(SoftEngine::KeyCode::KEY_A))
		{
			camera_position[0] -= 0.01f;
		}
		if (SoftEngine::Input::is_key_pressed(SoftEngine::KeyCode::KEY_D))
		{
			camera_position[0] += 0.01f;
		}
		if (SoftEngine::Input::is_key_pressed(SoftEngine::KeyCode::KEY_Q))
		{
			camera_position[1] += 0.01f;
		}
		if (SoftEngine::Input::is_key_pressed(SoftEngine::KeyCode::KEY_E))
		{
			camera_position[1] -= 0.01f;
		}
		if (SoftEngine::Input::is_key_pressed(SoftEngine::KeyCode::KEY_UP))
		{
			camera_rotation[0] += 0.5f;
		}
		if (SoftEngine::Input::is_key_pressed(SoftEngine::KeyCode::KEY_DOWN))
		{
			camera_rotation[0] -= 0.5f;
		}
		if (SoftEngine::Input::is_key_pressed(SoftEngine::KeyCode::KEY_RIGHT))
		{
			camera_rotation[1] += 0.5f;
		}
		if (SoftEngine::Input::is_key_pressed(SoftEngine::KeyCode::KEY_LEFT))
		{
			camera_rotation[1] -= 0.5f;
		}
	}

	virtual void on_ui_draw() override
	{
		ImGui::Begin("Editor");
		ImGui::SliderFloat3("camera position", camera_position, -10.f, 10.f);
		ImGui::SliderFloat3("camera rotation", camera_rotation, 0.f, 360.f);
		ImGui::Checkbox("pespective camera", &b_perspective_camera);
		ImGui::End();
	}

};

int main() {
	auto pSoftEngineEditor = std::make_unique<SoftEngineEditor>();

	int returnCode = pSoftEngineEditor->start(1024, 768, "SoftEngine Editor");

	return returnCode;
}