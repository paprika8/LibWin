#pragma once

#include "Components.h"
#include "Visual.h"


using namespace Util;

namespace LibWin {

	/// <summary>
	/// ������ ����������� ��� ������
	/// </summary>
	class ProcessButton : public ProcessView {
	public:
		bool isDown = false;

		ProcessButton ( View* , HWND , const char* );
		// ������������ ����� ProcessView
		CMargin* getMargin () override;
		CPadding* getPadding () override;
		CSize GetContentSize () override;
	protected:
		CMargin margin = CMargin ( 0 , 0 , 0 , 0 );


	};

	/// <summary>
	/// ������ ��������� ��� ������
	/// </summary>
	class Button : virtual public View {
		// ������������ ����� View
		void PVDeleted ( ProcessView* ) override;
		const wchar_t* getSzWindowClass () override;

		int Register () override;
		void Unregister () override;
	};

	/// <summary>
	/// ������ ��������� ��� ������ � �������
	/// </summary>
	class ButtonWithText : public Content , public Button {
	public:
		WCHAR* text = new WCHAR[1]{0};//TODO draw
		TextPaintForm *textFormat = new TextPaintForm();
		//TODO ���������������� ����� wstring � stdminus.h, ���������� ������
		// ������������ ����� Content
		ButtonWithText ();
		ProcessView* configure ( HWND hWnd , ProcBuilder* ) override;
		void VPaint ( HWND hwnd , HDC hdc , RECT* rcDirty , BOOL bErase , ProcessView* pData ) override;
		LRESULT VProc ( HWND hwnd , UINT uMsg , WPARAM wParam , LPARAM lParam , ProcessView* pData ) override;
	};

	/// <summary>
	/// ������ ��������� ��� ������ � ���������� !TODO
	/// </summary>
	class ButtonWrap : public Component , public Button {
		// ������������ ����� Component
		void childDeleted ( Safety* ) override;
		ProcessView* configure ( HWND hWnd , ProcBuilder* ) override;
		void Unregister () override;
		void VPaint ( HWND hwnd , HDC hdc , RECT* rcDirty , BOOL bErase , ProcessView* pData ) override;
		void setContent ( View* view ) override;
	};
}
