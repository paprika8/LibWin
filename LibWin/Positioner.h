#pragma once
#include "Components.h"
#include "UIStructs.h"


namespace LibWin {
	class Positioner
	{

	public:
		ProcessView *parent;
		Positioner ( ProcessView* aparent ) {
			parent = aparent;
		}
		void Positioning(){
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
		void PositioningOne(PComponent* comp){
			if ( !comp->getContent () ) {
				return;
			}
			CPadding *padding = parent->getPadding ();
			CPoint cord = ( parent->point );
			CSize size = CSize ( parent->size);
			padding->reRect (cord, size);
			comp->getContent()->getMargin()->reRect (cord , size , comp->size , MarginType::CONTENT);
			comp->getContent ()->Move ( cord, size);

		}
		void PositioningFew(PComposite* compos){
			CPadding* padding = parent->getPadding ();
			CPoint cord = ( parent->point );
			CSize size = CSize ( parent->size );
			padding->reRect ( cord , size );

			CSize contSize = parent->GetContentSize ();
			CMargin tempMargin = CMargin ( 0, 0, 0, 0 );
			CPoint start = ( parent->point );
			CSize buffer = CSize ( parent->size );
			tempMargin.reRect (start, buffer, contSize, compos->marginType);
			MarginType MBuffer = compos->marginType;

			if ( start.x < cord.x ) {
				MBuffer = (MarginType)( MBuffer ^ MarginType::HCENTER | MarginType::LEFT);
			}
			if ( start.y < cord.y ) {
				MBuffer = ( MarginType ) ( MBuffer ^ MarginType::VCENTER | MarginType::TOP );
			}
			for ( int cont = 0; cont < compos->len (); cont++ ) {
				compos->get ( cont )->getMargin ()->reRect ( cord , size, compos->get ( cont )->size, MBuffer );
				compos->get ( cont )->Move ( cord , size );
			}
		}



	};
};


