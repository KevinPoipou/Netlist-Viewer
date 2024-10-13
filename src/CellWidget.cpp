// -*- explicit-buffer-name: "CellWidget.cpp<M1-MOBJ/8-10>" -*-
#include  <QCheckBox>
#include  <QResizeEvent>
#include  <QMainWindow>
#include  <QPainter>
#include  <QPen>
#include  <QBrush>
#include  <QFont>
#include  <QApplication>
#include  <QHBoxLayout>
#include  <QVBoxLayout>
#include  <QGridLayout>
#include  <QtCore>
#include  "CellWidget.h"
#include  "Term.h"
#include  "Instance.h"
#include  "Symbol.h"
#include  "Shape.h"
#include  "Cell.h"
#include  "Line.h"
#include  "Node.h"
#include  "Net.h"

namespace Netlist {

  using namespace std;


  ostream& operator<< ( ostream& o, const QRect& rect )
  {
    o << "<QRect x:" << rect.x()
      <<       " y:" << rect.y()
      <<       " w:" << rect.width()
      <<       " h:" << rect.height() << ">";
    return o;
  }


  ostream& operator<< ( ostream& o, const QPoint& p )
  { o << "<QRect x:" << p.x() << " y:" << p.y() << ">"; return o; }



  CellWidget::CellWidget ( QWidget* parent )
    : QWidget(parent)
    , cell_  (NULL)
	, viewport_ (Box(0,0,500,500))
	, linesColor_(Qt::cyan), linesNodesColor_(Qt::cyan)
	, boxShapeColor_(Qt::darkGreen), lineShapeColor_(Qt::darkGreen)
	, termShapeColor_(Qt::darkRed), termShapeNameColor_(Qt::darkRed)
	, ellipseShapeColor_(Qt::darkGreen) , arcShapeColor_(Qt::darkGreen)
	, termColor_(Qt::darkRed) , termNameColor_(Qt::darkRed), instanceNameColor_(Qt::darkGreen)
	, transistorPColor_(Qt::darkYellow) , transistorNColor_(Qt::magenta)
	, vddColor_(Qt::blue), gndColor_(Qt::darkGray)
	, backgroundColor_(Qt::black), titleColor_(Qt::white)
	, titleBackgroundColor_(Qt::black), titleFrameColor_(Qt::white), zoom_(1.0)
  {
		setAttribute    ( Qt::WA_OpaquePaintEvent );
		setAttribute    ( Qt::WA_NoSystemBackground );
		setAttribute    ( Qt::WA_StaticContents ); // pas dans le cours ?
		setSizePolicy   ( QSizePolicy::Expanding, QSizePolicy::Expanding );
		setFocusPolicy  ( Qt::StrongFocus );
		setMouseTracking( true );	
  }


  CellWidget::~CellWidget ()
  { }


  void  CellWidget::setCell ( Cell* cell )
  {
    cell_ = cell;
    repaint();
  }


  QSize  CellWidget::minimumSizeHint () const
  { return QSize(500,500); }


	void  CellWidget::resizeEvent ( QResizeEvent* event ){
		repaint(); 
		
	}


  void  CellWidget::paintEvent ( QPaintEvent* event )
  {
    QFont  bigFont = QFont( "URW Bookman L", 36 );
    QString cellName = "NULL";
    QPainter painter(this);
    painter.setBackground( QBrush( backgroundColor_ ) );
    painter.eraseRect    ( QRect( QPoint(0,0), size() ) );

    if (cell_) cellName = cell_->getName().c_str();

	query(0,painter);
	painter.setBrush(titleBackgroundColor_);	
	painter.setFont      ( bigFont );
	
    int frameWidth  = 460;
    int frameHeight = 100;
    QRect nameRect ( 0
                   , 0
                   , frameWidth
                   , frameHeight
                   );   

	painter.setPen(titleFrameColor_);
    painter.drawRect( nameRect );
	painter.setPen(titleColor_);
    painter.drawText( nameRect, Qt::AlignCenter, cellName );
	
	/*QFontMetrics fontMetrics(painter.font());
    int textWidth = fontMetrics.width("test");
    int textHeight = fontMetrics.height();
	
	painter.drawText( width()-textWidth, height()- textHeight, "test" );*/
  }

	void CellWidget::keyPressEvent(QKeyEvent * event) {
		event -> ignore();
		if (event -> modifiers() & (Qt::ControlModifier | Qt::ShiftModifier))
			return;
		switch (event -> key()) {
		case Qt::Key_Up:
			goUp();
			break;
		case Qt::Key_Down:
			goDown();
			break;
		case Qt::Key_Left:
			goLeft();
			break;
		case Qt::Key_Right:
			goRight();
			break;
		default:
			return;
		}
		event -> accept();
	}

