#pragma once

namespace Charcoal
{

	class Input
	{

	public:
		Input(const Input&) = delete;
		Input& operator=(const Input&) = delete;

		inline static bool IsKeyPressed(int keyCode) { return s_Instance->IsKeyPressedImpl(keyCode); }
		inline static bool IsMouseButtonPressed(int mouseCode) { return s_Instance->IsMouseButtonPressedImpl(mouseCode); }
		inline static std::pair<float, float> GetMousePos() { return s_Instance->GetMousePosImpl(); }
		inline static float GetMouseX() { return s_Instance->GetMouseXImpl(); }
		inline static float GetMouseY() { return s_Instance->GetMouseYImpl(); }
		
	protected:
		Input() = default;

		virtual inline bool IsKeyPressedImpl(int keyCode) = 0;
		virtual inline bool IsMouseButtonPressedImpl(int mouseCode) = 0;
		virtual inline std::pair<float, float> GetMousePosImpl() = 0;
		virtual inline float GetMouseXImpl() = 0;
		virtual inline float GetMouseYImpl() = 0;

	private:
		static Input* s_Instance;

	};

}