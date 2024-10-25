#include "Buttons.h"
#include "Visual.h"
namespace LibWin {
	void LibWin::Button::PVDeleted ( ProcessView* process )
	{
		wnds->rem ( process->getHWND () );
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
		wcex.hbrBackground = NULL;
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

		margin = new CMargin ( 0 , 0 , 0 , 0 );
		padding = new CPadding ( 0 , 0 , 0 , 0 );
	}

	ProcessButtonWrap::ProcessButtonWrap ( View* aModel , HWND hwnd , const char* _id = "" ) : PComponent ( aModel , hwnd , _id )
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

		margin = new CMargin ( 0 , 0 , 0 , 0 );
		padding = new CPadding ( 0 , 0 , 0 , 0 );
	}

	LibWin::ButtonWithText::ButtonWithText () : Content(), Button(){
		wnds = new SingleWnd ();
		stringFormat->SetAlignment(StringAlignmentNear);
		stringFormat->SetLineAlignment(StringAlignmentNear);
	}
	ProcessView* LibWin::ButtonWithText::configure ( HWND hwnd , ProcBuilder* builder)
	{
		ProcessButton* process = new ProcessButton ( this , hwnd );
		wnds->add ( process );
		if( builder )
			builder->build ( process );
		return process;
	}

	const wchar_t* LibWin::Button::getSzWindowClass ()
	{
		return L"Button";
	}

	void LibWin::ButtonWithText::VPaint ( HWND hwnd , HDC hdc , RECT* rcDirty , BOOL bErase , ProcessView* pData )
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
			pButton->isDown = false;
			return DefWindowProc ( hwnd , uMsg , wParam , lParam );
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

	ButtonWrap::ButtonWrap ()
	{
		wnds = new SingleWnd ();
	}

	void LibWin::ButtonWrap::childDeleted ( Safety* )
	{
		content = 0;
	}

	ProcessView* LibWin::ButtonWrap::configure ( HWND hwnd , ProcBuilder* builder )
	{
		ProcessButtonWrap* process = new ProcessButtonWrap ( this , hwnd );
		wnds->add ( process );
		if ( builder )
			builder->build ( process );
		if ( content ) {
			ProcessView* child = content->configure ( process->getHWND () );
			PComponent* comp = dynamic_cast< PComponent* >( process );
			comp->setContent ( child );
			Positioner positioner = Positioner ( process );
			positioner.Positioning ();
		}
		return process;
	}

	void LibWin::ButtonWrap::Unregister ()
	{
	}

	void LibWin::ButtonWrap::VPaint ( HWND hwnd , HDC hdc , RECT* rcDirty , BOOL bErase , ProcessView* pData )
	{
		Graphics g ( hdc );
		SolidBrush* brush;
		if ( ( ( ProcessButtonWrap* ) pData )->isDown )
			brush = new SolidBrush ( Color ( 30 , 0 , 0 ) );
		else
			brush = new SolidBrush ( Color ( 80 , 0 , 0 ) );

		g.FillRectangle ( brush , //TODO color
			rcDirty->left ,
			rcDirty->top ,
			( int ) ( rcDirty->right - rcDirty->left ) ,
			( int ) ( rcDirty->bottom - rcDirty->top )
		);
		delete brush;
	}

	LRESULT ButtonWrap::VProc ( HWND hwnd , UINT uMsg , WPARAM wParam , LPARAM lParam , ProcessView* pData )
	{
		ProcessButtonWrap* pButton = ( ProcessButtonWrap* ) pData;
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
			pButton->isDown = false;
			return DefWindowProc ( hwnd , uMsg , wParam , lParam );
		}
		case WM_LBUTTONUP:
		{
			pButton->isDown = false;
			InvalidateRect ( hwnd , 0 , 0 );

			ReleaseCapture ();
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

	void LibWin::ButtonWrap::setContent ( View* view )
	{
		if ( content )
			delete content;
		content = view;
		if ( !view )
			return;
		view->parent = this;
		ProcessView* child , * process = 0;
		if ( process = wnds->get ( 0 ) ) {
			child = content->configure ( process->getHWND () );
			PComponent* comp = dynamic_cast< PComponent* >( process );
			comp->setContent ( child );
			Positioner positioner = Positioner ( process );
			positioner.Positioning ();
		}
	}

	CMargin* LibWin::ProcessButton::getMargin ()
	{
		return margin;
	}

	CPadding* LibWin::ProcessButton::getPadding ()
	{
		return padding;
	}

	CSize LibWin::ProcessButton::GetContentSize ()
	{
		return size;
	}
	CMargin* LibWin::ProcessButtonWrap::getMargin ()
	{
		return margin;
	}

	CPadding* LibWin::ProcessButtonWrap::getPadding ()
	{
		return padding;
	}

	CSize LibWin::ProcessButtonWrap::GetContentSize ()
	{
		return size;
	}
	void ProcessButtonWrap::setContent ( ProcessView* view )
	{
		content = view;
	}
}
