// -*- explicit-buffer-name: "Shape.cpp<M1-MOBJ/7>" -*-
#include "XmlUtil.h"
#include "Shape.h"
#include "Box.h"
#include "Symbol.h"
#include "Indentation.h"
#include "Term.h"
#include "Cell.h"

namespace Netlist {
	using namespace std;
	
	Shape::Shape(Symbol* symbol) : owner_(symbol){
		owner_->add(this);
	}
	
	Shape::~Shape(){
		owner_->remove(this);
	}
	
	Symbol* Shape::getSymbol()const {
		return owner_;
	}
	
	Box Shape::getBoundingBox()const{
		return Box();
	}
	
	// void Shape::toXml(ostream & stream) const {
		// cout << " je pu " << endl;
	// }
	
    Shape* Shape::fromXml ( Symbol* owner, xmlTextReaderPtr reader )
    {
    // Factory-like method.
      const xmlChar* boxTag = xmlTextReaderConstString( reader, (const xmlChar*)"box" );
      const xmlChar* ellipseTag = xmlTextReaderConstString( reader, (const xmlChar*)"ellipse" );
      const xmlChar* arcTag = xmlTextReaderConstString( reader, (const xmlChar*)"arc" );
      const xmlChar* lineTag = xmlTextReaderConstString( reader, (const xmlChar*)"line" );
      const xmlChar* termTag = xmlTextReaderConstString( reader, (const xmlChar*)"term" );
      const xmlChar* nodeName = xmlTextReaderConstLocalName( reader );

      Shape* shape = NULL;
      if (boxTag == nodeName)
        shape = BoxShape::fromXml( owner, reader );
      if (ellipseTag == nodeName)
        shape = EllipseShape::fromXml( owner, reader );
      if (arcTag == nodeName)
        shape = ArcShape::fromXml( owner, reader );
      if (lineTag == nodeName)
        shape = LineShape::fromXml( owner, reader );
      if (termTag == nodeName)
        shape = TermShape::fromXml( owner, reader );

      if (shape == NULL)
        cerr << "[ERROR] Unknown or misplaced tag <" << nodeName << "> (line:"
             << xmlTextReaderGetParserLineNumber(reader) << ")." << endl;

      return shape;
    }


	
//BoxShape

	BoxShape::BoxShape(Symbol* symbol, const Box& box) : Shape(symbol), box_(box){}
	
	BoxShape::~BoxShape(){}
	
	Box BoxShape::getBoundingBox()const{
		return box_;
	}

	
	//toXml
	void BoxShape::toXml(ostream & stream) const {
		stream << indent << "<box x1=\"" << box_.getX1() << "\" y1=\"" << box_.getY1() << "\" x2=\"" << box_.getX2() << "\" y2=\"" << box_.getY2() << "\"/>\n";
	}

	//fromXml
	Shape* BoxShape::fromXml(Symbol* symbol, xmlTextReaderPtr reader){
		Shape* shape = NULL;
		string boxshapeX1 = xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar * ) "x1"));
		string boxshapeY1 = xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar * ) "y1"));
		string boxshapeX2 = xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar * ) "x2"));
		string boxshapeY2 = xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar * ) "y2"));
		
		int x1 = atoi(boxshapeX1.c_str());
		int x2 = atoi(boxshapeX2.c_str());
		int y1 = atoi(boxshapeY1.c_str());
		int y2 = atoi(boxshapeY2.c_str());

		if(not boxshapeX1.empty() and not boxshapeY1.empty()){
			shape = new BoxShape(symbol, Box(x1, y1, x2, y2));
		}
		return shape;
	}
	
//LineShape
	LineShape::LineShape(Symbol* symbol, int x1, int y1, int x2, int y2) : Shape(symbol), x1_(x1), y1_(y1), x2_(x2), y2_(y2){}
	
	LineShape::~LineShape(){}
	
	Box LineShape::getBoundingBox()const{
		return Box(x1_, y1_, x2_, y2_);
	}

	//toXml
	void LineShape::toXml(ostream & stream) const {
		stream << indent << "<line x1=\"" << x1_ << "\" y1=\"" << y1_ << "\" x2=\"" << x2_ << "\" y2=\"" << y2_ << "\"/>\n";
	}

	//fromXml
	Shape* LineShape::fromXml(Symbol* symbol, xmlTextReaderPtr reader){
		Shape* shape = NULL;
		string lineshapeX1 = xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar * ) "x1"));
		string lineshapeY1 = xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar * ) "y1"));
		string lineshapeX2 = xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar * ) "x2"));
		string lineshapeY2 = xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar * ) "y2"));
		
		int x1 = atoi(lineshapeX1.c_str());
		int x2 = atoi(lineshapeX2.c_str());
		int y1 = atoi(lineshapeY1.c_str());
		int y2 = atoi(lineshapeY2.c_str());

		
		if(not lineshapeX1.empty() and not lineshapeY1.empty()){
			shape = new LineShape(symbol, x1, y1, x2, y2);
			// symbol->add(shape);
			
		}
		return shape;
	}

