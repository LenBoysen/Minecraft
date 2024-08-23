#pragma once


#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)


#include "Application.h"
#include "Layer.h"

#include "Core/TimeStep.h"

#include "Input.h"
#include "KeyCodes.h"
#include "MouseButtonCodes.h"


#include "ImGui/ImGuiLayer.h"



#include "Renderer/Renderer.h"
#include "Renderer/RenderCommand.h"


#include "Renderer/Buffer.h"
#include "Renderer/Shader.h"
#include "Renderer/Texture.h"
#include "Renderer/VertexArray.h"




#include "Renderer/OrthographicCamera.h"
#include "Renderer/PerspectiveCamera.h"



#include "EntryPoint.h"