#include "Visual.h"

namespace Util {
	void drawText ( HDC hdc , RECT rect , WCHAR* str, TextPaintForm* textFormat, _Out_ TextModel* model) 
	{
		HDC bufHdc = CreateCompatibleDC ( hdc );
		LPRECT bufRect = new RECT ( rect );
		//DrawText ( bufHdc , str , -1 , bufRect , 0 );
		switch ( textFormat->wrap & 0b11110000 )
		{
		case NoWrap:
		{
			WCHAR* it = str;
			for ( ; *it != '\0'; it++ )
			{
				ABCFLOAT abc;
				GetCharABCWidthsFloatA ( hdc , *it , *it ,&abc);
			}
		}
			break;
		case Wrap:
		{

		}
			break;
		case WordWrap:
		{

		}
			break;
		default:
		{

		}
			break;
		}
	}
	inline void getRect () {

	}
}
