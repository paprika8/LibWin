#pragma once
#include "Components.h"
namespace LibWin {
	/// <summary>
	/// Устанавливает размер ProcessView при его создании
	/// </summary>
	class SizeProcBuilder : public ProcBuilder
	{
	public:
		MarginType marginType;
		CSize size;
		CMargin* margin = 0;
		CPadding* padding = 0;
		SizeProcBuilder ( CSize , MarginType );

		void build ( ProcessView* ) override;

		~SizeProcBuilder () {
			if ( margin )
				delete margin;
			if ( padding )
				delete padding;
		}
	};


	class LambdaProcBuilder : public ProcBuilder
	{
	public:
		std::function<void( ProcessView* )> lambdaBuild;
		LambdaProcBuilder ( std::function<void ( ProcessView* )> abuild );

		void build ( ProcessView* ) override;
	};
}