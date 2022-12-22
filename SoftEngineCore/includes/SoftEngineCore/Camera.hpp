#pragma once

#include <glm/vec3.hpp>
#include <glm/ext/matrix_float4x4.hpp>

namespace SoftEngine {

	class Camera
	{
	public:
		enum class ProjectionMode
		{
			Perspective,
			Orthographic
		};

		Camera(const glm::vec3& position = { 0, 0, 0 },
			   const glm::vec3& rotation = { 0, 0, 0 },
		 	   const ProjectionMode projection_mode = ProjectionMode::Perspective);

		void set_position(const glm::vec3& position);
		void set_rotation(const glm::vec3& rotation);
		void set_position_rotation(const glm::vec3& position, const glm::vec3& rotation);
		void set_projection_mode(const ProjectionMode projection_mode);
		void set_far_clip_plane(const float far);
		void set_near_clip_plane(const float near);
		void set_viewport_size(const float width, const float height);
		void set_field_of_view(const float fov);

		const glm::vec3& get_position() const { return m_position; }
		const glm::vec3& get_rotation() const { return m_rotation; }
		
		const float get_far_clip_plane() const { return m_far_clip_plane; }
		const float get_near_clip_plane() const { return m_near_clip_plane; }
		const float get_field_of_view() const { return m_field_of_view; }

		const glm::mat4& get_view_matrix();
		const glm::mat4& get_projection_matrix() const { return m_projection_matrix; }

		void move_forward(const float delta);
		void move_right(const float delta);
		void move_up(const float delta);

		void add_movement_rotation(const glm::vec3& movement_delta, const glm::vec3& rotation_delta);

	private:
		void update_view_matrix();
		void update_projection_matrix();

		glm::vec3 m_position;
		glm::vec3 m_rotation; //x - roll, y - pitch, z - yaw

		glm::vec3 m_direction;
		glm::vec3 m_right;
		glm::vec3 m_up;
		float m_near_clip_plane = 0.1f;
		float m_far_clip_plane = 100.f;	
		float m_viewport_width = 800.f;
		float m_viewport_height = 600.f;
		float m_field_of_view = 60.f;

		static constexpr glm::vec3 s_world_up = { 0.0f, 0.0f, 1.0f };
		static constexpr glm::vec3 s_world_right = { 0.0f, -1.0f, 0.0f };
		static constexpr glm::vec3 s_world_forward = { 1.0f, 0.0f, 0.0f };

		glm::mat4 m_view_matrix;
		glm::mat4 m_projection_matrix;

		ProjectionMode m_projection_mode;
		bool m_update_view_matrix = false;
	};

}