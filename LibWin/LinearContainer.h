#pragma once
#include "Components.h"
namespace LibWin {
	class LinearContainer : public Composite
	{
	public:
		arr <View* , 1> pointerArr;
		LinearContainer () : Composite (){
			wnds = new SingleWnd ();
		}
		virtual View* get ( int i ) override;

		virtual int len () override;
		
		ProcessView* configure ( HWND hWnd , ProcBuilder* ) override;

		const wchar_t* getSzWindowClass () override;

		int Register () override;

		void VPaint ( HWND hwnd , HDC hdc , RECT* rcDirty , BOOL bErase , ProcessView* pData ) override;

		void add ( View* ) override;

		void remove ( View* ) override;

		void childDeleted ( Safety* child ) override;
		

		LRESULT VProc ( HWND hwnd , UINT uMsg , WPARAM wParam , LPARAM lParam , ProcessView* pData ) override;


	private:

	};

	class PLContainer : public PComposite
	{
	public:
		
		arr <ProcessView* , 1> pointerArr;

		PLContainer ( View* aModel , HWND hwnd , const char* _id = "" );
		virtual ProcessView* get ( int i ) override;

		virtual int len () override;

		void add ( ProcessView* ) override;

		void remove ( ProcessView* ) override;

		void childDeleted ( Safety* child ) override;

		CSize GetContentSize (CSize size) override
		{
			CSize res;
			CSize asize;
			CMargin amargin ( 0 , 0 , 0 , 0 );
			for ( int i = 0; i < len (); i++ ) {
				asize = get ( i )->size.toAbsolut ( size );
				amargin = get ( i )->getMargin ()->toAbsolut ( size );
				if ( !orientation )
					res = res.plusRight ( asize, amargin );
				else
					res = res.plusBottom ( asize , amargin );
			}
			return res;
		}

	private:

	};
}


