#include "SandboxApp.h"

class ExampleLayer : public Charcoal::Layer
{

public:
	ExampleLayer() : Layer("Example Layer"){}

	void OnAttach() override
	{
		m_Camera = Charcoal::OrthographicCamera(1280.0f / 720.0f);

		float squareVertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f
		};
		unsigned int squareIndices[6] = { 0, 1, 2, 2, 3, 0 };

		m_SquareVA.reset(Charcoal::VertexArray::Create());
		m_SquareVA->Bind();

		Charcoal::BufferLayout layout =
		{
			{Charcoal::ShaderDataType::Float3, "a_Position"},
			{Charcoal::ShaderDataType::Float2, "a_TexCoord"}
		};

		squareVB.reset(Charcoal::VertexBuffer::Create(sizeof(squareVertices), squareVertices));
		squareVB->SetLayout(layout);

		squareIB.reset(Charcoal::IndexBuffer::Create(sizeof(squareIndices), squareIndices));

		m_SquareVA->AddVertexBuffer(squareVB);
		m_SquareVA->SetIndexBuffer(squareIB);

		std::string flatShaderVertexSrc = R"(
			#version 410 core
			
			layout(location = 0) in vec3 a_Position;

			uniform mat4 m_ViewProjection;
			uniform mat4 m_Transform;

			void main()
			{
				gl_Position = m_ViewProjection * m_Transform * vec4(a_Position, 1.0f);
			}
		)";

		std::string flatShaderFragmentSrc = R"(
			#version 410 core
			
			out vec4 Colour;

			uniform vec3 u_Colour;

			void main()
			{
				Colour = vec4(u_Colour, 1.0f);
			}
		)";

		m_FlatShader.reset(Charcoal::Shader::Create(flatShaderVertexSrc, flatShaderFragmentSrc));

		std::string textureShaderVertexSrc = R"(
			#version 410 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec2 a_TexCoord;

			out vec2 v_TexCoord;

			uniform mat4 m_ViewProjection;
			uniform mat4 m_Transform;

			void main()
			{
				v_TexCoord = a_TexCoord;
				gl_Position = m_ViewProjection * m_Transform * vec4(a_Position, 1.0f);
			}
		)";

		std::string textureShaderFragmentSrc = R"(
			#version 410 core
			
			in vec2 v_TexCoord;

			out vec4 Colour;

			uniform vec3 u_Colour;
			uniform sampler2D u_Texture;

			void main()
			{
				Colour = vec4(v_TexCoord, 0.0f, 1.0f);
				Colour = texture(u_Texture, v_TexCoord);
			}
		)";

		m_TextureShader.reset(Charcoal::Shader::Create(textureShaderVertexSrc, textureShaderFragmentSrc));

		m_Texture = Charcoal::Texture2D::Create("res/textures/test.png");
		//m_Texture = Charcoal::Texture2D::Create("test.png");
	}

	void OnUpdate(Charcoal::Timestep timestep) override
	{
		Charcoal::Renderer::BeginScene(m_Camera);
		if (Charcoal::Input::IsKeyPressed(CH_KEY_A))
			m_Camera.SetPosition(m_Camera.GetPostion() + glm::vec3(-2.5f * timestep, 0.0f, 0.0f));
		if (Charcoal::Input::IsKeyPressed(CH_KEY_D))
			m_Camera.SetPosition(m_Camera.GetPostion() + glm::vec3(2.5f * timestep, 0.0f, 0.0f));
		if (Charcoal::Input::IsKeyPressed(CH_KEY_W))
			m_Camera.SetPosition(m_Camera.GetPostion() + glm::vec3(0.0f, 2.5f * timestep, 0.0f));
		if (Charcoal::Input::IsKeyPressed(CH_KEY_S))
			m_Camera.SetPosition(m_Camera.GetPostion() + glm::vec3(0.0f, -2.5f * timestep, 0.0f));
		if (Charcoal::Input::IsKeyPressed(CH_KEY_C))
			m_Camera.SetRotation(m_Camera.GetRotation() + -200.0f * timestep);
		if (Charcoal::Input::IsKeyPressed(CH_KEY_V))
			m_Camera.SetRotation(m_Camera.GetRotation() + 200.0f * timestep);

		m_Texture->Bind();
		Charcoal::Renderer::Submit(m_SquareVA, m_TextureShader);

		Charcoal::Renderer::EndScene();
	}

	void OnEvent(Charcoal::Event& event) override
	{
		
	}

	void OnImGuiRender() override
	{
		ImGui::Begin("Colour Picker");
		ImGui::ColorEdit3("Square Colour", glm::value_ptr(m_SquareColour));
		ImGui::End();
		m_FlatShader->SetVec3("u_Colour", m_SquareColour);
	}

private:
	Charcoal::OrthographicCamera m_Camera;
	Charcoal::Ref<Charcoal::VertexBuffer> squareVB;
	Charcoal::Ref<Charcoal::IndexBuffer> squareIB;
	Charcoal::Ref<Charcoal::Shader> m_FlatShader;
	Charcoal::Ref<Charcoal::Shader> m_TextureShader;
	Charcoal::Ref<Charcoal::VertexArray> m_SquareVA;
	Charcoal::Ref<Charcoal::Texture> m_Texture;
	glm::vec3 m_SquareColour = {0.02f, 0.8f, 0.9f};

};

SandboxApp::SandboxApp()
{
	PushLayer(new ExampleLayer());
}

SandboxApp::~SandboxApp()
{

}

//Application creation
Charcoal::Application* CreateApplication()
{
	Charcoal::Application* app = new SandboxApp();
	return app;
}