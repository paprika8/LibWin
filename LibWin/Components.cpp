#include "Components.h"

namespace Components {
	ULONG_PTR gdiplusToken = 0;
	void moveWnd ( ProcessView* process , ProcessView* child );
	void View::VDPaintBuffer ( HWND hwnd , PAINTSTRUCT* pPaintStruct )
	{
		int cx = pPaintStruct->rcPaint.right - pPaintStruct->rcPaint.left;
		int cy = pPaintStruct->rcPaint.bottom - pPaintStruct->rcPaint.top;
		HDC hMemDC;
		HBITMAP hBmp;
		HBITMAP hOldBmp;
		POINT ptOldOrigin;

		CData* pData = ( CData* ) GetWindowLongPtr ( hwnd , 0 );
		ProcessView* that = pData->that;

		// Create new bitmap-back device context, large as the dirty rectangle.
		hMemDC = CreateCompatibleDC ( pPaintStruct->hdc );
		hBmp = CreateCompatibleBitmap ( pPaintStruct->hdc , cx , cy );
		hOldBmp = ( HBITMAP ) SelectObject ( hMemDC , hBmp );

		// Do the painting into the memory bitmap.
		OffsetViewportOrgEx ( hMemDC , -( pPaintStruct->rcPaint.left ) ,
			-( pPaintStruct->rcPaint.top ) , &ptOldOrigin );

		RECT rcPs = pPaintStruct->rcPaint;
		that->getModel ()->VPaint ( hwnd , hMemDC , &pPaintStruct->rcPaint , TRUE , that );

		SetViewportOrgEx ( hMemDC , ptOldOrigin.x , ptOldOrigin.y , NULL );

		// Blit the bitmap into the screen. This is really fast operation and although
		   // the CustomPaint() can be complex and slow there will be no flicker any more.
		BitBlt ( pPaintStruct->hdc , pPaintStruct->rcPaint.left , pPaintStruct->rcPaint.top ,
			cx , cy , hMemDC , 0 , 0 , SRCCOPY );

		// Clean up.
		SelectObject ( hMemDC , hOldBmp );
		DeleteObject ( hBmp );
		DeleteDC ( hMemDC );
	}
	LRESULT View::SVProc ( HWND hwnd , UINT uMsg , WPARAM wParam , LPARAM lParam )
	{
		CData* pData = ( CData* ) GetWindowLongPtr ( hwnd , 0 );
		switch ( uMsg ) {
		case WM_NCCREATE:
		{
			pData = ( CData* ) malloc ( sizeof ( CData ) );
			if ( pData == NULL )
				return FALSE;
			pData->that = 0;
			SetWindowLongPtr ( hwnd , 0 , ( LONG_PTR ) pData );
		}
		return DefWindowProc ( hwnd , uMsg , wParam , lParam );
		case WM_SIZE:
		{
			View *view = pData->that->getModel ();
			if (Component* comp = dynamic_cast < Component* >( view ) ) {
				moveWnd ( pData->that , comp->getContent ()->wnds->get ( 0 ) );
			}
		}
		return 0;
		case WM_DESTROY:
		case WM_NCDESTROY:
			if ( pData != NULL ) {
				SetWindowLongPtr ( hwnd , 0 , 0 );
				{
					auto buf = pData->that->getModel ()->eve.has ( uMsg );
					if ( buf )
						for ( auto i = buf->m; i != buf->m + buf->size (); i++ )
							( *i )( hwnd , uMsg , wParam , lParam );
				}
				delete pData->that;
				free ( pData );
				pData = 0;
			}
			return DefWindowProc ( hwnd , uMsg , wParam , lParam );
		default:
			if ( pData )
				if ( pData->that ) {
					{
						auto buf = pData->that->getModel ()->eve.has ( uMsg );
						if ( buf )
							for ( auto i = buf->m; i != buf->m + buf->size (); i++ )
								if ( LRESULT res = ( *i )( hwnd , uMsg , wParam , lParam ) )
									return res;
					}
					return pData->that->getModel ()->VProc ( hwnd , uMsg , wParam , lParam , pData->that );
				}
		}
		return DefWindowProc ( hwnd , uMsg , wParam , lParam );
	}
	int InitLib ( HINSTANCE ahInstance )
	{
		hInstance = hInstance;
		GdiplusStartupInput gdiplusStartupInput;

		Status st = GdiplusStartup ( &gdiplusToken , &gdiplusStartupInput , NULL );
		if ( st != Ok )
		{
			MessageBox ( NULL ,
				_T ( "Call to GdiplusStartup failed!" ) ,
				_T ( "GdiplusStartup ist kaputt" ) ,
				NULL );

			return -1;
		}

		return 0;
	}
	void ExitLib ()
	{
		GdiplusShutdown ( gdiplusToken );
	}

