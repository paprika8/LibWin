#pragma once

#include "Components.h"

namespace Components {

	class ProcessButton : public ProcessView {
	public:
		bool isDown = false;

		ProcessButton ( View* , HWND , const char* );
		// ������������ ����� ProcessView
		CMargin* getMargin () override;
		CPadding* getPadding () override;
		CSize GetContentSize () override;
	};

	class Button : virtual public View {
		// ������������ ����� View
		void PVDeleted ( ProcessView* ) override;
		TCHAR* getSzWindowClass () override;

		int Register () override;
		void Unregister () override;
	};
	class ButtonWithText : public Content , public Button {
	public:
		WCHAR* text = new WCHAR[1]{0};//TODO draw
		//TODO ���������������� ����� wstring � stdminus.h, ���������� ������
		// ������������ ����� Content
		void configure ( HWND hWnd , ProcBuilder* ) override;
		void VPaint ( HWND hwnd , HDC hdc , RECT* rcDirty , BOOL bErase , ProcessView* pData ) override;
		LRESULT VProc ( HWND hwnd , UINT uMsg , WPARAM wParam , LPARAM lParam , ProcessView* pData ) override;
	};
	class ButtonWrap : public Component , public Button {
		// ������������ ����� Component
		void childDeleted ( Safety* ) override;
		void configure ( HWND hWnd , ProcBuilder* ) override;
		void Unregister () override;
		void VPaint ( HWND hwnd , HDC hdc , RECT* rcDirty , BOOL bErase , ProcessView* pData ) override;
		void setContent ( View* view ) override;
	};
}
