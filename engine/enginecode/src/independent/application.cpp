/** \file application.cpp
*/
#include "engine_pch.h"

#include "core/application.h"
#include "core/core.h"
#include "core/codes.h"

#ifdef NG_PLATFORM_WINDOWS
#include "platform/GLFW/GLFWWindowsSystem.h"
#endif // NG_PLATFORM_WINDOWS

//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//
#include <ft2build.h>
#include FT_FREETYPE_H

namespace Engine 
{
	Application* Application::s_instance = nullptr;


#pragma region TempGlobalVars
	//glm::mat4 FCmodel, TPmodel;
#pragma endregion TempGlobalVars


	Application::Application()
	{
		if (s_instance == nullptr)
		{
			s_instance = this;
		}

		m_bRunning = true; // Application is running
		m_fTotalTimeElapsed = 0; // Set the time elapsed to nothing

		// Create and get instances of the system objects
		m_pLogger = Log::getInstance();
		m_pLogger->start();
		m_pTimer = Timer::getInstance();
		m_pTimer->start();

#ifdef NG_PLATFORM_WINDOWS // If the engine is running on a windows computer
		m_pWindows = GLFWWindowsSystem::getInstance(); // Create an instance of the GLFW windows system
#endif // NG_PLATFORM_WINDOWS
		m_pWindows->start(); // Start the windows system
		LOG_INFO("Windows system initialised");

		// Create a window
		m_pWindow.reset(Window::create());
		// Set the windows event callback to call the onEvent function in Application
		m_pWindow->setEventCallback(std::bind(&Application::onEvent, this, std::placeholders::_1));

		// Make a new resource manager
		m_pResources.reset(new ResourceManager);

		m_pLayerStack.reset(new Layerstack());
		m_pLayerStack->start();

		//// Make a new basic renderer
		//m_pRenderer.reset(Renderer::createBasic3D());
		//
		//// Set depth test and backface culling
		//m_pRenderer->actionCommand(RenderCommand::setDepthTestLessCommand(true));
		//m_pRenderer->actionCommand(RenderCommand::setBackfaceCullingCommand(true));
		//
		//float FCvertices[6 * 24] = {
		//	-0.5f, -0.5f, -0.5f, 0.8f, 0.2f, 0.2f, // red square
		//	 0.5f, -0.5f, -0.5f, 0.8f, 0.2f, 0.2f,
		//	 0.5f,  0.5f, -0.5f, 0.8f, 0.2f, 0.2f,
		//	-0.5f,  0.5f, -0.5f,  0.8f, 0.2f, 0.2f,
		//	-0.5f, -0.5f, 0.5f, 0.2f, 0.8f, 0.2f, // green square
		//	 0.5f, -0.5f, 0.5f, 0.2f, 0.8f, 0.2f,
		//	 0.5f,  0.5f, 0.5f, 0.2f, 0.8f, 0.2f,
		//	-0.5f,  0.5f, 0.5f,  0.2f, 0.8f, 0.2f,
		//	-0.5f, -0.5f, -0.5f, 0.8f, 0.2f, 0.8f, // magenta(ish) square
		//	 0.5f, -0.5f, -0.5f, 0.8f, 0.2f, 0.8f,
		//	 0.5f, -0.5f, 0.5f, 0.8f, 0.2f, 0.8f,
		//	-0.5f, -0.5f, 0.5f,  0.8f, 0.2f, 0.8f,
		//	-0.5f, 0.5f, -0.5f, 0.8f, 0.8f, 0.2f, // yellow square 
		//	 0.5f, 0.5f, -0.5f, 0.8f, 0.8f, 0.2f,
		//	 0.5f, 0.5f, 0.5f, 0.8f, 0.8f, 0.2f,
		//	-0.5f, 0.5f, 0.5f,  0.8f, 0.8f, 0.2f,
		//	-0.5f, -0.5f, -0.5f, 0.2f, 0.8f, 0.8f, // Cyan(ish) square 
		//	-0.5f,  0.5f, -0.5f,  0.2f, 0.8f, 0.8f,
		//	-0.5f,  0.5f, 0.5f, 0.2f, 0.8f, 0.8f,
		//	-0.5f,  -0.5f, 0.5f, 0.2f, 0.8f, 0.8f,
		//	0.5f, -0.5f, -0.5f, 0.2f, 0.2f, 0.8f, // Blue square 
		//	0.5f,  0.5f, -0.5f,  0.2f, 0.2f, 0.8f,
		//	0.5f,  0.5f, 0.5f, 0.2f, 0.2f, 0.8f,
		//	0.5f,  -0.5f, 0.5f, 0.2f, 0.2f, 0.8f
		//};
		//
		//unsigned int indices[3 * 12] = {
		//	2, 1, 0,
		//	0, 3, 2,
		//	4, 5, 6,
		//	6, 7, 4,
		//	8, 9, 10,
		//	10, 11, 8,
		//	14, 13, 12,
		//	12, 15, 14,
		//	18, 17, 16,
		//	16, 19, 18,
		//	20, 21, 22,
		//	22, 23, 20
		//};
		//
		//// Temporary pointers to create shaders, vertex array objects and materials
		//std::shared_ptr<Shader> tempSetupShader;
		//std::shared_ptr<VertexArray> tempSetupVAO;
		//
		//// Create flat colour shader from shader file
		//tempSetupShader = m_pResources->addShader("assets/shaders/flatColour2.glsl");
		//// Create a vertex array object
		//tempSetupVAO = m_pResources->addVAO("flatColourCube");
		//// Create a vertex buffer and an index buffer and give pointers to them to the vertex array object
		//tempSetupVAO->setVertexBuffer(m_pResources->addVBO("flatColourVBO", FCvertices, sizeof(FCvertices), tempSetupShader->getBufferLayout()));
		//tempSetupVAO->setIndexBuffer(m_pResources->addIndexBuffer("FCCubeIndices", indices, sizeof(indices) / sizeof(unsigned int)));
		//// Create the flat colour material
		//m_pFCMat = m_pResources->addMaterial("flatColourMat", tempSetupShader, tempSetupVAO);
		//
		///////////////////////////////////////////////////////////
		//// Added textuer phong shader and cube //////////////////
		///////////////////////////////////////////////////////////
		//
		//float TPvertices[8 * 24] = {
		//	-0.5f, -0.5f, -0.5f, 0.f, 0.f, -1.f, 0.33f, 0.5f,
		//	 0.5f, -0.5f, -0.5f, 0.f, 0.f, -1.f, 0.f, 0.5f,
		//	 0.5f,  0.5f, -0.5f, 0.f, 0.f, -1.f, 0.f, 0.f,
		//	-0.5f,  0.5f, -0.5f, 0.f, 0.f, -1.f, 0.33f, 0.f,
		//	-0.5f, -0.5f, 0.5f,  0.f, 0.f, 1.f, 0.33f, 0.5f,
		//	 0.5f, -0.5f, 0.5f,  0.f, 0.f, 1.f, 0.66f, 0.5f,
		//	 0.5f,  0.5f, 0.5f,  0.f, 0.f, 1.f, 0.66f, 0.f,
		//	-0.5f,  0.5f, 0.5f,  0.f, 0.f, 1.f, 0.33, 0.f,
		//	-0.5f, -0.5f, -0.5f, 0.f, -1.f, 0.f, 1.f, 0.f,
		//	 0.5f, -0.5f, -0.5f, 0.f, -1.f, 0.f, 0.66f, 0.f,
		//	 0.5f, -0.5f, 0.5f,  0.f, -1.f, 0.f, 0.66f, 0.5f,
		//	-0.5f, -0.5f, 0.5f,  0.f, -1.f, 0.f, 1.0f, 0.5f,
		//	-0.5f, 0.5f, -0.5f,  0.f, 1.f, 0.f, 0.33f, 1.0f,
		//	 0.5f, 0.5f, -0.5f,  0.f, 1.f, 0.f, 0.f, 1.0f,
		//	 0.5f, 0.5f, 0.5f, 0.f, 1.f, 0.f, 0.f, 0.5f,
		//	-0.5f, 0.5f, 0.5f,   0.f, 1.f, 0.f, 0.3f, 0.5f,
		//	-0.5f, -0.5f, -0.5f, -1.f, 0.f, 0.f, 0.33f, 1.0f,
		//	-0.5f,  0.5f, -0.5f, -1.f, 0.f, 0.f, 0.33f, 0.5f,
		//	-0.5f,  0.5f, 0.5f,  -1.f, 0.f, 0.f, 0.66f, 0.5f,
		//	-0.5f,  -0.5f, 0.5f, -1.f, 0.f, 0.f, 0.66f, 1.0f,
		//	0.5f, -0.5f, -0.5f,  1.f, 0.f, 0.f, 1.0f, 1.0f,
		//	0.5f,  0.5f, -0.5f,  1.f, 0.f, 0.f, 1.0f, 0.5f,
		//	0.5f,  0.5f, 0.5f, 1.f, 0.f, 0.f,  0.66f, 0.5f,
		//	0.5f,  -0.5f, 0.5f,  1.f, 0.f, 0.f, 0.66f, 1.0f
		//};
		//// Same as flat colour stuff
		//tempSetupShader = m_pResources->addShader("assets/shaders/texturedPhong2.glsl");
		//tempSetupVAO = m_pResources->addVAO("texturedPhongCube");
		//tempSetupVAO->setVertexBuffer(m_pResources->addVBO("texturedPhongVBO", TPvertices, sizeof(TPvertices), tempSetupShader->getBufferLayout()));
		//tempSetupVAO->setIndexBuffer(m_pResources->addIndexBuffer("TPCubeIndices", indices, sizeof(indices) / sizeof(unsigned int)));
		//// Create the textures phong material
		//m_pTPMat = m_pResources->addMaterial("texturedPhongMat", tempSetupShader, tempSetupVAO);
		//
		//// Load textures
		//m_pLetterCubeTexture = m_pResources->addTexture("assets/textures/letterCube.png");
		//m_pNumberCubeTexture = m_pResources->addTexture("assets/textures/numberCube.png");
		//
		//FCmodel = glm::translate(glm::mat4(1), glm::vec3(1.5, 0, 3));
		//TPmodel = glm::translate(glm::mat4(1), glm::vec3(-1.5, 0, 3));
		//
		//m_matricesUBO = m_pResources->getUBO("Matrices");
		//m_lightUBO = m_pResources->getUBO("Light");
		//
		//
		m_pTextRenderer.reset(Renderer::createText());
		
		FT_Library ft;
		FT_Face face;
		std::string filepath("assets/fonts/arial_narrow_7.ttf");
		int iCharSize = 128;
		
		if (FT_Init_FreeType(&ft))
			LOG_CRITICAL("Could not start FreeType");
		
		if (FT_New_Face(ft, filepath.c_str(), 0, &face))
			LOG_CRITICAL("FreeType coudn't load font: {0}", filepath);
		
		if (FT_Set_Pixel_Sizes(face, 0, iCharSize))
			LOG_CRITICAL("FreeType couldn't set font face size of {0}", iCharSize);
		
		if (FT_Load_Char(face, 'G', FT_LOAD_RENDER))
			LOG_CRITICAL("Could not load the character {0}", 'G');
		
		m_pGlyphTexture.reset(Texture::createFromRawData(
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			1,
			face->glyph->bitmap.buffer)
		);
		
		Character ch(
			glm::vec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::vec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			face->glyph->advance.x,
			glm::vec2(0.0f),
			glm::vec2(1.0f)
		);

		float textVertices[4 * 4] = {
			0,              0,              ch.getStartUV().x, ch.getStartUV().y,
			ch.getSize().x, 0,              ch.getEndUV().x,   ch.getStartUV().y,
			ch.getSize().x, ch.getSize().y, ch.getEndUV().x,   ch.getEndUV().y,
			0,              ch.getSize().y, ch.getStartUV().x, ch.getEndUV().y
		};



		if (FT_Load_Char(face, 'F', FT_LOAD_RENDER))
			LOG_CRITICAL("Could not load the character {0}", 'F');

		m_pGlyphTexture2.reset(Texture::createFromRawData(
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			1,
			face->glyph->bitmap.buffer)
		);

		Character ch2(
			glm::vec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::vec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			face->glyph->advance.x,
			glm::vec2(0.0f),
			glm::vec2(1.0f)
		);

		float textVertices2[4 * 4] = {
			0,              0,              ch2.getStartUV().x, ch2.getStartUV().y,
			ch2.getSize().x, 0,              ch2.getEndUV().x,   ch2.getStartUV().y,
			ch2.getSize().x, ch2.getSize().y, ch2.getEndUV().x,   ch2.getEndUV().y,
			0,              ch2.getSize().y, ch2.getStartUV().x, ch2.getEndUV().y
		};
		
		unsigned int textIndices[4] = { 0, 1, 2, 3 };
		
		std::shared_ptr<Shader> tempSetupShader = m_pResources->addShader("assets/shaders/text2.glsl");
		std::shared_ptr<VertexArray> tempSetupVAO = m_pResources->addVAO("text");
		tempSetupVAO->setVertexBuffer(m_pResources->addVBO("textVBO", textVertices, sizeof(textVertices), tempSetupShader->getBufferLayout()));
		tempSetupVAO->setIndexBuffer(m_pResources->addIndexBuffer("textIndices", textIndices, 4));
		
		m_pTextMat = m_pResources->addMaterial("textMat", tempSetupShader, tempSetupVAO);

		tempSetupShader = m_pResources->addShader("assets/shaders/text3.glsl");
		tempSetupVAO = m_pResources->addVAO("text2");
		tempSetupVAO->setVertexBuffer(m_pResources->addVBO("textVBO2", textVertices2, sizeof(textVertices2), tempSetupShader->getBufferLayout()));
		tempSetupVAO->setIndexBuffer(m_pResources->addIndexBuffer("textIndices2", textIndices, 4));

		m_pTextMat2 = m_pResources->addMaterial("textMat2", tempSetupShader, tempSetupVAO);

		m_pTextUBO = m_pResources->getUBO("TextMatrices");
		
		
		m_2DCam.reset(new FreeOrthoCameraController2D());
		m_2DCam->init(0.f, 0.f, 800.f, 600.f);
		
		//m_FPSCam.reset(new FPSCameraControllerEuler());
		//m_FPSCam->init(45.f, 4.f / 3.f, 0.1f, 100.f);

		TIMER_NEWFRAME; // Tell the timer to start for a new frame
	}

