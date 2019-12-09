#pragma once

#include <Engine.h>

// entry point
#include "include/independent/core/entrypoint.h"



class engineApp : public Engine::Application
{
public:
	engineApp();
	~engineApp() override;
};