	int RectPWindow::init ( _In_ DWORD dwExStyle , _In_ DWORD dwStyle , _In_ int X , _In_ int Y , _In_ int nWidth , _In_ int nHeight ,
		_In_opt_ HWND hWndParent , _In_opt_ HMENU hMenu , _In_opt_ HINSTANCE hInstance , _In_opt_ LPVOID lpParam )
	{
		size = CSize ( nWidth , nHeight );
		hWnd = CreateWindowEx (
			dwExStyle ,
			model->getSzWindowClass () ,
			L"Rect" ,
			dwStyle | WS_OVERLAPPEDWINDOW ,
			X , Y ,
			nWidth , nHeight ,
			hWndParent ,
			hMenu ,
			hInstance ,
			lpParam
		);
		if ( !hWnd )
		{
			MessageBox ( NULL ,
				_T ( "Call to CreateWindowEx failed!" ) ,
				_T ( "Windows Desktop Guided Tour" ) ,
				NULL );
			return -1;
		}
		CData* cData = new CData ();
		cData->that = this;
		SetWindowLongPtr ( hWnd , 0 , ( LONG_PTR ) cData );
	}

	RectPWindow::~RectPWindow ()
	{
		if ( hWnd )
			SendMessage ( hWnd , WM_CLOSE , 0 , 0 );
		PostQuitMessage ( 0 );
	}

	int RectWindow::Register ()
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
	void RectWindow::Unregister ()
	{
		UnregisterClass ( getSzWindowClass () , NULL );
	}

	RectPWindow::RectPWindow ( View* aModel , ProcBuilder* data ) : ProcessWindow ( aModel , 0 )
	{
		padding = new CPadding ();
		ProcessWindow::init ( 400 , 300 );

		if ( data )
			data->build ( this );
	}

	RectPWindow::RectPWindow ( View* aModel ) : RectPWindow ( aModel , 0 ) {}

	RectWindow::RectWindow ()
	{
		wnds = new SingleWnd ();
		configure ( 0 , 0 );
	}

	ProcessView* RectWindow::configure ( HWND hWnd , ProcBuilder* )
	{
		RectPWindow* win = new RectPWindow ( this );
		wnds->add ( win );
		if ( content )
			content->configure ( win->getHWND () );
		return win;
	}

	RectWindow::~RectWindow ()
	{
		if ( content )
			delete content;
	}

	void RectWindow::childDeleted ( Safety* )
	{
		content = 0;
	}

	void RectWindow::PVDeleted ( ProcessView* )
	{
		delete this;
	}

	void RectWindow::VPaint ( HWND hwnd , HDC hdc , RECT* rcDirty , BOOL bErase , ProcessView* pData )
	{
		Graphics g ( hdc );
		SolidBrush* brush = new SolidBrush ( Color ( 255 , 255 , 255 ));
		g.FillRectangle ( brush , //TODO background
			0 ,
			0 ,
			( int ) ( rcDirty->right - rcDirty->left ) ,
			( int ) ( rcDirty->bottom - rcDirty->top )
		);

		delete brush;
	}

	LRESULT View::VProc ( HWND hwnd , UINT uMsg , WPARAM wParam , LPARAM lParam , ProcessView* pData )
	{
		return DefWindowProc ( hwnd , uMsg , wParam , lParam );
	}
	LRESULT RectWindow::VProc ( HWND hwnd , UINT uMsg , WPARAM wParam , LPARAM lParam , ProcessView* pData )
	{
		if ( uMsg == WM_PAINT )
		{
			PAINTSTRUCT paintStruct;
			HDC hDC = BeginPaint ( hwnd , &paintStruct );

			VDPaintBuffer ( hwnd , &paintStruct );

			EndPaint ( hwnd , &paintStruct );
		}

		return DefWindowProc ( hwnd , uMsg , wParam , lParam );
	}

	void moveWnd (ProcessView*process, ProcessView*child) {
		CPoint cord = ( 0 , 0 );
		CSize size = CSize (process->size );
		child->getMargin ()->reRect ( cord , size , child->size , MarginType::CONTENT );
		MoveWindow ( child->getHWND () , cord.x , cord.y , size.width , size.height , true );
	}

	void RectWindow::setContent ( View* view )
	{
		if ( content )
			delete content;
		content = view;
		view->parent = this;
		ProcessView* child, *process  = 0;
		if (process = wnds->get(0)) {
			child = content->configure(process->getHWND());
			moveWnd(process, child);
		}

	}

	void SingleWnd::add ( ProcessView* proc )
	{
		if ( single )
			delete single;
		single = proc;
	}
	void SingleWnd::rem ( HWND hwnd )
	{
		if ( single->getHWND () == hwnd ) {
			delete single;
			single = 0;
		}
	}
	ProcessView* SingleWnd::get ( int i )
	{
		if ( i )
			return 0;
		return single;
	}
	int SingleWnd::len ()
	{
		if ( single )
			return 1;
		return 0;
	}
	bool SingleWnd::isEmpty ()
	{
		return !single;
	}
	void SizeProcBuilder::build ( ProcessView* process )
	{
		process->size = size;
	}
	
}