	Application::~Application()
	{
		// Run the stop functions of the systems
		m_pLayerStack->stop();
		m_pResources->stop();
		m_pWindows->stop();
		m_pTimer->stop();
		m_pLogger->stop();
	}

	void Application::onEvent(Event& e)
	{
		// Create event dispatcher
		EventDispatcher dispatcher(e);
		// If the Event type matches, call the corresponding function
		dispatcher.dispatch<WindowCloseEvent>(std::bind(&Application::onClose, this, std::placeholders::_1));
		dispatcher.dispatch<WindowResizeEvent>(std::bind(&Application::onResize, this, std::placeholders::_1));
		//dispatcher.dispatch<WindowFocusEvent>(std::bind(&Application::onFocus, this, std::placeholders::_1));
		//dispatcher.dispatch<WindowLostFocusEvent>(std::bind(&Application::onLostFocus, this, std::placeholders::_1));
		//dispatcher.dispatch<WindowMovedEvent>(std::bind(&Application::onWindowMoved, this, std::placeholders::_1));
		dispatcher.dispatch<KeyPressedEvent>(std::bind(&Application::onKeyPressed, this, std::placeholders::_1));

		for (auto it = m_pLayerStack->end(); it != m_pLayerStack->begin(); )
		{
			(*--it)->onEvent(e);
			if (e.handled()) break;
		}
	}

