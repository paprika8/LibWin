// LibWin.cpp : Определяет функции для статической библиотеки.
//
#include "pch.h"
#include "LibWin.h"
#include <objidl.h>

#pragma comment (lib,"Gdiplus.lib")
#include <gdiplus.h>

#include <stdio.h>
using namespace Gdiplus;

namespace Windows{
	ULONG_PTR gdiplusToken = 0;
	int InitLib(HINSTANCE hInstance) {
		GdiplusStartupInput gdiplusStartupInput;

		Status st = GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
		if (st != Ok)
		{
			MessageBox(NULL,
				_T("Call to GdiplusStartup failed!"),
				_T("GdiplusStartup ist kaputt"),
				NULL);

			return -1;
		}
		Window::reg(hInstance);
		RoundWin::reg(hInstance);
		Components::CRegister();
		return 0;
	}
	void ExitLib() {
		GdiplusShutdown(gdiplusToken);
		Components::CUnregister();
	}

#pragma region baseWindow
	int baseWindow::init(_In_ int nCmdShow) {
		return init(
				WS_EX_OVERLAPPEDWINDOW,
				0,
				CW_USEDEFAULT, CW_USEDEFAULT,
				400, 500,
				NULL,
				NULL,
				GetModuleHandle(NULL),
				NULL
		);
	}
	int baseWindow::init(int width, int hight, _In_ int nCmdShow) {
		return init(
				WS_EX_OVERLAPPEDWINDOW,
				0,
				CW_USEDEFAULT, CW_USEDEFAULT,
				width, hight,
				NULL,
				NULL,
				GetModuleHandle(NULL),
				NULL
		);
	}
	int baseWindow::init(int width, int hight, unsigned long dwStyle, _In_ int nCmdShow) {
		return init(
				WS_EX_OVERLAPPEDWINDOW,
				dwStyle,
				CW_USEDEFAULT, CW_USEDEFAULT,
				width, hight,
				NULL,
				NULL,
				GetModuleHandle(NULL),
				NULL
		);
	}
	int baseWindow::init(int width, int hight, unsigned long dwStyle, unsigned long dwExStyle, _In_ int nCmdShow) {
		return init(
				dwExStyle,
				dwStyle,
				CW_USEDEFAULT, CW_USEDEFAULT,
				width, hight,
				NULL,
				NULL,
				GetModuleHandle(NULL),
				NULL
		);
	}
	int baseWindow::run() {
		MSG msg;
		while (GetMessage(&msg, NULL, 0, 0))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		return (int)msg.wParam;
	}
	LRESULT CALLBACK baseWindow::SWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		WData* wData = (WData*)GetWindowLongPtr(hWnd, 0);
		switch (uMsg) {
		case WM_NCCREATE:
		{
			wData = (WData*)malloc(sizeof(WData));
			if (wData == NULL)
				return FALSE;
			wData->that = 0;
			SetWindowLongPtr(hWnd, 0, (LONG_PTR)wData);
		}
		return TRUE;
		case WM_DESTROY:
		case WM_NCDESTROY:
			SetWindowLongPtr(hWnd, 0, 0);
			if(wData != NULL)
			{
				auto buf = wData->that->eve.has(uMsg);
				if (buf)
					for (auto i = buf->m; i != buf->m + buf->len; i++)
						(*i)(hWnd, uMsg, wParam, lParam);
				delete wData->that;
				free(wData);
				wData = 0;
			}
			break;
		default:
			if (wData)
				if (wData->that) {
					{
						auto buf = wData->that->eve.has(uMsg);
						if (buf)
							for (auto i = buf->m; i != buf->m + buf->len; i++)
								if (LRESULT res = (*i)(hWnd, uMsg, wParam, lParam))
									return res;
					}
					return wData->that->wndProc(hWnd, uMsg, wParam, lParam);
				}
			break;
		}

		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
#pragma endregion
#pragma region Window
	TCHAR* Window::szWindowClass = (TCHAR*)((void*)L"WindRect");
	int Window::init(_In_ DWORD dwExStyle,_In_ DWORD dwStyle,_In_ int X,_In_ int Y,_In_ int nWidth,_In_ int nHeight,
		_In_opt_ HWND hWndParent,_In_opt_ HMENU hMenu,_In_opt_ HINSTANCE hInstance,_In_opt_ LPVOID lpParam,_In_ int nCmdShow
	) {
		hWnd = CreateWindowExW(
			dwExStyle,
			szWindowClass,
			szTitle,
			dwStyle | WS_OVERLAPPEDWINDOW,
			X, Y,
			nWidth, nHeight,
			hWndParent,
			hMenu,
			hInstance,
			lpParam
		);
		if (!hWnd)
		{
			MessageBox(NULL,
				_T("Call to CreateWindowEx failed!"),
				_T("Windows Desktop Guided Tour"),
				NULL);
			return -1;
		}
		WData* wData = new WData();
		wData->that = this;
		SetWindowLongPtr(hWnd, 0, (LONG_PTR)wData);

		ShowWindow(hWnd,
			nCmdShow);
		UpdateWindow(hWnd);

	}
	int Window::reg(_In_ HINSTANCE hInstance) {
		WNDCLASSEX wcex;

		wcex.cbSize = sizeof(WNDCLASSEX);
		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = SWndProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = sizeof(WData*);
		wcex.hInstance = hInstance;
		wcex.hIcon = LoadIcon(wcex.hInstance, IDI_APPLICATION);
		wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
		wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		wcex.lpszMenuName = NULL;
		wcex.lpszClassName = szWindowClass;
		wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);

