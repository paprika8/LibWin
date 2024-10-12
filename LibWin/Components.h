#pragma once

#include "stdminus.h"
#include <Windows.h>
#include <tchar.h>
#include <math.h>
#include <objidl.h>
#include <functional>
#include <thread>

#pragma comment (lib,"Gdiplus.lib")
#include <gdiplus.h>
using namespace Gdiplus;
using namespace stdminus;

namespace Components {
	class View;			//model
	class ProcessView;	//view

	int InitLib ( HINSTANCE hInstance );
	void ExitLib ();
	int run () {
		MSG msg;
		while ( GetMessage ( &msg , NULL , 0 , 0 ) )
		{
			TranslateMessage ( &msg );
			DispatchMessage ( &msg );
		}
		return ( int ) msg.wParam;
	}

	HINSTANCE hInstance;
	set<const char*> isRegistry = set<const char*> ();//TODO сохранять имена классов wnd
#pragma region structures
	enum MarginType : char
	{
		CONTENT = 0b00000000 ,
		HPARENT = 0b00000001 ,
		VPARENT = 0b00000010 ,
		PARENT = 0b00000011 ,
		LEFT = 0b00001000 ,
		HCENTER = 0b00001100 ,
		RIGHT = 0b00000100 ,
		TOP = 0b00010000 ,
		VCENTER = 0b00110000 ,
		BOTTOM = 0b00100000 ,
	};
	enum CalcType : unsigned short
	{
		px ,
		percent ,
		em
	};

	struct pointUI {
		short value : 14;
		CalcType type : 2;

		pointUI () {
			value = 0;
			type = px;
		}
		pointUI ( short avalue ) {
			value = avalue;
			type = px;
		}
		void operator=( short a ) {
			value = a;
		}
		void operator+=( short a ) {
			value += a;
		}
		void operator-=( short a ) {
			value -= a;
		}

		operator short ()
		{
			return value;
		}
	};
	struct upointUI {
		unsigned short value : 14;
		CalcType type : 2;

		upointUI () {
			value = 0;
			type = px;
		}
		upointUI ( unsigned short avalue ) {
			value = avalue;
			type = px;
		}
		void operator=( unsigned short a ) {
			value = a;
		}
		void operator+=( unsigned short a ) {
			value += a;
		}
		void operator-=( unsigned short a ) {
			value -= a;
		}

		operator unsigned short ()
		{
			return value;
		}
	};

	struct CData
	{
		ProcessView* that;
	};
	struct CPoint
	{
		short x;
		short y;
		CPoint ()
		{
			x = 0;
			y = 0;
		}
		CPoint ( short ax , short ay )
		{
			x = ax;
			y = ay;
		}
		CPoint ( int l )
		{
			*( ( int* ) this ) = l;
		}
		int toInt ()
		{
			return *( ( int* ) this );
		}
	};
	struct CSize
	{
		upointUI width;
		upointUI height;

		CSize ( unsigned short w , unsigned short h )
		{
			width.type = px;
			height.type = px;
			width = w;
			height = h;
		}
		CSize ( int l )
		{
			*( ( int* ) this ) = l;
		}
		CSize plusRight ( CSize b )
		{
			return CSize ( width + b.width , max ( height , b.height ) );
		}
		CSize plusBottom ( CSize b )
		{
			return CSize ( max ( width , b.width ) , height + b.height );
		}
		int toInt ()
		{
			return *( ( int* ) this );
		}
	};

	struct CPadding
	{
		pointUI left;
		pointUI top;
		pointUI right;
		pointUI bottom;
		virtual void reRect ( CPoint& point , CSize& drawSize )
		{
			point.x += left;
			drawSize.width -= left + right;//TODO в процентах
			point.y += top;
			drawSize.height -= top + bottom;
		}
		virtual void reSize ( CSize& size)
		{
			size.width += left + right;
			size.height += top + bottom;
		}
		CPadding ( CPadding& orig )
		{
			right = orig.right;
			top = orig.top;
			left = orig.left;
			bottom = orig.bottom;
		};
		CPadding ( const CPadding& orig )
		{
			right = orig.right;
			top = orig.top;
			left = orig.left;
			bottom = orig.bottom;
		}
		CPadding ( short aleft ,
			short atop ,
			short aright ,
			short abottom )
		{
			right = aright;
			top = atop;
			left = aleft;
			bottom = abottom;
		}
		CPadding ( ll l )
		{
			*( ( ll* ) this ) = l;
		}
		ll toLong ()
		{
			return *( ( ll* ) this );
		}
	};
	struct CMargin
	{
	public:
		pointUI left;
		pointUI top;
		pointUI right;
		pointUI bottom;

