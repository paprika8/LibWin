#include "Buttons.h"
#include "Visual.h"
namespace Components {
	void Components::Button::PVDeleted ( ProcessView* )
	{
		delete this;
	}
	int Button::Register ()
	{
		WNDCLASS wcex;

		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = SVProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = sizeof ( CData* );
		wcex.hInstance = hInstance;
		wcex.hIcon = LoadIcon ( wcex.hInstance , IDI_APPLICATION );
		wcex.hCursor = LoadCursor ( NULL , IDC_ARROW );
		wcex.hbrBackground = ( HBRUSH ) ( COLOR_WINDOW + 1 );
		wcex.lpszMenuName = NULL;
		wcex.lpszClassName = getSzWindowClass ();

		if ( !RegisterClass ( &wcex ) )
		{
			MessageBox ( NULL ,
				_T ( "Call to RegisterClassEx failed!" ) ,
				_T ( "Windows Desktop Guided Tour" ) ,
				NULL );

			return -1;
		}
		return 0;
	}
	void Button::Unregister ()
	{
	}
	ProcessButton::ProcessButton ( View* aModel , HWND hwnd , const char* _id = "" ) : ProcessView ( aModel , hwnd , _id )
	{
		hWnd = CreateWindowEx (
			0 ,
			model->getSzWindowClass () ,
			L"" ,
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON ,
			0 , 0 ,
			100 , 100 ,
			hwnd ,
			NULL ,
			hInstance ,
			NULL
		);
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
	}
	ButtonWithText::ButtonWithText ()
	{
		stringFormat->SetAlignment ( StringAlignmentNear );
		stringFormat->SetLineAlignment ( StringAlignmentNear );
	}
	void Components::ButtonWithText::configure ( HWND hwnd , ProcBuilder* builder)
	{
		ProcessButton* process = new ProcessButton ( this , hwnd );
		if( builder )
			builder->build ( process );
	}

	TCHAR* Components::Button::getSzWindowClass ()
	{
		return ( TCHAR* ) L"Button";
	}

	void Components::ButtonWithText::VPaint ( HWND hwnd , HDC hdc , RECT* rcDirty , BOOL bErase , ProcessView* pData )
	{
		Graphics g ( hdc );
		SolidBrush* brush; 
		if(((ProcessButton*) pData )->isDown)
			brush = new SolidBrush ( Color ( 30 , 30 , 30 ) );
		else
			brush = new SolidBrush ( Color ( 80 , 80 , 80 ) );

		g.FillRectangle ( brush , //TODO color
			rcDirty->left ,
			rcDirty->top ,
			( int ) ( rcDirty->right - rcDirty->left ) ,
			( int ) ( rcDirty->bottom - rcDirty->top )
		);
		Util::drawText ( &g , RECT ( *rcDirty ) , text, stringFormat, font, this->brush);
		delete brush;
	}

	LRESULT ButtonWithText::VProc ( HWND hwnd , UINT uMsg , WPARAM wParam , LPARAM lParam , ProcessView* pData )
	{
		ProcessButton* pButton = ( ProcessButton* )pData;
		switch ( uMsg )
		{
		case WM_LBUTTONDOWN:
		case WM_LBUTTONDBLCLK:
		{
			pButton->isDown = true;
			InvalidateRect ( hwnd , 0 , 0 );

			SetCapture ( hwnd );
			return DefWindowProc ( hwnd , uMsg , wParam , lParam );
		}
		case LBN_KILLFOCUS:
		case WM_MOUSEMOVE: 
		{
			LPPOINT point = new POINT ();
			if ( pButton->isDown && GetCursorPos ( point ) && WindowFromPoint ( *point ) == hwnd ) {
				delete point;
				return DefWindowProc ( hwnd , uMsg , wParam , lParam );
			}
			delete point;
		}
		case WM_LBUTTONUP:
		{
			pButton->isDown = false;
			InvalidateRect ( hwnd , 0 , 0 );
			
			ReleaseCapture ( );
			return DefWindowProc ( hwnd , uMsg , wParam , lParam );
		}
		case WM_PAINT:
		{
			PAINTSTRUCT paintStruct;
			HDC hDC = BeginPaint ( hwnd , &paintStruct );

			VDPaintBuffer ( hwnd , &paintStruct );

			EndPaint ( hwnd , &paintStruct );
		
			return 0;
		}
		default:
			return DefWindowProc ( hwnd , uMsg , wParam , lParam );
		}
	}

	void Components::ButtonWrap::childDeleted ( Safety* )
	{
	}

	void Components::ButtonWrap::configure ( HWND hWnd , ProcBuilder* )
	{
	}

	void Components::ButtonWrap::Unregister ()
	{
	}

	void Components::ButtonWrap::VPaint ( HWND hwnd , HDC hdc , RECT* rcDirty , BOOL bErase , ProcessView* pData )
	{
	}

	void Components::ButtonWrap::setContent ( View* view )
	{
	}

	CMargin* Components::ProcessButton::getMargin ()
	{
		return nullptr;
	}

	CPadding* Components::ProcessButton::getPadding ()
	{
		return nullptr;
	}

	CSize Components::ProcessButton::GetContentSize ()
	{
		return size;
	}
}
