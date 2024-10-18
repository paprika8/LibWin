#include "Visual.h"

namespace Util {
	Font *createFont ( int size )
	{
		size *= kSize;
		FontFamily family ( L"Times New Roman" );
		return new Font ( &family , size , FontStyleRegular , UnitPixel );
	}
	void drawText ( Gdiplus::Graphics *g , RECT rect , WCHAR* str , StringFormat* stringFormat, Font *font, Brush* brush )
	{
		g->DrawString ( str , -1 , font , RectF ( rect.left , rect.top , rect.right - rect.left , rect.bottom - rect.top ) , stringFormat , brush );
	}
}
