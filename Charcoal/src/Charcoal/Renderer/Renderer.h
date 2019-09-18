#pragma once

namespace Charcoal
{

	enum class RendererAPI
	{
		None = 0, OpenGL
	};

	class Renderer
	{
	public:
		inline static RendererAPI GetAPI() { return m_RendererAPI; }

	private:
		static RendererAPI m_RendererAPI;
	};

}