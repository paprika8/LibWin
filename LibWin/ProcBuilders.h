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
		SizeProcBuilder ( CSize, MarginType);

		void build ( ProcessView* ) override;
	};
}