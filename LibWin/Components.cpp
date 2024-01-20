#include "pch.h"
#include "Components.h"
#include "LibWin.h"


//#pragma comment(lib, "Opengl32.lib")
//#include <gl/GL.h>

namespace Components{
#pragma region Component
	TCHAR *Component::Comp_WC = (TCHAR*)((void*)L"Comp");
	void CRegister(){
		Components::Component::Register();
		Components::Text::Register();
	}
	void CUnregister(void) {
		Components::Component::Unregister();
		Components::Text::Unregister();
	}
	/*void Component::GLInit(HWND hThis)
	{
		// Получение контекста WinAPI GDI
		HDC hDC = GetDC(hThis);
		CData* pData = (CData*)GetWindowLongPtr(hThis, 0);
		pData->hDC = hDC;
		// Настройка формата пикселей
		PIXELFORMATDESCRIPTOR pfd;
		memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
		pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
		pfd.nVersion = 1;
		pfd.dwFlags = PFD_DRAW_TO_WINDOW
			| PFD_SUPPORT_OPENGL // Поддержка OpenGL
			| PFD_DOUBLEBUFFER; // Двойная буферизация (чтобы изображение не мерцало)
		pfd.iPixelType = PFD_TYPE_RGBA; // Пиксели поддерживают прозрачность Alpha (поэтому, RGBA, а не RGB)
		pfd.cColorBits = 16; // Глубина цвета - 16 бит
		pfd.cDepthBits = 16;

		int iPixelFormat = ChoosePixelFormat(hDC, &pfd);

		PIXELFORMATDESCRIPTOR bestMatch_pfd;
		DescribePixelFormat(hDC, iPixelFormat, sizeof(pfd), &bestMatch_pfd);

		SetPixelFormat(hDC, iPixelFormat, &pfd);

		// Создание контекста OpenGL
		HGLRC hRC = wglCreateContext(hDC);
		wglMakeCurrent(hDC, hRC);

		// Заливка фона черным:
		// R     = 0/255   = 0.0
		// G     = 0/255   = 0.0
		// B     = 0/255   = 0.0
		// Alpha = 255/255 = 1.0
		//glClearColor(0.0, 0.0, 0.0, 1.0);
	}*/
	void Component::CPaint(HWND hwnd, HDC hdc, RECT* rcDirty, BOOL bErase, CData* pData)
	{

	}
	void Component::CDPaintBuffer(HWND hwnd, PAINTSTRUCT* pPaintStruct)
	{
		int cx = pPaintStruct->rcPaint.right - pPaintStruct->rcPaint.left;
		int cy = pPaintStruct->rcPaint.bottom - pPaintStruct->rcPaint.top;
		HDC hMemDC;
		HBITMAP hBmp;
		HBITMAP hOldBmp;
		POINT ptOldOrigin;

		CData* pData = (CData*)GetWindowLongPtr(hwnd, 0);
		Component* that = pData->that;

		// Create new bitmap-back device context, large as the dirty rectangle.
		hMemDC = CreateCompatibleDC(pPaintStruct->hdc);
		hBmp = CreateCompatibleBitmap(pPaintStruct->hdc, cx, cy);
		hOldBmp = (HBITMAP)SelectObject(hMemDC, hBmp);

		// Do the painting into the memory bitmap.
		OffsetViewportOrgEx(hMemDC, -(pPaintStruct->rcPaint.left),
			-(pPaintStruct->rcPaint.top), &ptOldOrigin);
		{
			int i = 0;
			RECT rcPs = pPaintStruct->rcPaint;
			for(;i < that->psComp.len; i++)
				that->psComp[i]->ChainTouchRect(pPaintStruct->rcPaint);

			that->CPaint(hwnd, hMemDC, &pPaintStruct->rcPaint, TRUE, pData);

			while(i--)
				that->psComp[i]->ChainCPaint(hwnd, hMemDC, rcPs);
		}
		SetViewportOrgEx(hMemDC, ptOldOrigin.x, ptOldOrigin.y, NULL);

		// Blit the bitmap into the screen. This is really fast operation and although
		   // the CustomPaint() can be complex and slow there will be no flicker any more.
		BitBlt(pPaintStruct->hdc, pPaintStruct->rcPaint.left, pPaintStruct->rcPaint.top,
			cx, cy, hMemDC, 0, 0, SRCCOPY);

		// Clean up.
		SelectObject(hMemDC, hOldBmp);
		DeleteObject(hBmp);
		DeleteDC(hMemDC);
	}

