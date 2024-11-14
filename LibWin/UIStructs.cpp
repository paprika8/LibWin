#include "UIStructs.h"

namespace LibWin {
	CSize CSize::plusRight ( CSize b , CMargin m )
	{
		return CSize ( ( short ) ( width + b.width + m.left + m.right) , ( short ) max ( ( int ) height , b.height + m.top + m.bottom ) );
	}
	CSize CSize::minusRight(CSize b, CMargin m)
	{
		return CSize((short)(width - b.width - m.left - m.right), height.value);
	}
	CSize CSize::plusBottom ( CSize b , CMargin m )
	{
		return CSize ( ( short ) max ( ( int ) width , b.width + m.left + m.right ) , ( short ) ( height + b.height + m.top + m.bottom) );
	}
	CSize CSize::minusBottom(CSize b, CMargin m )
	{
		return CSize(width.value, (short)(height - b.height - m.top - m.bottom));
	}
}