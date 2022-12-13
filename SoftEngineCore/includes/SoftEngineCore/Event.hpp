#pragma once

#include "Keys.hpp"

#include <array>
#include <functional>

namespace SoftEngine {

	enum class EventType
	{
		WindowResize = 0,
		WindowClose,

		KeyPressed,
		KeyReleased,

		MouseButtonPressed,
		MouseButtonReleased,
		MouseMoved,


		EventCount
	};

	struct BaseEvent
	{
		virtual ~BaseEvent() = default;
		virtual EventType get_type() const = 0;
	};

	class EventDispatcher
	{
	public:
		template<typename EventType>
		void add_event_listener(std::function<void(EventType&)> callback)
		{
			auto baseCallback = [func = std::move(callback)](BaseEvent& e)
			{
				if (e.get_type() == EventType::type)
				{
					func(static_cast<EventType&>(e));
				}
			};
			
			m_eventCallbacks[static_cast<size_t>(EventType::type)] = std::move(baseCallback);
		}

		void dispatch(BaseEvent& event)
		{
			auto& callback = m_eventCallbacks[static_cast<size_t>(event.get_type())];
			if (callback)
			{
				callback(event);
			}
		}

	private:
		std::array<std::function<void(BaseEvent&)>, static_cast<size_t>(EventType::EventCount)> m_eventCallbacks;
	};

	struct EventMouseMoved : public BaseEvent
	{
		EventMouseMoved(const double new_x, const double new_y)
			: x(new_x)
			, y(new_y)
		{
		}

		EventType get_type() const override
		{
			return type;
		}

		double x;
		double y;

		static const EventType type = EventType::MouseMoved;
	};

	struct EventWindowResize : public BaseEvent
	{
		EventWindowResize(const unsigned new_width, const unsigned new_height)
			: width(new_width)
			, height(new_height)
		{
		}

		EventType get_type() const override
		{
			return type;
		}

		unsigned width;
		unsigned height;

		static const EventType type = EventType::WindowResize;
	};

	struct EventWindowClose : public BaseEvent
	{
		EventType get_type() const override
		{
			return type;
		}

		static const EventType type = EventType::WindowClose;
	};

	struct EventKeyPressed : public BaseEvent
	{
		EventKeyPressed(const KeyCode key_code, const bool repeated)
			: key_code(key_code)
			, repeated(repeated)
		{
		}

		EventType get_type() const override
		{
			return type;
		}

		KeyCode key_code;
		bool repeated;

		static const EventType type = EventType::KeyPressed;
	};

	struct EventKeyReleased : public BaseEvent
	{
		EventKeyReleased(const KeyCode key_code)
			: key_code(key_code)
		{
		}

		EventType get_type() const override
		{
			return type;
		}

		KeyCode key_code;

		static const EventType type = EventType::KeyReleased;
	};

	struct EventMouseButtonPressed : public BaseEvent
	{
		EventMouseButtonPressed(const MouseButton mouse_buttom, const double x_pos, const double y_pos)
			: mouse_buttom(mouse_buttom)
			, x_pos(x_pos)
			, y_pos(y_pos)
		{
		}

		EventType get_type() const override
		{
			return type;
		}

		MouseButton mouse_buttom;
		const double x_pos;
		const double y_pos;

		static const EventType type = EventType::MouseButtonPressed;
	};

	struct EventMouseButtonReleased : public BaseEvent
	{
		EventMouseButtonReleased(const MouseButton mouse_buttom, const double x_pos, const double y_pos)
			: mouse_buttom(mouse_buttom)
			, x_pos(x_pos)
			, y_pos(y_pos)
		{
		}

		EventType get_type() const override
		{
			return type;
		}

		MouseButton mouse_buttom;
		const double x_pos;
		const double y_pos;

		static const EventType type = EventType::MouseButtonReleased;
	};

}