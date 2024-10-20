#pragma once

#include "Components.h"

namespace LibWin {
	/// <summary>
	/// Базовый класс для моделей поведения окон
	/// </summary>
	class __declspec( novtable ) Window : public Component
	{
	public:
		virtual void show ( int nCmdShow = SW_NORMAL ) = 0;
	protected:

	private:

	};

	/// <summary>
	/// Базовый класс для моделей отображения окон
	/// </summary>
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

	/// <summary>
	/// Класс отображения прямоугольного окна
	/// </summary>
	class RectPWindow : public ProcessWindow
	{
	public:
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

	/// <summary>
	/// Класс поведения прямоугольного окна
	/// </summary>
	class RectWindow : public Window
	{
	public:
		RectWindow ();

		virtual void show ( int nCmdShow = SW_NORMAL ) override {
			( ( RectPWindow* ) wnds->get ( 0 ) )->show ( nCmdShow );
		}
		virtual void configure ( HWND hWnd , ProcBuilder* ) override;
		void setContent ( View* view ) override;

		~RectWindow ();
	protected:

		virtual int Register () override;
		virtual void Unregister () override;

		virtual const wchar_t* getSzWindowClass () override { return L"WindRect"; }

		void childDeleted ( Safety* ) override;
		void PVDeleted ( ProcessView* ) override;
		void VPaint ( HWND hwnd , HDC hdc , RECT* rcDirty , BOOL bErase , ProcessView* pData ) override;
		LRESULT VProc ( HWND hwnd , UINT uMsg , WPARAM wParam , LPARAM lParam , ProcessView* pData ) override;

	};
}

