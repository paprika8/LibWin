#pragma once

namespace LibWin {
	/// <summary>
	/// Родительский класс для обеспечения безопасного удаления дочерних элементовы
	/// </summary>
	class __declspec( novtable ) Safety
	{
	public:
		Safety* parent = 0;
		virtual void childDeleted ( Safety* ) = 0;
		/// <summary>
		/// Сообщает об удалении все связанные объекты
		/// </summary>
		virtual ~Safety ()
		{
			if ( parent )
				parent->childDeleted ( this );
		}
	};
}
