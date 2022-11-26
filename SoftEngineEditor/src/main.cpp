#include "SoftEngineCore/Application.hpp"

#include <imgui/imgui.h>

#include <iostream>
#include <memory>


class SoftEngineEditor : public SoftEngine::Application 
{
	virtual void on_update() override 
	{
		//std::cout << "Update frame: " << frame++ << std::endl;
	}

	virtual void on_ui_draw() override
	{
		ImGui::Begin("Editor");
		ImGui::SliderFloat3("camera position", camera_position, -10.f, 10.f);
		ImGui::SliderFloat3("camera rotation", camera_rotation, 0.f, 360.f);
		ImGui::Checkbox("pespective camera", &b_perspective_camera);
		ImGui::End();
	}

	int frame = 0;
};

int main() {
	auto pSoftEngineEditor = std::make_unique<SoftEngineEditor>();

	int returnCode = pSoftEngineEditor->start(1024, 768, "SoftEngine Editor");

	return returnCode;
}