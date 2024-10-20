#pragma once

#include <Windows.h>
#include <tchar.h>
#include <math.h>
#include <objidl.h>
#include <functional>
#include <thread>

#include "stdminus.h"
#include "UIStructs.h"
#include "Safety.h"

#pragma comment (lib,"Gdiplus.lib")
#include <gdiplus.h>
using namespace Gdiplus;
using namespace stdminus;

typedef const wchar_t* WNDName;

namespace LibWin {
	class View;			//model
	class ProcessView;	//view

	/// <summary>
	/// Идентификатор приложения
	/// </summary>
	HINSTANCE hInstance;
	set<WNDName, false> isRegistry = set<WNDName , false> ();
	void moveWnd ( ProcessView* process , ProcessView* child );

	int InitLib ( HINSTANCE hInstance );
	void ExitLib ();

	/// <summary>
	/// Метод запуска цикла работы приложения
	/// </summary>
	/// <returns></returns>
	int run () {
		MSG msg;
		while ( GetMessage ( &msg , NULL , 0 , 0 ) )
		{
			TranslateMessage ( &msg );
			DispatchMessage ( &msg );
		}
		return ( int ) msg.wParam;
	}

	struct CData
	{
		ProcessView* that;
	};

	/// <summary>
	/// Класс для хранения и доступа к ProcessView из View
	/// </summary>
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

	/// <summary>
	/// Указывает как должен создаваться ProcessView
	/// </summary>
	class __declspec( novtable ) ProcBuilder
	{
	public:
		/// <summary>
		/// Изменяет ProcessView в соответствии со своими настройками
		/// </summary>
		/// <param name="process:"> ProcessView к которому применяются изменения </param>
		virtual void build ( ProcessView* process ) = 0;
	};

	/// <summary>
	/// Родительский класс для всех моделей поведения компонентов
	/// </summary>
	class __declspec( novtable ) View : virtual public Safety
	{
		friend ProcessView;
	public:
		stdminus::WEvents<LRESULT , HWND , UINT , WPARAM , LPARAM> eve;
		virtual ProcessView* configure ( HWND hWnd , ProcBuilder* ) = 0;
		ProcessView* configure ( HWND hWnd ) {
			return this->configure ( hWnd , defaultBuild );
		}

		virtual const wchar_t* getSzWindowClass () = 0;

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


	/// <summary>
	/// Родительский класс для всех моделей отображения компонентов
	/// </summary>
	class __declspec( novtable ) ProcessView
	{
	public:
		ProcessView ( View* aModel , HWND hwnd ) : ProcessView ( aModel , hwnd , "" ) {}
		ProcessView ( View* aModel , HWND hwnd , const char* _id )
		{
			if ( !isRegistry.has ( aModel->getSzWindowClass () ) ){
				aModel->Register ();
				isRegistry.add ( aModel->getSzWindowClass () );
			}
			id = _id;
			model = aModel;
		}
		void Move ( CPoint apoint , CSize asize ) {
			point = apoint;
			size = asize;
			MoveWindow (hWnd, point.x, point.y, size.width, size.height, 1);
		}
		virtual CSize GetContentSize () = 0;
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
		CPoint point = CPoint ( 0 , 0 );

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
	/// Pview с несколькими Pview внутри
	/// </summary>
	class __declspec( novtable ) PComposite : virtual public ProcessView
	{
	public:
		MarginType marginType = MarginType::CONTENT;
		virtual void add ( ProcessView* ) = 0;
		virtual void remove ( ProcessView* ) = 0;
		virtual ProcessView* get ( int i ) = 0;
		virtual int len () = 0;

		PComposite ( View* aModel , HWND hwnd ) : ProcessView ( aModel , hwnd ) {}
		PComposite ( View* aModel , HWND hwnd , const char* _id ) : ProcessView ( aModel , hwnd , _id ) {}
	};

	/// <summary>
	/// Pview с одним Pview внутри
	/// </summary>
	class __declspec( novtable ) PComponent : virtual public ProcessView
	{
	public:
		virtual void setContent ( ProcessView* view ) = 0;
		virtual ProcessView* getContent () { return content; }
	protected:
		ProcessView* content = 0;
	public:

		PComponent ( View* aModel , HWND hwnd ) : ProcessView ( aModel , hwnd ) {}
		PComponent ( View* aModel , HWND hwnd , const char* _id ) : ProcessView ( aModel , hwnd , _id ) {}
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
		virtual int len () = 0;
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


}