		CMargin ( CMargin& orig )
		{
			right = orig.right;
			top = orig.top;
			left = orig.left;
			bottom = orig.bottom;
		}
		CMargin ( const CMargin& orig )
		{
			right = orig.right;
			top = orig.top;
			left = orig.left;
			bottom = orig.bottom;
		}
		CMargin ( short aleft ,
			short atop ,
			short aright ,
			short abottom )
		{
			right = aright;
			top = atop;
			left = aleft;
			bottom = abottom;
		}
		CMargin ( ll l )
		{
			*( ( ll* ) this ) = l;
		}
		ll toLong ()
		{
			return *( ( ll* ) this );
		}
		virtual void reRect ( CPoint& point , CSize& drawSize , CSize thisSize , MarginType type)
		{
			short x = point.x;
			short y = point.y;
			ushort w = thisSize.width;
			ushort h = thisSize.height;
			if ( type & HPARENT ) {
				w = drawSize.width - left - right;
				x = point.x + left;
			}
			else {
				switch ( type & HCENTER )
				{
				case LEFT:
					x = point.x + left;
					break;
				case RIGHT:
					x = point.x + drawSize.width - right - w;
					break;
				default:
					x = point.x + drawSize.width / 2 - thisSize.width / 2;
					break;
				}
			}
			if ( type & VPARENT ) {
				h = drawSize.height - bottom - top;
				y = point.y + bottom;
			}
			else {
				switch ( type & VCENTER )
				{
				case TOP:
					y = point.y + top;
					break;
				case BOTTOM:
					y = point.y + h - bottom;
					break;
				default:
					y = top + point.y + drawSize.height / 2 - thisSize.height / 2;
					break;
				}
			}
			point.x = x;
			point.y = y;
			drawSize.width = w;
			drawSize.height = h;
		}
		virtual void reSize ( CSize& size )//TODO переписать
		{
			size.width += left + right;
			size.height += top + bottom;
		}
	};
#pragma endregion
	/// <summary>
	/// указывает как должен создаваться ProcessView
	/// </summary>
	class __declspec( novtable ) ProcBuilder
	{
	public:
		virtual void build ( ProcessView* ) = 0;
	};

	class __declspec( novtable ) WndsManager
	{
	public:
		virtual void add ( ProcessView* ) = 0;
		virtual void rem ( HWND ) = 0;
		virtual ProcessView* get ( int ) = 0;
		virtual int len () = 0;
		virtual bool isEmpty () = 0;
	};

	class SingleWnd : public WndsManager
	{
	public:
		SingleWnd () {
			single = 0;
		}
		virtual void add ( ProcessView* ) override;
		virtual void rem ( HWND ) override;
		virtual ProcessView* get ( int ) override;
		virtual int len () override;
		virtual bool isEmpty () override;
	private:
		ProcessView* single;
	};

	class __declspec( novtable ) Safety
	{
	public:
		Safety* parent = 0;
		virtual void childDeleted ( Safety* ) = 0;
		/// <summary>
		/// Сообщает об удалении все связанные объекты
		/// </summary>
		virtual ~Safety ()
		{
			if ( parent )
				parent->childDeleted ( this );
		}
	};

	class __declspec( novtable ) View : virtual public Safety
	{
		friend ProcessView;
	public:
		stdminus::WEvents<LRESULT , HWND , UINT , WPARAM , LPARAM> eve;
		virtual ProcessView* configure ( HWND hWnd , ProcBuilder* ) = 0;
		ProcessView* configure ( HWND hWnd ) {
			return this->configure ( hWnd , defaultBuild );
		}

		virtual TCHAR* getSzWindowClass () = 0;

		WndsManager* wnds = 0;

		ProcBuilder* defaultBuild = 0;
	protected:

		virtual int Register () = 0;
		virtual void Unregister () = 0;

		virtual void PVDeleted ( ProcessView* ) = 0;
		virtual void VPaint ( HWND hwnd , HDC hdc , RECT* rcDirty , BOOL bErase , ProcessView* pData ) = 0;
		virtual LRESULT CALLBACK VProc ( HWND hwnd , UINT uMsg , WPARAM wParam , LPARAM lParam , ProcessView* pData );

		static void VDPaintBuffer ( HWND hwnd , PAINTSTRUCT* pPaintStruct );
		static LRESULT CALLBACK SVProc ( HWND hwnd , UINT uMsg , WPARAM wParam , LPARAM lParam );
	private:

	};

