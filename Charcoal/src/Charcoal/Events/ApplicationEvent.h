#pragma once

#include "Event.h"

namespace Charcoal {

	class WindowClosedEvent : public Event
	{
	public:
		WindowClosedEvent() {}
		virtual ~WindowClosedEvent() {}

		GET_TYPE_FUNC(EventType::WindowClosed)
		GET_CATEGORY_FLAGS_FUNC(EventCategory::WindowEvent)
		IS_IN_CATEGORY_FUNC
		inline std::string ToString() const override { return "Window Closed"; }
	};

	class WindowResizedEvent : public Event
	{
	public:
		WindowResizedEvent(int width, int height) : m_Width(width), m_Height(height) {}
		virtual ~WindowResizedEvent() {}

		inline int GetWidth() { return m_Width; }
		inline int GetHeight() { return m_Height; }

		GET_TYPE_FUNC(EventType::WindowResized)
			GET_CATEGORY_FLAGS_FUNC(EventCategory::WindowEvent)
			IS_IN_CATEGORY_FUNC
			inline std::string ToString() const override { return "Window Resized : Width = " + std::to_string(m_Width) + ", Height = " + std::to_string(m_Height); }
	private:
		int m_Width, m_Height;
	};

}