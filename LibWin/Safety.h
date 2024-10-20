#pragma once

namespace LibWin {
	/// <summary>
	/// ������������ ����� ��� ����������� ����������� �������� �������� ����������
	/// </summary>
	class __declspec( novtable ) Safety
	{
	public:
		Safety* parent = 0;
		virtual void childDeleted ( Safety* ) = 0;
		/// <summary>
		/// �������� �� �������� ��� ��������� �������
		/// </summary>
		virtual ~Safety ()
		{
			if ( parent )
				parent->childDeleted ( this );
		}
	};
}
