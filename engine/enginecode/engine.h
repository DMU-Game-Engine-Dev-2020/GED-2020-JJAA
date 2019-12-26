/** \file engine.h
* A header file which includes relevant headers for development
*/

#pragma once

#include "core/application.h"
#include "core/layer.h"
#include "systems/log.h"
#include "systems/timer.h"

#include "windows/inputPoller.h"
#include "core/codes.h"

#include "rendering/renderer/renderer.h"
#include "rendering/renderer/renderCommand.h"

#include "rendering/material.h"
#include "rendering/shader.h"
#include "rendering/texture.h"
#include "rendering/text/character.h"
#include "rendering/text/textLabel.h"

#include "rendering/vertexArray.h"
#include "rendering/vertexBuffer.h"
#include "rendering/indexBuffer.h"
#include "rendering/uniformBuffer.h"

#include "cameras/FPSCameraControllerEuler.h"
#include "cameras/freeOrthoCameraController2D.h"

#include "components/CGO.h"
#include "components/materialComponent.h"
#include "components/positionComponent.h"
#include "components/velocityComponent.h"
#include "components/oscilateComponent.h"
#include "components/textureComponent.h"
#include "components/textureSwitchComponent.h"
#include "components/rotateComponent.h"
#include "components/textComponent.h"

#include "load/JSONLayer.h"