//TermShape
	TermShape::TermShape(Symbol* symbol, Term* term, int x, int y, string align) : Shape(symbol), term_(term), x_(x), y_(y), align_(align) {} //A VERRIFIER SI CEST PAS STRING NAME AU LIEU DE TERM
	TermShape::~TermShape(){}
	
	Box TermShape::getBoundingBox()const{
		return Box(x_, y_, x_, y_);
	}
	
	//toXml
	void TermShape::toXml(ostream & stream) const {
		stream << indent << "<term name=\"" << term_->getName() << "\" x1=\"" << x_ << "\" y1=\"" << y_ << "\" align=\"" << align_ << "\"/>\n";
	}

	TermShape::NameAlign TermShape::toNameAlign(string string){
		if (string == "top_left") return TopLeft;
		if (string == "top_right") return TopRight;
		if (string == "bottom_left") return BottomLeft;
		if (string == "bottom_right") return BottomRight;
		cerr << "Erreur dans Shape.cpp, string toNameAlign(), Align pas trouve \n return TopLeft par defaut" << endl;
		return TopLeft;

	}

	string TermShape::toString(NameAlign name){
		if (name == 1) return "TopLeft";
		if (name == 2) return "TopRight";
		if (name == 3) return "BottomLeft";
		if (name == 4) return "BottomRight";
		cerr << "Erreur dans Shape.cpp, string toString(), Align pas trouve \n return TopLeft par defaut" << endl;
		return "TopLeft";
	}

	//fromXml
	Shape* TermShape::fromXml(Symbol* symbol, xmlTextReaderPtr reader){
		Shape* shape = NULL;

		string termshapeName = xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar * ) "name"));
		string termshapeX1 = xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar * ) "x1"));
		string termshapeY1 = xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar * ) "y1"));
		string termshapeAlign = xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar * ) "align"));
		
		string align = toString(toNameAlign(termshapeAlign));

		int x1 = atoi(termshapeX1.c_str());
		int y1 = atoi(termshapeY1.c_str());
		Cell* cell = symbol->getCell();
		Term* term = cell->getTerm(termshapeName);
		shape = new TermShape(symbol, term, x1, y1, align);
		
		return shape;
	}
	
//EllipseShape
	EllipseShape::EllipseShape(Symbol* symbol, const Box& box) : Shape(symbol), box_(box){}
	
	EllipseShape::~EllipseShape(){}
	
	Box EllipseShape::getBoundingBox()const{
		return box_;
	}
	
	void EllipseShape::toXml(ostream & stream) const {
		stream << indent << "<ellipse x1=\"" << box_.getX1() << "\" y1=\"" << box_.getY1() << "\" x2=\"" << box_.getX2() << "\" y2=\"" << box_.getY2() << "\"/>\n";
	}
	
	//fromXml
	Shape* EllipseShape::fromXml(Symbol* symbol, xmlTextReaderPtr reader){
		Shape* shape = NULL;

		string arcshapeX1 = xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar * ) "x1"));
		string arcshapeY1 = xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar * ) "y1"));
		string arcshapeX2 = xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar * ) "x2"));
		string arcshapeY2 = xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar * ) "y2"));

		int x1 = atoi(arcshapeX1.c_str());
		int y1 = atoi(arcshapeY1.c_str());
		int x2 = atoi(arcshapeX2.c_str());
		int y2 = atoi(arcshapeY2.c_str());
		
		shape = new EllipseShape(symbol, Box(x1, y1, x2, y2));
		
		return shape;
	}
	
//ArcShape
	ArcShape::ArcShape(Symbol* symbol, const Box& box, int start, int span) : Shape(symbol), box_(box), start_(start), span_(span){}
	
	ArcShape::~ArcShape(){}
	
	Box ArcShape::getBoundingBox()const{
		return box_;
	}
	
	void ArcShape::toXml(ostream & stream) const {
		stream << indent << "<arc x1=\"" << box_.getX1() << "\" y1=\"" << box_.getY1() << "\" x2=\"" << box_.getX2() << "\" y2=\"" << box_.getY2() << "\" start=\"" << start_ << "\" span=\""<< span_ <<"\"/>\n";
	}
	
	//fromXml
	Shape* ArcShape::fromXml(Symbol* symbol, xmlTextReaderPtr reader){
		Shape* shape = NULL;

		string arcshapeX1 = xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar * ) "x1"));
		string arcshapeY1 = xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar * ) "y1"));
		string arcshapeX2 = xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar * ) "x2"));
		string arcshapeY2 = xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar * ) "y2"));
		string arcshapeStart = xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar * ) "start"));
		string arcshapeSpan = xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar * ) "span"));
		
		int x1 = atoi(arcshapeX1.c_str());
		int y1 = atoi(arcshapeY1.c_str());
		int x2 = atoi(arcshapeX2.c_str());
		int y2 = atoi(arcshapeY2.c_str());
		int start = atoi(arcshapeStart.c_str());
		int span = atoi(arcshapeSpan.c_str());
		
		shape = new ArcShape(symbol, Box(x1, y1, x2, y2), start, span);
		
		return shape;
	}
}
