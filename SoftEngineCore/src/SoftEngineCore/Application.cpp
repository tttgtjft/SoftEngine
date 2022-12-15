#include "SoftEngineCore/Application.hpp"

#include "SoftEngineCore/Log.hpp"
#include "SoftEngineCore/Window.hpp"
#include "SoftEngineCore/Event.hpp"
#include "SoftEngineCore/Modules/UIModule.hpp"
#include "SoftEngineCore/Input.hpp"

#include "SoftEngineCore/Rendering/OpenGL/ShaderProgram.hpp"
#include "SoftEngineCore/Rendering/OpenGL/VertexBuffer.hpp"
#include "SoftEngineCore/Rendering/OpenGL/VertexArray.hpp"
#include "SoftEngineCore/Rendering/OpenGL/Renderer_OpenGL.hpp"
#include "SoftEngineCore/Camera.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/trigonometric.hpp>
#include <stb/stb_image.h>

#include <iostream>
#define STB_IMAGE_IMPLEMENTATION

namespace SoftEngine {

	GLfloat positions_colors[] = {
		0.0f, -0.5f, -0.5f,   1.0f, 1.0f, 0.0f,	  2.f, 2.f,
		0.0f,  0.5f, -0.5f,   0.0f, 1.0f, 1.0f,	  0.f, 2.f,
		0.0f, -0.5f,  0.5f,   1.0f, 0.0f, 1.0f,	  2.f, 0.f,
		0.0f,  0.5f,  0.5f,   1.0f, 0.0f, 0.0f,	  0.f, 0.f
	};

	GLint indices[] = {
		0, 1, 2, 3, 2, 1
	};

	const char* vertex_shader =
		R"(#version 460
        layout(location = 0) in vec3 vertex_position;
        layout(location = 1) in vec3 vertex_color;
        layout(location = 2) in vec2 texture_coord;

        uniform mat4 model_matrix;
        uniform mat4 view_projection_matrix;
		uniform int current_frame;

        out vec3 color;
		out vec2 tex_coord_background;
		out vec2 tex_coord_github;

