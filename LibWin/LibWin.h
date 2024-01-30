#pragma once

#include "pch.h"
#include "framework.h"
#include "Components.h"
#include <Windows.h>
#include <tchar.h>

#define CONTINUE_PROC -20;
namespace Windows {
	class Window;
	class baseWindow;
	class CastomMenu;
	typedef struct WData_tag WData;
	struct WData_tag {
		baseWindow* that;
		void* extraData;
	};
	typedef struct MData_tag MData;
	struct MData_tag {
		CastomMenu* that;
		void* extraData;
	};
	int InitLib(HINSTANCE hInstance);
	class baseWindow {
	public:
		//stdminus::map<int, LRESULT(*)(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)> eve;
		stdminus::WEvents<int, std::function<LRESULT(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)>> eve;
		virtual void add(Components::Component*) = 0;
		int init(_In_ int nCmdShow = SW_NORMAL);
		int init(int width, int hight, _In_ int nCmdShow = SW_NORMAL);
		int init(int width, int hight, unsigned long dwStyle, _In_ int nCmdShow = SW_NORMAL);
		int init(int width, int hight, unsigned long dwStyle, unsigned long dwExStyle, _In_ int nCmdShow = SW_NORMAL);
		virtual int init(
			_In_ DWORD dwExStyle,
			_In_ DWORD dwStyle,
			_In_ int X,
			_In_ int Y,
			_In_ int nWidth,
			_In_ int nHeight,
			_In_opt_ HWND hWndParent,
			_In_opt_ HMENU hMenu,
			_In_opt_ HINSTANCE hInstance,
			_In_opt_ LPVOID lpParam,
			_In_ int nCmdShow = SW_NORMAL
		) = 0;
		int run();
		void setBackground(HBRUSH bruch) {
			SetClassLongPtr(hWnd, GCLP_HBRBACKGROUND, (LONG_PTR)bruch);
			InvalidateRect(hWnd, NULL, FALSE);
		}
		HBRUSH getBackground(HBRUSH bruch) {
			return (HBRUSH)GetClassLongPtr(hWnd, GCLP_HBRBACKGROUND);
		}
		virtual HWND getHWND() {
			return hWnd;
		}
		virtual stdminus::arr<Components::Component*> getChildren() = 0;
		virtual void remC(Components::Component*) = 0;
		virtual LRESULT CALLBACK wndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) = 0;
	protected:
		HWND hWnd;
		TCHAR* szTitle = (TCHAR*)((void*)L"H17");
		static LRESULT CALLBACK SWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
		
	};
	class Window : public baseWindow {
		friend int Windows::InitLib(HINSTANCE hInstance);
	public:
		Window();
		Window(TCHAR* szTitle);
		~Window();
		int init(
			_In_ DWORD dwExStyle,
			_In_ DWORD dwStyle,
			_In_ int X,
			_In_ int Y,
			_In_ int nWidth,
			_In_ int nHeight,
			_In_opt_ HWND hWndParent,
			_In_opt_ HMENU hMenu,
			_In_opt_ HINSTANCE hInstance,
			_In_opt_ LPVOID lpParam,
			_In_ int nCmdShow = SW_NORMAL
		);
		void add(Components::Component*);
		stdminus::arr<Components::Component*> getChildren();
		void remC(Components::Component*);
		LRESULT CALLBACK wndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	private:
		static TCHAR* szWindowClass;
		stdminus::arr<Components::Component*> childs;
		WNDCLASS wc = { };
		static int reg(_In_ HINSTANCE hInstance);
		void initW();
	};

	class PRoundBorderWin : public Components::PseudoComponent {
	public:
		PRoundBorderWin();
		PRoundBorderWin(int h);
		void CPaint(HWND hwnd, HDC hdc, RECT& rcDirty);
		void TouchRect(RECT& rcDrawing);
		int h, frame;
	};
	class CastomMenu {
		friend int Windows::InitLib(HINSTANCE hInstance);
	public:
		//stdminus::map<int, LRESULT(*)(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)> eve;
		stdminus::WEvents<int, std::function<LRESULT(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)>> eve;
		Components::CRoundMargin margin = { 0, 0 };
		CastomMenu(HWND parent);
		CastomMenu(HWND parent, Components::CRoundMargin mar);
		void add(Components::Component*);
		void rem(Components::Component*);
		LRESULT CALLBACK wndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
		HWND getHWND();
	protected:
		HWND hWnd;
		TCHAR* szTitle = (TCHAR*)((void*)L"H17");
		static LRESULT CALLBACK SWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	private:
		const static TCHAR* szWindowClass;
		static int reg(_In_ HINSTANCE hInstance);
		int initW(DWORD dwStyle, int nWidth, int nHeight,
		HWND hWndParent, LPVOID lpParam);
		stdminus::arr<Components::Component*> components;
	};

	class RoundWin : public baseWindow {
		friend int Windows::InitLib(HINSTANCE hInstance);
	public:
		PRoundBorderWin border;
		RoundWin();
		RoundWin(TCHAR* szTitle);
		~RoundWin();
		int init(
			_In_ DWORD dwExStyle,
			_In_ DWORD dwStyle,
			_In_ int X,
			_In_ int Y,
			_In_ int nWidth,
			_In_ int nHeight,
			_In_opt_ HWND hWndParent,
			_In_opt_ HMENU hMenu,
			_In_opt_ HINSTANCE hInstance,
			_In_opt_ LPVOID lpParam,
			_In_ int nCmdShow = SW_NORMAL
		);
		void add(Components::Component*);
		stdminus::arr<Components::Component*> getChildren();
		void remC(Components::Component*);
		LRESULT CALLBACK wndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	private:
		const static TCHAR* szWindowClass;
		int backSize, isX = 0;
		stdminus::arr<Components::Component*> childs;
		WNDCLASS wc = { };
		static int reg(_In_ HINSTANCE hInstance);
		void initW();
	};
	
}