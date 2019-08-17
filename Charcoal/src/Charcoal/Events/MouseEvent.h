#pragma once

#include "Event.h"

#include <utility>

namespace Charcoal
{

	class MouseButtonEvent : public Event
	{

	public:
		inline int GetMouseButtonCode() const { return  m_MouseButtonCode; }

	protected:
		MouseButtonEvent(int& MouseButtonCode) : m_MouseButtonCode(MouseButtonCode) {}
		virtual ~MouseButtonEvent() {}
		int m_MouseButtonCode;

	};

	class MouseButtonPressedEvent : public MouseButtonEvent
	{

	public:
		MouseButtonPressedEvent(int MouseButtonCode) : MouseButtonEvent(MouseButtonCode) {}
		~MouseButtonPressedEvent() {}

		inline std::string ToString() const override { return "Mouse Button Pressed: " + std::to_string(m_MouseButtonCode); }

		GET_TYPE_FUNC(EventType::MouseButtonPressed)
		GET_CATEGORY_FLAGS_FUNC(EventCategory::InputEvent | EventCategory::MouseInputEvent)
		IS_IN_CATEGORY_FUNC

	};

	class MouseButtonReleasedEvent : public MouseButtonEvent
	{

	public:
		MouseButtonReleasedEvent(int MouseButtonCode) : MouseButtonEvent(MouseButtonCode) {}
		~MouseButtonReleasedEvent() {}

		inline std::string ToString() const override { return "Mouse Button Released: " + std::to_string(m_MouseButtonCode); }

		GET_TYPE_FUNC(EventType::MouseButtonReleased)
		GET_CATEGORY_FLAGS_FUNC(EventCategory::InputEvent | EventCategory::MouseInputEvent)
		IS_IN_CATEGORY_FUNC

	};



	class MouseScrolledEvent : public Event
	{

	public:
		MouseScrolledEvent(float value) : m_Value(value) {}
		~MouseScrolledEvent() {}

		inline float GetValue() const { return m_Value; }

		inline std::string ToString() const override { return "Mouse Scrolled: " + std::to_string(m_Value); }

		GET_TYPE_FUNC(EventType::MouseScrolled)
		GET_CATEGORY_FLAGS_FUNC(EventCategory::InputEvent | EventCategory::MouseInputEvent)
		IS_IN_CATEGORY_FUNC

	private:
		float m_Value;

	};

	class MouseMovedEvent : public Event
	{

	public:
		MouseMovedEvent(float xPos, float yPos) : m_XPos(xPos), m_YPos(yPos) {}
		~MouseMovedEvent() {}

		inline float GetX() const { return m_XPos; }
		inline float GetY() const { return m_YPos; }
		inline std::pair<float, float> GetPos() const { return std::pair(m_XPos, m_YPos); }

		inline std::string ToString() const override { return "Mouse Moved: XPos = " + std::to_string(m_XPos) + ", YPos = " + std::to_string(m_YPos); }

		GET_TYPE_FUNC(EventType::MouseMoved)
		GET_CATEGORY_FLAGS_FUNC(EventCategory::InputEvent | EventCategory::MouseInputEvent)
		IS_IN_CATEGORY_FUNC

	private:
		float m_XPos, m_YPos;

	};

}