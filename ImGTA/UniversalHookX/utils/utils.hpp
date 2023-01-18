#pragma once

enum RenderingBackend_t {
	NONE = 0,

	DIRECTX9,
	DIRECTX10,
	DIRECTX11,
	DIRECTX12,

	OPENGL,
	VULKAN,
};

namespace UtilsHook {
	typedef void(*InitCallback)(HWND hwnd);
	typedef void(*RenderCallback)();

	void SetRenderingBackend(RenderingBackend_t eRenderingBackend);
	RenderingBackend_t GetRenderingBackend( );
	const char* RenderingBackendToStr( );
	void SetInitCallback(InitCallback newInitCallback);
	void SetRenderCallback(RenderCallback newRenderCallback);
	RenderCallback GetRenderCallback();
	InitCallback GetInitCallback();

	HWND GetProcessWindow( );
	void UnloadDLL( );
	
	HMODULE GetCurrentImageBase( );

	int GetCorrectDXGIFormat(int eCurrentFormat);
}

namespace U = UtilsHook;
