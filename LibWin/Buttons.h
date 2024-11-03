#pragma once

#include "Components.h"
#include "Visual.h"


using namespace Util;

namespace LibWin {

	/// <summary>
	/// Модель отображения для кнопок
	/// </summary>
	class ProcessButton : public ProcessView {
	public:
		bool isDown = false;

		ProcessButton ( View* , HWND , const char* );
	protected:

	};
	class ProcessButtonWrap : public PComponent {
	public:
		bool isDown = false;

		ProcessButtonWrap ( View* , HWND , const char* );
	protected:

	};

	/// <summary>
	/// Модель поведения для кнопок
	/// </summary>
	class Button : virtual public View {
	public:
		std::function<void ()> click = [] ()-> void{};

		// Унаследовано через View
		const wchar_t* getSzWindowClass () override;
		virtual void Click () { click (); }

		int Register () override;
		void Unregister () override;
	};

	/// <summary>
	/// Модель поведения для кнопок с текстом
	/// </summary>
	class ButtonWithText : public Content , public Button {
	public:
		WCHAR* text = new WCHAR[1]{0};
		StringFormat* stringFormat = new StringFormat();
		Font *font = createFont(16);
		Brush* brush = new SolidBrush ( Color ( 255 , 255 , 255 ) );

		ButtonWithText ();
		
		ProcessView* configure ( HWND hwnd , ProcBuilder* ) override;
		void VPaint ( HWND hwnd , HDC hdc , RECT* rcDirty , BOOL bErase , ProcessView* pData ) override;
		LRESULT VProc ( HWND hwnd , UINT uMsg , WPARAM wParam , LPARAM lParam , ProcessView* pData ) override;
	};

	/// <summary>
	/// Модель поведения для кнопок с содержимым !TODO
	/// </summary>
	class ButtonWrap : public Component , public Button {
	public:
		ButtonWrap ();

		ProcessView* configure ( HWND hwnd , ProcBuilder* ) override;
		void VPaint ( HWND hwnd , HDC hdc , RECT* rcDirty , BOOL bErase , ProcessView* pData ) override;
		LRESULT VProc ( HWND hwnd , UINT uMsg , WPARAM wParam , LPARAM lParam , ProcessView* pData ) override;
	};
}
