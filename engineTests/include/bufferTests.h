#pragma once
#include <gtest/gtest.h>
// TESTS FOR THE BUFFER LAYOUT
#include "rendering/vertexBuffer.h"
#include "rendering/bufferLayout.h"
#include "rendering/shaderData.h"

std::shared_ptr<Engine::VertexBuffer> buffer;

Engine::ShaderDataType type1 = Engine::ShaderDataType::Bool;
Engine::ShaderDataType type2 = Engine::ShaderDataType::Float;
Engine::ShaderDataType type3 = Engine::ShaderDataType::Mat4;