	LRESULT CALLBACK Component::SCProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{

		CData* pData = (CData*)GetWindowLongPtr(hwnd, 0);
		switch (uMsg) {
			case WM_NCCREATE:
			{
				pData = (CData*)malloc(sizeof(CData));
				if (pData == NULL)
					return FALSE;
				pData->that = 0;
				SetWindowLongPtr(hwnd, 0, (LONG_PTR)pData);
			}
			return TRUE;
			case WM_DESTROY:
			case WM_NCDESTROY:
				if (pData != NULL) {
					SetWindowLongPtr(hwnd, 0, 0);
					{
						auto buf = pData->that->eve.has(uMsg);
						if (buf)
							return (*buf)(hwnd, uMsg, wParam, lParam);
					}
					delete pData->that;
					free(pData);
					pData = 0;
				}
				return 0;
			default:
				if (pData)
					if (pData->that){
						{
							auto buf = pData->that->eve.has(uMsg);
							if (buf) {
								LRESULT res = (*buf)(hwnd, uMsg, wParam, lParam);
								if (!res)
									return res;
							}
						}
						return pData->that->CProc(hwnd, uMsg, wParam, lParam);
					}
		}
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	LRESULT CALLBACK Component::CProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{	
		CData* pData = (CData*)GetWindowLongPtr(hwnd, 0);
		switch (uMsg) {
			case WM_PAINT:
			{
				PAINTSTRUCT paintStruct;
				HDC hDC = BeginPaint(hwnd, &paintStruct);
				/*SetTextColor(hDC, NULL);
				HFONT hFont = CreateFont(90, 0, 0, 0, 0, 0, 0, 0,
					DEFAULT_CHARSET,
					0, 0, 0, 0,
					L"Arial Bold"
				);
				
				SelectObject(hDC, hFont);*/
				CDPaintBuffer(hwnd, &paintStruct);
				
				//DeleteObject(hFont);
				EndPaint(hwnd, &paintStruct);
			}
			break;
		}
		
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	CSize Component::GetSize() {
		return size;
	}
	IMargin *Component::GetMergin() {
		return margin;
	}
	HWND Component::GetHWND() {
		return hwnd;
	}
	void Component::SetSize(CSize sz) {
		size = sz;
		reRect(pPoint, pSize);
	}
	void Component::SetMergin(IMargin *mr) {
		delete margin;
		margin = mr;
		reRect(pPoint, pSize);
	}
	void Component::reRect() {
		reRect(pPoint, pSize);
	}
	void Component::reRect(CPoint p, CSize s) {
		reRect(p, s, typeResize);
	}
	void Component::reRect(CPoint p, CSize s, char type) {
		typeResize = type;
		pPoint = p;
		pSize = s;
		margin->reRect(p, s, size, GetContentSize(), type);
		MoveWindow(hwnd,
			p.x, p.y, s.width, s.hight, 1);
	}
	stdminus::arr<Component*> Component::GetChildren() {
		stdminus::arr<Component*> elems = stdminus::arr<Component*>();
		auto g = [](HWND hwnd, LPARAM param) -> BOOL {
			((stdminus::arr<Component*>*)param)->add(((CData*)GetWindowLongPtr(hwnd, 0))->that);
			return true;
			};
		EnumChildWindows(hwnd, g, (LPARAM)&elems);
		return elems;
	}
	void Component::Configure(HWND hWnd) {
		CData* pData = (CData*)GetWindowLongPtr(hWnd, 0);
		pData = (CData*)malloc(sizeof(CData));
		if (pData == NULL)
			return;
		SetWindowLongPtr(hWnd, 0, (LONG_PTR)pData);
		pData->that = this;
		{
			auto buf = eve.has(WM_CREATE);
			if (buf)
				(*buf)(hWnd, WM_CREATE, 0, 0);
			buf = eve.has(WM_NCCREATE);
			if (buf)
				(*buf)(hWnd, WM_NCCREATE, 0, 0);
		}
		hwnd = hWnd;
		reRect(pPoint, pSize);
	}
	Component::Component()
	{
	}

	Component::~Component()
	{
	}
	void Component::Register()
	{
		WNDCLASS wc = { 0 };

		wc.style = CS_GLOBALCLASS | CS_HREDRAW | CS_VREDRAW;
		wc.lpfnWndProc = SCProc;
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.lpszClassName = Comp_WC;
		wc.cbClsExtra = sizeof(LONG_PTR*);
		RegisterClass(&wc);
	}

	void Component::Unregister()
	{
		UnregisterClass(Comp_WC, NULL);
	}
	CSize Component::GetContentSize() {
		return { 0, 0 };
	}
#pragma endregion
#pragma region Text
	Text::Text() : Component() {
		
		hFont = CreateFont(14, 0, 0, 0, 0, 0, 0, 0,
			DEFAULT_CHARSET,
			0, 0, 0, 0,
			L"Arial Bold"
		);
		str = (wchar_t*)L"";
	}
	Text::Text(HFONT font) : Component() {
		hFont = font;
		str = (wchar_t*)L"";
	}
	Text::Text(wchar_t* st) : Component() {
		hFont = CreateFont(14, 0, 0, 0, 0, 0, 0, 0,
			DEFAULT_CHARSET,
			0, 0, 0, 0,
			L"Arial Bold"
		);
		str = st;
	}
	Text::Text(HFONT font, wchar_t* st) : Component() {
		str = st;
		hFont = font;
	}
	Text::~Text(){

	}
	void Text::Register()
	{
		WNDCLASS wc = { 0 };

		wc.style = CS_HREDRAW | CS_VREDRAW;
		wc.lpfnWndProc = SCProc;
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.lpszClassName = L"WText";
		wc.cbWndExtra = sizeof(LONG_PTR*);
		RegisterClass(&wc);
	}	
	void Text::Unregister()
	{
		UnregisterClass(L"WText", NULL);
	}
	void Text::Configure(HWND hWnd){
		HWND hThis = CreateWindow(L"WText", NULL, WS_CHILD | WS_VISIBLE,
			10, 10, 100, 100, hWnd, 0, GetModuleHandle(NULL), NULL);
		Component::Configure(hThis);
	}
	int getStringWidth(wchar_t* text, HFONT font) {
		HDC dc = GetDC(NULL);
		SelectObject(dc, font);

		RECT rect = { 0, 0, 0, 0 };
		DrawText(dc, text, strlen((char*)text), &rect, DT_CALCRECT | DT_NOPREFIX | DT_SINGLELINE);
		int textWidth = abs(rect.right - rect.left);

		DeleteDC(dc);
		return textWidth;
	}
	void Text::CPaint(HWND hwnd, HDC hdc, RECT* rcDirty, BOOL bErase, CData* pData) {
		
		SetBkMode(hdc, TRANSPARENT);
		HBRUSH brush = CreateSolidBrush((COLORREF)0x000FF000);
		Gdiplus::Graphics gr(hdc);
		HRGN rgn = CreateRectRgn(rcDirty->left, rcDirty->top, rcDirty->right, rcDirty->bottom);
		FillRgn(hdc, rgn, brush);

		// Initialize arguments.
		Font font(hdc, hFont);
		RectF rectf(rcDirty->left, rcDirty->top, rcDirty->right - rcDirty->left, rcDirty->bottom - rcDirty->top);
		SolidBrush br(Gdiplus::Color(125, 0, 0, 0));

		// Draw string.
		gr.DrawString(str,lstrlenW(str), &font, rectf, &stForm, &br);
					
		//DrawText(hdc, st, lstrlenW(st), rcDirty, 0);

		SwapBuffers(hdc);
		
		UpdateWindow(hwnd);
	}
	void Text::SetFont(HFONT font) {
		hFont = font;
	}
	HFONT Text::GetFont() {
		return hFont;
	}
#pragma endregion
#pragma region PseudoComponent
	void PseudoComponent::ChainCPaint(HWND hwnd, HDC hdc, RECT &rcDirty) {
		CPaint(hwnd, hdc, rcDirty);
		if (!next)
			return;
		TouchRect(rcDirty);
		next->ChainCPaint(hwnd, hdc, rcDirty);
	}
	void PseudoComponent::ChainTouchRect(RECT& rcDrawing) {
		TouchRect(rcDrawing);
		if (next)
			next->ChainTouchRect(rcDrawing);
	}

	PBorder::PBorder(CMargin mar) {
		margin = mar;
	}
	void PBorder::CPaint(HWND hwnd, HDC hdc, RECT& rcDirty) {
		HRGN hRgn = CreateRectRgn(rcDirty.left, rcDirty.top, rcDirty.right, rcDirty.bottom);

		HRGN hHdrRgn = CreateRectRgn(rcDirty.left + margin.left, rcDirty.top + margin.top, rcDirty.right - margin.right, rcDirty.bottom - margin.bottom);
		CombineRgn(hRgn, hRgn, hHdrRgn, RGN_XOR);
		Gdiplus::Graphics gr(hdc);
		SolidBrush br(Gdiplus::Color(255, 0, 0, 0));
		Region *reg = new Region(hRgn);
		gr.FillRegion(&br, reg);
	}
	void PBorder::TouchRect(RECT& rcDrawing) {
		rcDrawing.left = rcDrawing.left + margin.left;
		rcDrawing.top = rcDrawing.top + margin.top;
		rcDrawing.right = rcDrawing.right - margin.right - margin.left;
		rcDrawing.bottom = rcDrawing.bottom - margin.bottom - margin.top;
	}
#pragma endregion
	
}