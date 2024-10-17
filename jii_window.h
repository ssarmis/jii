/* Copyright (C) 2024 Streanga Sarmis-Stefan - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the CC0 license. Which can be found
 * here: https://creativecommons.org/public-domain/cc0/
 *
 * JII aims to be a set of libraries really easy to use and link.
 * It should take away the headache C++ libraries tend to give nowadays
 * with self building libraries that don't build out of the box,
 * have weird dependencies and waste precious time that could
 * be spent doing actual work.
 *
 * #define JII_WIN_IMPLMENTATION in some C/C++ file to compile the functions
 * so the symbols can be found
 *
 * Don't define JII_WIN_IMPLMENTATION in more than one file because
 * there will be duplicated symbols, I am pretty sure people don't even
 * read these comments on the top of the header but a man can hope.
	
	void Callback0(JIIWindow* data, JIIWinKeyState state, JIIWinKeyCode code, int scancode) {
	}

	void Callback1(JIIWindow* window, double x, double y) {
	}

	void Callback2(JIIWindow* window, JIIWinKeyState state, JIIWinKeyCode code) {
	}

	int main(){
		JIIWindow window;

		// JII_NEED_OPENGL is not needed if there is no opengl context needed
		JIIWinCreateWindow("Some window title", 640, 480, &window, JII_NEED_OPENGL);

		// setting a callback for when a key is down/up
		JIIWinSetKeyboardCallback(&window, Callback0);

		// setting a callback for when the mouse moves
		JIIWinSetMouseMoveCallback(&window, Callback1);

		// setting a callback for when a mouse button is pressed
		JIIWinSetMouseButtonCallback(&window, Callback2);

		while (!JIIWinExited(&window)) {
			JIIWinEvent event = JIIWinPollEvent(&window);
			switch (event.type) {
				case JIIWinEventType::Quit: {
				}
			}
		}
		JIIWinCleanWindow(&window);
		...
	}
 */

#pragma once

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

#ifndef JII_PRIMITIVE_DEFINES
#define JII_PRIMITIVE_DEFINES
#include <stdint.h>
typedef int32_t i32;
typedef int16_t i16;
typedef int8_t i8;
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t u8;
#endif

typedef u32 JIIWinHint;

enum JIIWinKeyState {
	Down,
	Up
};

enum JIIWinKeyCode {
	MouseLeftButton,
	MouseRightButton,
	KeyBreak,
	MouseMiddleButton,
	MouseX1Button,
	MouseX2Button,
	KeyBackspace,
	KeyTab,
	KeyClear,
	KeyEnter,
	KeyShift,
	KeyControl,
	KeyAlt,
	KeyPause,
	KeyCapsLock,
	KeyEsc,
	KeySpace,
	KeyPageUp,
	KeyPageDown,
	KeyEnd,
	KeyHome,
	KeyLeft,
	KeyUp,
	KeyRight,
	KeyDown,
	KeySelect,
	KeyPrint,
	KeyExecute,
	KeyPrintScreen,
	KeyIns,
	KeyDel,
	KeyHelp,
	Key0,
	Key1,
	Key2,
	Key3,
	Key4,
	Key5,
	Key6,
	Key7,
	Key8,
	Key9,
	KeyA,
	KeyB,
	KeyC,
	KeyD,
	KeyE,
	KeyF,
	KeyG,
	KeyH,
	KeyI,
	KeyJ,
	KeyK,
	KeyL,
	KeyM,
	KeyN,
	KeyO,
	KeyP,
	KeyQ,
	KeyR,
	KeyS,
	KeyT,
	KeyU,
	KeyV,
	KeyW,
	KeyX,
	KeyY,
	KeyZ,
	KeyLeftWindows,
	KeyRightWindows,
	KeyApplications,
	KeySleep,
	KeyNumpad0,
	KeyNumpad1,
	KeyNumpad2,
	KeyNumpad3,
	KeyNumpad4,
	KeyNumpad5,
	KeyNumpad6,
	KeyNumpad7,
	KeyNumpad8,
	KeyNumpad9,
	KeyMultiply,
	KeyPlus,
	KeySeparator,
	KeyMinus,
	KeyDot,
	KeyDivide,
	KeyF1,
	KeyF2,
	KeyF3,
	KeyF4,
	KeyF5,
	KeyF6,
	KeyF7,
	KeyF8,
	KeyF9,
	// ???
	KeyF10,
	KeyF11,
	KeyF12,
	KeyF13,
	KeyF14,
	KeyF15,
	KeyF16,
	KeyF17,
	KeyF18,
	KeyF19,
	KeyF20,
	KeyF21,
	KeyF22,
	KeyF23,
	KeyF24,
	KeyNumlock,
	KeyScrollLock,
	KeyLeftShift,
	KeyRightShift,
	KeyLeftControl,
	KeyRightControl,
	KeyLeftAlt,
	KeyRightAlt,
};

