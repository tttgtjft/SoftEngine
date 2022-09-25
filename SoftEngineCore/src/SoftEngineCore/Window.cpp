#include <SoftEngineCore/Window.hpp>
#include <SoftEngineCore/Log.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace SoftEngine {

	static bool s_GLFW_initializated = false;

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

        if (!s_GLFW_initializated)
        {
            if (!glfwInit())
            {
                LOG_CRITICAL("Failed to initialize GLFW");
                return -1;
            }
            s_GLFW_initializated = true;
        }

        m_pWindow = glfwCreateWindow(m_data.width, m_data.height, m_data.title.c_str(), nullptr, nullptr);
        if (!m_pWindow)
        {
            LOG_CRITICAL("Failed to create window {0} width size {1}x{2}", m_data.title, m_data.width, m_data.height);
            glfwTerminate();
            return -2;
        }

        glfwMakeContextCurrent(m_pWindow);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            LOG_CRITICAL("Failed to initialize GLAD");
            return -3;
        }

        glfwSetWindowUserPointer(m_pWindow, &m_data);

        glfwSetWindowSizeCallback(m_pWindow,
            [](GLFWwindow* pWindow, int width, int height)
            {
                LOG_INFO("New size {0}x{1}", width, height);

                WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(pWindow));
                data.width = width;
                data.height = height;

                Event event;
                event.width = width;
                event.height = height;
                data.eventCallbackFn(event);
            }
        );

        return 0;
	}

    void Window::shutdown()
    {
        glfwDestroyWindow(m_pWindow);
        glfwTerminate();
    }

	void Window::on_update()
	{
        glClearColor(1, 0, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(m_pWindow);
        glfwPollEvents();
	}

}