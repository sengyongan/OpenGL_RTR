#include "App.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>


namespace Opengl {
	// settings

	//const char* vertexShaderSource = "#version 330 core\n"
	//	"layout (location = 0) in vec3 aPos;\n"
	//	"void main()\n"
	//	"{\n"
	//	"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
	//	"}\0";
	//const char* fragmentShaderSource = "#version 330 core\n"
	//	"out vec4 FragColor;\n"
	//	"void main()\n"
	//	"{\n"
	//	"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
	//	"}\n\0";
    std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 aPos;
			
			void main()
			{
				gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
			}
		)";

    std::string fragmentSrc = R"(
			#version 330 core
			out vec4 FragColor;
			void main()
			{
                FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
			}
		)";
    float vertices[] = {
     0.5f,  0.5f, 0.0f,  // top right
     0.5f, -0.5f, 0.0f,  // bottom right
    -0.5f, -0.5f, 0.0f,  // bottom left
    -0.5f,  0.5f, 0.0f   // top left 
    };
    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,  // first Triangle
        1, 2, 3   // second Triangle
    };

    App* App::s_Instance = nullptr;

	App::App()
	{
        s_Instance = this;

		//shader
        m_Shader.reset(new Shader(vertexSrc, fragmentSrc));
		////////////////////
		//glGenVertexArrays(1, &VAO);

		//glBindVertexArray(VAO);

		m_VertexArray = std::make_unique<VertexArray>();
		//m_VertexBuffer = std::make_unique<OpenGLVertexBuffer>(vertices, sizeof(vertices));
		std::shared_ptr<VertexBuffer> vertexBuffer;//因为全局变量只需要VAO即可

		vertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

		//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		//glEnableVertexAttribArray(0);
		// 
		//设置布局
		
		BufferLayout layout = {
			{ ShaderDataType::Float3, "aPos" },
		};

		vertexBuffer->SetLayout(layout);
		//进行布局
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		//m_IndexBuffer = std::make_unique<OpenGLIndexBuffer>(indices, sizeof(indices) / sizeof(unsigned int));
		std::shared_ptr<IndexBuffer> indexBuffer;
		indexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(indexBuffer);
        // set up vertex data (and buffer(s)) and configure vertex attributes
        // ------------------------------------------------------------------
        //glGenVertexArrays(1, &VAO);
        //glGenBuffers(1, &VBO);
        //glGenBuffers(1, &EBO);
        // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
        //glBindVertexArray(VAO);

        //glBindBuffer(GL_ARRAY_BUFFER, VBO);
        //glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        //glEnableVertexAttribArray(0);

        // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
        //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
        // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
        glBindVertexArray(0);
	}
	App::~App()
	{
		// glfw: terminate, clearing all previously allocated GLFW resources.
		// ------------------------------------------------------------------
        //glDeleteVertexArrays(1, &VAO);
        //glDeleteBuffers(1, &VBO);
        //glDeleteBuffers(1, &EBO);
        //glDeleteProgram(shaderProgram);
		m_Window.~Window();
		//glfwTerminate();

	}
	void App::Run()
	{
		while(!glfwWindowShouldClose(m_Window.m_Window)){

            // input
			if (glfwGetKey(m_Window.m_Window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
				glfwSetWindowShouldClose(m_Window.m_Window, true);

			// ------
			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			// render
            m_Shader->Bind();
			m_VertexArray->Bind();// seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
            //glDrawArrays(GL_TRIANGLES, 0, 6);
            glDrawElements(GL_TRIANGLES, m_VertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, 0);

            // glBindVertexArray(0); // no need to unbind it every time 
			m_Window.OnUpdate();
		}
	}
}