	void CellWidget :: goRight () {
		viewport_.translate(Point(20,0));
		repaint ();
	}

	void CellWidget :: goLeft () {
		viewport_.translate(Point(-20,0));
		repaint ();
	}

	void CellWidget :: goUp () {
		viewport_.translate(Point(0,20));
		repaint ();
	}

	void CellWidget :: goDown () {
		viewport_.translate(Point(0,-20));
		repaint ();
	}

	void CellWidget::query(unsigned int flags, QPainter& painter) {
		QFont  font = QFont( "URW Bookman L", 12 );
        if(not cell_) return;
        //QCheckBox* cb_box = new QCheckBox("test", this);
        QPen pen;
		pen.setWidth(2);
		//painter.setPen(pen);
		
		const vector<Instance*>& instances = cell_->getInstances();
		const vector<Net*>& nets = cell_->getNets();
		const vector<Term*>& terms = cell_->getTerms();
		
	
		painter.save();
		painter.setWorldMatrixEnabled(true);
		painter.setWorldMatrix(QMatrix(zoom_, 0, 0, zoom_, 0, 0), true);

		if (instances.size() != 0){
			for (size_t i = 0; i < instances.size(); ++i){
				Point instPos = instances[i]->getPosition();
				const Symbol* symbol = instances[i]->getMasterCell()->getSymbol();
				const vector <Shape*>& shapes = symbol->getShapes();
				

				for(size_t j = 0; j < shapes.size(); ++j){
					BoxShape* boxshape = dynamic_cast <BoxShape*>(shapes[j]);
					LineShape* lineshape = dynamic_cast <LineShape*>(shapes[j]);
					TermShape* termshape = dynamic_cast <TermShape*>(shapes[j]);
					EllipseShape* ellipseshape = dynamic_cast <EllipseShape*>(shapes[j]);
					ArcShape* arcshape = dynamic_cast <ArcShape*>(shapes[j]);
					string instname = instances[i] -> getMasterCell()-> getName();
						
					if (boxshape) { //Si c'est une boxshape j'affiche une box
					Box box = boxshape->getBoundingBox();
					QRect rect = boxToScreenRect(box.translate(instPos));
					pen.setWidth(3);
					pen.setColor(boxShapeColor_);
					painter.setPen(pen);
					painter.drawRect (rect);
					}
					pen.setWidth(3);
					pen.setColor(Qt::darkGreen);
					
					
					if (instname == "TransistorP"){
						pen.setColor(transistorPColor_);
					}
					if (instname == "TransistorN"){
						pen.setColor(transistorNColor_);
					}
					
					if (instname == "vdd"){
						pen.setColor(vddColor_);
						
					}
					if (instname == "gnd"){
						pen.setColor(gndColor_);
					}
					painter.setPen(pen);
					
					if(lineshape){
						if(instname != "TransistorP" and instname != "TransistorN" and instname != "vdd" and instname != "gnd"){
							pen.setColor(lineShapeColor_);
							painter.setPen(pen);
						}
						int X1 = lineshape->getX1();
						int X2 = lineshape->getX2();
						int Y1 = lineshape->getY1();
						int Y2 = lineshape->getY2();
						
						Point p1 = Point(X1,Y1);
						Point p2 = Point(X2,Y2);

						QPoint qp1 = pointToScreenPoint(p1.translate(instPos));
						QPoint qp2 = pointToScreenPoint(p2.translate(instPos));
						
						painter.drawLine(qp1, qp2);
						//painter.restore();
					}
					
					
					if(ellipseshape){
						if(instname != "TransistorP" and instname != "TransistorN" and instname != "vdd" and instname != "gnd"){
							pen.setColor(ellipseShapeColor_);
							painter.setPen(pen);
						}
						painter.setPen(pen);
						Box box = ellipseshape->getBoundingBox();
						QRect rect = boxToScreenRect(box.translate(instPos));
						painter.drawEllipse(rect);
					}
					
					
					if(arcshape){
						if(instname != "TransistorP" and instname != "TransistorN" and instname != "vdd" and instname != "gnd"){
							pen.setColor(arcShapeColor_);
							painter.setPen(pen);
						}
						Box box = arcshape->getBoundingBox();
						//cout << "x1 : " << box.getX1() << " y1 : " << box.getY1();
						QRect rect = boxToScreenRect(box.translate(instPos));
						int start = arcshape->getStart();
						int span = arcshape->getSpan();
						//cout << "start : " << start << " span : " << span << endl;
						painter.drawArc(rect, start*16, span*16); //*16 car Qt documentation
					}
					if(termshape){
						Box box = termshape->getBoundingBox();
						QRect rect = boxToScreenRect(box.translate(instPos));
						
						int hl = 7; //Hauteur et largeur du carré
						string align = termshape->getAlign();
						string name = termshape->getName();
						painter.setFont(font);
						
						pen.setColor(termShapeNameColor_);
						painter.setPen(pen);
						if (align == "TopLeft"){
							painter.fillRect(rect.x(), rect.y()-3, hl, hl, termShapeColor_);
							painter.drawText(rect.x()-15, rect.y()-hl, QString::fromStdString(name));
						}else if (align == "TopRight"){
							painter.fillRect(rect.x()-hl, rect.y()-3, hl, hl, termShapeColor_);
							painter.drawText(rect.x()+hl, rect.y()-hl, QString::fromStdString(name));
						}else if (align == "BottomLeft"){
							painter.fillRect(rect.x(), rect.y()-3, hl, hl, termShapeColor_);
							painter.drawText(rect.x()-15, rect.y()+hl, QString::fromStdString(name));
						}else if (align == "BottomRight"){
							painter.fillRect(rect.x()-hl, rect.y()-3, hl, hl, termShapeColor_);
							painter.drawText(rect.x()+hl, rect.y()+hl, QString::fromStdString(name));
						}
					}
					
				}
				pen.setColor(instanceNameColor_);
				painter.setPen(pen);
				painter.drawText( xToScreenX(instPos.getX()+5) , yToScreenY(instPos.getY()-20) , QString::fromStdString(instances[i]->getName()));
			
			}

		}
		
		for (size_t i = 0; i < nets.size(); ++i){
			const vector<Line*>& lines = nets[i]->getLines();
			for (size_t k = 0; k < lines.size() ; ++k){
				
				Point source = lines[k]->getSourcePosition();
				Point target = lines[k]->getTargetPosition();
				
				int source_x = source.getX();
				int source_y = source.getY();
				int target_x = target.getX();
				int target_y = target.getY();
				

				//cout << "sources : [" << source_x << ";" << source_y << "]\n"<<"target :  [" << target_x << ";" << target_y << "]\n"<<endl;
				QPoint qp3(xToScreenX(source_x), yToScreenY(source_y));
				QPoint qp4(xToScreenX(target_x), yToScreenY(target_y));
				

				QLine line(qp3,qp4);
			
				pen.setWidth(1);
				pen.setColor(linesColor_);
				painter.setPen(pen);
				painter.drawLine(line);
				
				if(lines[k]->getSource()->getDegree() > 2){ // Si la lignes a + de 2 connexion faut dessiner un point
					
					int pointSize = 8; // Taille du point
					pen.setColor(linesNodesColor_);
					painter.setBrush(linesNodesColor_);
					painter.setPen(pen);
					QRect rect(qp3.x() - pointSize/2, qp3.y() - pointSize/2, pointSize, pointSize);
					painter.drawEllipse(rect);
				
				}
				
			}
		}
		for (size_t i = 0;  i < terms.size() ; ++i){
	
			Point point = terms[i]->getPosition();
			int x = xToScreenX(point.getX());
			int y = yToScreenY(point.getY());
			
			QPoint qp1;
			QPoint qp2;
			QPoint qp3;
			QPoint qp4;
			QPoint qp5;
		   	QPolygon polygon;
			string name = terms[i]->getName();

			pen.setWidth(3);
			pen.setColor(termColor_);
			painter.setPen(pen);
			painter.setBrush(termColor_);
			if(terms[i]->getDirection() == 1){
				qp1 = QPoint(x-20,y-10);
				qp2 = QPoint(x-10,y-10);
				qp3 = QPoint(x,y);
				qp4 = QPoint(x-10,y+10);
				qp5 = QPoint(x-20,y+10);
				
				polygon << qp1 << qp2 << qp3 << qp4 <<qp5;
				painter.setFont(font);
				painter.drawPolygon(polygon);
				
				pen.setColor(termNameColor_);
				painter.setPen(pen);
				painter.drawText(qp5.x()-5, qp5.y()+16, QString::fromStdString(name));
			}else{
				pen.setColor(termColor_);
				painter.setPen(pen);
				qp1 = QPoint(x+20,y-10);
				qp2 = QPoint(x+10,y-10);
				qp3 = QPoint(x,y);
				qp4 = QPoint(x+10,y+10);
				qp5 = QPoint(x+20,y+10);
				polygon << qp1 << qp2 << qp3 << qp4 <<qp5;
				painter.drawPolygon(polygon);
				pen.setColor(termNameColor_);
				painter.setPen(pen);
				painter.drawText(qp5.x(), qp5.y()+16, QString::fromStdString(name));
			}
		}
		painter.restore();
	}
	
