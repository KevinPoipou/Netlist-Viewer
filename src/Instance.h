// -*- explicit-buffer-name: "Instance.h<M1-MOBJ/4-5>" -*-

#ifndef NETLIST_INSTANCE_H
#define NETLIST_INSTANCE_H
#include <iostream>
#include <string>
#include <vector>
#include  "Cell.h"
#include  "Term.h"

namespace Netlist{
	
	class Cell;
	class Net;
	class Term;
	
	class Instance{
		public :
			static Instance*	fromXml(Cell*, xmlTextReaderPtr);
		public :
			//Constructeurs & Destructeurs
			Instance	( Cell* owner,  Cell* model, const std::string& name);
			~Instance	();
			
			//Accesseurs 
			const std::string&        getName       () const;
			Cell*                     getMasterCell () const;
			Cell*                     getCell       () const;
			const std::vector<Term*>& getTerms      () const;
			Term*                     getTerm       ( const std::string& ) const;
			Point                     getPosition   () const;
			
			//Modificateurs
			bool  connect       ( const std::string& name, Net* );
			void  add           ( Term* );
			void  remove        ( Term* );
			void  setPosition   ( const Point& );
			void  setPosition   ( int x, int y );
			void  toXml(std::ostream&);
		private :
			Cell*               owner_;
			Cell*               masterCell_;
			std::string         name_;
			std::vector<Term*>  terms_;
			Point               position_;
		
		
	};

}
#endif  // NETLIST_NET_H
