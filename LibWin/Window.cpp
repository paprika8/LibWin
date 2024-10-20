#include "Window.h"
#include "Positioner.h"

namespace LibWin {
	int RectPWindow::init ( _In_ DWORD dwExStyle , _In_ DWORD dwStyle , _In_ int X , _In_ int Y , _In_ int nWidth , _In_ int nHeight ,
		_In_opt_ HWND hWndParent , _In_opt_ HMENU hMenu , _In_opt_ HINSTANCE hInstance , _In_opt_ LPVOID lpParam )
	{
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
		wcex.hbrBackground = ( HBRUSH ) ( COLOR_WINDOWTEXT );
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

	RectPWindow::RectPWindow ( View* aModel ) : ProcessWindow ( aModel , 0 )
	{
		padding = new CPadding ( 0 , 0 , 0 , 0 );
		ProcessWindow::init ( 400 , 300 );

	}

	RectWindow::RectWindow ()
	{
		wnds = new SingleWnd ();
		configure ( 0 , 0 );
	}

	ProcessView* RectWindow::configure ( HWND hWnd , ProcBuilder* data )
	{
		RectPWindow* win = new RectPWindow ( this );
		if ( data )
			data->build ( win );
		wnds->add ( win );
		if ( content )
			content->configure ( win->getHWND () );
		return win;
	}

	RectWindow::~RectWindow () //TODO удалить ProcessView
	{
		if ( content )
			delete content;
	}

	void RectWindow::childDeleted ( Safety* )
	{
		content = 0;
	}

	void RectWindow::PVDeleted ( ProcessView* ) //TODO обнулять ссылки на PV
	{
		delete this;
	}

	void RectWindow::VPaint ( HWND hwnd , HDC hdc , RECT* rcDirty , BOOL bErase , ProcessView* pData )
	{
		Graphics g ( hdc );
		SolidBrush* brush = new SolidBrush ( Color ( 255 , 255 , 255 ) ); //TODO background
		g.FillRectangle ( brush , //TODO background
			0 ,
			0 ,
			( int ) ( rcDirty->right - rcDirty->left ) ,
			( int ) ( rcDirty->bottom - rcDirty->top )
		);

		delete brush;
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

	void RectWindow::setContent ( View* view )
	{
		if ( content )
			delete content;
		content = view;
		view->parent = this;
		ProcessView* child , * process = 0;
		if ( process = wnds->get ( 0 ) ) {
			child = content->configure ( process->getHWND () );
			PComponent* comp = dynamic_cast< PComponent* >( process );
			comp->setContent (child);
			Positioner positioner = Positioner ( process);
			positioner.Positioning ();
		}
	}

	void ProcessWindow::setContent ( ProcessView* view )
	{
		content = view;

	}
}

