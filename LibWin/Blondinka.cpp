#include "Blondinka.h"
namespace LibWin {
	ProcessView* Blondinka::configure ( HWND hWnd , ProcBuilder* builder)
	{
		PBlond* process = new PBlond ( this , hWnd, 0, 0, CSize(100, 100));
		wnds->add ( process );
		if ( builder )
			builder->build ( process );
		return process;
	}
	const wchar_t* Blondinka::getSzWindowClass ()
	{
		return L"Blondinka";
	}
	int Blondinka::Register ()
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
	void Blondinka::VPaint ( HWND hwnd , HDC hdc , RECT* rcDirty , BOOL bErase , ProcessView* pData )
	{
		if ( paint )
		{
			( *paint ) ( hwnd , hdc , rcDirty , bErase , pData );
			return;
		}
		Graphics g ( hdc );
		SolidBrush* brush = new SolidBrush(BGColor);
		g.FillRectangle ( brush , rcDirty->left ,rcDirty->top ,( int ) ( rcDirty->right - rcDirty->left ) ,( int ) ( rcDirty->bottom - rcDirty->top ));
		delete brush;
	}
	LRESULT Blondinka::VProc ( HWND hwnd , UINT uMsg , WPARAM wParam , LPARAM lParam , ProcessView* pData )
	{
		if ( uMsg == WM_PAINT ) {
			PAINTSTRUCT paintStruct;
			HDC hDC = BeginPaint ( hwnd , &paintStruct );
			VDPaintBuffer ( hwnd , &paintStruct );
			EndPaint ( hwnd , &paintStruct );
		}
		else if( uMsg != WM_MOUSEMOVE )
			return SendMessage ((( ProcessView* )pData->parent)->getHWND() , uMsg , wParam , lParam);
		return DefWindowProc ( hwnd , uMsg , wParam , lParam );
	}
}