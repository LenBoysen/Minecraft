#pragma once

enum class RendererAPI {
	None = 0, OpenGL = 1
};

class Renderer {
public:
	inline static void SetAPI(RendererAPI renderer) { s_RendererAPI = renderer; }
	inline static RendererAPI GetAPI() { return s_RendererAPI; }

	static RendererAPI s_RendererAPI;
};