	bool Application::onClose(WindowCloseEvent& e)
	{
		// Log what's happening
		LOG_TRACE("Window closing");
		m_bRunning = false; // No longer running
		return true;
	}

	bool Application::onResize(WindowResizeEvent& e)
	{
		// Log what's happening
		LOG_TRACE("Window resize event. Width {0}, Height {1}", e.getWidth(), e.getHeight());
		m_pWindow->onResize(e.getWidth(), e.getHeight()); // Set the windows new size in its properties
		return true;
	}

	//bool Application::onFocus(WindowFocusEvent& e)
	//{
	//	// Log what's happening
	//	LOG_TRACE("Window being focused on");
	//	return true;
	//}
	//
	//bool Application::onLostFocus(WindowLostFocusEvent& e)
	//{
	//	// Log what's happening
	//	LOG_TRACE("Window losing focus");
	//	return true;
	//}
	//
	//bool Application::onWindowMoved(WindowMovedEvent & e)
	//{
	//	// Log what's happening
	//	LOG_TRACE("Window moved event. Position: {0}, {1}", e.getXPos(), e.getYPos());
	//	return true;
	//}

	bool Application::onKeyPressed(KeyPressedEvent& e)
	{
		// If the space key is pressed
		if (e.getKeyCode() == ENGINE_KEY_SPACE)
		{
			// If the window is not in fullscreen mode
			if (!m_pWindow->isFullScreenMode())
			{
				LOG_TRACE("Entering fullscreen"); // Log what's happening
				m_pWindow->setFullscreen(true); // Set the window to fullscreen
			}
			else // If the window is in fullscreen mode
			{
				LOG_TRACE("Exiting fullscreen"); // Log what's happening
				m_pWindow->setFullscreen(false); // Set the window to not fullscreen
			}
			return true; // Leave the function
		}
		// If escape key is pressed
		if (e.getKeyCode() == ENGINE_KEY_ESCAPE)
		{
			LOG_TRACE("Window closing"); // Log what's happening
			m_bRunning = false; // Stop the application from running
			return true; // Leave the function
		}
		// If the enter key is pressed
		if (e.getKeyCode() == ENGINE_KEY_ENTER)
		{
			//m_bCam = !m_bCam;
			return true; // Leave the function
		}
		return false;
	}

