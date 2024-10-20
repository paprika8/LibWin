#include "UIStructs.h"

namespace LibWin {
	CSize CSize::plusRight ( CSize b , CMargin m )
	{
		return CSize ( width + b.width + m.left , max ( height , b.height ) );
	}
	CSize CSize::plusBottom ( CSize b , CMargin margin )
	{
		return CSize ( max ( width , b.width ) , height + b.height + margin.top );
	}
}