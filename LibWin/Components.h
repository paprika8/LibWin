#pragma once

#include "pch.h"
#include "stdminus.h"
#include <Windows.h>
#include <tchar.h>
#include <math.h>
#include <objidl.h>
#include <functional>

#pragma comment (lib,"Gdiplus.lib")
#include <gdiplus.h>
//#include "gdiplusgraphics.h"

using namespace Gdiplus;

namespace Components {
	class PseudoComponent;
	class Component;
	void CRegister();
	void CUnregister();
#pragma region structs
	typedef struct CData_tag CData;
	typedef struct CMargin CMargin;
	typedef struct CSize CSize;
	typedef struct CPoint CPoint;
	struct CData_tag {
		Component* that;
		void* extraData;
	};
	struct CPoint {
		int x;
		int y;
	};
	struct CSize {
		CSize(int w, int h) {
			width = w;
			hight = h;
		}
		int width;
		int hight;
	};
	struct IMargin
	{
		virtual void reRect(CPoint& point, CSize& drawSize, CSize size, CSize cSize, char type) = 0;
	};
	struct CRoundMargin : IMargin{
	public:
		int radius;
		int angle;
		CRoundMargin(CRoundMargin& orig) {
			radius = orig.radius;
			angle = orig.angle;
		}
		CRoundMargin(const CRoundMargin& orig) {
			radius = orig.radius;
			angle = orig.angle;
		}
		CRoundMargin(int r, int a) {
			radius = r;
			angle = a;
		}
		void reRect(CPoint& point, CSize& drawSize, CSize size, CSize cSize, char type) {
			int x = point.x;
			int y = point.y;
			int w = size.width;
			int h = size.hight;
			if (w == 0)
				w = cSize.width;
			if (h == 0)
				h = cSize.hight;
			x = point.x + radius * cos(angle);
			y = point.y + radius * sin(angle);
			x -= (drawSize.width / 2);
			y -= (drawSize.hight / 2);
			point = { x, y };
			drawSize = { w, h };
		}
	private:
		int hor(int rad, int h) {
			return 2 * sqrt(rad*rad - h*h);
		}
	};
	struct CMargin : IMargin {
	public:
		int left;
		int top;
		int right;
		int bottom;
		
		CMargin(CMargin& orig) {
			right = orig.right;
			top = orig.top;
			left = orig.left;
			bottom = orig.bottom;
		}
		CMargin(const CMargin& orig) {
			right = orig.right;
			top = orig.top;
			left = orig.left;
			bottom = orig.bottom;
		}
		CMargin(int aleft,
			int atop,
			int aright,
			int abottom) {
			right = aright;
			top = atop;
			left = aleft;
			bottom = abottom;
		}
		void reRect(CPoint& point, CSize& drawSize, CSize size, CSize cSize, char type) {
			int x = point.x;
			int y = point.y;
			int w = size.width;
			int h = size.hight;
			if (w == 0)
				w = cSize.width;
			if (h == 0)
				h = cSize.hight;
			if (type & 0b00000001) {
				w = drawSize.width - left - right;
				x = point.x + left;
			}
			else {
				switch (type & 0b00001100)
				{
				case 0b00000100:
					x = point.x + left;
					break;
				case 0b00001000:
					x = point.x + drawSize.width - right - w;
					break;
				default:
					x = left + point.x + drawSize.width / 2 - size.width / 2;
					break;
				}
			}
			if (type & 0b00000010) {
				h = drawSize.hight - bottom - top;
				y = point.y + bottom;
			}
			else {
				switch (type & 0b00110000)
				{
				case 0b00010000:
					y = point.y + top;
					break;
				case 0b00100000:
					y = point.y + h - bottom;
					break;
				default:
					y = top + point.y + drawSize.hight / 2 - size.hight / 2;
					break;
				}
			}
			point = { x, y };
			drawSize = { w, h };
		}
	};

	enum MarginType : char {
		GPARENT    = 0b00000001,
		HPARENT    = 0b00000010,
		CONTENT    = 0b00000000,
		LEFT       = 0b00001000,
		GCENTER    = 0b00001100,
		RIGHT      = 0b00000100,
		TOP        = 0b00010000,
		HCENTER    = 0b00110000,
		BOTTOM     = 0b00100000,
	};
#pragma endregion
	class Component
	{
	public:
		stdminus::WEvents<int, std::function<LRESULT(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)>> eve;
		stdminus::arr<PseudoComponent*> psComp;
		Component();
		~Component();
		static void Register();
		static void Unregister();
		virtual void Configure(HWND hWnd);
		CSize GetSize();
		IMargin *GetMergin();
		HWND GetHWND();
		void SetSize(CSize sz);
		void SetMergin(IMargin *mr);
		void reRect();
		void reRect(CPoint p, CSize s);
		void reRect(CPoint p, CSize s, char type);
		stdminus::arr<Components::Component*> GetChildren();
	private:
		HWND hWnd = 0;
		char typeResize = 0;
		IMargin* margin = new CMargin( 0, 0, 0, 0 );
		CPoint pPoint = { 0, 0 };
		CSize pSize = { 0, 0 };
		CSize size = CSize( 0, 0 );
		static void CDPaintBuffer(HWND hwnd, PAINTSTRUCT* pPaintStruct);
		static TCHAR *Comp_WC;
		
	protected:
		static LRESULT CALLBACK SCProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		virtual void CPaint(HWND hwnd, HDC hdc, RECT* rcDirty, BOOL bErase, CData* pData);
		virtual LRESULT CALLBACK CProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		virtual CSize GetContentSize();
	};
	class Text : public Component {
	public:
		Text();
		Text(HFONT font);
		Text(wchar_t* st);
		Text(HFONT font, wchar_t* st);
		~Text();
		static void Register();
		static void Unregister();
		void Configure(HWND hWnd) override;
		wchar_t* str;
		void SetFont(HFONT font);
		HFONT GetFont();
		StringFormat stForm;
	private:
		HFONT hFont;
		
	protected:
		void CPaint(HWND hwnd, HDC hdc, RECT* rcDirty, BOOL bErase, CData* pData) override;
	};
	class Button : public Component {
	public:
		Button();
		Button(HFONT font);
		Button(wchar_t* st);
		Button(HFONT font, wchar_t* st);
		~Button();
		static void Register();
		static void Unregister();
		void Configure(HWND hWnd) override;
		wchar_t* str;
		void SetFont(HFONT font);
		HFONT GetFont();
		StringFormat stForm;
	private:
		HFONT hFont;

	protected:
		void CPaint(HWND hwnd, HDC hdc, RECT* rcDirty, BOOL bErase, CData* pData) override;
	};

	class PseudoComponent {
	public:
		PseudoComponent* next = 0;
		virtual void CPaint(HWND hwnd, HDC hdc, RECT &rcDirty) = 0;
		virtual void TouchRect(RECT& rcDrawing) = 0;
		void ChainCPaint(HWND hwnd, HDC hdc, RECT &rcDirty);
		void ChainTouchRect(RECT& rcDrawing);
	private:

	};
	class PBorder : public PseudoComponent {
	public:
		PBorder(CMargin mar);
		void CPaint(HWND hwnd, HDC hdc, RECT& rcDirty);
		void TouchRect(RECT& rcDrawing);
	protected:
		CMargin margin = { 0, 0, 0, 0 };
	};
}