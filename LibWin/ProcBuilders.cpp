#include "ProcBuilders.h"

LibWin::LambdaProcBuilder::LambdaProcBuilder ( std::function<void ( ProcessView* )> abuild )
{
	lambdaBuild = abuild;
}

void LibWin::LambdaProcBuilder::build ( ProcessView* process)
{
	lambdaBuild ( process );
}
