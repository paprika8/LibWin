#pragma once
#include "Components.h"
#include "UIStructs.h"


namespace LibWin {
	class Positioner
	{

	public:
		ProcessView *parent;
		Positioner ( ProcessView* aparent ) 
		{
			parent = aparent;
		}
		void Positioning() 
		{
			PComponent* comp = dynamic_cast < PComponent* >( parent );
			if ( comp ) {
				PositioningOne (comp);
			}
			else{
				PComposite* compos = dynamic_cast < PComposite* >( parent );
				if ( compos ) {
					PositioningFew (compos);
				}
			}
		}

	private:
		void PositioningOne(PComponent* comp) 
		{
			if ( !comp->getContent () ) 
			{
				return;
			}
			CSize absolutParentSize = comp->getAbsoluteSize ();
			CPadding padding = *parent->getPadding ();
			padding = padding.toAbsolut ( absolutParentSize );
			CPoint cord = ( 0 , 0 );
			padding.reRect (cord, absolutParentSize );
			
			CSize absolutSize = comp->getContent ()->size.toAbsolut ( absolutParentSize );
			MarginType marginType = (comp->marginType & ~MarginType::PARENT) | (comp->getContent()->marginType & MarginType::PARENT);
			comp->getContent ()->getMargin ()->toAbsolut ( absolutParentSize ).reRect (cord , absolutParentSize , absolutSize , marginType);
			comp->getContent ()->Move ( cord, absolutSize );

		}
		void PositioningFew(PComposite* compos) 
		{
			CPadding* padding = parent->getPadding ();
			CPoint cursor = ( 0 , 0 );
			CSize size = compos->getAbsoluteSize ();
			padding->reRect ( cursor , size );

			CSize contSize = parent->GetContentSize ( size );
			CMargin tempMargin = CMargin ( 0, 0, 0, 0 );
			CPoint start = ( 0 , 0 );
			CSize buffer = CSize ( size );
			tempMargin.reRect ( start , buffer , contSize , compos->marginType );
			MarginType MBuffer = compos->marginType;

			CSize contentSize = size;

			if ( start.x < cursor.x ) {
				MBuffer = ( MarginType ) ( MBuffer ^ MarginType::HCENTER | MarginType::LEFT );
			}
			if ( start.y < cursor.y ) {
				MBuffer = ( MarginType ) ( MBuffer ^ MarginType::VCENTER | MarginType::TOP );
			}
			cursor.y = start.y;
			cursor.x = start.x;
			for ( int cont = 0; cont < compos->len (); cont++ ) {
				CMargin amargin = compos->get ( cont )->getMargin ()->toAbsolut ( size );
				CSize asize = compos->get ( cont )->size.toAbsolut ( size );
				CPoint cord = cursor;
				MarginType marginType = MBuffer & ~MarginType::PARENT | (compos->get(cont)->marginType & MarginType::PARENT);
				//amargin.reRect ( cord , contentSize , asize , marginType);
				cord.x += amargin.left;
				cord.y += amargin.top;
				compos->get ( cont )->Move ( cord , asize );
				if( compos->orientation )
				{
					if ( marginType & MarginType::LEFT )
						contentSize = contentSize.minusBottom ( asize , amargin );
					cursor.y += asize.height + amargin.top + amargin.bottom;
				}
				else
				{
					if ( marginType & MarginType::TOP )
						contentSize = contentSize.minusRight ( asize , amargin );
					cursor.x += asize.width + amargin.left + amargin.right;
					
				}
				contentSize = size;
			}
		}
	};
};