        void main() {
           color = vertex_color;
		   tex_coord_background = texture_coord;
		   tex_coord_github = texture_coord + vec2(current_frame / 1000.f, current_frame / 1000.f);
           gl_Position = view_projection_matrix * model_matrix * vec4(vertex_position, 1.0);
        })";

	const char* fragment_shader =
		R"(#version 460
        in vec3 color;
		in vec2 tex_coord_background;
		in vec2 tex_coord_github;

		layout(binding = 0) uniform sampler2D InTexture_Background;
		layout(binding = 1) uniform sampler2D InTexture_Github;

        out vec4 frag_color;

        void main() {
           //frag_color = vec4(color, 1.0);
		   frag_color = texture(InTexture_Background, tex_coord_background) * texture(InTexture_Github, tex_coord_github);
        })";

	std::unique_ptr<ShaderProgram> p_shader_program;

	std::unique_ptr<VertexBuffer> p_positions_colors_vbo;
	std::unique_ptr<IndexBuffer> p_index_buffer;
	std::unique_ptr<VertexArray> p_vao;

	float scale[3] = { 1.f, 1.f, 1.f };
	float rotate = 0.f;
	float translate[3] = { 0.f, 0.f, 0.f };
	float m_background_color[4] = { 0.45f, 0.45f, 0.45f, 1.f };

	Application::Application()
	{	
		LOG_INFO("Starting Application");
	}

	Application::~Application()
	{
		LOG_INFO("Closing Application");
	}

	int Application::start(unsigned int window_width, unsigned int window_height, const char* title)
	{
		m_pWindow = std::make_unique<Window>(title, window_width, window_height);

		m_event_dispatcher.add_event_listener<EventMouseMoved>(
			[](EventMouseMoved& event)
			{
				LOG_INFO("[MouseMoved] Mouse moved to {0}x{1}", event.x, event.y);
			}
		);

		m_event_dispatcher.add_event_listener<EventWindowResize>(
			[](EventWindowResize& event)
			{
				LOG_INFO("[Resized] Changed size to {0}x{1}", event.width, event.height);
			}
		);

		m_event_dispatcher.add_event_listener<EventWindowClose>(
			[&](EventWindowClose& event)
			{
				LOG_INFO("[WindowClose]");
				m_bCloseWindow = true;
			}
		);

		m_event_dispatcher.add_event_listener<EventMouseButtonPressed>(
			[&](EventMouseButtonPressed& event)
			{
				LOG_INFO("[Mouse button pressed: {0}, at ({1}, {2})]", static_cast<size_t>(event.mouse_buttom), event.x_pos, event.y_pos);
				Input::press_mouse_button(event.mouse_buttom);
				on_mouse_button_event(event.mouse_buttom, event.x_pos, event.y_pos, true);
			}
		);

		m_event_dispatcher.add_event_listener<EventMouseButtonReleased>(
			[&](EventMouseButtonReleased& event)
			{
				LOG_INFO("[Mouse button released: {0}, at ({1}, {2})]", static_cast<size_t>(event.mouse_buttom), event.x_pos, event.y_pos);
				Input::release_mouse_button(event.mouse_buttom);
				on_mouse_button_event(event.mouse_buttom, event.x_pos, event.y_pos, false);
			}
		);

		m_event_dispatcher.add_event_listener<EventKeyPressed>(
			[&](EventKeyPressed& event)
			{
				if (event.repeated)
				{
					LOG_INFO("[Key pressed: {0}, repeated]", static_cast<char>(event.key_code));
				}
				else
				{
					LOG_INFO("[Key pressed: {0}]", static_cast<char>(event.key_code));
				}
				Input::press_key(event.key_code);
			}
		);

		m_event_dispatcher.add_event_listener<EventKeyReleased>(
			[&](EventKeyReleased& event)	
			{
				LOG_INFO("[Key released: {0}]", static_cast<char>(event.key_code));
				Input::release_key(event.key_code);
			}
		);

		m_pWindow->set_event_callback(
			[&](BaseEvent& event)
			{
				m_event_dispatcher.dispatch(event);
			}
		);

		int width;
		int height;
		int channels;
		unsigned char* data = stbi_load("../../../container.jpg", &width, &height, &channels, 0);

		GLuint textureHandle_Background;
		glCreateTextures(GL_TEXTURE_2D, 1, &textureHandle_Background);
		glTextureStorage2D(textureHandle_Background, 1, GL_RGB8, width, height);
		glTextureSubImage2D(textureHandle_Background, 0, 0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);

		glTextureParameteri(textureHandle_Background, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTextureParameteri(textureHandle_Background, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(textureHandle_Background, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(textureHandle_Background, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBindTextureUnit(0, textureHandle_Background);
		
		width = 0;
		height = 0;
		channels = 0;
		unsigned char* data_ = stbi_load("../../../pubg.jpg", &width, &height, &channels, 0);
	
		GLuint textureHandle_Github;
		glCreateTextures(GL_TEXTURE_2D, 1, &textureHandle_Github);
		glTextureStorage2D(textureHandle_Github, 1, GL_RGB8, width, height);
		glTextureSubImage2D(textureHandle_Github, 0, 0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, data_);

		glTextureParameteri(textureHandle_Github, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTextureParameteri(textureHandle_Github, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(textureHandle_Github, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(textureHandle_Github, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBindTextureUnit(1, textureHandle_Github);
	
		delete[] data;
		delete[] data_;

		//-------------------------------------------------//
		p_shader_program = std::make_unique<ShaderProgram>(vertex_shader, fragment_shader);
		if (!p_shader_program->is_compiled())
		{
			return false;
		}

		BufferLayout buffer_layout_vec3_vec3_vec2
		{
			ShaderDataType::Float3,
			ShaderDataType::Float3,
			ShaderDataType::Float2
		};

		p_positions_colors_vbo = std::make_unique<VertexBuffer>(positions_colors, sizeof(positions_colors), buffer_layout_vec3_vec3_vec2);
		p_index_buffer = std::make_unique<IndexBuffer>(indices, sizeof(indices) / sizeof(GLint));
		p_vao = std::make_unique<VertexArray>();

		p_vao->add_vertex_buffer(*p_positions_colors_vbo);
		p_vao->set_index_buffer(*p_index_buffer);
		//-------------------------------------------------//

		int current_frame = 0;

		while (!m_bCloseWindow)
		{
			Renderer_OpenGl::set_clear_color(m_background_color[0], m_background_color[1], m_background_color[2], m_background_color[3]);
			Renderer_OpenGl::clear();

			p_shader_program->bind();

			glm::mat4 scale_matrix(scale[0], 0, 0, 0,
				0, scale[1], 0, 0,
				0, 0, scale[2], 0,
				0, 0, 0, 1);
			float rotate_in_radians = glm::radians(rotate);
			glm::mat4 rotate_matrix(cos(rotate_in_radians), sin(rotate_in_radians), 0, 0,
				-sin(rotate_in_radians), cos(rotate_in_radians), 0, 0,
				0, 0, 1, 0,
				0, 0, 0, 1);

			glm::mat4 translate_matrix(1, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, 1, 0,
				translate[0], translate[1], translate[2], 1);

			glm::mat4 model_matrix = translate_matrix * rotate_matrix * scale_matrix;
			p_shader_program->set_matrix4("model_matrix", model_matrix);
			p_shader_program->set_int("current_frame", current_frame++);

			camera.set_projection_mode(b_perspective_camera ? Camera::ProjectionMode::Perspective : Camera::ProjectionMode::Orthographic);
			p_shader_program->set_matrix4("view_projection_matrix", camera.get_projection_matrix() * camera.get_view_matrix());

			Renderer_OpenGl::draw(*p_vao);

			//-------------------------------------------------//
			UIModule::on_ui_draw_begin();

			bool show;
			UIModule::ShowExampleAppDockSpace(&show);
			ImGui::ShowDemoWindow();

			ImGui::Begin("Background Color Window");
			ImGui::ColorEdit4("Background Color", m_background_color);
			ImGui::SliderFloat3("scale", scale, 0.f, 2.f);
			ImGui::SliderFloat("rotate", &rotate, 0.f, 360.f);
			ImGui::SliderFloat3("translate", translate, -1.f, 1.f);
			ImGui::End();

			on_ui_draw();

			UIModule::on_ui_draw_end();
			//-------------------------------------------------//

			m_pWindow->on_update();
			on_update();
		}
		glDeleteTextures(1, &textureHandle_Background);
		glDeleteTextures(1, &textureHandle_Github);

		m_pWindow = nullptr;

		return 0;
	}

	glm::vec2 Application::get_current_cursor_position() const
	{
		return m_pWindow->get_current_cursor_position();
	}

}