		if (!RegisterClassEx(&wcex))
		{
			MessageBox(NULL,
				_T("Call to RegisterClassEx failed!"),
				_T("Windows Desktop Guided Tour"),
				NULL);

			return -1;
		}
		return 0;
	}
	void Window::initW() {

	}
	Window::Window() {
		initW();
	}
	Window::Window(
		TCHAR *szTitle
	) {
		this->szTitle = szTitle;
		initW();
	}
	LRESULT CALLBACK Window::wndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		PAINTSTRUCT ps;
		HDC hdc;
		TCHAR greeting[] = (L"For more information, <A HREF=\"http://www.microsoft.com\">click here</A> " \
			L"or <A ID=\"idInfo\">here</A>.");

		switch (message)
		{
		case WM_SIZE:
		{
			RECT rc;
			GetWindowRect(hWnd, &rc);
			if (childs.len != 0)
				childs[0]->reRect({ 0, 0 }, { rc.right - rc.left, rc.bottom - rc.top });
		}
		break;
		case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps);

			EndPaint(hWnd, &ps);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
			break;
		}

		return 0;
	}
	Window::~Window() {
		if (hWnd)
			SendMessage(hWnd, WM_DESTROY, 0, 0);
		// TODO WORK!
	}
	stdminus::arr<Components::Component*> Window::getChildren() {
		stdminus::arr<Components::Component*> elems;
		auto g = [](HWND hwnd, LPARAM param) -> BOOL {
			((stdminus::arr<Components::Component*>*)param)->add(((Components::CData*)GetWindowLongPtr(hwnd, 0))->that);
			return true;
			};
		EnumChildWindows(hWnd, g, (LPARAM)&elems);
		return elems;
	}
	void Window::add(Components::Component* comp) {
		if (childs.len != 0)
			remC(childs[0]);
		comp->Configure(hWnd);
		RECT rc;
		GetWindowRect(hWnd, &rc);	
		comp->reRect({ 0, 0 }, { rc.right - rc.left, rc.bottom - rc.top });
		childs.add(comp);
		comp->eve[WM_NCDESTROY].add([=](HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)-> LRESULT { remC(comp); return 0; });
	}
	void Window::remC(Components::Component* comp) {
		SendMessage(comp->GetHWND(), WM_DESTROY, 0, 0);
		childs.rem(comp);
	}
