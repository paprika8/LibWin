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
		PScrollText* f = dynamic_cast < PScrollText* > ( pData );
		Graphics g ( hdc );
		SolidBrush* brush = new SolidBrush ( BGColor );
		g.FillRectangle ( brush , rcDirty->left , rcDirty->top , ( int ) ( rcDirty->right - rcDirty->left ) , ( int ) ( rcDirty->bottom - rcDirty->top ) );
		delete brush;


		CSize AbsSize = pData->getAbsoluteSize ();
		pData->getPadding ()->reSize ( AbsSize );
		if ( AbsSize.height < f->TextHeight ) {
			//подложка для скролл бара
			brush = new SolidBrush ( BGColor - 18 );
			Rect ScrollRect = Rect ( rcDirty->right - 20 , rcDirty->top , 20 , rcDirty->bottom - rcDirty->top );
			g.FillRectangle ( brush , ScrollRect );
			delete brush;

			//скролл бар
			brush = new SolidBrush ( BGColor - 38 );
			int c = max ( pData->getAbsoluteSize ().height * AbsSize.height / f->TextHeight , 20 );
			Rect ScrollBar = Rect ( rcDirty->right - 18 , rcDirty->top + ( ( double ) f->WPos ) / f->MaxTextHeight * ( pData->getAbsoluteSize ().height - c ) , 16 , c );
			g.FillRectangle ( brush , ScrollBar );
			delete brush;
		}

		RECT r = RECT ( *rcDirty );
		r.top = -f->WPos;
		pData->getPadding ()->reRect ( r );
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
		break;
		
		case WM_LBUTTONDOWN:
		{
			CSize AbsSize = pData->getAbsoluteSize ();
			int width = AbsSize.width;
			pData->getPadding ()->reSize ( AbsSize );
			int c = max ( pData->getAbsoluteSize ().height * AbsSize.height / f->TextHeight , 20 );
			int a = pData->getAbsoluteSize ().height;
			int pos = pData->getPadding()->top + ( ( double ) f->WPos ) / f->MaxTextHeight * ( pData->getAbsoluteSize ().height - c );
			int xPos = LOWORD ( lParam );
			int yPos = HIWORD ( lParam );
			if ( pos < yPos && yPos < pos + c && xPos > width-18 && xPos < width-2) {
				f->isDown = 1;
				SetCapture ( pData->getHWND () );
				f->oldY = yPos;
			}
			

		}
		break;
		case WM_MOUSEMOVE:
		{
			if ( f->isDown ) {
				CSize AbsSize = pData->getAbsoluteSize ();
				pData->getPadding ()->reSize ( AbsSize );
				int c = max ( pData->getAbsoluteSize ().height * AbsSize.height / f->TextHeight , 20 );
				int a = pData->getAbsoluteSize ().height;
				short yPos = HIWORD ( lParam );

				int deltaPos = yPos - f->oldY;
				f->WPos += deltaPos * f->MaxTextHeight / ( a - c );
				if ( f->WPos > f->MaxTextHeight )
				{
					f->WPos = f->MaxTextHeight;
				}

				if ( f->WPos < 0 )
				{
					f->WPos = 0;
				}
				f->oldY = yPos;
				InvalidateRect ( pData->getHWND () , 0 , 0 );
			}


		}
		break;
		case WM_MOUSELEAVE:
		case WM_LBUTTONUP:
		{
			if( f->isDown )
				ReleaseCapture ();
			f->isDown = 0;
		}
		break;

		case WM_MOUSEWHEEL:
		{
			int WPDelta = GET_WHEEL_DELTA_WPARAM ( wParam );
			WPDelta /= 15 * Util::kSize;

			f->WPos -= WPDelta;
			if ( f->WPos > f->MaxTextHeight )
			{
				f->WPos = f->MaxTextHeight;
			}
			
			if ( f->WPos < 0 )
			{
				f->WPos = 0;
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
			f->MaxTextHeight = rect2.Height - buff.height;
			f->TextHeight = rect2.Height;
			if ( f->WPos > f->MaxTextHeight )
			{
				f->WPos = f->MaxTextHeight;
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
