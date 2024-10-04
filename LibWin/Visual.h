#pragma once

#include "Components.h"

namespace Util {
	struct TextModel {

	};
	enum textAligment : unsigned char {
		Left = 0b00000001 ,
		Right = 0b00000010 ,
		GCenter = 0b00000011 ,
		Top = 0b00000100 ,
		Botton = 0b00001000 ,
		HCenter = 0b00001100
	};
	enum textWrap : unsigned char {
		NoWrap = 0b00010000 ,
		Wrap ,
		WordWrap ,
		Cut
	};
	struct TextPaintForm {
		TextPaintForm () {
			aligment = Left;
			wrap = NoWrap;
			interval = 0;
			indent = 0;
			color = Color ( 0 , 0 , 0 );
			backcolor = Color ( 255 , 255 , 255 );
		}
		union {
		public:
			textAligment aligment;
			textWrap wrap;
		};
		short interval;
		short indent;
		Color color;
		Color backcolor;
	};
	void drawText ( HDC hdc, RECT rect , WCHAR* str , TextPaintForm* textFormat , _Out_ TextModel* model );
}