#pragma endregion
#pragma region RoundWin
	const TCHAR* RoundWin::szWindowClass = (TCHAR*)((void*)L"WindRound");
	int RoundWin::init(_In_ DWORD dwExStyle, _In_ DWORD dwStyle, _In_ int X, _In_ int Y, _In_ int nWidth, _In_ int nHeight,
		_In_opt_ HWND hWndParent, _In_opt_ HMENU hMenu, _In_opt_ HINSTANCE hInstance, _In_opt_ LPVOID lpParam, _In_ int nCmdShow
	) {
		hWnd = CreateWindowExW(
			dwExStyle,
			szWindowClass,
			szTitle,
			dwStyle,
			X, Y,
			nWidth, nWidth,
			hWndParent,
			hMenu,
			hInstance,
			lpParam
		);
		if (!hWnd)
		{
			MessageBox(NULL,
				_T("Call to CreateWindowEx failed!"),
				_T("Windows Desktop Guided Tour"),
				NULL);
			return -1;
		}
		WData* wData = new WData();
		wData->that = this;
		SetWindowLongPtr(hWnd, 0, (LONG_PTR)wData);

		/*LONG lStyle = GetWindowLong(hWnd, GWL_STYLE);
		lStyle &= ~(WS_CAPTION | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU);*/
		SetWindowLong(hWnd, GWL_STYLE, 0);
		SetWindowLong(hWnd, GWL_EXSTYLE, 0);

		RECT rc;
		GetWindowRect(hWnd, &rc);
		HRGN rgn = CreateEllipticRgn(0, 0, rc.right - rc.left, rc.bottom - rc.top);
		SetWindowRgn(hWnd, rgn, TRUE);

		
		backSize = nWidth;

		// Inform the application of the frame change.
		SetWindowPos(hWnd,
					 NULL,
					 rc.left, rc.top,
					 rc.right - rc.left, rc.bottom - rc.top,
					 SWP_FRAMECHANGED);
		ShowWindow(hWnd,
			nCmdShow);
		UpdateWindow(hWnd);

	}
	int RoundWin::reg(_In_ HINSTANCE hInstance) {
		WNDCLASSEX wcex;

		wcex.cbSize = sizeof(WNDCLASSEX);
		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = SWndProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = sizeof(WData*);
		wcex.hInstance = hInstance;
		wcex.hIcon = LoadIcon(wcex.hInstance, IDI_APPLICATION);
		wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
		wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		wcex.lpszMenuName = NULL;
		wcex.lpszClassName = szWindowClass;
		wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);

		if (!RegisterClassEx(&wcex))
		{
			MessageBox(NULL,
				_T("Call to RegisterClassEx failed!"),
				_T("Windows Desktop Guided Tour"),
				NULL);

			return -1;
		}
		return 0;
	}
	void RoundWin::initW() {
		border.h = 3;
	}
	RoundWin::RoundWin() {
		initW();
	}
	RoundWin::RoundWin(
		TCHAR* szTitle
	) {
		this->szTitle = szTitle;
		initW();
	}
	LRESULT CALLBACK RoundWin::wndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch (message)
		{
		
		case WM_WINDOWPOSCHANGING:
		{
			WINDOWPOS* winPos = (WINDOWPOS*)lParam;
			if (winPos->flags & 1)
				return 1;
			if (winPos->cx != backSize)
				if(isX){
					winPos->cy = winPos->cx;
					backSize = winPos->cx;
					OutputDebugString(L"x\n");
				}
				else
					winPos->cx = winPos->cy;
			if (winPos->cy != backSize) 
				if (!isX) {
					winPos->cx = winPos->cy;
					backSize = winPos->cy;
					OutputDebugString(L"y\n");
				}
				else
					winPos->cy = winPos->cx;
			
			return 0;
		}
		break;
		case WM_SIZE:
		{
			RECT rc;
			GetWindowRect(hWnd, &rc);
			if (childs.len != 0)
				childs[0]->reRect({ 0, 0 }, { rc.right - rc.left, rc.bottom - rc.top });

			HRGN rgn;
			rgn = CreateEllipticRgn(0, 0, rc.right - rc.left, rc.bottom - rc.top);
			SetWindowRgn(hWnd, rgn, TRUE);
			
		}
		break;
		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);
			
			RECT rc;
			GetWindowRect(hWnd, &rc);
			rc.right = rc.right - rc.left;
			rc.bottom = rc.bottom - rc.top;
			rc.left = 0;
			rc.top = 0;
			border.TouchRect(ps.rcPaint);
			border.CPaint(hWnd, hdc, rc);
			
			EndPaint(hWnd, &ps);
		}
		break;
		case WM_NCHITTEST:
		{
			POINT ptMouse = { LOWORD(lParam), HIWORD(lParam) };

			RECT rcWindow;
			GetWindowRect(hWnd, &rcWindow);

			POINT size = { rcWindow.right - rcWindow.left, rcWindow.bottom - rcWindow.top };
			
			int r = size.x / 2;
			ptMouse.x -= rcWindow.left + r;
			ptMouse.y -= rcWindow.top + r;

			USHORT uRow = 1;
			USHORT uCol = 1;
			bool fOnResizeBorder = true;
			int d = ptMouse.x * ptMouse.x + ptMouse.y * ptMouse.y;
			int l = r * r - (r - border.frame) * (r - border.frame);

			if (r * r >= d && (r - border.h) * (r - border.h) <= d) 
			{
				if (abs(ptMouse.x) > abs(ptMouse.y)) 
				{
					if (ptMouse.x > 0)
						uCol = 2;
					else
						uCol = 0;
				}
				else
				{ 
					if (ptMouse.y > 0) 
					{
						uRow = 2;
					}	
					else
						uRow = 0;
				}
			}
			
			if (ptMouse.x * ptMouse.x <= l && ptMouse.y * ptMouse.y <= r*r && (r - border.frame) * (r - border.frame) <= ptMouse.y * ptMouse.y && ptMouse.y < 0){
				fOnResizeBorder = false;
				uRow = 0;
			}

			LRESULT hitTests[3][3] =
			{
				{ 0,    fOnResizeBorder ? HTTOP : HTCAPTION,    0 },
				{ HTLEFT,       HTNOWHERE,     HTRIGHT },
				{ 0, HTBOTTOM, 0 },
			};
			if (uRow == 1)
				isX = 1;
			else
				isX = 0;
			return hitTests[uRow][uCol]; 
		}
		break;
		case WM_KEYDOWN:
			switch (wParam)
			{
			case VK_ESCAPE:
				PostQuitMessage(0);
				break;
			}
			break;
		break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
			break;
		}

		return 0;
	}
	RoundWin::~RoundWin() {
		// TODO WORK!
	}
	stdminus::arr<Components::Component*> RoundWin::getChildren() {
		stdminus::arr<Components::Component*> elems;
		auto g = [](HWND hwnd, LPARAM param) -> BOOL {
			((stdminus::arr<Components::Component*>*)param)->add(((Components::CData*)GetWindowLongPtr(hwnd, 0))->that);
			return true;
			};
		EnumChildWindows(hWnd, g, (LPARAM)&elems);
		return elems;
	}
	void RoundWin::add(Components::Component* comp) {
		if (childs.len != 0)
			remC(childs[0]);
		comp->Configure(hWnd);
		RECT rc;
		GetWindowRect(hWnd, &rc);
		comp->reRect({ 0, 0 }, { rc.right - rc.left, rc.bottom - rc.top });
		childs.add(comp);
	}
	void RoundWin::remC(Components::Component* comp) {
		childs.rem(comp);
		SendMessage(comp->GetHWND(), WM_DESTROY, 0, 0);
	}