struct JIIWindow;
typedef void(*JIIWinSetKeyboardCallbackType)(JIIWindow* window, JIIWinKeyState state, JIIWinKeyCode code, int scancode);
typedef void(*JIIWinSetMousePositionCallbackType)(JIIWindow* window, double x, double y);
typedef void(*JIIWinSetMouseButtonCallbackType)(JIIWindow* window, JIIWinKeyState state, JIIWinKeyCode code);

struct JIIWin32Context{
	WNDCLASSA windowClass;
	HWND windowHandle;

	HGLRC glContext;
	HDC deviceContext;
};

struct JIIWindowCallabacks {
	JIIWinSetKeyboardCallbackType keyboardCallback;
	JIIWinSetMousePositionCallbackType mouseMoveCallback;
	JIIWinSetMouseButtonCallbackType mouseButtonCallback;
};

struct JIIWindow {
	char* title;
	i32 width;
	i32 height;
	bool exited;

	JIIWinHint hints;

	JIIWin32Context win32;

	JIIWindowCallabacks callbacks;
};

enum JIIWinEventType {
	None,
	Quit
};

struct JIIWinEvent {
	JIIWinEventType type;
};

enum JIIWinStatus {
	Ok,
	Error
};

#ifdef __cplusplus
extern "C" {
#endif

#ifndef JIIDef
#ifdef JII_WIN_STATIC
#define JIIDef static
#else
#define JIIDef extern
#endif
#endif

	JIIDef const JIIWinHint JII_NO_HINT = 0;
	JIIDef const JIIWinHint JII_NEED_OPENGL = 1;

	JIIDef bool JIIWinGLMakeContextCurrent(JIIWindow* window);

	JIIDef JIIWinStatus JIIWinCreateWindow(const char* title, int width, int height, JIIWindow* window, JIIWinHint hints=JII_NO_HINT);
	JIIDef bool JIIWinExited(JIIWindow* window);
	JIIDef void JIIWinCleanWindow(JIIWindow* window);

	JIIDef void JIIWinSetKeyboardCallback(JIIWindow* window, JIIWinSetKeyboardCallbackType callback);
	JIIDef void JIIWinSetMouseMoveCallback(JIIWindow* window, JIIWinSetMousePositionCallbackType callback);
	JIIDef void JIIWinSetMouseButtonCallback(JIIWindow* window, JIIWinSetMouseButtonCallbackType callback);

	JIIDef JIIWinEvent JIIWinPollEvent(JIIWindow* window);
	JIIDef JIIWinEvent JIIWinWaitEvent(JIIWindow* window);

#ifdef __cplusplus
}
#endif

#ifdef JII_WIN_IMPLMENTATION

#ifndef JIIPrivate
#define JIIPrivate static
#endif

#ifndef JIIAssert
#include <assert.h>
#define JIIAssert(...) assert(__VA_ARGS__)
#endif

#ifndef JIIMalloc
#include <stdlib.h>
#define JIIMalloc(...) malloc(__VA_ARGS__)
#endif

#ifndef JIIFree
#include <stdlib.h>
#define JIIFree(...) free(__VA_ARGS__)
#endif

#define JIIHasHint(hints, hint) (hints & hint)

JIIPrivate JIIWinKeyCode jii_WinKeyCodeMap[0xff] = {};

