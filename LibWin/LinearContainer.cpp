#include "LinearContainer.h"
#include "ProcBuilders.h"
#include "Positioner.h"
namespace LibWin {
	View* LinearContainer::get ( int i )
	{
		return pointerArr[i];
	}
	int LinearContainer::len ()
	{
		return pointerArr.size();
	}
	ProcessView* LinearContainer::configure ( HWND hWnd , ProcBuilder* builder)
	{
		PLContainer* PLC = new PLContainer ( this , hWnd );
		wnds->add ( PLC );
		if ( builder )
			builder->build ( PLC );
		for ( int i = 0; i < len (); i++ ) {
			ProcessView* child = get ( i )->configure ( PLC->getHWND () );
			PComposite* comp = dynamic_cast< PComposite* >( PLC );
			comp->add ( child );
		}
		Positioner positioner = Positioner ( PLC );
		positioner.Positioning ();
		return PLC;
	}
	const wchar_t* LinearContainer::getSzWindowClass ()
	{
		return L"PLContainer";
	}
	int LinearContainer::Register ()
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
	void LinearContainer::VPaint ( HWND hwnd , HDC hdc , RECT* rcDirty , BOOL bErase , ProcessView* pData )
	{
		Graphics g ( hdc );
		SolidBrush* brush = new SolidBrush ( Color ( 205 , 255 , 255 ) ); //TODO background
		g.FillRectangle ( brush , //TODO background
			0 ,
			0 ,
			( int ) ( rcDirty->right - rcDirty->left ) ,
			( int ) ( rcDirty->bottom - rcDirty->top )
		);
		delete brush;
	}
	void LinearContainer::add ( View* view)
	{
		if ( !view ) {
			return;
		}
		view->parent = this;
		pointerArr.add ( view );
		ProcessView* child , * process = 0;
		if ( process = wnds->get ( 0 ) ) {
			for ( int i = 0; i < len(); i++ ) {
				child = get ( i )->configure ( process->getHWND () );
				PComposite* comp = dynamic_cast< PComposite* >( process );
				comp->add ( child );
			}
			Positioner positioner = Positioner ( process );
			positioner.Positioning ();
		}
	}
	void LinearContainer::remove ( View* view)
	{
		pointerArr.rem ( view );
	}
	void LinearContainer::childDeleted ( Safety* child )
	{
		View* view = dynamic_cast< View* >( child );
		if ( view )
			pointerArr.rem ( view , 0);
	}
	LRESULT LinearContainer::VProc ( HWND hwnd , UINT uMsg , WPARAM wParam , LPARAM lParam , ProcessView* pData )
	{
		if ( uMsg == WM_PAINT ) {
			PAINTSTRUCT paintStruct;
			HDC hDC = BeginPaint ( hwnd , &paintStruct );
			VDPaintBuffer ( hwnd , &paintStruct );
			EndPaint ( hwnd , &paintStruct );
			if ( pData->parent )
				return SendMessage ( ( ( ProcessView* ) pData->parent )->getHWND () , uMsg , wParam , lParam );
		}
		return DefWindowProc ( hwnd , uMsg , wParam , lParam );
	}
	PLContainer::PLContainer ( View* aModel , HWND hwnd , const char* _id ) : PComposite ( aModel , hwnd , _id )
	{
		hWnd = CreateWindowEx (
			0 ,
			model->getSzWindowClass () ,
			L"" ,
			WS_VISIBLE | WS_CHILD ,
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
	ProcessView* PLContainer::get ( int i )
	{
		return pointerArr[i];
	}
	int PLContainer::len ()
	{
		return pointerArr.size ();
	}
	void PLContainer::add ( ProcessView* pview)
	{
		if ( !pview ) {
			return;
		} 
		pointerArr.add ( pview );
		pview->parent = this;
	}
	void PLContainer::remove ( ProcessView* pview)
	{
		pointerArr.rem ( pview );
	}
	void PLContainer::childDeleted ( Safety* child )
	{
		pointerArr.rem ( ( ProcessView* )child , false );
		Positioner positioner (this);
		positioner.Positioning ();
	}
}