	class __declspec( novtable ) ProcessView
	{
	public:
		ProcessView ( View* aModel , HWND hwnd ) : ProcessView ( aModel , hwnd , "" ) {}
		ProcessView ( View* aModel , HWND hwnd , const char* _id )
		{
			if ( !isRegistry.has ( typeid( aModel ).name () ) )
				aModel->Register ();
			id = _id;
			model = aModel;
		}
		View* getModel () { return model; }
		virtual ~ProcessView ()
		{
			model->PVDeleted ( this );
			/*if ( hWnd )
				SendMessage(hWnd, WM_DESTROY, 0, 0);*/
		}
		const char* getId () { return id; }

		virtual CMargin* getMargin () = 0;
		virtual CPadding* getPadding () = 0;

		CSize size = CSize ( 0 , 0 );

#pragma region get_set_ClLongPtr
		ATOM getAtom ()
		{
			return ( ATOM ) GetClassLongPtr ( hWnd , GCW_ATOM );
		}
		int getExtra ()
		{
			return ( int ) GetClassLongPtr ( hWnd , GCL_CBCLSEXTRA );
		}
		int getExtraWindow ()
		{
			return ( int ) GetClassLongPtr ( hWnd , GCL_CBWNDEXTRA );
		}
		void setBackground ( HBRUSH value )
		{
			SetClassLongPtr ( hWnd , GCLP_HBRBACKGROUND , ( LONG_PTR ) value );
			InvalidateRect ( hWnd , NULL , FALSE );
		}
		HBRUSH getBackground ()
		{
			return ( HBRUSH ) GetClassLongPtr ( hWnd , GCLP_HBRBACKGROUND );
		}
		HCURSOR getCursor ()
		{
			return ( HCURSOR ) GetClassLongPtr ( hWnd , GCLP_HCURSOR );
		}
		HICON getIcon ()
		{
			return ( HICON ) GetClassLongPtr ( hWnd , GCLP_HICON );
		}
		HICON getSmallIcon ()
		{
			return ( HICON ) GetClassLongPtr ( hWnd , GCLP_HICONSM );
		}
		HMODULE getModule ()
		{
			return ( HMODULE ) GetClassLongPtr ( hWnd , GCLP_HMODULE );
		}
		LPCWSTR getMenuName ()
		{
			return ( LPCWSTR ) GetClassLongPtr ( hWnd , GCLP_MENUNAME );
		}
		UINT getStyle ()
		{
			return ( UINT ) GetClassLongPtr ( hWnd , GCL_STYLE );
		}
		WNDPROC getWindowProcess ()
		{
			return ( WNDPROC ) GetClassLongPtr ( hWnd , GCLP_WNDPROC );
		}
		HWND getHWND () const { return hWnd; };
#pragma endregion
	protected:

		View* model;
		CPadding* padding = 0;
		CMargin* margin = 0;
		HWND hWnd = 0;


		virtual CSize GetContentSize () = 0;

#pragma region get_set_ClLongPtr
		void setExtra ( int value )
		{
			SetClassLongPtr ( hWnd , GCL_CBCLSEXTRA , ( LONG_PTR ) value );
		}
		void setExtraWindow ( int value )
		{
			SetClassLongPtr ( hWnd , GCL_CBWNDEXTRA , ( LONG_PTR ) value );
		}
		void setCursor ( HCURSOR value )
		{
			SetClassLongPtr ( hWnd , GCLP_HCURSOR , ( LONG_PTR ) value );
			InvalidateRect ( hWnd , NULL , FALSE );
		}
		void setMenuName ( LPCWSTR value )
		{
			SetClassLongPtr ( hWnd , GCLP_MENUNAME , ( LONG_PTR ) value );
			InvalidateRect ( hWnd , NULL , FALSE );
		}
		void setSmallIcon ( HICON value )
		{
			SetClassLongPtr ( hWnd , GCLP_HICONSM , ( LONG_PTR ) value );
			InvalidateRect ( hWnd , NULL , FALSE );
		}
		void setIcon ( HICON value )
		{
			SetClassLongPtr ( hWnd , GCLP_HICON , ( LONG_PTR ) value );
			InvalidateRect ( hWnd , NULL , FALSE );
		}
		void setStyle ( UINT value )
		{
			SetClassLongPtr ( hWnd , GCL_STYLE , ( LONG_PTR ) value );
			InvalidateRect ( hWnd , NULL , FALSE );
		}
		void setWindowProcess ( WNDPROC value )
		{
			SetClassLongPtr ( hWnd , GCLP_WNDPROC , ( LONG_PTR ) value );
		}
		void setModule ( HMODULE value )
		{
			SetClassLongPtr ( hWnd , GCLP_HMODULE , ( LONG_PTR ) value );
		}
#pragma endregion
	private:
		const char* id = "";
	};

