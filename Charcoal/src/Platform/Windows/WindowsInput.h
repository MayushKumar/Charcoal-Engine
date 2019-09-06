#pragma once

#include "Charcoal/Input/Input.h"

namespace Charcoal
{

	class WindowsInput : public Input
	{
		virtual inline bool IsKeyPressedImpl(int keyCode) override;
		virtual inline bool IsMouseButtonPressedImpl(int mouseCode) override;
		virtual inline std::pair<float, float> GetMousePosImpl() override;
		virtual inline float GetMouseXImpl() override;
		virtual inline float GetMouseYImpl() override;
	};

}