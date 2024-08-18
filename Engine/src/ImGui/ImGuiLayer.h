#pragma once
//#include<imgui.h>
#include"Layer.h"


//#define IMGUI_IMPL_OPENGL_LOADER_CUSTOM
//#include "../Platform/OpenGL/imgui_impl_glfw.h"



class ImGuiLayer : public Layer
{
public:
	ImGuiLayer();
	~ImGuiLayer();
	
	virtual void onAttach() override;
	virtual void onDetach() override;
	virtual void onImGuiRender() override;

	void begin();
	void end();

private:
	float m_Time = 0.0f;

};

