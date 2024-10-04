#pragma once

#include "Components.h"
#include "Visual.h"

using namespace Util;

namespace Components {

	class ProcessButton : public ProcessView {
	public:
		bool isDown = false;

		ProcessButton ( View* , HWND , const char* );
		// ”наследовано через ProcessView
		CMargin* getMargin () override;
		CPadding* getPadding () override;
		CSize GetContentSize () override;
	protected:
		CMargin margin = CMargin ( 0 , 0 , 0 , 0 );


	};

	class Button : virtual public View {
		// ”наследовано через View
		void PVDeleted ( ProcessView* ) override;
		TCHAR* getSzWindowClass () override;

		int Register () override;
		void Unregister () override;
	};
	class ButtonWithText : public Content , public Button {
	public:
		WCHAR* text = new WCHAR[1]{0};//TODO draw
		TextPaintForm *textFormat = new TextPaintForm();
		//TODO оптимизированный класс wstring в stdminus.h, измен€емые строки
		// ”наследовано через Content
		ProcessView* configure ( HWND hWnd , ProcBuilder* ) override;
		void VPaint ( HWND hwnd , HDC hdc , RECT* rcDirty , BOOL bErase , ProcessView* pData ) override;
		LRESULT VProc ( HWND hwnd , UINT uMsg , WPARAM wParam , LPARAM lParam , ProcessView* pData ) override;
	};
	class ButtonWrap : public Component , public Button {
		// ”наследовано через Component
		void childDeleted ( Safety* ) override;
		ProcessView* configure ( HWND hWnd , ProcBuilder* ) override;
		void Unregister () override;
		void VPaint ( HWND hwnd , HDC hdc , RECT* rcDirty , BOOL bErase , ProcessView* pData ) override;
		void setContent ( View* view ) override;
	};
}