JIIPrivate void JIIWinInitializeKeycodes() {
	jii_WinKeyCodeMap[VK_LBUTTON] = JIIWinKeyCode::MouseLeftButton;
	jii_WinKeyCodeMap[VK_RBUTTON] = JIIWinKeyCode::MouseRightButton;
	jii_WinKeyCodeMap[VK_CANCEL] = JIIWinKeyCode::KeyBreak;
	jii_WinKeyCodeMap[VK_MBUTTON] = JIIWinKeyCode::MouseMiddleButton;
	jii_WinKeyCodeMap[VK_XBUTTON1] = JIIWinKeyCode::MouseX1Button;
	jii_WinKeyCodeMap[VK_XBUTTON2] = JIIWinKeyCode::MouseX2Button;
	jii_WinKeyCodeMap[VK_BACK] = JIIWinKeyCode::KeyBackspace;
	jii_WinKeyCodeMap[VK_TAB] = JIIWinKeyCode::KeyTab;
	jii_WinKeyCodeMap[VK_CLEAR] = JIIWinKeyCode::KeyClear;
	jii_WinKeyCodeMap[VK_RETURN] = JIIWinKeyCode::KeyEnter;
	jii_WinKeyCodeMap[VK_SHIFT] = JIIWinKeyCode::KeyShift;
	jii_WinKeyCodeMap[VK_CONTROL] = JIIWinKeyCode::KeyControl;
	jii_WinKeyCodeMap[VK_MENU] = JIIWinKeyCode::KeyAlt;
	jii_WinKeyCodeMap[VK_PAUSE] = JIIWinKeyCode::KeyPause;
	jii_WinKeyCodeMap[VK_CAPITAL] = JIIWinKeyCode::KeyCapsLock;
	jii_WinKeyCodeMap[VK_ESCAPE] = JIIWinKeyCode::KeyEsc;
	jii_WinKeyCodeMap[VK_SPACE] = JIIWinKeyCode::KeySpace;
	jii_WinKeyCodeMap[VK_PRIOR] = JIIWinKeyCode::KeyPageUp;
	jii_WinKeyCodeMap[VK_NEXT] = JIIWinKeyCode::KeyPageDown;
	jii_WinKeyCodeMap[VK_END] = JIIWinKeyCode::KeyEnd;
	jii_WinKeyCodeMap[VK_HOME] = JIIWinKeyCode::KeyHome;
	jii_WinKeyCodeMap[VK_LEFT] = JIIWinKeyCode::KeyLeft;
	jii_WinKeyCodeMap[VK_UP] = JIIWinKeyCode::KeyUp;
	jii_WinKeyCodeMap[VK_RIGHT] = JIIWinKeyCode::KeyRight;
	jii_WinKeyCodeMap[VK_DOWN] = JIIWinKeyCode::KeyDown;
	jii_WinKeyCodeMap[VK_SELECT] = JIIWinKeyCode::KeySelect;
	jii_WinKeyCodeMap[VK_PRINT] = JIIWinKeyCode::KeyPrint;
	jii_WinKeyCodeMap[VK_EXECUTE] = JIIWinKeyCode::KeyExecute;
	jii_WinKeyCodeMap[VK_SNAPSHOT] = JIIWinKeyCode::KeyPrintScreen;
	jii_WinKeyCodeMap[VK_INSERT] = JIIWinKeyCode::KeyIns;
	jii_WinKeyCodeMap[VK_DELETE] = JIIWinKeyCode::KeyDel;
	jii_WinKeyCodeMap[VK_HELP] = JIIWinKeyCode::KeyHelp;
	jii_WinKeyCodeMap[0x30] = JIIWinKeyCode::Key0; 
	jii_WinKeyCodeMap[0x31] = JIIWinKeyCode::Key1;
	jii_WinKeyCodeMap[0x32] = JIIWinKeyCode::Key2;
	jii_WinKeyCodeMap[0x33] = JIIWinKeyCode::Key3;
	jii_WinKeyCodeMap[0x34] = JIIWinKeyCode::Key4;
	jii_WinKeyCodeMap[0x35] = JIIWinKeyCode::Key5;
	jii_WinKeyCodeMap[0x36] = JIIWinKeyCode::Key6;
	jii_WinKeyCodeMap[0x37] = JIIWinKeyCode::Key7;
	jii_WinKeyCodeMap[0x38] = JIIWinKeyCode::Key8;
	jii_WinKeyCodeMap[0x39] = JIIWinKeyCode::Key9;
	jii_WinKeyCodeMap[0x41] = JIIWinKeyCode::KeyA;
	jii_WinKeyCodeMap[0x42] = JIIWinKeyCode::KeyB;
	jii_WinKeyCodeMap[0x43] = JIIWinKeyCode::KeyC;
	jii_WinKeyCodeMap[0x44] = JIIWinKeyCode::KeyD;
	jii_WinKeyCodeMap[0x45] = JIIWinKeyCode::KeyE;
	jii_WinKeyCodeMap[0x46] = JIIWinKeyCode::KeyF;
	jii_WinKeyCodeMap[0x47] = JIIWinKeyCode::KeyG;
	jii_WinKeyCodeMap[0x48] = JIIWinKeyCode::KeyH;
	jii_WinKeyCodeMap[0x49] = JIIWinKeyCode::KeyI;
	jii_WinKeyCodeMap[0x4A] = JIIWinKeyCode::KeyJ;
	jii_WinKeyCodeMap[0x4B] = JIIWinKeyCode::KeyK;
	jii_WinKeyCodeMap[0x4C] = JIIWinKeyCode::KeyL;
	jii_WinKeyCodeMap[0x4D] = JIIWinKeyCode::KeyM;
	jii_WinKeyCodeMap[0x4E] = JIIWinKeyCode::KeyN;
	jii_WinKeyCodeMap[0x4F] = JIIWinKeyCode::KeyO;
	jii_WinKeyCodeMap[0x50] = JIIWinKeyCode::KeyP;
	jii_WinKeyCodeMap[0x51] = JIIWinKeyCode::KeyQ;
	jii_WinKeyCodeMap[0x52] = JIIWinKeyCode::KeyR;
	jii_WinKeyCodeMap[0x53] = JIIWinKeyCode::KeyS;
	jii_WinKeyCodeMap[0x54] = JIIWinKeyCode::KeyT;
	jii_WinKeyCodeMap[0x55] = JIIWinKeyCode::KeyU;
	jii_WinKeyCodeMap[0x56] = JIIWinKeyCode::KeyV;
	jii_WinKeyCodeMap[0x57] = JIIWinKeyCode::KeyW;
	jii_WinKeyCodeMap[0x58] = JIIWinKeyCode::KeyX;
	jii_WinKeyCodeMap[0x59] = JIIWinKeyCode::KeyY;
	jii_WinKeyCodeMap[0x5A] = JIIWinKeyCode::KeyZ;
	jii_WinKeyCodeMap[VK_LWIN] = JIIWinKeyCode::KeyLeftWindows;
	jii_WinKeyCodeMap[VK_RWIN] = JIIWinKeyCode::KeyRightWindows;
	jii_WinKeyCodeMap[VK_APPS] = JIIWinKeyCode::KeyApplications;
	jii_WinKeyCodeMap[VK_SLEEP] = JIIWinKeyCode::KeySleep;
	jii_WinKeyCodeMap[VK_NUMPAD0] = JIIWinKeyCode::KeyNumpad0;
	jii_WinKeyCodeMap[VK_NUMPAD1] = JIIWinKeyCode::KeyNumpad1;
	jii_WinKeyCodeMap[VK_NUMPAD2] = JIIWinKeyCode::KeyNumpad2;
	jii_WinKeyCodeMap[VK_NUMPAD3] = JIIWinKeyCode::KeyNumpad3;
	jii_WinKeyCodeMap[VK_NUMPAD4] = JIIWinKeyCode::KeyNumpad4;
	jii_WinKeyCodeMap[VK_NUMPAD5] = JIIWinKeyCode::KeyNumpad5;
	jii_WinKeyCodeMap[VK_NUMPAD6] = JIIWinKeyCode::KeyNumpad6;
	jii_WinKeyCodeMap[VK_NUMPAD7] = JIIWinKeyCode::KeyNumpad7;
	jii_WinKeyCodeMap[VK_NUMPAD8] = JIIWinKeyCode::KeyNumpad8;
	jii_WinKeyCodeMap[VK_NUMPAD9] = JIIWinKeyCode::KeyNumpad9;
	jii_WinKeyCodeMap[VK_MULTIPLY] = JIIWinKeyCode::KeyMultiply;
	jii_WinKeyCodeMap[VK_ADD] = JIIWinKeyCode::KeyPlus;
	jii_WinKeyCodeMap[VK_SEPARATOR] = JIIWinKeyCode::KeySeparator;
	jii_WinKeyCodeMap[VK_SUBTRACT] = JIIWinKeyCode::KeyMinus;
	jii_WinKeyCodeMap[VK_DECIMAL] = JIIWinKeyCode::KeyDot;
	jii_WinKeyCodeMap[VK_DIVIDE] = JIIWinKeyCode::KeyDivide;
	jii_WinKeyCodeMap[VK_F1] = JIIWinKeyCode::KeyF1;
	jii_WinKeyCodeMap[VK_F2] = JIIWinKeyCode::KeyF2;
	jii_WinKeyCodeMap[VK_F3] = JIIWinKeyCode::KeyF3;
	jii_WinKeyCodeMap[VK_F4] = JIIWinKeyCode::KeyF4;
	jii_WinKeyCodeMap[VK_F5] = JIIWinKeyCode::KeyF5;
	jii_WinKeyCodeMap[VK_F6] = JIIWinKeyCode::KeyF6;
	jii_WinKeyCodeMap[VK_F7] = JIIWinKeyCode::KeyF7;
	jii_WinKeyCodeMap[VK_F8] = JIIWinKeyCode::KeyF8;
	jii_WinKeyCodeMap[VK_F9] = JIIWinKeyCode::KeyF9;
	// ???
	jii_WinKeyCodeMap[VK_F10] = JIIWinKeyCode::KeyF10;
	jii_WinKeyCodeMap[VK_F11] = JIIWinKeyCode::KeyF11;
	jii_WinKeyCodeMap[VK_F12] = JIIWinKeyCode::KeyF12;
	jii_WinKeyCodeMap[VK_F13] = JIIWinKeyCode::KeyF13;
	jii_WinKeyCodeMap[VK_F14] = JIIWinKeyCode::KeyF14;
	jii_WinKeyCodeMap[VK_F15] = JIIWinKeyCode::KeyF15;
	jii_WinKeyCodeMap[VK_F16] = JIIWinKeyCode::KeyF16;
	jii_WinKeyCodeMap[VK_F17] = JIIWinKeyCode::KeyF17;
	jii_WinKeyCodeMap[VK_F18] = JIIWinKeyCode::KeyF18;
	jii_WinKeyCodeMap[VK_F19] = JIIWinKeyCode::KeyF19;
	jii_WinKeyCodeMap[VK_F20] = JIIWinKeyCode::KeyF20;
	jii_WinKeyCodeMap[VK_F21] = JIIWinKeyCode::KeyF21;
	jii_WinKeyCodeMap[VK_F22] = JIIWinKeyCode::KeyF22;
	jii_WinKeyCodeMap[VK_F23] = JIIWinKeyCode::KeyF23;
	jii_WinKeyCodeMap[VK_F24] = JIIWinKeyCode::KeyF24;
	jii_WinKeyCodeMap[VK_NUMLOCK] = JIIWinKeyCode::KeyNumlock;
	jii_WinKeyCodeMap[VK_SCROLL] = JIIWinKeyCode::KeyScrollLock;
	jii_WinKeyCodeMap[VK_LSHIFT] = JIIWinKeyCode::KeyLeftShift;
	jii_WinKeyCodeMap[VK_RSHIFT] = JIIWinKeyCode::KeyRightShift;
	jii_WinKeyCodeMap[VK_LCONTROL] = JIIWinKeyCode::KeyLeftControl;
	jii_WinKeyCodeMap[VK_RCONTROL] = JIIWinKeyCode::KeyRightControl;
	jii_WinKeyCodeMap[VK_LMENU] = JIIWinKeyCode::KeyLeftAlt;
	jii_WinKeyCodeMap[VK_RMENU] = JIIWinKeyCode::KeyRightAlt;
}

