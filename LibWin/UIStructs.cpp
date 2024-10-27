#include "UIStructs.h"

namespace LibWin {
	CSize CSize::plusRight ( CSize b , CMargin m )
	{
		return CSize ( ( short ) ( width + b.width + m.left ) , ( short ) max ( height , b.height ) );
	}
	CSize CSize::plusBottom ( CSize b , CMargin margin )
	{
		return CSize ( ( short ) max ( width , b.width ) , ( short ) ( height + b.height + margin.top ) );
	}
}