// -*- explicit-buffer-name: "Term.cpp<M1-MOBJ/4-5>" -*-
#include  <limits>

#include <libxml/xmlreader.h>
#include  "Node.h"
#include  "Term.h"
#include  "Net.h"
#include  "Instance.h"
#include  "Cell.h"
#include  "XmlUtil.h"
#include <fstream>
#include <cstdlib>


namespace Netlist{
	
	using namespace std;
	
	string Term::toString(Type type){
		if (type == 1) return "Internal";
		if (type == 2) return "External";
		else return "ERROR";
	}
	string Term::toString(Direction direction){
		if (direction == 1) return "In";
		if (direction == 2) return "Out";
		if (direction == 3) return "Inout";
		if (direction == 4) return "Tristate";
		if (direction == 5) return "Transcv";
		if (direction == 6) return "Unknown";
		else return "ERROR";
	}
	Term::Direction Term::toDirection (string string){
		if (string == "In") return In;
		if (string == "Out") return Out;
		if (string == "Inout") return Inout;
		if (string == "Tristate") return Tristate;
		if (string == "Transcv") return Transcv;
		else return Unknown;
	}

	
	Term::Term(Cell* cell, const string& name, Direction direction) 
		: owner_(cell) 
		, name_(name)
		, direction_(direction)
		, type_(External)
		, net_(NULL)
		, node_(this, Node::noid)
	{
		static_cast<Cell*>(owner_)->add(this);

	}
	
	Term::Term(Instance* owner, const Term* modelTerm)
		: owner_(owner)
		, name_(modelTerm->getName())
		, direction_(modelTerm->getDirection())
		, type_(Internal)
		, net_(NULL)
		, node_(this, Node::noid) 
	{
		static_cast<Instance*>(owner_)->add(this);
	}
	
	Term::~Term(){
		if(type_ == External){ //C'est une Cell
			static_cast<Cell*>(owner_)->remove(this);
		}
		if(type_ == Internal){ //C'est une instance
			static_cast<Instance*>(owner_)->remove(this);
		}
		
		net_->remove(&node_);
		
	}
	
	Cell* Term::getOwnerCell() const {
		if (type_ == External) return static_cast<Cell*>(owner_);
		else return static_cast<Instance*>(owner_)->getCell();
	}
	
	void Term::setNet(Net* net){
		if (net != NULL){
			net_ = net;
			net_->add(&node_);
		}else{
			cerr << "erreur dans Term.cpp term::setNet(net*)" << endl;
		}
	}
	
	void Term::setNet(const string& netName) {
		net_= getOwnerCell()->getNet(netName);
		net_->add(&node_); // a verif
		
	}
	
	void Term::setPosition(const Point& point){
		node_.setPosition(point);
	}
	
	void Term::setPosition(int x, int y){
		node_.setPosition(x,y);
	}

// toXml :
	void  Term::toXml ( ostream& stream){
		// stream << indent << "<term name=\"" << name_ << "\" direction=\"" << toString(direction_)<< "\"/>\n";
		stream << indent << "<term name=\"" << name_ << "\" direction=\"" << toString(direction_)<< "\" x=\"" << (node_.getPosition()).getX() << "\" y=\"" << (node_.getPosition()).getY() << "\"/>\n";
	}
	
// FromXml :
	Term*	Term::fromXml(Cell* cell, xmlTextReaderPtr reader){

	  	Term* term   = NULL;

			string termName = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"name" ) );
			string termDirection = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"direction"));
			string termGetX = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"x"));
			string termGetY = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"y"));

			Term::Direction dir = Term::toDirection(termDirection);
			
			if (not termName.empty() and not termDirection.empty()) {
		        term = new Term(cell ,termName, dir);
				term->setPosition(atoi(termGetX.c_str()), atoi(termGetY.c_str()));
            }
		return term;
	}
}
