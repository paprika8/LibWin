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
			PComposite* compos = dynamic_cast < PComposite* >( parent );
			if ( compos ) {
				PositioningFew (compos);
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
			CPoint cord = ( 0 , 0 );
			CSize size = compos->getAbsoluteSize ();
			padding->reRect ( cord , size );

			CSize contSize = parent->GetContentSize ();
			CMargin tempMargin = CMargin ( 0, 0, 0, 0 );
			CPoint start = ( parent->point );
			CSize buffer = CSize ( size );
			tempMargin.reRect (start, buffer, contSize, compos->marginType);
			MarginType MBuffer = compos->marginType;

			if ( start.x < cord.x ) {
				MBuffer = (MarginType)( MBuffer ^ MarginType::HCENTER | MarginType::LEFT);
			}
			if ( start.y < cord.y ) {
				MBuffer = ( MarginType ) ( MBuffer ^ MarginType::VCENTER | MarginType::TOP );
			}
			for ( int cont = 0; cont < compos->len (); cont++ ) {
				compos->get ( cont )->getMargin ()->toAbsolut ( size );
				compos->get ( cont )->size.toAbsolut ( size );
				MarginType marginType = MBuffer & ~MarginType::PARENT | compos->get(cont)->marginType;
				compos->get ( cont )->getMargin ()->reRect ( cord , size, compos->get ( cont )->size, marginType);
				compos->get ( cont )->Move ( cord , compos->get ( cont )->size );
			}
		}
	};
};