JIIDef bool JIIWinGLMakeContextCurrent(JIIWindow* window) {
	// TODO(Sarmis) we could silently ignore and just notify so the user handles the error
	// TODO(Sarmis) platform specific
	JIIAssert(window && window->win32.deviceContext && window->win32.glContext);

	return wglMakeCurrent(window->win32.deviceContext, window->win32.glContext);
}

JIIDef JIIWinEvent JIIWinWaitEvent(JIIWindow* window) {
	JIIAssert(window && window->win32.windowHandle);

	JIIWinEvent result = {};
	result.type = JIIWinEventType::None;

	MSG msg;
	if (GetMessageA(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessageA(&msg);

		switch (msg.message) {
			case WM_QUIT: {
				window->exited = true;
				result.type = JIIWinEventType::Quit;
				break;
			}
		}
	}

	return result;
}

JIIDef JIIWinEvent JIIWinPollEvent(JIIWindow* window) {
	JIIAssert(window && window->win32.windowHandle);

	JIIWinEvent result = {};
	result.type = JIIWinEventType::None;
	
	MSG msg;
	if (PeekMessageA(&msg, NULL, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessageA(&msg);

		switch (msg.message) {
			case WM_QUIT: {
				window->exited = true;
				result.type = JIIWinEventType::Quit;
				break;
			}
		}
	}

	return result;
}

JIIPrivate JIIWinStatus JIIWinGLCreateContext(JIIWindow* window) {
	JIIAssert(window);

	if (!JIIHasHint(window->hints, JII_NEED_OPENGL)) {
		return JIIWinStatus::Ok;
	}

	// https://www.khronos.org/opengl/wiki/Creating_an_OpenGL_Context_(WGL)
	PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,    //Flags
		PFD_TYPE_RGBA,        // The kind of framebuffer. RGBA or palette.
		32,                   // Colordepth of the framebuffer.
		0, 0, 0, 0, 0, 0,
		0,
		0,
		0,
		0, 0, 0, 0,
		24,                   // Number of bits for the depthbuffer
		8,                    // Number of bits for the stencilbuffer
		0,                    // Number of Aux buffers in the framebuffer.
		PFD_MAIN_PLANE,
		0,
		0, 0, 0
	};

	HDC dc = GetDC(window->win32.windowHandle);
	window->win32.deviceContext = dc;

	int pixelFormat = ChoosePixelFormat(dc, &pfd);
	SetPixelFormat(dc, pixelFormat, &pfd);

	HGLRC context = wglCreateContext(dc);
	window->win32.glContext = context;

	return JIIWinStatus::Ok;
}

JIIPrivate void JIIWinHandleMouseMove(HWND wnd, UINT message, WPARAM wParam, LPARAM lParam) {
	JIIWindow* window = (JIIWindow*)GetWindowLongPtrA(wnd, 0);
	JIIAssert(window);

	if (!window->callbacks.mouseMoveCallback) {
		return;
	}

	u32 masklo = ((1 << 16) - 1);
	u32 maskhi = masklo << 16;

	double x = masklo & lParam;
	double y = (maskhi & lParam) >> 16;

	window->callbacks.mouseMoveCallback(window, x, y);
}

JIIPrivate void JIIWinHandleMouseButton(HWND wnd, UINT message, WPARAM wParam, LPARAM lParam) {
	JIIWindow* window = (JIIWindow*)GetWindowLongPtrA(wnd, 0);
	JIIAssert(window);

	if (!window->callbacks.mouseButtonCallback) {
		return;
	}
	
	JIIWinKeyState state;
	JIIWinKeyCode code;
	switch (message) {
		case WM_LBUTTONDOWN: { code = JIIWinKeyCode::MouseLeftButton; state = JIIWinKeyState::Down; break; }
		case WM_LBUTTONUP: { code = JIIWinKeyCode::MouseLeftButton; state = JIIWinKeyState::Up; break; }

		case WM_RBUTTONDOWN: { code = JIIWinKeyCode::MouseRightButton; state = JIIWinKeyState::Down; break; }
		case WM_RBUTTONUP: { code = JIIWinKeyCode::MouseRightButton; state = JIIWinKeyState::Up; break; }

		case WM_MBUTTONDOWN: { code = JIIWinKeyCode::MouseMiddleButton; state = JIIWinKeyState::Down; break; }
		case WM_MBUTTONUP: { code = JIIWinKeyCode::MouseMiddleButton; state = JIIWinKeyState::Up; break; }

		default: {
			// why are we even here?
			return;
		}
	}

	window->callbacks.mouseButtonCallback(window, state, code);
}

JIIPrivate void JIIWinHandleKeyDownAndUp(HWND wnd, UINT message, WPARAM wParam, LPARAM lParam) {
	JIIWindow* window = (JIIWindow*)GetWindowLongPtrA(wnd, 0);
	JIIAssert(window);

	if (!window->callbacks.keyboardCallback) {
		return;
	}

	JIIWinKeyState state = (HIWORD(lParam) & KF_UP) ? JIIWinKeyState::Up : JIIWinKeyState::Down;

	i32 scancode = (HIWORD(lParam) & (KF_EXTENDED | 0xff));
	if (!scancode) {
		scancode = MapVirtualKeyW((UINT)wParam, MAPVK_VK_TO_VSC);
	}

	if (scancode > 0xff || scancode < 0) {
		return;
	}

	JIIWinKeyCode key = jii_WinKeyCodeMap[scancode];

	window->callbacks.keyboardCallback(window, state, key, scancode);
}

JIIPrivate LRESULT CALLBACK JIIWndProc(HWND wnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
		case WM_CREATE: {
			break;
		}
		case WM_DESTROY: {
			PostQuitMessage(0);
			break;
		}
		
		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP: 
		case WM_MBUTTONDOWN:
		case WM_MBUTTONUP: {
			JIIWinHandleMouseButton(wnd, message, wParam, lParam);
			break;
		}

		case WM_MOUSEMOVE: {
			JIIWinHandleMouseMove(wnd, message, wParam, lParam);
			break;
		}

		case WM_KEYUP: 
		case WM_KEYDOWN: {
			JIIWinHandleKeyDownAndUp(wnd, message, wParam, lParam);
			break;
		};

		default: {
			return DefWindowProcA(wnd, message, wParam, lParam);
		}
	}
	return 0;

}

