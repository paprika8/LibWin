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
	}
	void LinearContainer::add ( View* view)
	{
		if ( !view ) {
			return;
		}
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
}