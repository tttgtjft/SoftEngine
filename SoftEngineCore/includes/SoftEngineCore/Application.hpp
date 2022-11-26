#pragma once

#include "SoftEngineCore/Event.hpp"
#include "SoftEngineCore/Camera.hpp"

#include <memory>

namespace SoftEngine {

	class Application 
	{
	public:
		Application();
		virtual ~Application();

		Application(const Application&) = delete;
		Application(Application&&) = delete;
		Application& operator=(const Application&) = delete;
		Application& operator=(Application&&) = delete;

		virtual int start(unsigned int window_weight, unsigned int window_height, const char* title);

		virtual void on_update() {}
		virtual void on_ui_draw() {}

		float camera_position[3] = { 0.0f, 0.0f, 1.0f };
		float camera_rotation[3] = { 0.0f, 0.0f, 0.0f };
		bool b_perspective_camera = false;
		Camera camera;

	private:
		std::unique_ptr<class Window> m_pWindow;

		EventDispatcher m_event_dispatcher;
		bool m_bCloseWindow = false;
	};

}