	/// <summary>
	/// view с несколькими view внутри
	/// </summary>
	class __declspec( novtable ) Composite : virtual public View
	{
	public:
		virtual void add ( View* ) = 0;
		virtual void remove ( View* ) = 0;
		virtual View* get ( int i ) = 0;
	};

	/// <summary>
	/// view с одним view внутри
	/// </summary>
	class __declspec( novtable ) Component : virtual public View
	{
	public:
		virtual void setContent ( View* view ) = 0;
		virtual View* getContent () { return content; }
	protected:
		View* content;
	};

	/// <summary>
	/// view без view внутри
	/// </summary>
	class __declspec( novtable ) Content : virtual public View
	{
		void childDeleted ( Safety* ) override
		{

		}
	};
	/// <summary>
	/// базовый класс для окон
	/// </summary>
	class __declspec( novtable ) Window : public Component
	{
	public:
		virtual void show ( int nCmdShow = SW_NORMAL ) = 0;
	protected:

	private:

	};

	class __declspec( novtable ) ProcessWindow : public ProcessView
	{
	public:
		ProcessWindow ( View* aModel , HWND hwnd ) : ProcessView ( aModel , hwnd ) {}
		ProcessWindow ( View* aModel , HWND hwnd , const char* _id ) : ProcessView ( aModel , hwnd , _id ) {}

		virtual void show ( int nCmdShow ) {
			ShowWindow ( hWnd ,
			nCmdShow );
			UpdateWindow ( hWnd );
		}
	protected:
		virtual int init ()
		{
			return init ( 400 , 500 );
		}
		virtual int init ( int width , int hight )
		{
			return init ( width , hight , 0 );
		}
		virtual int init ( int width , int hight , unsigned long dwStyle )
		{
			return init ( width , hight , dwStyle , WS_EX_OVERLAPPEDWINDOW );
		}
		virtual int init ( int width , int hight , unsigned long dwStyle , unsigned long dwExStyle )
		{
			return init (
				dwExStyle ,
				dwStyle ,
				CW_USEDEFAULT , CW_USEDEFAULT ,
				width , hight ,
				NULL ,
				NULL ,
				hInstance ,
				NULL
			);
		}
		virtual int init (
			_In_ DWORD dwExStyle ,
			_In_ DWORD dwStyle ,
			_In_ int X ,
			_In_ int Y ,
			_In_ int nWidth ,
			_In_ int nHeight ,
			_In_opt_ HWND hWndParent ,
			_In_opt_ HMENU hMenu ,
			_In_opt_ HINSTANCE hInstance ,
			_In_opt_ LPVOID lpParam
		) = 0;
	};


	class SizeProcBuilder : public ProcBuilder
	{
	public:
		SizeProcBuilder ( CSize asize ) {
			size = asize;
		}
		CSize size = CSize(0, 0);

		// Унаследовано через ProcBuilder
		void build ( ProcessView* ) override;
	};


	class RectPWindow : public ProcessWindow
	{
	public:
		RectPWindow ( View* aModel , ProcBuilder* data );
		RectPWindow ( View* aModel );
	protected:
		virtual CMargin* getMargin () override { return 0; };
		virtual CPadding* getPadding () override
		{
			return padding;
		};
		virtual CSize GetContentSize () override
		{
			return size;
		};

		virtual int init (
			_In_ DWORD dwExStyle ,
			_In_ DWORD dwStyle ,
			_In_ int X ,
			_In_ int Y ,
			_In_ int nWidth ,
			_In_ int nHeight ,
			_In_opt_ HWND hWndParent ,
			_In_opt_ HMENU hMenu ,
			_In_opt_ HINSTANCE hInstance ,
			_In_opt_ LPVOID lpParam
		) override;
		virtual ~RectPWindow ();
	};

	class RectWindow : public Window
	{
	public:
		RectWindow ();

		virtual void show ( int nCmdShow = SW_NORMAL ) override {
			( ( RectPWindow* ) wnds->get ( 0 ) )->show ( nCmdShow );
		}
		virtual ProcessView* configure ( HWND hWnd , ProcBuilder* ) override;
		void setContent ( View* view ) override;

		~RectWindow ();
	protected:

		virtual int Register () override;
		virtual void Unregister () override;

		virtual TCHAR* getSzWindowClass () override { return ( TCHAR* ) ( ( void* ) L"WindRect" ); }

		void childDeleted ( Safety* ) override;
		void PVDeleted ( ProcessView* ) override;
		void VPaint ( HWND hwnd , HDC hdc , RECT* rcDirty , BOOL bErase , ProcessView* pData ) override;
		LRESULT VProc ( HWND hwnd , UINT uMsg , WPARAM wParam , LPARAM lParam , ProcessView* pData ) override;
		
	};
}