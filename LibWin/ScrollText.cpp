#include "ScrollText.h"
namespace LibWin {
	ProcessView* ScrollText::configure ( HWND hWnd , ProcBuilder* builder )
	{
		PScrollText* process = new PScrollText ( this , hWnd );
		SendMessage ( process->getHWND () , WM_SETSCROLLEDTEXT , 0 , 0 );
		wnds->add ( process );
		if ( builder )
			builder->build ( process );
		return process;
	}
	const wchar_t* ScrollText::getSzWindowClass ()
	{
		return L"PScrollText";
	}
	int ScrollText::Register ()
	{
		WNDCLASS wcex;

		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = SVProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = sizeof ( CData* );
		wcex.hInstance = hInstance;
		wcex.hIcon = NULL;
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
	void ScrollText::VPaint ( HWND hwnd , HDC hdc , RECT* rcDirty , BOOL bErase , ProcessView* pData )
	{
		Graphics g ( hdc );
		SolidBrush* brush = new SolidBrush ( BGColor );
		g.FillRectangle ( brush , rcDirty->left , rcDirty->top , ( int ) ( rcDirty->right - rcDirty->left ) , ( int ) ( rcDirty->bottom - rcDirty->top ) );

		RECT r = RECT ( *rcDirty );
		r.top = -dynamic_cast < PScrollText* > ( pData )->WPos;
		pData->getPadding ()->reRect ( r );
		delete brush;
		brush = new SolidBrush ( Color ( 0 , 0 , 0 ) );
		Util::drawText ( &g , r , ( WCHAR* ) text.c_str () , stringFormat , font , brush );
		delete brush;
	}
	LRESULT ScrollText::VProc ( HWND hwnd , UINT uMsg , WPARAM wParam , LPARAM lParam , ProcessView* pData )
	{
		PScrollText* f = dynamic_cast < PScrollText* > ( pData );
		switch ( uMsg )
		{
		case WM_PAINT:
		{
			PAINTSTRUCT paintStruct;
			HDC hDC = BeginPaint ( hwnd , &paintStruct );
			VDPaintBuffer ( hwnd , &paintStruct );
			EndPaint ( hwnd , &paintStruct );
			break;
		}
		case WM_MOUSEWHEEL:
		{
			int WPDelta = GET_WHEEL_DELTA_WPARAM ( wParam );
			WPDelta /= 30 * Util::kSize;

			f->WPos += WPDelta;
			if ( f->WPos < 0 )
			{
				f->WPos = 0;
			}
			if ( f->WPos > f->TextHeight )
			{
				f->WPos = f->TextHeight;
			}
			InvalidateRect ( pData->getHWND (), 0, 0 );
			return 0;
		}
		case WM_SIZE:
		case WM_SETSCROLLEDTEXT:
		{
			CSize buff = f->getAbsoluteSize();
			f->getPadding ()->reSize ( buff );
			RectF rect1 ( 0 , 0 , buff.width , 0 );
			RectF rect2;
			Graphics graph = GetDC ( 0 );
			graph.MeasureString ( text.c_str () , -1 , font , rect1 , stringFormat , &rect2 , 0 , 0 );
			f->TextHeight = rect2.Height - buff.height;
			if ( f->WPos > f->TextHeight )
			{
				f->WPos = f->TextHeight;
			}
			if ( f->WPos < 0 )
			{
				f->WPos = 0;
			}
			return 0;
		}
		default:
		{
			return DefWindowProc ( hwnd , uMsg , wParam , lParam );
		}
		}


	}
}
