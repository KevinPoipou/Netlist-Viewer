// -*- explicit-buffer-name: "Net.h<M1-MOBJ/7>" -*-

#ifndef NETLIST_NET_H
#define NETLIST_NET_H
#include <iostream>
#include <vector>
#include "Indentation.h"
#include  "Term.h"
#include  "Line.h"
#include  "Node.h"
#include  "Instance.h"
#include  "Cell.h"
#include  "XmlUtil.h"

namespace Netlist{
	// class Node;
	// class Cell;
	// class Term;
	// class Line;

	class Net{
	
		public :
			static	Net* fromXml(Cell*, xmlTextReaderPtr);
			//Constructeurs & Destructeurs
			Net     ( Cell*, const std::string&, Term::Type );
			~Net     ();
			
			//Accesseurs 
			Cell*                     	getCell       () const;
			const std::string&       	getName       () const;
			unsigned int              	getId         () const;
			Term::Type                  getType       () const;
			const std::vector<Node*>& 	getNodes      () const;
			Node*						getNode		  (size_t) const;	
			size_t                    	getFreeNodeId () const;
			
			//Modificateurs
			void  add    ( Node* n);
			bool  remove ( Node* );
			void  toXml( std::ostream&);
			
			//Methode de Line
			inline const std::vector<Line*>& getLines () const { return lines_; }
			void  add ( Line* line );
			bool  remove ( Line* line );
			
		private :
			Cell*               owner_;
			std::string         name_;
			unsigned int        id_;
			Term::Type          type_;
			std::vector<Node*>  nodes_;
			std::vector<Line*>	lines_;
	};

}
#endif  // NETLIST_NET_H