#pragma endregion


	PRoundBorderWin::PRoundBorderWin() {
		this->h = 0;
	}
	PRoundBorderWin::PRoundBorderWin(int h) {
		this->h = h;
	}
	void PRoundBorderWin::CPaint(HWND hwnd, HDC hdc, RECT& rcDirty) {
		HRGN hFrame = CreateRectRgn(rcDirty.left, rcDirty.top, rcDirty.right, rcDirty.top + frame);
		HRGN hRgn = CreateEllipticRgn(rcDirty.left + h, rcDirty.top + h, rcDirty.right - h, rcDirty.bottom - h);

		HRGN hHdrRgn = CreateEllipticRgn(rcDirty.left, rcDirty.top, rcDirty.right, rcDirty.bottom);
		CombineRgn(hRgn, hRgn, hHdrRgn, RGN_XOR);
		Gdiplus::Graphics gr(hdc);

		SolidBrush br(Gdiplus::Color(255, 0, 0, 0));
		SolidBrush brF(Gdiplus::Color(255, 20, 20, 20));
	
		Region* reg = new Region(hFrame);
		gr.FillRegion(&brF, reg);
		delete reg;

		reg = new Region(hRgn);
		gr.FillRegion(&br, reg);
		delete reg;

		DeleteObject(hFrame);
		DeleteObject(hRgn);
		DeleteObject(hHdrRgn);
	}
	void PRoundBorderWin::TouchRect(RECT& rcDrawing) {
		rcDrawing.left = rcDrawing.left + h;
		rcDrawing.top = rcDrawing.top + h;
		rcDrawing.right = rcDrawing.right - h - h;
		rcDrawing.bottom = rcDrawing.bottom - h - h;
	}


	const TCHAR* CastomMenu::szWindowClass = L"CastomMenu";
	int CastomMenu::reg(_In_ HINSTANCE hInstance) {
		WNDCLASSEX wcex;

		wcex.cbSize = sizeof(WNDCLASSEX);
		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = SWndProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = sizeof(WData*);
		wcex.hInstance = hInstance;
		wcex.hIcon = LoadIcon(wcex.hInstance, IDI_APPLICATION);
		wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
		wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		wcex.lpszMenuName = NULL;
		wcex.lpszClassName = szWindowClass;
		wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);

		if (!RegisterClassEx(&wcex))
		{
			MessageBox(NULL,
				_T("Call to RegisterClassEx failed!"),
				_T("Windows Desktop Guided Tour"),
				NULL);

			return -1;
		}
		return 0;
	}
	CastomMenu::CastomMenu(HWND parent) {
		initW(NULL, 50, 50, parent, 0);
	}
	CastomMenu::CastomMenu(HWND parent, Components::CRoundMargin mar) {
		margin = mar;
		initW(NULL, 50, 50, parent, 0);
	}
	int CastomMenu::initW(DWORD dwStyle, int nWidth, int nHeight,
		HWND hWndParent, LPVOID lpParam
	) {
		int X = 0;
		int Y = 0;
		hWnd = CreateWindow(
			szWindowClass,
			szTitle,
			dwStyle | WS_OVERLAPPEDWINDOW,
			X, Y,
			nWidth, nHeight,
			hWndParent,
			NULL,
			GetModuleHandle(NULL),
			lpParam
		);
		if (!hWnd)
		{
			MessageBox(NULL,
				_T("Call to CreateWindowEx failed!"),
				_T("Windows Desktop Guided Tour"),
				NULL);
			return -1;
		}
		MData* mData = new MData();
		mData->that = this;
		SetWindowLongPtr(hWnd, 0, (LONG_PTR)mData);
		return 0;
	}
	void CastomMenu::add(Components::Component* comp) {
		components.add(comp);
	}
	void CastomMenu::rem(Components::Component* comp) {
		components.rem(comp);
	}
	LRESULT CALLBACK CastomMenu::wndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
		return 0;
	}
	LRESULT CALLBACK CastomMenu::SWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
		MData* mData = (MData*)GetWindowLongPtr(hWnd, 0);
		switch (message) {
		case WM_NCCREATE:
		{
			mData = (MData*)malloc(sizeof(MData));
			if (mData == NULL)
				return FALSE;
			mData->that = 0;
			SetWindowLongPtr(hWnd, 0, (LONG_PTR)mData);
		}
		return TRUE;
		case WM_DESTROY:
		case WM_NCDESTROY:
			SetWindowLongPtr(hWnd, 0, 0);
			if (mData != NULL)
			{
				auto buf = mData->that->eve.has(message);
				if (buf)
					for(auto i = buf->m; i != buf->m + buf->len; i++)
						(*i)(hWnd, message, wParam, lParam);
				delete mData->that;
				free(mData);
				mData = NULL;
			}
			break;
		default:
			if (mData)
				if (mData->that) {
					{
						auto buf = mData->that->eve.has(message);
						if (buf)
							for (auto i = buf->m; i != buf->m + buf->len; i++)
								if (LRESULT res = (*i)(hWnd, message, wParam, lParam))
									return res;
					}
					return mData->that->wndProc(hWnd, message, wParam, lParam);
				}
			break;
		}

		return DefWindowProc(hWnd, message, wParam, lParam);
	}
};
