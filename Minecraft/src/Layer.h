#pragma once
#include<string>


class Layer
{
public:
	Layer();
	virtual ~Layer();

	virtual void OnAttach() {}
	virtual void OnDetach() {}
	virtual void OnUpdate() {}
	virtual void OnEvent()  {}

	inline const std::string& GetName() { return m_Name; }

protected:
	std::string m_Name;

};

