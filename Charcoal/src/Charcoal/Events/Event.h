#pragma once

#include <chpch.h>

#include "Charcoal/Core/Core.h"

namespace Charcoal
{

	enum EventType
	{
		Unknown = 0,
		KeyPressed, KeyReleased, KeyTyped,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled,
		WindowResized, WindowClosed
	};

	enum EventCategory
	{
		InputEvent        = BIT(1),
		KeyInputEvent     = BIT(2),
		MouseInputEvent   = BIT(3),
		ApplicationEvent  = BIT(4),
		WindowEvent       = BIT(5)
	};

	class Event
	{

	public:
		virtual inline EventType GetType() const  = 0;
		virtual inline int GetCategoryFlags() const = 0;
		virtual inline bool IsInCategory(int category) const = 0;
		virtual inline std::string ToString() const = 0;

		inline bool Handled() { return m_Handled; };
		virtual ~Event() {};

	public:
		bool m_Handled;

	protected:
		Event() : m_Handled(false) {};

	};

	class EventDispatcher
	{

		template<typename T>
		using EventFn = std::function<bool(T&)>;
		
	public:
		EventDispatcher(Event& event) : m_Event(event) {}
		~EventDispatcher() {}

		template<typename T>
		bool Dispatch(EventFn<T> func)
		{
			if (m_Event.GetType() == T::GetStaticType())
			{
				m_Event.m_Handled = func(*(T*)& m_Event);
				return true;
			}
			return false;
		}

	private:
		Event& m_Event;

	};

	inline std::ostream& operator<<(std::ostream& os, const Event& event)
	{
		return os << event.ToString();
	}

}

#define GET_TYPE_FUNC(x) inline static EventType GetStaticType() { return x; } \
inline EventType GetType() const override { return GetStaticType(); }

#define GET_CATEGORY_FLAGS_FUNC(x) inline static int GetStaticCategoryFlags() { return x; } \
inline int GetCategoryFlags() const override { return GetStaticCategoryFlags(); }

#define IS_IN_CATEGORY_FUNC inline static bool StaticIsInCategory(int category) { return GetStaticCategoryFlags() & category;} \
inline bool IsInCategory(int category) const override { return StaticIsInCategory(category); }
