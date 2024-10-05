#include "Visual.h"

namespace Util {
	void drawText ( HDC hdc , RECT rect , WCHAR* str, TextPaintForm* textFormat, _Out_ TextModel* model)
	{
		COLORREF color = GetBkColor ( hdc );
		SetBkColor ( hdc , textFormat->backcolor.ToCOLORREF() );
		SetTextColor ( hdc , textFormat->color.ToCOLORREF () );
		HDC bufHdc = CreateCompatibleDC ( hdc );
		LPRECT bufRect = new RECT ( rect );
		//DrawText ( bufHdc , str , -1 , bufRect , 0 );
		switch ( textFormat->wrap & 0b11110000 )
		{
		case NoWrap:
		{
			const WCHAR* points = L"...\n";
			ABCFLOAT abc;
			GetCharABCWidthsFloatA ( hdc , '.' , '.' , &abc);
			float lengthEnd = abc.abcfB + abc.abcfA + abc.abcfC;
			lengthEnd *= 3;
			WCHAR* it = str;
			for ( ; *it != '\0'; it++ )
			{
				GetCharABCWidthsFloatA ( hdc , *it , *it ,&abc);
				float length = abc.abcfB + abc.abcfA + abc.abcfC;
				if ( bufRect->left + length + lengthEnd > bufRect->right )
				{
					float lengthToEnd = 0;
					for ( WCHAR* bufIt = it + 1; *bufIt != '\0' && bufRect->left + length + lengthToEnd <= bufRect->right; bufIt++ )
					{
						GetCharABCWidthsFloatA ( hdc , *bufIt , *bufIt , &abc );
						lengthToEnd += abc.abcfB + abc.abcfA + abc.abcfC;
					}
					if ( bufRect->left + length + lengthToEnd > bufRect->right ){
						DrawText ( hdc , points , -1 , bufRect , 0 );
						break;
					}
				}
				DrawText ( hdc , it, 1, bufRect , 0);
				bufRect->left += length;
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
			WCHAR* it = str;
			for ( ; *it != '\0' && bufRect->left < bufRect->right; it++ )
			{
				ABCFLOAT abc;
				GetCharABCWidthsFloatA ( hdc , *it , *it , &abc );
				DrawText ( hdc , it , 1 , bufRect , 0 );
				bufRect->left += abc.abcfB + abc.abcfA + abc.abcfC;
			}
		}
			break;
		}
		SetBkColor ( hdc , color );
		delete bufRect;
		DeleteObject ( bufHdc );
	}
}