	void CellWidget::setColor(QColor color, QString name){
	
		if (name.toStdString() == "Lines") linesColor_ = color;
		if (name.toStdString() == "Lines Node") linesNodesColor_ = color;
		if (name.toStdString() == "BoxShape") boxShapeColor_ = color;
		if (name.toStdString() == "LineShape") lineShapeColor_ = color;
		if (name.toStdString() == "EllipseShape") ellipseShapeColor_ = color;
		if (name.toStdString() == "ArcShape") arcShapeColor_ = color;
		if (name.toStdString() == "TermShape") termShapeColor_ = color;
		if (name.toStdString() == "TermShape Name") termShapeNameColor_ = color;
		if (name.toStdString() == "Term") termColor_ = color;
		if (name.toStdString() == "Term Name") termNameColor_ = color;
		if (name.toStdString() == "Instance Name") instanceNameColor_ = color;
		if (name.toStdString() == "TransistorP") transistorPColor_ = color;
		if (name.toStdString() == "TransistorN") transistorNColor_ = color;
		if (name.toStdString() == "vdd") vddColor_ = color;
		if (name.toStdString() == "gnd") gndColor_ = color;
		if (name.toStdString() == "Background") backgroundColor_ = color;
		if (name.toStdString() == "Title") titleColor_ = color;
		if (name.toStdString() == "Title Background") titleBackgroundColor_ = color;
		if (name.toStdString() == "Title frame") titleFrameColor_ = color;
		update(); // moins lourd que repaint()
		//repaint();
		
	}
	
