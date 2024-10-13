// -*- explicit-buffer-name: "Term.h<M1-MOBJ/4-5>" -*-

#ifndef NETLIST_TERM_H
#define NETLIST_TERM_H
#include <libxml/xmlreader.h>
#include <string>
#include <iostream>
#include <vector>
#include "Indentation.h"
#include "Point.h"
#include "Node.h"


namespace Netlist{
	class Node;
	class Net;
	class Instance;
	class Cell;
	
	class Term{
		public :
			static Term*	fromXml(Cell*, xmlTextReaderPtr);
		public :
			enum 	Type{ Internal=1, External=2 };
			enum 	Direction { In=1, Out=2, Inout=3, Tristate=4, Transcv=5, Unknown=6 };
			//enum 	NameAlign { TopLeft=1, TopRight=2, BottomLeft=3, BottomRight=4 };
			static 	std::string  toString    (Type type);
			static	std::string  toString    ( Direction direction );
			static	Direction    toDirection ( std::string string);
			//static	NameAlign	 toNameAlign ( std::string string); //Ajout tme9

			Term ( Cell* cell, const std::string& name, Direction direction);
			Term ( Instance*, const Term* modelTerm );
			~Term ();
				
			//Accesseurs
			inline 	bool               	isInternal   () const{return type_ == Internal;}
			inline 	bool               	isExternal   () const{return type_ == External;}
			inline 	const std::string& 	getName      () const;
			inline 	NodeTerm*         	getNode      (){return &node_;}
			inline 	Net*               	getNet       () const{return net_;}
			inline	Cell*     	        getCell      () const{return ((type_ == External) ? static_cast<Cell*>(owner_) : NULL);}
					Cell*              	getOwnerCell () const;
			inline 	Instance*          	getInstance  () const{return ((type_ == Internal) ? static_cast<Instance*>(owner_) : NULL);}
			inline 	Direction          	getDirection () const{return direction_;}
			inline 	Point              	getPosition  () const{return node_.getPosition();}
			inline	Type             	getType      () const{return type_;}

			//Modificateurs
			void  setNet       ( Net* );
			void  setNet       ( const std::string& );
			inline void  setDirection ( Direction );
			void  setPosition  ( const Point& );
			void  setPosition  ( int x, int y );
				
			void toXml( std::ostream&);
		
		private :
			void*         owner_;
			std::string   name_;
			Direction     direction_;
			Type          type_;
			Net*          net_;
			NodeTerm      node_;
			
	};
	inline const std::string&            Term::getName      () const {return name_; }
}
#endif  // NETLIST_TERM_H
