/** \file engineApp.cpp
*/

#include "engineApp.h"

GameLayer::GameLayer(const std::string& name) : Layer(name)
{
	// Make a new basic renderer
	m_pRenderer.reset(Engine::Renderer::createBasic3D());

	float FCvertices[6 * 24] = {
		-0.5f, -0.5f, -0.5f, 0.8f, 0.2f, 0.2f, // red square
		 0.5f, -0.5f, -0.5f, 0.8f, 0.2f, 0.2f,
		 0.5f,  0.5f, -0.5f, 0.8f, 0.2f, 0.2f,
		-0.5f,  0.5f, -0.5f,  0.8f, 0.2f, 0.2f,
		-0.5f, -0.5f, 0.5f, 0.2f, 0.8f, 0.2f, // green square
		 0.5f, -0.5f, 0.5f, 0.2f, 0.8f, 0.2f,
		 0.5f,  0.5f, 0.5f, 0.2f, 0.8f, 0.2f,
		-0.5f,  0.5f, 0.5f,  0.2f, 0.8f, 0.2f,
		-0.5f, -0.5f, -0.5f, 0.8f, 0.2f, 0.8f, // magenta(ish) square
		 0.5f, -0.5f, -0.5f, 0.8f, 0.2f, 0.8f,
		 0.5f, -0.5f, 0.5f, 0.8f, 0.2f, 0.8f,
		-0.5f, -0.5f, 0.5f,  0.8f, 0.2f, 0.8f,
		-0.5f, 0.5f, -0.5f, 0.8f, 0.8f, 0.2f, // yellow square 
		 0.5f, 0.5f, -0.5f, 0.8f, 0.8f, 0.2f,
		 0.5f, 0.5f, 0.5f, 0.8f, 0.8f, 0.2f,
		-0.5f, 0.5f, 0.5f,  0.8f, 0.8f, 0.2f,
		-0.5f, -0.5f, -0.5f, 0.2f, 0.8f, 0.8f, // Cyan(ish) square 
		-0.5f,  0.5f, -0.5f,  0.2f, 0.8f, 0.8f,
		-0.5f,  0.5f, 0.5f, 0.2f, 0.8f, 0.8f,
		-0.5f,  -0.5f, 0.5f, 0.2f, 0.8f, 0.8f,
		0.5f, -0.5f, -0.5f, 0.2f, 0.2f, 0.8f, // Blue square 
		0.5f,  0.5f, -0.5f,  0.2f, 0.2f, 0.8f,
		0.5f,  0.5f, 0.5f, 0.2f, 0.2f, 0.8f,
		0.5f,  -0.5f, 0.5f, 0.2f, 0.2f, 0.8f
	};

	unsigned int indices[3 * 12] = {
		2, 1, 0,
		0, 3, 2,
		4, 5, 6,
		6, 7, 4,
		8, 9, 10,
		10, 11, 8,
		14, 13, 12,
		12, 15, 14,
		18, 17, 16,
		16, 19, 18,
		20, 21, 22,
		22, 23, 20
	};

	// Temporary pointers to create shaders, vertex array objects and materials
	std::shared_ptr<Engine::Shader> tempSetupShader;
	std::shared_ptr<Engine::VertexArray> tempSetupVAO;

	// Create flat colour shader from shader file
	tempSetupShader = m_pResources->addShader("assets/shaders/flatColour2.glsl");
	// Create a vertex array object
	tempSetupVAO = m_pResources->addVAO("flatColourCube");
	// Create a vertex buffer and an index buffer and give pointers to them to the vertex array object
	tempSetupVAO->setVertexBuffer(m_pResources->addVBO("flatColourVBO", FCvertices, sizeof(FCvertices), tempSetupShader->getBufferLayout()));
	tempSetupVAO->setIndexBuffer(m_pResources->addIndexBuffer("FCCubeIndices", indices, sizeof(indices) / sizeof(unsigned int)));
	// Create the flat colour material
	m_pFCcube = m_pResources->addMaterial("flatColourMat", tempSetupShader, tempSetupVAO);

	/////////////////////////////////////////////////////////
	// Added textuer phong shader and cube //////////////////
	/////////////////////////////////////////////////////////

	float TPvertices[8 * 24] = {
		-0.5f, -0.5f, -0.5f, 0.f, 0.f, -1.f, 0.33f, 0.5f,
		 0.5f, -0.5f, -0.5f, 0.f, 0.f, -1.f, 0.f, 0.5f,
		 0.5f,  0.5f, -0.5f, 0.f, 0.f, -1.f, 0.f, 0.f,
		-0.5f,  0.5f, -0.5f, 0.f, 0.f, -1.f, 0.33f, 0.f,
		-0.5f, -0.5f, 0.5f,  0.f, 0.f, 1.f, 0.33f, 0.5f,
		 0.5f, -0.5f, 0.5f,  0.f, 0.f, 1.f, 0.66f, 0.5f,
		 0.5f,  0.5f, 0.5f,  0.f, 0.f, 1.f, 0.66f, 0.f,
		-0.5f,  0.5f, 0.5f,  0.f, 0.f, 1.f, 0.33, 0.f,
		-0.5f, -0.5f, -0.5f, 0.f, -1.f, 0.f, 1.f, 0.f,
		 0.5f, -0.5f, -0.5f, 0.f, -1.f, 0.f, 0.66f, 0.f,
		 0.5f, -0.5f, 0.5f,  0.f, -1.f, 0.f, 0.66f, 0.5f,
		-0.5f, -0.5f, 0.5f,  0.f, -1.f, 0.f, 1.0f, 0.5f,
		-0.5f, 0.5f, -0.5f,  0.f, 1.f, 0.f, 0.33f, 1.0f,
		 0.5f, 0.5f, -0.5f,  0.f, 1.f, 0.f, 0.f, 1.0f,
		 0.5f, 0.5f, 0.5f, 0.f, 1.f, 0.f, 0.f, 0.5f,
		-0.5f, 0.5f, 0.5f,   0.f, 1.f, 0.f, 0.3f, 0.5f,
		-0.5f, -0.5f, -0.5f, -1.f, 0.f, 0.f, 0.33f, 1.0f,
		-0.5f,  0.5f, -0.5f, -1.f, 0.f, 0.f, 0.33f, 0.5f,
		-0.5f,  0.5f, 0.5f,  -1.f, 0.f, 0.f, 0.66f, 0.5f,
		-0.5f,  -0.5f, 0.5f, -1.f, 0.f, 0.f, 0.66f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.f, 0.f, 0.f, 1.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.f, 0.f, 0.f, 1.0f, 0.5f,
		0.5f,  0.5f, 0.5f, 1.f, 0.f, 0.f,  0.66f, 0.5f,
		0.5f,  -0.5f, 0.5f,  1.f, 0.f, 0.f, 0.66f, 1.0f
	};
	// Same as flat colour stuff
	tempSetupShader = m_pResources->addShader("assets/shaders/texturedPhong2.glsl");
	tempSetupVAO = m_pResources->addVAO("texturedPhongCube");
	tempSetupVAO->setVertexBuffer(m_pResources->addVBO("texturedPhongVBO", TPvertices, sizeof(TPvertices), tempSetupShader->getBufferLayout()));
	tempSetupVAO->setIndexBuffer(m_pResources->addIndexBuffer("TPCubeIndices", indices, sizeof(indices) / sizeof(unsigned int)));
	// Create the textures phong material
	m_pTPcube = m_pResources->addMaterial("texturedPhongMat", tempSetupShader, tempSetupVAO);

	// Load textures
	m_pLetterTex = m_pResources->addTexture("assets/textures/letterCube.png");
	m_pNumberTex = m_pResources->addTexture("assets/textures/numberCube.png");

	m_FCmodel = glm::translate(glm::mat4(1), glm::vec3(1.5, 0, 3));
	m_TPmodel = glm::translate(glm::mat4(1), glm::vec3(-1.5, 0, 3));

	m_pUBOMatrices = m_pResources->getUBO("Matrices");
	m_pUBOLights = m_pResources->getUBO("Light");

	m_pCamera.reset(new Engine::FPSCameraControllerEuler());
	m_pCamera->init(45.f, 4.f / 3.f, 0.1f, 100.f);
}

