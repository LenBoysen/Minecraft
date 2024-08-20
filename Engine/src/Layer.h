#pragma once
#include<string>
#include"events/Event.h"
#include "Core/TimeStep.h"

class Layer
{
public:
	Layer(const std::string& name = "layer");
	virtual ~Layer();

	virtual void onAttach() {}
	virtual void onDetach() {}
	virtual void onUpdate(TimeStep ts) {}
	virtual void onImGuiRender() {}
	virtual void onEvent(Event& e) {}

	inline const std::string& GetName() { return m_Name; }

protected:
	std::string m_Name;

};

