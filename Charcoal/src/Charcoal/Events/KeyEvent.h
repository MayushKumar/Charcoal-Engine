#pragma once

#include "Event.h"

namespace Charcoal
{

	class KeyPressedEvent : public Event
	{

	public:
		KeyPressedEvent(int keyCode, bool repeated) : m_KeyCode(keyCode), m_Repeated(repeated) {}

		inline int GetKeyCode() const { return m_KeyCode; }
		inline bool IsRepeated() const { return m_Repeated; }

		inline std::string ToString() const override { return "Key Pressed: " + std::to_string(m_KeyCode) + (m_Repeated ? " Repeated" : " Not Repeated"); }

		GET_TYPE_FUNC(EventType::KeyPressed)
		GET_CATEGORY_FLAGS_FUNC(EventCategory::InputEvent | EventCategory::KeyInputEvent)
		IS_IN_CATEGORY_FUNC

	private:
		int m_KeyCode;
		bool m_Repeated;

	};

	class KeyReleasedEvent : public Event
	{

	public:
		KeyReleasedEvent(int keyCode) : m_KeyCode(keyCode) {}

		inline int GetKeyCode() const { return m_KeyCode; }

		inline std::string ToString() const override { return "Key Released: " + std::to_string(m_KeyCode); }

		GET_TYPE_FUNC(EventType::KeyReleased)
		GET_CATEGORY_FLAGS_FUNC(EventCategory::InputEvent | EventCategory::KeyInputEvent)
		IS_IN_CATEGORY_FUNC

	private:
		int m_KeyCode;

	};

	class KeyTypedEvent : public Event
	{

	public:
		KeyTypedEvent(int character) : m_Character(character){}

		inline int GetCharacter() const { return m_Character; }

		inline std::string ToString() const override { return "Key Typed: " + std::to_string(m_Character); }

		GET_TYPE_FUNC(EventType::KeyTyped)
		GET_CATEGORY_FLAGS_FUNC(EventCategory::InputEvent | EventCategory::KeyInputEvent)
		IS_IN_CATEGORY_FUNC

	private:
		int m_Character;
		
	};

}