JIIPrivate HINSTANCE JIIWinGetInstance() {
	return GetModuleHandle(NULL);
}

JIIDef bool JIIWinExited(JIIWindow* window) {
	return window->exited;
}

JIIDef void JIIWinSetKeyboardCallback(JIIWindow* window, JIIWinSetKeyboardCallbackType callback) {
	JIIAssert(callback);
	window->callbacks.keyboardCallback = callback;
}

JIIDef void JIIWinSetMouseMoveCallback(JIIWindow* window, JIIWinSetMousePositionCallbackType callback) {
	JIIAssert(callback);
	window->callbacks.mouseMoveCallback = callback;
}

JIIDef void JIIWinSetMouseButtonCallback(JIIWindow* window, JIIWinSetMouseButtonCallbackType callback) {
	JIIAssert(callback);
	window->callbacks.mouseButtonCallback = callback;
}

JIIDef JIIWinStatus JIIWinCreateWindow(const char* title, int width, int height, JIIWindow* window, JIIWinHint hints){
	// TODO(Sarmis) platform specific
	JIIAssert(window && title);

	// TODO(Sarmis) this should be hard coded without a function call needed, this should not be called here, but for now
	// this will have to do.
	JIIWinInitializeKeycodes();

	window->callbacks.keyboardCallback = NULL;
	window->callbacks.mouseMoveCallback = NULL;
	window->callbacks.mouseButtonCallback = NULL;

	// simply combine hints
	window->hints = hints;
	window->exited = false;

	window->width = width;
	window->height = height;
	window->title = (char*)title;

	window->win32.windowClass = {};
	window->win32.windowClass.cbWndExtra = sizeof(JIIWindow*);
	window->win32.windowClass.lpfnWndProc = JIIWndProc;
	window->win32.windowClass.hInstance = JIIWinGetInstance();
	window->win32.windowClass.lpszClassName = title;
	
	if (!RegisterClassA(&window->win32.windowClass)) {
		return JIIWinStatus::Error;
	}

	window->win32.windowHandle = CreateWindowA(
		window->win32.windowClass.lpszClassName, title,
		WS_TILEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		width, height,
		NULL, NULL, window->win32.windowClass.hInstance, NULL);

	if(!window->win32.windowHandle) {
		return JIIWinStatus::Error;
	}

	SetWindowLongPtrA(window->win32.windowHandle, 0, (LONG_PTR)window);

	ShowWindow(window->win32.windowHandle, SW_NORMAL);

	if (JIIHasHint(window->hints, JII_NEED_OPENGL)) {
		JIIWinStatus status = JIIWinGLCreateContext(window);
		if (status != JIIWinStatus::Ok) {
			return status;
		}
		JIIWinGLMakeContextCurrent(window);
	}


	return JIIWinStatus::Ok;
}

JIIDef void JIIWinCleanWindow(JIIWindow* window) {
	// TODO(Sarmis) platform specific
	JIIAssert(window);

	if (window->win32.glContext) {
		wglDeleteContext(window->win32.glContext);
	}
}

#endif // JII_WIN_IMPLMENTATION
