#pragma once
#include "Components.h"
#include <string>
#include "Visual.h"
using namespace std;

namespace LibWin {
	class ScrollText : public Content
	{
	public:
		
		Gdiplus::StringFormat* stringFormat = new StringFormat ();
		Gdiplus::Font* font = createFont ( 16 );
		Color BGColor = Color ( 231 , 237 , 216 );


		ScrollText () : Content ()
		{
			wnds = new SingleWnd ();
		};



		// Inherited via Content
		ProcessView* configure ( HWND hWnd , ProcBuilder* ) override;

		const wchar_t* getSzWindowClass () override;

		int Register () override;

		void VPaint ( HWND hwnd , HDC hdc , RECT* rcDirty , BOOL bErase , ProcessView* pData ) override;
		LRESULT VProc ( HWND hwnd , UINT uMsg , WPARAM wParam , LPARAM lParam , ProcessView* pData );

		void SetText (wstring atext)
		{
			text = atext;
			if ( wnds->get ( 0 ) != 0 )
			{
				SendMessage ( wnds->get ( 0 )->getHWND () , WM_SETSCROLLEDTEXT , 0 , 0 );
			}
		}
		wstring GetText ()
		{
			return text;
		}


	private:
		wstring text = L"";

	};

	class PScrollText : public ProcessView
	{
		friend ScrollText;
	public:
		int WPos = 0;
		int TextHeight = 0;
		int MaxTextHeight = 0;

		PScrollText ( View* aModel , HWND hwnd , const char* _id = "" ) : ProcessView ( aModel , hwnd , _id ) {

			hWnd = CreateWindowEx ( 0 , model->getSzWindowClass () , L"" , WS_VISIBLE | WS_CHILD , 0 , 0, 0, 0, hwnd , NULL , hInstance , NULL );
			if ( !hWnd )
			{
				MessageBox ( NULL ,
					_T ( "Call to CreateWindowEx failed!" ) ,
					_T ( "Windows Desktop Guided Tour" ) ,
					NULL );
			}
			CData* cData = new CData ();
			padding->right = 30;
			padding->left = 5;
			cData->that = this;
			SetWindowLongPtr ( hWnd , 0 , ( LONG_PTR ) cData );

		};
	private:
		int oldY;
		bool isDown = 0;
	};
	


	Color operator-( Color start, Color DeltaColor) {
		Color finish = Color ( start.GetA () , start.GetR () - DeltaColor.GetR () , start.GetG () - DeltaColor.GetG () , start.GetB () - DeltaColor.GetB () );
		return finish;
	}

	Color operator+( Color start , Color DeltaColor ) {
		Color finish = Color ( start.GetA () , start.GetR () + DeltaColor.GetR () , start.GetG () + DeltaColor.GetG () , start.GetB () + DeltaColor.GetB () );
		return finish;
	}

	Color operator-( Color start , int Delta ) {
		Color finish = Color ( start.GetA () , start.GetR () - Delta , start.GetG () - Delta , start.GetB () - Delta );
		return finish;
	}

	Color operator+( Color start , int Delta ) {
		Color finish = Color ( start.GetA () , start.GetR () + Delta , start.GetG () + Delta , start.GetB () + Delta );
		return finish;
	}

}


