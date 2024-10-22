#pragma once

#include "Components.h"

namespace Util {

	float kSize = 1.f;
	Font* createFont (int size = 14 );

	void drawText ( Gdiplus::Graphics *g , RECT rect , WCHAR* str , StringFormat* stringFormat , Font *font , Brush* brush );
}