	void Application::run()
	{
		while (m_bRunning)
		{
			m_fTimestep = TIMER_NEWFRAME; // Tell timer to start for a new frame
		
#ifdef NG_DEBUG
			LOG_INFO("FPS: {0} T: {1}", (int)(1 / m_fTimestep), m_fTimestep);
#endif
			for (auto it = m_pLayerStack->begin(); it != m_pLayerStack->end(); it++)
				(*it)->onUpdate(m_fTimestep);


			m_2DCam->onUpdate(m_fTimestep);

			
			SceneData sceneData;
			std::vector<void*> tempData;
			// Add Matrices data to vector
			tempData.push_back((void*)&m_2DCam->getCamera()->getProjection()[0][0]);
			tempData.push_back((void*)&m_2DCam->getCamera()->getView()[0][0]);

			sceneData.insert(std::make_pair(m_pTextUBO, tempData));

			m_pTextRenderer->beginScene(sceneData);

			glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(100, 200, 0));
			unsigned int texSlot = m_pGlyphTexture->getSlot();
			glm::vec3 colour(1.0f, 0.3f, 0.6f);
			m_pTextMat->setDataElement("u_model", (void*)&model[0][0]);
			m_pTextMat->setDataElement("u_texData", (void*)&texSlot);
			m_pTextMat->setDataElement("u_fontColour", (void*)&colour[0]);


			

			//m_pTextRenderer->actionCommand(RenderCommand::clearColourBufferCommand());
			m_pTextRenderer->actionCommand(RenderCommand::setOneMinusAlphaBlendingCommand(true));

			m_pTextRenderer->submit(m_pTextMat);

			model = glm::translate(glm::mat4(1.0f), glm::vec3(300, 200, 0));
			texSlot = m_pGlyphTexture2->getSlot();
			m_pTextMat2->setDataElement("u_model", (void*)&model[0][0]);
			m_pTextMat2->setDataElement("u_texData", (void*)&texSlot);
			m_pTextMat2->setDataElement("u_fontColour", (void*)&colour[0]);
			m_pTextRenderer->submit(m_pTextMat2);

			m_pTextRenderer->actionCommand(RenderCommand::setOneMinusAlphaBlendingCommand(false));



			m_pWindow->onUpdate(m_fTimestep);
		}

