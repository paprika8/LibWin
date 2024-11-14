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
	const uint WM_SETSCROLLEDTEXT = 0x0401;
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
		virtual void add ( ProcessView* process ) = 0;
		virtual void rem ( HWND hwnd ) = 0;
		virtual ProcessView* get ( int i ) { return 0; };
		virtual int len () { return 0; };
		virtual bool isEmpty () = 0;
		virtual ~WndsManager () {};
	protected:
		View* &getModel ( ProcessView* process );
	};

	class SingleWnd : public WndsManager
	{
	public:
		SingleWnd () {
			single = 0;
		}
		virtual void add ( ProcessView* process ) override;
		virtual void rem ( HWND hwnd ) override;
		virtual ProcessView* get ( int i ) override;
		virtual int len () override;
		virtual bool isEmpty () override;
		virtual ~SingleWnd ();
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
		stdminus::WEvents<HWND , UINT , WPARAM , LPARAM> eve;
		virtual ProcessView* configure ( HWND hWnd , ProcBuilder* builder ) = 0;
		ProcessView* configure ( HWND hWnd ) {
			return this->configure ( hWnd , defaultBuild );
		}

		virtual const wchar_t* getSzWindowClass () = 0;

		WndsManager* wnds = 0;

		ProcBuilder* defaultBuild = 0;

		virtual ~View () {
			if ( wnds )
				delete wnds;
		}
	protected:

		virtual int Register () = 0;
		virtual void Unregister () {
		}

		virtual void PVDeleted ( ProcessView* process );
		
		virtual void VPaint ( HWND hwnd , HDC hdc , RECT* rcDirty , BOOL bErase , ProcessView* pData ) = 0;
		virtual LRESULT VProc ( HWND hwnd , UINT uMsg , WPARAM wParam , LPARAM lParam , ProcessView* pData );

		static void VDPaintBuffer ( HWND hwnd , PAINTSTRUCT* pPaintStruct );
		static LRESULT SVProc ( HWND hwnd , UINT uMsg , WPARAM wParam , LPARAM lParam );
	private:

	};


	/// <summary>
	/// Родительский класс для всех моделей отображения компонентов
	/// </summary>
	class __declspec( novtable ) ProcessView : public Safety
	{
		friend WndsManager;
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
			MoveWindow (hWnd, point.x, point.y, asize.width, asize.height, 1);
		}
		View* getModel () { return model; }
		virtual ~ProcessView ()
		{
			if( model ){
				model->PVDeleted ( this );
			}
			if ( margin )
				delete margin;
			if ( padding )
				delete padding;
			if ( hWnd ){
				SetWindowLongPtr ( hWnd , 0 , 0 );
				SendMessage(hWnd, WM_CLOSE, 0, 0);
			}
		}
		const char* getId () { return id; }

		virtual CMargin* getMargin ()
		{
			return margin;
		}

		virtual CPadding* getPadding ()
		{
			return padding;
		}

		virtual CSize GetContentSize (CSize size)
		{
			return size;
		}
		CSize getAbsoluteSize () {
			LPRECT buf = new RECT ();
			GetClientRect ( hWnd , buf );
			CSize size = CSize ( buf->right , buf->bottom );
			delete buf;
			return size;
		}

		CSize size = CSize ( 0 , 0 );
		CPoint point = CPoint ( 0 , 0 );
		MarginType marginType = MarginType::CONTENT;
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
		CPadding* padding = new CPadding(0, 0, 0, 0);
		CMargin* margin = new CMargin ( 0 , 0 , 0 , 0 );
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
	public:
		void childDeleted ( Safety* child ) override;
};

	/// <summary>
	/// Pview с несколькими Pview внутри
	/// </summary>
	class __declspec( novtable ) PComposite : public ProcessView
	{
	public:

		enum {
			Horizontally ,
			Vertically
		};

		uint orientation : 1 = Horizontally;

		virtual void add ( ProcessView* process ) = 0;
		virtual void remove ( ProcessView* process ) = 0;
		virtual ProcessView* get ( int i ) = 0;
		virtual int len () = 0;

		PComposite ( View* aModel , HWND hwnd ) : ProcessView ( aModel , hwnd ) {}
		PComposite ( View* aModel , HWND hwnd , const char* _id ) : ProcessView ( aModel , hwnd , _id ) {}

		void childDeleted ( Safety* ) override;
	};

	/// <summary>
	/// Pview с одним Pview внутри
	/// </summary>
	class __declspec( novtable ) PComponent : public PComposite
	{
	public:

		void add ( ProcessView* process ) override;
		void remove ( ProcessView* process ) override;
		ProcessView* get ( int i ) override;
		int len () override;

		virtual void setContent ( ProcessView* view );
		virtual ProcessView* getContent () { return content; }
	protected:
		ProcessView* content = 0;
	public:

		PComponent ( View* aModel , HWND hwnd ) : PComposite ( aModel , hwnd ) {}
		PComponent ( View* aModel , HWND hwnd , const char* _id ) : PComposite ( aModel , hwnd , _id ) {}

		virtual ~PComponent () {
			if( content ){
				content->parent = 0;
				delete content;
			}
		}

		void childDeleted ( Safety* child ) override;
	};

	/// <summary>
	/// view с несколькими view внутри
	/// </summary>
	class __declspec( novtable ) Composite : virtual public View
	{
	public:
		virtual void add ( View* view ) = 0;
		virtual void remove ( View* view ) = 0;
		virtual View* get ( int i ) { return 0; }
		virtual int len () { return 0; }

		void childDeleted ( Safety* child ) override
		{
			View* view = dynamic_cast< View* >( child );
			if( view )
				remove ( view );
		}

		~Composite () {
			for ( int i = 0; i < len (); i++ )
				delete get ( i );
		}
	};

	/// <summary>
	/// view с одним view внутри
	/// </summary>
	class __declspec( novtable ) Component : public Composite
	{
	public:

		void add ( View* view ) override;
		void remove ( View* view ) override;
		View* get ( int i ) override;
		int len () override;

		virtual void setContent ( View* view );
		void childDeleted ( Safety* ) override {
			content = 0;
		}
		void PVDeleted ( ProcessView* process ) override {
			if( parent )
				parent->childDeleted ( this ); 
			wnds->rem ( process->getHWND () );
		}
		~Component(){
			if ( content )
				delete content;
		}
	protected:
		View* content;
	};

	/// <summary>
	/// view без view внутри
	/// </summary>
	class __declspec( novtable ) Content : virtual public View
	{
		void childDeleted ( Safety* child ) override
		{

		}
	};


}