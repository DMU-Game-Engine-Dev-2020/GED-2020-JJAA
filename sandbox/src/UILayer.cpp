//#include "UILayer.h"
//
//UILayer::UILayer(const std::string& name = "Layer") : Layer(name)
//{
//	m_pRenderer.reset(Engine::Renderer::createText());
//
//	FT_Library ft;
//	FT_Face face;
//	std::string filepath("assets/fonts/arial_narrow_7.ttf");
//	int iCharSize = 128;
//
//	if (FT_Init_FreeType(&ft))
//		LOG_CRITICAL("Could not start FreeType");
//
//	if (FT_New_Face(ft, filepath.c_str(), 0, &face))
//		LOG_CRITICAL("FreeType coudn't loat font: {0}", filepath);
//
//	if (FT_Set_Pixel_Sizes(face, 0, iCharSize))
//		LOG_CRITICAL("FreeType couldn't set font face size of {0}", iCharSize);
//
//	if (FT_Load_Char(face, 'G', FT_LOAD_RENDER))
//		LOG_CRITICAL("Could not load the character {0}", 'G');
//
//	m_pTexture.reset(Engine::Texture::createFromRawData(
//		face->glyph->bitmap.width,
//		face->glyph->bitmap.rows,
//		1,
//		face->glyph->bitmap.buffer)
//	);
//
//	Engine::Character ch(
//		glm::vec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
//		glm::vec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
//		face->glyph->advance.x,
//		glm::vec2(0.0f),
//		glm::vec2(1.0f)
//	);
//
//	float textVertices[4 * 4] = {
//		0,              0,              ch.getStartUV().x, ch.getStartUV().y,
//		ch.getSize().x, 0,              ch.getEndUV().x,   ch.getStartUV().y,
//		ch.getSize().x, ch.getSize().y, ch.getEndUV().x,   ch.getEndUV().y,
//		0,              ch.getSize().y, ch.getStartUV().x, ch.getEndUV().y
//	};
//
//	unsigned int textIndices[4] = { 0, 1, 2, 3 };
//
//	// Temporary pointers to create shaders, vertex array objects and materials
//	std::shared_ptr<Engine::Shader> tempSetupShader;
//	std::shared_ptr<Engine::VertexArray> tempSetupVAO;
//
//	tempSetupShader = m_pResources->addShader("assets/shaders/text1.glsl");
//	tempSetupVAO = m_pResources->addVAO("text");
//	tempSetupVAO->setVertexBuffer(m_pResources->addVBO("textVBO", textVertices, sizeof(textVertices), tempSetupShader->getBufferLayout()));
//	tempSetupVAO->setIndexBuffer(m_pResources->addIndexBuffer("textIndices", textIndices, 4));
//
//	m_pMat = m_pResources->addMaterial("textMat", tempSetupShader, tempSetupVAO);
//
//	m_pCamera.reset(new Engine::FreeOrthoCameraController2D());
//	m_pCamera->init(0.f, 0.f, 800.f, 600.f);
//}
//
//void UILayer::onAttach()
//{
//
//}
//
//void UILayer::onDetach()
//{
//
//}
//
//void UILayer::onUpdate(float timestep)
//{
//	glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(100, 200, 0));
//	m_iTexSlot = m_pTexture->getSlot();
//	glm::vec3 colour(1.0f, 0.3f, 0.6f);
//	m_pMat->setDataElement("u_model", (void*)&model[0][0]);
//	m_pMat->setDataElement("u_viewProjection", (void*)&m_pCamera->getCamera()->getViewProjection()[0][0]);
//	m_pMat->setDataElement("u_texData", (void*)&m_iTexSlot);
//	m_pMat->setDataElement("u_fontColour", (void*)&colour[0]);
//
//	m_pRenderer->actionCommand(Engine::RenderCommand::clearColourBufferCommand());
//	m_pRenderer->actionCommand(Engine::RenderCommand::setOneMinusAlphaBlendingCommand(true));
//
//	m_pRenderer->submit(m_pMat);
//
//	m_pRenderer->actionCommand(Engine::RenderCommand::setOneMinusAlphaBlendingCommand(false));
//}
//
//void UILayer::onEvent(Engine::Event& event)
//{
//
//}