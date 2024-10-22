#pragma once

#include "stdminus.h"
#include <Windows.h>

using namespace stdminus;

namespace LibWin {
	/// <summary>
	/// ��������� ��� ������ ����������� ������������ ������� ���� �������� ��������
	/// </summary>
	enum MarginType : char
	{
		/// <summary>
		/// ������ ������� ����� �������������� ��������, ���� ��� ������� ��������, �� ��� ����� ������� �����������
		/// </summary>
		CONTENT = 0b00000000 ,
		/// <summary>
		/// ������ ����� �������� ����� ������� ��� �������� (��� ��������� �������)
		/// </summary>
		HPARENT = 0b00000001 ,
		/// <summary>
		/// ������ ����� �������� ����� ������� ��� �������� (��� ��������� �������)
		/// </summary>
		VPARENT = 0b00000010 ,
		/// <summary>
		/// ������ � ������ ����� �������� ����� ������� ��� �������� (��� ��������� �������)
		/// </summary>
		PARENT = 0b00000011 ,
		/// <summary>
		/// �������� �������� ������������� �� ������ ����
		/// </summary>
		LEFT = 0b00001000 ,
		/// <summary>
		/// �������� �������� ������������� �� ����������� �� ������
		/// </summary>
		HCENTER = 0b00001100 ,
		/// <summary>
		/// �������� �������� ������������� �� ������� ����
		/// </summary>
		RIGHT = 0b00000100 ,
		/// <summary>
		/// �������� �������� ������������� �� �������� ����
		/// </summary>
		TOP = 0b00010000 ,
		/// <summary>
		/// �������� �������� ������������� �� ��������� �� ������
		/// </summary>
		VCENTER = 0b00110000 ,
		/// <summary>
		/// �������� �������� ������������� �� ������� ����
		/// </summary>
		BOTTOM = 0b00100000 ,
	};
	/// <summary>
	/// ��������� ������������ ���� ������ ������
	/// </summary>
	enum CalcType : unsigned short
	{
		/// <summary>
		/// ������� ������� ������������� ������ �������
		/// </summary>
		px ,
		/// <summary>
		/// ������� ������� ������������� 1/10 �������� �� ������� ������������� ��������
		/// </summary>
		percent ,
		/// <summary>
		/// ������������ ����� ���� ���������� ������������ � ����-����������, ����� 1/80 ��������
		/// </summary>
		fr


	};

	struct CMargin;
	struct CPadding;

	/// <summary>
	/// ��� ��� �������� ��������� ��������� �� �������� �� ����������� � CalcType
	/// </summary>
	struct pointUI {
		short value : 14;
		CalcType type : 2;

		pointUI () {
			value = 0;
			type = px;
		}
		pointUI ( short avalue ) {
			value = avalue;
			type = px;
		}
		pointUI toAbsolut ( short original ) {
			switch ( type ) {
			case px: {
				return *this;
			}
			case percent: {
				return (*this).value = original / 1000. * value;
			}
			case fr: {
				return ( *this ).value = original / 8000. * value;

			}
			default:{
				return*this;
			}

			}

		};
		void operator=( short a ) {
			value = a;
		}
		void operator+=( short a ) {
			value += a;
		}
		void operator-=( short a ) {
			value -= a;
		}

		operator short ()
		{
			return value;
		}
	};

	/// <summary>
	/// ��� ��� �������� ��������� ���������������� ��������� �� �������� �� ����������� � CalcType
	/// </summary>
	struct upointUI {
		unsigned short value : 14;
		CalcType type : 2;

		upointUI () {
			value = 0;
			type = px;
		}
		upointUI ( unsigned short avalue ) {
			value = avalue;
			type = px;
		}
		upointUI toAbsolut ( short original ) {
			switch ( type ) {
			case px: {
				return *this;
			}
			case percent: {
				return ( *this ).value = original / 1000. * value;
			}
			case fr: {
				return ( *this ).value = original / 8000. * value;

			}
			default: {
				return*this;
			}

			}
		}
		void operator=( unsigned short a ) {
			value = a;
		}
		void operator+=( unsigned short a ) {
			value += a;
		}
		void operator-=( unsigned short a ) {
			value -= a;
		}

		operator unsigned short ()
		{
			return value;
		}
	};

	/// <summary>
	/// ��� ��� �������� �������� ����� � ������������� ����
	/// </summary>
	struct CPoint
	{
		short x;
		short y;
		CPoint ()
		{
			x = 0;
			y = 0;
		}
		CPoint ( short ax , short ay )
		{
			x = ax;
			y = ay;
		}
		CPoint ( int l )
		{
			*( ( int* ) this ) = l;
		}
		int toInt ()
		{
			return *( ( int* ) this );
		}
	};

	/// <summary>
	/// ��� ��� �������� �������� �������
	/// </summary>
	struct CSize
	{
		upointUI width;
		upointUI height;

		CSize ()
		{
			width.type = px;
			height.type = px;
			width = 0;
			height = 0;
		}
		CSize ( unsigned short w , unsigned short h )
		{
			width.type = px;
			height.type = px;
			width = w;
			height = h;
		}
		CSize ( int l )
		{
			*( ( int* ) this ) = l;
		}
		CSize plusRight ( CSize b )
		{
			return CSize ( width + b.width , max ( height , b.height ) );
		}
		CSize plusRight ( CSize b , CMargin m );
		CSize plusBottom ( CSize b )
		{
			return CSize ( max ( width , b.width ) , height + b.height );
		}
		CSize plusBottom ( CSize b , CMargin margin );
		int toInt ()
		{
			return *( ( int* ) this );
		}