	int CellWidget::getZoom(){
		return zoom_*100;
	}
	
	void CellWidget::zoomIn(){
		
		if (zoom_ < 9.9) zoom_ += 0.10;
		findCenter();
		update();
	}
	void CellWidget::zoomOut(){
		if(zoom_ >= 0.20) zoom_ -= 0.10;
		findCenter();
		update();
	}
	void CellWidget::zoomDefault(){
		zoom_ = 1.0;
		findCenter();
		update();
	}
	
	bool CellWidget::findCenter(){
		const vector<Term*>& terms = cell_->getTerms();
		
		for (size_t i = 0;  i < terms.size() ; ++i){
	
			Point point = terms[i]->getPosition();
			//cout << zoom_ << endl;
			if (point.getX() != 0 and point.getY() != 0){
				viewport_.setX1(0);
				viewport_.setY1(0);
				viewport_.setX2(0);
				viewport_.setY2(point.getY()+400);
				if (zoom_ < 0.5) viewport_.setY2(point.getY()+1200);
				if (zoom_ > 1.2) viewport_.setY2(point.getY()+100);
				if (zoom_ > 8.0) {
					viewport_.setY2(point.getY()+10);
					viewport_.setX1(-10);
				}
				update();
				return true;
			}
		}
		viewport_.setX1(0-230);
		viewport_.setY1(0);
		viewport_.setX2(0);
		viewport_.setY2(0+400);
		cerr << "no netlist found" << endl;
		update();
		return false;

	}
	QColor CellWidget::getColor(const QString& name){
		if ( name == "Lines") return linesColor_;
		if ( name == "Lines Node") return linesNodesColor_;
		if ( name == "BoxShape") return boxShapeColor_;
		if ( name == "LineShape") return lineShapeColor_;
		if ( name == "EllipseShape") return ellipseShapeColor_;
		if ( name == "ArcShape") return arcShapeColor_;
		if ( name == "TermShape") return termShapeColor_;
		if ( name == "TermShape Name") return termShapeNameColor_;
		if ( name == "Term") return termColor_;
		if ( name == "Term Name") return termNameColor_;
		if ( name == "Instance Name") return instanceNameColor_;
		if ( name == "TransistorP") return transistorPColor_;
		if ( name == "TransistorN") return transistorNColor_;
		if ( name == "vdd") return vddColor_;
		if ( name == "gnd") return gndColor_;
		if ( name == "Background") return backgroundColor_;
		if ( name == "Title") return titleColor_;
		if ( name == "Title Background") return titleBackgroundColor_;
		if ( name == "Title frame") return titleFrameColor_;
		cerr << "erreur, couleur par defaut noir" << endl;
		return Qt::black;
	}
	
}  // Netlist namespace.
