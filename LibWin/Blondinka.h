#pragma once
#include "Components.h"
namespace LibWin {
	class Blondinka : public View
	{
	public:

		Blondinka () :View ()  
		{ 
			wnds = new SingleWnd (); 
		};
		Color BGColor = Color(246, 190, 241);
		std::function< bool ( HWND , HDC , RECT* , BOOL , ProcessView* )> *paint = 0;

		ProcessView* configure ( HWND hWnd , ProcBuilder* ) override;

		const wchar_t* getSzWindowClass () override;

		int Register () override;

		void VPaint ( HWND hwnd , HDC hdc , RECT* rcDirty , BOOL bErase , ProcessView* pData ) override;
		LRESULT VProc ( HWND hwnd , UINT uMsg , WPARAM wParam , LPARAM lParam , ProcessView* pData ) override; //TODO переброс сообщений родителю

	};

	class PBlond : public ProcessView
	{
	public:
		PBlond ( View* aModel , HWND hwnd , int startX, int startY, CSize startSize, const char* _id = "" ) : ProcessView ( aModel , hwnd , _id ) {

			hWnd = CreateWindowEx (0 ,model->getSzWindowClass () ,L"" , WS_VISIBLE | WS_CHILD , startX , startY , startSize.width , startSize.height ,hwnd ,NULL ,hInstance ,NULL);
			if ( !hWnd )
			{
				MessageBox ( NULL ,
					_T ( "Call to CreateWindowEx failed!" ) ,
					_T ( "Windows Desktop Guided Tour" ) ,
					NULL );
			}
			CData* cData = new CData ();
			cData->that = this;
			SetWindowLongPtr ( hWnd , 0 , ( LONG_PTR ) cData );
			size = startSize;
			margin = new CMargin ( 0 , 0 , 0 , 0 );
			padding = new CPadding ( 0 , 0 , 0 , 0 );
		};

	};

};