		CSize toAbsolut ( CSize original ) {
			return CSize ( width.toAbsolut ( original.width ) ,
				height.toAbsolut ( original.height ) );
		}

	};

	/// <summary>
	/// ��� ��� �������� �������� ����������� �������� �� ��� ����
	/// </summary>
	struct CPadding
	{
		pointUI left;
		pointUI top;
		pointUI right;
		pointUI bottom;
		/// <summary>
		/// ���������� �������� � ������� ��������
		/// </summary>
		/// <param name="point"></param>
		/// <param name="contentSize"></param>
		virtual void reRect ( CPoint& point , CSize& contentSize )
		{
			point.x += left;
			contentSize.width -= left + right;//TODO � ���������
			point.y += top;
			contentSize.height -= top + bottom;
		}
		/// <summary>
		/// ���������� �������� ��� ������� �������� ��������
		/// </summary>
		/// <param name="point"></param>
		/// <param name="contentSize"></param>
		virtual void reSize ( CSize& size )
		{
			size.width += left + right;
			size.height += top + bottom;
		}
		CPadding ( CPadding& orig )
		{
			right = orig.right;
			top = orig.top;
			left = orig.left;
			bottom = orig.bottom;
		};
		CPadding ( const CPadding& orig )
		{
			right = orig.right;
			top = orig.top;
			left = orig.left;
			bottom = orig.bottom;
		}
		CPadding ( short aleft ,
			short atop ,
			short aright ,
			short abottom )
		{
			right = aright;
			top = atop;
			left = aleft;
			bottom = abottom;
		}
		CPadding ( ll l )
		{
			*( ( ll* ) this ) = l;
		}
		ll toLong ()
		{
			return *( ( ll* ) this );
		}

		CPadding toAbsolut ( CSize original ) {
			return CPadding ( left.toAbsolut ( original.width ) ,
				top.toAbsolut ( original.height ) ,
				right.toAbsolut ( original.width ) ,
				bottom.toAbsolut ( original.height ) );
		}
	};

	/// <summary>
	/// ��� ��� �������� �������� ������� � ������� �������� �� ���� ��� ��������
	/// </summary>
	struct CMargin
	{
	public:
		pointUI left;
		pointUI top;
		pointUI right;
		pointUI bottom;

		CMargin ( CMargin& orig )
		{
			right = orig.right;
			top = orig.top;
			left = orig.left;
			bottom = orig.bottom;
		}
		CMargin ( const CMargin& orig )
		{
			right = orig.right;
			top = orig.top;
			left = orig.left;
			bottom = orig.bottom;
		}
		CMargin ( short aleft ,
			short atop ,
			short aright ,
			short abottom )
		{
			right = aright;
			top = atop;
			left = aleft;
			bottom = abottom;
		}
		CMargin ( ll l )
		{
			*( ( ll* ) this ) = l;
		}
		ll toLong ()
		{
			return *( ( ll* ) this );
		}
		CMargin toAbsolut ( CSize original ) {
			return CMargin (left.toAbsolut(original.width),
				top.toAbsolut ( original.height ),
				right.toAbsolut ( original.width ),
				bottom.toAbsolut ( original.height ));
		}

		/// <summary>
		/// ���������� �������� � ��������������
		/// </summary>
		/// <param name="point"> - ����� ������� ����� �������������� </param>
		/// <param name="contentSize"> - ������� ������� �������� �������� </param>
		/// <param name="thisSize"> - ������� ����� �������� </param>
		/// <param name="type"> - ��� ������������ </param>
		virtual void reRect ( CPoint& point , CSize& contentSize , CSize thisSize , MarginType type )
		{
			short x = point.x;
			short y = point.y;
			ushort w = thisSize.width;
			ushort h = thisSize.height;
			if ( type & HPARENT ) {
				w = contentSize.width - left - right;
				x = point.x + left;
			}
			else {
				switch ( type & HCENTER )
				{
				case LEFT:
					x = point.x + left;
					break;
				case RIGHT:
					x = point.x + contentSize.width - right - w;
					break;
				default:
					x = point.x + contentSize.width / 2 - thisSize.width / 2;
					break;
				}
			}
			if ( type & VPARENT ) {
				h = contentSize.height - bottom - top;
				y = point.y + bottom;
			}
			else {
				switch ( type & VCENTER )
				{
				case TOP:
					y = point.y + top;
					break;
				case BOTTOM:
					y = point.y + contentSize.height - h - bottom;
					break;
				default:
					y = top + point.y + contentSize.height / 2 - thisSize.height / 2;
					break;
				}
			}
			point.x = x;
			point.y = y;
			contentSize.width = w;
			contentSize.height = h;
		}
		/// <summary>
		/// ���������� �������� � ������� ��������
		/// </summary>
		/// <param name="size"> - ������� ��������</param>
		virtual void reSize ( CSize& size )
		{
			size.width += left + right;
			size.height += top + bottom;
		}
	};
}