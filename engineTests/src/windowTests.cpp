#include "windowTests.h"

TEST(Window, EventCallback)
{	
	window->setEventCallback(eventCallbackFunc);

	Engine::KeyPressedEvent e(0, 0);
	window->m_callback(e);

	EXPECT_TRUE(worked);
}