		// While application is running
		//while (m_bRunning)
		//{
		//	TIMER_NEWFRAME; // Tell timer to start for a new frame
		//
		//	m_fTimestep = TIMER_TIMESTEP; // Get the time taken to run the previous frame
		//
		//	// Calculate and display the FPS
		//	float FPS = 1 / m_fTimestep;
		//	//LOG_INFO("FPS: {0}", FPS);
		//
		//	m_fTotalTimeElapsed += m_fTimestep; // Add the time to run the previous frame to the total time elapsed
		//
		//	// Clear colour
		//	m_pRenderer->actionCommand(RenderCommand::setBackfaceCullingCommand(true));
		//	m_pRenderer->actionCommand(RenderCommand::setClearColourCommand(0.8f, 0.8f, 0.8f, 1));
		//	m_pRenderer->actionCommand(RenderCommand::clearDepthColourBufferCommand());
		//
		//
		//	//glm::mat4 projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f); // Basic 4:3 camera
		//	//
		//	//glm::mat4 view = glm::lookAt(
		//	//	glm::vec3(0.0f, 0.0f, -4.5f), // Camera is at (0.0,0.0,-4.5), in World Space
		//	//	glm::vec3(0.f, 0.f, 0.f), // and looks at the origin
		//	//	glm::vec3(0.f, 1.f, 0.f)  // Standing straight  up
		//	//);
		//
		//	if (m_bCam)
		//		m_FPSCam->onUpdate(m_fTimestep);
		//	else
		//		m_2DCam->onUpdate(m_fTimestep);
		//
		//	/////////////////////////////////////
		//	// Moving stuff /////////////////////
		//	/////////////////////////////////////
		//
		//	// Code to make the cube move, you can ignore this more or less.
		//	glm::mat4 FCtranslation, TPtranslation;
		//
		//	if (m_goingUp)
		//	{
		//		FCtranslation = glm::translate(FCmodel, glm::vec3(0.0f, 0.2f * m_fTimestep, 0.0f));
		//		TPtranslation = glm::translate(TPmodel, glm::vec3(0.0f, -0.2f * m_fTimestep, 0.0f));
		//	}
		//	else
		//	{
		//		FCtranslation = glm::translate(FCmodel, glm::vec3(0.0f, -0.2f * m_fTimestep, 0.0f));
		//		TPtranslation = glm::translate(TPmodel, glm::vec3(0.0f, 0.2f * m_fTimestep, 0.0f));
		//	}
		//
		//	m_timeSummed += m_fTimestep;
		//	if (m_timeSummed > 20.0f) 
		//	{
		//		m_timeSummed = 0.f;
		//		m_goingUp = !m_goingUp;
		//	}
		//
		//
		//	FCmodel = glm::rotate(FCtranslation, glm::radians(20.f) * m_fTimestep, glm::vec3(0.f, 1.f, 0.f)); // Spin the cube at 20 degrees per second
		//	TPmodel = glm::rotate(TPtranslation, glm::radians(-20.f) * m_fTimestep, glm::vec3(0.f, 1.f, 0.f)); // Spin the cube at 20 degrees per second
		//
		//	// End of code to make the cube move.
		//
		//	/////////////////////////////////////
		//	// Set scene data ///////////////////
		//	/////////////////////////////////////
		//
		//	// Data for light uniform block
		//	glm::vec4 lightColour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		//	glm::vec4 lightPos = glm::vec4(1.0f, 4.0f, -6.0f, 1.0f);
		//	glm::vec4 viewPos = glm::vec4(0.0f, 0.0f, -4.5f, 1.0f);
		//
		//	// To give to the renderer to begin a scene
		//	SceneData sceneData;
		//	
		//	// Two vectors of data for Matrices uniform block and Light uniform block
		//	std::vector<void*> tempData[2];
		//	// Add Matrices data to vector
		//	tempData[0].push_back((void*)&m_FPSCam->getCamera()->getProjection()[0][0]);
		//	tempData[0].push_back((void*)&m_FPSCam->getCamera()->getView()[0][0]);
		//	
		//	sceneData.insert(std::make_pair(m_matricesUBO, tempData[0]));
		//	
		//	// Add Light data to vector
		//	tempData[1].push_back((void*)&lightColour[0]);
		//	tempData[1].push_back((void*)&lightPos[0]);
		//	tempData[1].push_back((void*)&viewPos[0]);
		//	
		//	sceneData.insert(std::make_pair(m_lightUBO, tempData[1]));
		//	
		//	// Begin the scene
		//	m_pRenderer->beginScene(sceneData);
		//	
		//	///////////////////////////////////////
		//	// colour cube ////////////////////////
		//	///////////////////////////////////////
		//
		//	// Set the flat colour cube model uniform
		//	m_pFCMat->setDataElement("u_model", (void*)&FCmodel[0][0]);
		//	m_pRenderer->submit(m_pFCMat); // Submit to the renderer to draw the cube
		//
		//	///////////////////////////////////////
		//	// textured phong cube ////////////////
		//	///////////////////////////////////////
		//
		//	unsigned int texSlot;
		//	if (m_goingUp) texSlot = m_pLetterCubeTexture->getSlot();
		//	else texSlot = m_pNumberCubeTexture->getSlot();
		//	
		//	// Set the textured phong cube model and texture uniform
		//	m_pTPMat->setDataElement("u_model", (void*)&TPmodel[0][0]);
		//	m_pTPMat->setDataElement("u_texData", (void*)&texSlot);
		//	m_pRenderer->submit(m_pTPMat); // Submit to the renderer to draw the cube
		//	
		//	m_pRenderer->actionCommand(RenderCommand::setBackfaceCullingCommand(false));
		//
		//m_pTextRenderer->actionCommand(RenderCommand::setClearColourCommand(0.8f, 0.8f, 0.8f, 1));
		//m_pTextRenderer->actionCommand(RenderCommand::clearColourBufferCommand());
		//m_pTextRenderer->actionCommand(RenderCommand::setOneMinusAlphaBlendingCommand(true));
		//
		//glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(100, 200, 0));
		//unsigned int texSlot = m_pGlyphTexture->getSlot();
		//glm::vec3 colour(1.0f, 0.3f, 0.6f);
		//m_pTextMat->setDataElement("u_model", (void*)&model[0][0]);
		//m_pTextMat->setDataElement("u_viewProjection", (void*)&m_2DCam->getCamera()->getViewProjection()[0][0]);
		//m_pTextMat->setDataElement("u_texData", (void*)&texSlot);
		//m_pTextMat->setDataElement("u_fontColour", (void*)&colour[0]);
		//
		//m_pTextRenderer->submit(m_pTextMat);
		//
		//m_pTextRenderer->actionCommand(RenderCommand::setOneMinusAlphaBlendingCommand(false));
		//
		//
		//m_pWindow->onUpdate(m_fTimestep); // Update the window
		//}

		/*// Test the timer
		// Start two timers with the same tag
		TIMER_START("Tag");
		TIMER_START("Tag");

		// Get and display time from a timer with tag 'tag'
		float time = TIMER_END("Tag");
		LOG_INFO("Tag time:  {0}", time);

		// Try to end a timer with a tag that wasn't used to start a timer
		float time2 = TIMER_END("Test");
		LOG_INFO("Time:  {0}", time2);*/
	}
}