void GameLayer::onAttach()
{

}

void GameLayer::onDetach()
{

}

void GameLayer::onUpdate(float timestep)
{
	m_pCamera->onUpdate(timestep);

	/////////////////////////////////////
	// Moving stuff /////////////////////
	/////////////////////////////////////

	// Code to make the cube move, you can ignore this more or less.
	glm::mat4 FCtranslation, TPtranslation;

	if (m_bGoingUp)
	{
		FCtranslation = glm::translate(m_FCmodel, glm::vec3(0.0f, 0.2f * timestep, 0.0f));
		TPtranslation = glm::translate(m_TPmodel, glm::vec3(0.0f, -0.2f * timestep, 0.0f));
	}
	else
	{
		FCtranslation = glm::translate(m_FCmodel, glm::vec3(0.0f, -0.2f * timestep, 0.0f));
		TPtranslation = glm::translate(m_TPmodel, glm::vec3(0.0f, 0.2f * timestep, 0.0f));
	}

	m_fTimeSummed += timestep;
	if (m_fTimeSummed > 20.0f)
	{
		m_fTimeSummed = 0.f;
		m_bGoingUp = !m_bGoingUp;
	}

	m_FCmodel = glm::rotate(FCtranslation, glm::radians(20.f) * timestep, glm::vec3(0.f, 1.f, 0.f)); // Spin the cube at 20 degrees per second
	m_TPmodel = glm::rotate(TPtranslation, glm::radians(-20.f) * timestep, glm::vec3(0.f, 1.f, 0.f)); // Spin the cube at 20 degrees per second

	/////////////////////////////////////
	// Set scene data ///////////////////
	/////////////////////////////////////

	// Data for light uniform block
	m_lightColour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	m_lightPosition = glm::vec4(1.0f, 4.0f, -6.0f, 1.0f);
	m_viewPosition = glm::vec4(0.0f, 0.0f, -4.5f, 1.0f);

	// Two vectors of data for Matrices uniform block and Light uniform block
	std::vector<void*> tempData[2];
	// Add Matrices data to vector
	tempData[0].push_back((void*)&m_pCamera->getCamera()->getProjection()[0][0]);
	tempData[0].push_back((void*)&m_pCamera->getCamera()->getView()[0][0]);

	m_sceneData.insert(std::make_pair(m_pUBOMatrices, tempData[0]));

	// Add Light data to vector
	tempData[1].push_back((void*)&m_lightColour[0]);
	tempData[1].push_back((void*)&m_lightPosition[0]);
	tempData[1].push_back((void*)&m_viewPosition[0]);

	m_sceneData.insert(std::make_pair(m_pUBOLights, tempData[1]));

	// Set the flat colour cube model uniform
	m_pFCcube->setDataElement("u_model", (void*)&m_FCmodel[0][0]);

	if (m_bGoingUp)
		m_iTexSlot = m_pLetterTex->getSlot();
	else
		m_iTexSlot = m_pNumberTex->getSlot();

	// Set the textured phong cube model and texture uniform
	m_pTPcube->setDataElement("u_model", (void*)&m_TPmodel[0][0]);
	m_pTPcube->setDataElement("u_texData", (void*)&m_iTexSlot);


	m_pRenderer->actionCommand(Engine::RenderCommand::setDepthTestLessCommand(true));
	m_pRenderer->actionCommand(Engine::RenderCommand::setBackfaceCullingCommand(true));
	m_pRenderer->actionCommand(Engine::RenderCommand::setClearColourCommand(0, 0, 0, 1));
	m_pRenderer->actionCommand(Engine::RenderCommand::clearDepthColourBufferCommand());

	// Begin the scene
	m_pRenderer->beginScene(m_sceneData);

	m_pRenderer->submit(m_pFCcube); // Submit to the renderer to draw the cube
	m_pRenderer->submit(m_pTPcube); // Submit to the renderer to draw the cube

	m_pRenderer->actionCommand(Engine::RenderCommand::setDepthTestLessCommand(false));
	m_pRenderer->actionCommand(Engine::RenderCommand::setBackfaceCullingCommand(false));
}

void GameLayer::onEvent(Engine::Event& event)
{
	m_pCamera->onEvent(event);
}


engineApp::engineApp()
{
	m_pLayerStack->push(std::make_shared<GameLayer>(GameLayer("Game Layer")));
	//m_pLayerStack->push(std::make_shared<UILayer>(UILayer("UI Layer")));
}

engineApp::~engineApp()
{

}

Engine::Application* Engine::startApplication()
{
	return new engineApp();
}