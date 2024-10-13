// -*- explicit-buffer-name: "Instance.cpp<M1-MOBJ/4-5>" -*-
#include  <limits>
#include <libxml/xmlreader.h>
#include  "Instance.h"
#include  "XmlUtil.h"
#include  "Shape.h"



namespace Netlist {

	using namespace std;
	
	Instance::Instance(Cell* owner, Cell* model, const string& name) 
		: owner_(owner)
		, masterCell_(model)
		, name_(name)
		, terms_() //terms_ copie model mais il doit le dupliquer.
		, position_()
		{
			owner_->add(this);
			for(vector<Term*>::const_iterator iterm=model->getTerms().begin(); iterm != model->getTerms().end(); ++iterm){
				add(new Term(this, *iterm));
			}
		}

	Instance::~Instance(){
		owner_->remove(this);
	}
	
	const string& Instance::getName() const{
		return name_;
	}
	
	Cell* Instance::getMasterCell() const{
		return masterCell_;
	}
	
	Cell* Instance::getCell() const{
		return owner_;
	}
	
	const vector<Term*>& Instance::getTerms() const{
		return terms_;
	}
	
	Term* Instance::getTerm(const string& name) const{
		for ( vector<Term*>::const_iterator iterm=terms_.begin() ; iterm != terms_.end() ; ++iterm ) {
			if ((*iterm)->getName() == name)  return *iterm;
		}
    return NULL;
	}	
	
	Point Instance::getPosition() const{
		return position_;
	}
	
	bool Instance::connect(const string& name, Net* net){
		Term* term = getTerm( name );
		if (net == NULL){
		}
		
		if (term == NULL) return false;
		term->setNet( net );
		return true;
	}
	
	void Instance::add(Term* term){
		terms_.push_back(term);
	}
	void Instance::remove(Term* term){
		for ( vector<Term*>::iterator iterm=terms_.begin() ; iterm != terms_.end() ; ++iterm ) {
			if ((*iterm) == term){
				terms_.insert(iterm, static_cast<Term*>(NULL));
			}
		}
	}
	void Instance::setPosition(const Point& p){
		setPosition(p.getX(), p.getY()); //on lance la methode void Instance::setPosition(int x, int y)
	}
	
	void Instance::setPosition(int x, int y){ //A verifier
		position_.setX(x);
		position_.setY(y);
		
		for ( vector<Term*>::iterator iterm=terms_.begin() ; iterm != terms_.end() ; ++iterm ) {
			
			Shape* shape = getMasterCell()->getSymbol()->getTermShape(*iterm);
			TermShape* termshape = dynamic_cast<TermShape*>(shape);
			if(termshape){
				//(*iterm)->setPosition(termshape->getX()+x, termshape->getY()+y);
				(*iterm)->setPosition(getPosition().translate(termshape->getX(), termshape->getY()));
			}
			
		}
	}

// toXml :
	void Instance::toXml ( ostream& stream){
		stream << indent << "<instance name=\"" << name_ << "\" mastercell=\"" << masterCell_->getName() << "\" x=\"" << position_.getX() <<"\" y=\"" << position_.getY() << "\"/>\n";
	}
	
// FromXml :
	Instance* Instance::fromXml(Cell* cell, xmlTextReaderPtr reader){

	  	Instance* inst   = NULL;

			string instName = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"name" ) );
			string instMasterCell = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"mastercell"));
			string termGetX = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"x"));
			string termGetY = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"y"));
			
			Cell* mastercell = cell->find(instMasterCell);	//Recherche de ma mastercell
			
			if (not instName.empty() and not instMasterCell.empty()) {
		        inst = new Instance(cell ,mastercell, instName);
				inst->setPosition(atoi(termGetX.c_str()), atoi(termGetY.c_str())); //fixe la position pour l'instance
            }
		
		return inst;
	}
}
