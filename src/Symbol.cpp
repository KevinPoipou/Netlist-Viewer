// -*- explicit-buffer-name: "Symbol.cpp<M1-MOBJ/7>" -*-
#include "Symbol.h"
#include <fstream>
#include <cstdlib>
#include  "Term.h"
#include  "XmlUtil.h"
#include  "Shape.h"
#include  "Cell.h"

namespace Netlist {

    using namespace std;
	
	
    Symbol::Symbol(Cell* cell)
		: owner_(cell)
		, shapes_(){}
	
	Symbol::~Symbol(){
	}
	
	Cell* Symbol::getCell()const{
		return owner_;
	}
	
	Box Symbol::getBoundingBox()const{
		Box bb;
		for (vector<Shape*> ::const_iterator ishape = shapes_.begin(); ishape != shapes_.end(); ++ishape) {
			bb.merge((*ishape)-> getBoundingBox());
		}
		return bb;
	}
	
	Point Symbol::getTermPosition(Term* term) const{
		TermShape* termshape = getTermShape(term);
		if (termshape != NULL){
			return Point(termshape->getX(), termshape->getY());
		}
		cerr << "Retourne un point vide dans Symbol.cpp, Point Symbol::getTermPosition(Term* term) const" << endl;
		return Point(); //retourne un point vide
		
		// return term->getPosition();
	}
	
	TermShape* Symbol::getTermShape(Term* term) const{
		for (vector<Shape*>::const_iterator ishape = shapes_.begin(); ishape != shapes_.end(); ++ishape) {
			TermShape* ts = dynamic_cast<TermShape*>((*ishape));
			if (ts and (ts->getName() == term->getName())){
				return ts;
			}
		}
		cerr << "erreur dans symbol.cpp getTermShape" << endl;
		return NULL;
	}
	
	void Symbol::add(Shape* shape){
		shapes_.push_back(shape);
	}
	
	void Symbol::remove(Shape* shape){
        for (vector < Shape * > ::iterator ishape = shapes_.begin(); ishape != shapes_.end(); ++ishape) {
            if ( * ishape == shape) {
                shapes_.erase(ishape);
                break; // car pas besoin d'aller chercher plus loin
            }
		}
	}
	
	void Symbol::toXml(ostream& stream) const{
		stream << indent++ << "<symbol>\n";	
			vector<Shape*> shape = shapes_;
			for (vector<Shape*> ::const_iterator ishape = shapes_.begin(); ishape != shapes_.end(); ++ishape) {
				(*ishape)->toXml(stream);
			}
		stream << --indent << "</symbol>\n";
	}	

	Symbol* Symbol::fromXml(Cell* cell, xmlTextReaderPtr reader){
		
		Symbol* symbol = cell->getSymbol();
		const xmlChar * boxTag = xmlTextReaderConstString(reader, (const xmlChar * ) "box");
		const xmlChar * lineTag = xmlTextReaderConstString(reader, (const xmlChar * ) "line");
		const xmlChar * termTag = xmlTextReaderConstString(reader, (const xmlChar * ) "term");
		const xmlChar * arcTag = xmlTextReaderConstString(reader, (const xmlChar * ) "arc");
		const xmlChar * ellipseTag = xmlTextReaderConstString(reader, (const xmlChar * ) "ellipse");
		
		while (true) {
			int status = xmlTextReaderRead(reader);
			if (status != 1) {
				if (status != 0) {
					cerr << "[ERROR] Cell::fromXml(): Unexpected termination of the XML parser." << endl;
				}
				break;
			}

			switch (xmlTextReaderNodeType(reader)) {
			case XML_READER_TYPE_COMMENT:
			case XML_READER_TYPE_WHITESPACE:
			case XML_READER_TYPE_SIGNIFICANT_WHITESPACE:
				continue;
			}			
			
			const xmlChar * nodeName = xmlTextReaderConstLocalName(reader);
			if (nodeName == boxTag or nodeName == lineTag or nodeName == termTag or nodeName == arcTag or nodeName == ellipseTag){
				Shape::fromXml(symbol, reader);
				continue;
			}
		}
		return symbol;
	}
}
