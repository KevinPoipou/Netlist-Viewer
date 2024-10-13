// -*- explicit-buffer-name: "CellWidget.h<M1-MOBJ/8-10>" -*-

#ifndef NETLIST_CELL_WIDGET_H
#define NETLIST_CELL_WIDGET_H

#include <QWidget>
#include <QPixmap>
//#include <QPainter>
#include <QRect>
#include <QPoint>
class QPainter;
#include "Box.h"
#include "Color.h"
#include <QColorDialog>

namespace Netlist {

  class Cell;
  class NodeTerm;

class CellWidget: public QWidget {
    Q_OBJECT;
    public:
						CellWidget(QWidget * parent = NULL);
		virtual			~CellWidget();
				void 	setCell(Cell * );
		inline 	Cell * 	getCell() const;
		
		inline 	int  xToScreenX ( int x ) const{return x - viewport_.getX1();} // Xscreen = Xschema     - X1_viewport
		inline 	int  yToScreenY ( int y ) const{return viewport_.getY2() - y;} // Yscreen = Y2_viewport - Yschema
		
		inline 	int  screenXToX ( int x ) const{return x + viewport_.getX1();} // Xschema = Xscreen     + X1_viewport
		inline 	int  screenYToY ( int y ) const{return viewport_.getY2() - y;} // Yschema = Y2_viewport - Yscreen
		inline 	QRect 	boxToScreenRect(const Box& box) const;

		inline 	QPoint	pointToScreenPoint(const Point & point) const;
		inline 	Box 	screenRectToBox(const QRect & ) const;
		inline 	Point	screenPointToPoint(const QPoint & ) const;
		
		virtual QSize 	minimumSizeHint() const;
		virtual	void 	resizeEvent(QResizeEvent * );
		void	query(unsigned int, QPainter&);	
			
		//Ajout pour couleur
		QColor	getColor(const QString& name);
		
		void	setColor(QColor, QString); // ajout
		
		bool	findCenter();
		void	zoomIn();
		void	zoomOut();
		void	zoomDefault();
		int		getZoom();
		
	public slots:
				
				void	goRight();
				void	goLeft();
				void	goUp();
				void	goDown();
				
				
				
				
    protected:
        virtual void 	paintEvent(QPaintEvent * );
		virtual void 	keyPressEvent(QKeyEvent * );
	
    private:
        Cell * 	cell_;
		Box 	viewport_;
		
		QColor	linesColor_;
		QColor	linesNodesColor_;
		QColor	boxShapeColor_;
		QColor	lineShapeColor_;
		QColor	termShapeColor_;
		QColor	termShapeNameColor_;
		QColor	ellipseShapeColor_;
		QColor	arcShapeColor_;
		QColor	termColor_;
		QColor	termNameColor_;
		QColor	instanceNameColor_; //Ajout
		QColor	transistorPColor_;
		QColor	transistorNColor_;
		QColor	vddColor_;
		QColor	gndColor_;
		QColor	backgroundColor_;
		QColor	titleColor_;
		QColor	titleBackgroundColor_;
		QColor	titleFrameColor_;
		
		qreal	zoom_;
};



  	inline Cell* CellWidget::getCell () const { return cell_; }
  	
	inline 	QRect CellWidget::boxToScreenRect(const Box& box) const{	
		return QRect(xToScreenX(box.getX1())
					, yToScreenY(box.getY1())
					, box.getX2() - box.getX1()
					, box.getY1() - box.getY2());
	}
	inline 	QPoint CellWidget::pointToScreenPoint(const Point & point) const{
		return QPoint(xToScreenX(point.getX())
					, yToScreenY(point.getY()));
	}

}  // Netlist namespace.

#endif  // NETLIST_CELL_WIDGET_H
