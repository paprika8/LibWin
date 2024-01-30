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
		ATOM getAtom() {
			return (ATOM)GetClassLongPtr(hWnd, GCL_CBWNDEXTRA);
		}
		void setExtra(int value) {
			SetClassLongPtr(hWnd, GCL_CBCLSEXTRA, (LONG_PTR)value);
			InvalidateRect(hWnd, NULL, FALSE);
		}
		int getExtra() {
			return (int)GetClassLongPtr(hWnd, GCL_CBCLSEXTRA);
		}
		void setExtraWindow(int value) {
			SetClassLongPtr(hWnd, GCL_CBWNDEXTRA, (LONG_PTR)value);
			InvalidateRect(hWnd, NULL, FALSE);
		}
		int getExtraWindow() {
			return (int)GetClassLongPtr(hWnd, GCL_CBWNDEXTRA);
		}
		void setBackground(HBRUSH value) {
			SetClassLongPtr(hWnd, GCLP_HBRBACKGROUND, (LONG_PTR)value);
			InvalidateRect(hWnd, NULL, FALSE);
		}
		HBRUSH getBackground() {
			return (HBRUSH)GetClassLongPtr(hWnd, GCLP_HBRBACKGROUND);
		}
		void setCursor(HCURSOR value) {
			SetClassLongPtr(hWnd, GCLP_HCURSOR, (LONG_PTR)value);
			InvalidateRect(hWnd, NULL, FALSE);
		}
		HCURSOR getCursor() {
			return (HCURSOR)GetClassLongPtr(hWnd, GCLP_HCURSOR);
		}
		void setIcon(HICON value) {
			SetClassLongPtr(hWnd, GCLP_HICON, (LONG_PTR)value);
			InvalidateRect(hWnd, NULL, FALSE);
		}
		HICON getIcon() {
			return (HICON)GetClassLongPtr(hWnd, GCLP_HICON);
		}
		void setSmallIcon(HICON value) {
			SetClassLongPtr(hWnd, GCLP_HICONSM, (LONG_PTR)value);
			InvalidateRect(hWnd, NULL, FALSE);
		}
		HICON getSmallIcon() {
			return (HICON)GetClassLongPtr(hWnd, GCLP_HICONSM);
		}
		void setModule(HMODULE value) {
			SetClassLongPtr(hWnd, GCLP_HMODULE, (LONG_PTR)value);
			InvalidateRect(hWnd, NULL, FALSE);
		}
		HMODULE getModule() {
			return (HMODULE)GetClassLongPtr(hWnd, GCLP_HMODULE);
		}
		void setMenuName(LPCWSTR value) {
			SetClassLongPtr(hWnd, GCLP_MENUNAME, (LONG_PTR)value);
			InvalidateRect(hWnd, NULL, FALSE);
		}
		LPCWSTR getMenuName() {
			return (LPCWSTR)GetClassLongPtr(hWnd, GCLP_MENUNAME);
		}
		void setStyle(UINT value) {
			SetClassLongPtr(hWnd, GCL_STYLE, (LONG_PTR)value);
			InvalidateRect(hWnd, NULL, FALSE);
		}
		UINT getStyle() {
			return (UINT)GetClassLongPtr(hWnd, GCL_STYLE);
		}
		void setWindowProcess(WNDPROC value) {
			SetClassLongPtr(hWnd, GCLP_WNDPROC, (LONG_PTR)value);
			InvalidateRect(hWnd, NULL, FALSE);
		}
		WNDPROC getWindowProcess() {
			return (WNDPROC)GetClassLongPtr(hWnd, GCLP_WNDPROC);
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