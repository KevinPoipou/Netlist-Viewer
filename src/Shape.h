// -*- explicit-buffer-name: "Shape.h<M1-MOBJ/7>" -*-
#include <string>
#include <libxml/xmlreader.h>
#include "Box.h"
#include "Symbol.h"
#include "Indentation.h"
#include "Term.h"

/*
Pour que la classe ne soit pas instantiable, il faut qu’elle soit abstraite. C’est à dire qu’elle contienne,
au moins une méthode virtuelle pure. Ce sera bien le cas, car on ne peut implenter à ce niveau les
méthodes clone(), getType(), getString() et toXml().
--
Cette classe ne contenant pas d’attributs alloués dynamiquement, ou de pointeurs vers des objets
tiers, le constructeur par copie synthétisé par défaut est suffisant.
Le constructeur par copie, n’est pas (et ne peut pas être) une méthode virtuelle. Donc pour copier
des objets de type Parameter, il faudrait retrouver le type exact avant de pouvoir faire la copie (par
exemple avec un dynamic_cast<>). La clone() permet d’encapsuler le constructeur par copie
dans une méthode virtuelle, ce qui permet de dupliquer de façon transparente un objet au travers
d’un pointeur de type Parameter.
*/

namespace Netlist {
	class Box;
	class Symbol;
	class Term;
	
	class Shape{
		public :
			//enum 	Direction { In=1, Out=2, Inout=3, Tristate=4, Transcv=5, Unknown=6 };
		public :
			//Constructeur
			Shape(Symbol* symbol);
			virtual ~Shape();
			virtual Box			getBoundingBox()const = 0;
					Symbol*		getSymbol()const;
			virtual	void		toXml(std::ostream&) const = 0;
			static 	Shape*		fromXml(Symbol* ,xmlTextReaderPtr);
			
			
		private :
			Symbol*		owner_;
	};
	
	class BoxShape : public Shape{
		public :
			//CTOR
			BoxShape(Symbol* symbol, const Box& box);
			virtual ~BoxShape();
			
			Box					getBoundingBox()const;
			

			
			virtual	void		toXml(std::ostream&) const;
			static	Shape*		fromXml(Symbol*, xmlTextReaderPtr);
		
		private :
			Box			box_;

	};
	
	class LineShape : public Shape{
		public :
			LineShape(Symbol*, int x1, int y1, int x2, int y2);
			virtual ~LineShape();
			
			Box				getBoundingBox()const;
			
			inline 	int 		getX1() const{return x1_;} //ajout tme9
			inline 	int 		getY1() const{return y1_;}
			inline 	int 		getX2() const{return x2_;}
			inline 	int 		getY2() const{return y2_;}
			virtual	void		toXml(std::ostream&) const;
			static	Shape*		fromXml(Symbol*, xmlTextReaderPtr);
			
		private :
		int x1_, y1_, x2_, y2_;
		
	};
	
	class TermShape : public Shape{
		public :
			enum 	NameAlign { TopLeft=1, TopRight=2, BottomLeft=3, BottomRight=4 };
			static 	NameAlign	toNameAlign(std::string string);
			static 	std::string	toString(NameAlign);


					TermShape(Symbol*, Term*, int x, int y, std::string);
			virtual ~TermShape();
			
					Box					getBoundingBox()const;
			inline 	std::string			getName()const{return term_->getName();}
			inline	int					getX()const{return x_;}
			inline	int					getY()const{return y_;}
			inline	std::string			getAlign()const{return align_;} //ajout TME9
			
			virtual	void				toXml(std::ostream&) const;
			static	Shape*				fromXml(Symbol*, xmlTextReaderPtr);

		private :
		Term*	term_;
		int		x_, y_;
		std::string align_;
	};
	
	class EllipseShape : public Shape{
		public :
					EllipseShape(Symbol*, const Box& box);
			virtual	~EllipseShape();
			
					Box					getBoundingBox()const;
			
			virtual	void				toXml(std::ostream&) const;
			static	Shape*		fromXml(Symbol*, xmlTextReaderPtr);
		private :
			Box			box_;
	};
	
	class ArcShape : public Shape{
		public :
					ArcShape(Symbol*, const Box& box, int, int);
			virtual	~ArcShape();
			
					Box					getBoundingBox()const;
			
			virtual	void				toXml(std::ostream&) const;
			static	Shape*		fromXml(Symbol*, xmlTextReaderPtr);
			
			inline	int			getStart(){return start_;}
			inline 	int			getSpan(){return span_;}
		
		private :
			Box			box_;
			int			start_, span_;
	};
}
