#include "SoftEngineCore/Window.hpp"

#include "SoftEngineCore/Log.hpp"
#include "SoftEngineCore/Modules/UIModule.hpp"
#include "SoftEngineCore/Rendering/OpenGL/Renderer_OpenGL.hpp"

#include <GLFW/glfw3.h>
#include <imgui/imgui.h>

namespace SoftEngine {

	Window::Window(std::string title, const unsigned int width, const unsigned int height)
		: m_data({std::move(title), width, height})
	{
		int resultCode = init();
	}

	Window::~Window()
	{
		shutdown();
	}

	int Window::init()
	{
        LOG_INFO("Creating window {0} width size {1}x{2}", m_data.title, m_data.width, m_data.height);

        glfwSetErrorCallback([](int error_code, const char* description)
            {
                LOG_CRITICAL("GLFW error: {0}", description);
            });

        if (!glfwInit())
        {
            LOG_CRITICAL("Failed to initialize GLFW");
            return -1;
        }

        m_pWindow = glfwCreateWindow(m_data.width, m_data.height, m_data.title.c_str(), nullptr, nullptr);
        if (!m_pWindow)
        {
            LOG_CRITICAL("Failed to create window {0} width size {1}x{2}", m_data.title, m_data.width, m_data.height);
            return -2;
        }

        if (!Renderer_OpenGl::init(m_pWindow))
        {
            LOG_CRITICAL("Failed to initialize OpenGL renderer");
            return -3;
        }

        glfwSetWindowUserPointer(m_pWindow, &m_data); 

        glfwSetWindowSizeCallback(m_pWindow,
            [](GLFWwindow* pWindow, int width, int height)
            {
                WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(pWindow));
                data.width = width;
                data.height = height;
                EventWindowResize event(width, height);
                data.eventCallbackFn(event);
            }
        );

        glfwSetCursorPosCallback(m_pWindow,
            [](GLFWwindow* pWindow, double x, double y)
            {
                WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(pWindow));
                EventMouseMoved event(x, y);
                data.eventCallbackFn(event);
            }
        );

        glfwSetWindowCloseCallback(m_pWindow,
            [](GLFWwindow* pWindow)
            {
                WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(pWindow));
                EventWindowClose event;
                data.eventCallbackFn(event);
            }
        );

        glfwSetFramebufferSizeCallback(m_pWindow,
            [](GLFWwindow* pWindow, int width, int height)
            {
                Renderer_OpenGl::set_viewport(width, height);
            }
        );

        UIModule::on_window_create(m_pWindow);  
        
        return 0;
	}

    void Window::shutdown()
    {
        UIModule::on_window_close();
        glfwDestroyWindow(m_pWindow);
        glfwTerminate();
    }

	void Window::on_update()
	{
        glfwSwapBuffers(m_pWindow);
        glfwPollEvents();
	}

}