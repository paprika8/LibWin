#include "Components.h"
#include "ProcBuilders.h"
#include "Positioner.h"


namespace LibWin {
	ULONG_PTR gdiplusToken = 0;
	
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
			if(!pData->that )
				return DefWindowProc ( hwnd , uMsg , wParam , lParam );
			pData->that->size = lParam;
			Positioner positioner = Positioner ( pData->that );
			positioner.Positioning ();
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

	LRESULT View::VProc ( HWND hwnd , UINT uMsg , WPARAM wParam , LPARAM lParam , ProcessView* pData )
	{
		return DefWindowProc ( hwnd , uMsg , wParam , lParam );
	}

	void moveWnd (ProcessView*process, ProcessView*child) {
		CPoint cord = ( 0 , 0 );
		CSize size = CSize (process->size );
		child->getMargin ()->reRect ( cord , size , child->size , MarginType::CONTENT );
		MoveWindow ( child->getHWND () , cord.x , cord.y , size.width , size.height , true );
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
	SizeProcBuilder::SizeProcBuilder ( CSize asize, MarginType atype)
	{
		size = asize;
		marginType = atype;
	}
	void SizeProcBuilder::build ( ProcessView* process)
	{
		process->size = size;
		PComposite* isPComposite = dynamic_cast< PComposite* >( process );
		if ( isPComposite ) {
			isPComposite->marginType = this->marginType;
		}
		process->size = size;
		PComponent* isPComponent = dynamic_cast< PComponent* >( process );
		if ( isPComponent ) {
			isPComponent->marginType = this->marginType;
		}
	}
}
