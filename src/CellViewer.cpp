// -*- explicit-buffer-name: "CellViewer.cpp<M1-MOBJ/8-10>" -*-
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMenuBar>
#include "CellViewer.h"
#include <QToolBar>
#include <QColorDialog>
#include <QStatusBar> 
#include <QDebug>

namespace Netlist {

  using namespace std;

	CellViewer::CellViewer(QWidget * parent)
	: QMainWindow(parent)
	, cellWidget_(NULL)
	, cellsLib_(NULL)
	, instancesWidget_(NULL)
	, saveCellDialog_(NULL)
	, color_(NULL)
	, statusBarVisible_(false){
		
		
		setWindowTitle ( tr ( "Netlist viewer") );
		cellWidget_ = new CellWidget();
		saveCellDialog_ = new SaveCellDialog(this);
		instancesWidget_ = new InstancesWidget(parent);
		cellsLib_ = new CellsLib(parent); // TME 9
		
		
		color_ = new Color(this, cellWidget_);
		
		
		setCentralWidget(cellWidget_);
		
		instancesWidget_ -> setCellViewer(this);
		cellsLib_->setCellViewer(this); // TME 9
		
		QMenu * fileMenu = menuBar() -> addMenu("File");
		
		QAction * action = new QAction(" &Save As", this);
		action -> setStatusTip("Save to disk (rename) the Cell");
		action -> setShortcut(QKeySequence("CTRL+S"));
		action -> setVisible(true);
		fileMenu -> addAction(action);
		connect(action, SIGNAL(triggered()), this, SLOT(saveCell()));
		
		action = new QAction(" &Open Cell", this);
		action -> setStatusTip("Open to disk the Cell");
		action -> setShortcut(QKeySequence("CTRL+O"));
		action -> setVisible(true);
		fileMenu -> addAction(action);
		connect(action, SIGNAL(triggered()), this, SLOT(openCell()));
		
		action = new QAction("Open &InstancesWidget", this);//TME 9
		action -> setStatusTip("Open Instances Widget");
		action -> setShortcut(QKeySequence("CTRL+I"));
		action -> setVisible(true);
		fileMenu -> addAction(action);
		connect(action, SIGNAL(triggered()), this, SLOT(showInstancesWidget()));
		
		action = new QAction("Open &CellsLib", this); //TME 9
		action -> setStatusTip("Open CellsLib");
		action -> setShortcut(QKeySequence("CTRL+C"));
		action -> setVisible(true);
		fileMenu -> addAction(action);
		connect(action, SIGNAL(triggered()), this, SLOT(showCellsLib()));
		
		action = new QAction("Open both", this); //TME 9
		action -> setStatusTip("Open InstancesWidget and CellsLib");
		action -> setShortcut(QKeySequence("CTRL+A"));
		action -> setVisible(true);
		fileMenu -> addAction(action);
		connect(action, SIGNAL(triggered()), this, SLOT(showInstancesWidget()));
		connect(action, SIGNAL(triggered()), this, SLOT(showCellsLib()));
		
		action = new QAction("&Quit", this);
		action -> setStatusTip("Exit the Netlist Viewer");
		action -> setShortcut(QKeySequence("CTRL+Q"));
		action -> setVisible(true);
		fileMenu -> addAction(action);
		connect(action, SIGNAL(triggered()), this, SLOT(close()));
		
/* Tools menu*/
		QMenu * EditMenu = menuBar() -> addMenu("Tools");	
		action = new QAction("Set Color", this);
		action -> setStatusTip("Set new color for the Netlist");
		action -> setVisible(true);
		EditMenu -> addAction(action);
		connect(action, SIGNAL(triggered()), this, SLOT(chooseColor()));
		
		action = new QAction("Nearest cell", this);
		action -> setStatusTip("Bring back to the nearest cell");
		action -> setVisible(true);
		EditMenu -> addAction(action);
		connect(action, SIGNAL(triggered()), this, SLOT(findCenter()));	
		
		action = new QAction("Zoom in", this);
		action -> setStatusTip("Zoom in (+25%, max +1000%)");
		action -> setShortcut(QKeySequence("CTRL++"));
		action -> setVisible(true);
		EditMenu -> addAction(action);
		connect(action, SIGNAL(triggered()), this, SLOT(zoomIn()));	
		
		action = new QAction("Zoom out", this);
		action -> setStatusTip("Zoom out (-10%, min -90%)");
		action -> setShortcut(QKeySequence("CTRL+-"));
		action -> setVisible(true);
		EditMenu -> addAction(action);
		connect(action, SIGNAL(triggered()), this, SLOT(zoomOut()));	
		
		action = new QAction("Default zoom", this);
		action -> setStatusTip("Zoom 100%");
		action -> setShortcut(QKeySequence("CTRL+="));
		action -> setVisible(true);
		EditMenu -> addAction(action);
		connect(action, SIGNAL(triggered()), this, SLOT(zoomDefault()));	
		
/* Option menu */
		QMenu * OptionMenu = menuBar() -> addMenu("Options");
		
		action_wp_ = new QAction("Widget Priority (disabled)", this);
		action_wp_ -> setStatusTip("Enable/Disable Widget to Stay on Top (only for Instances and CellsLib)");
		OptionMenu -> addAction(action_wp_);
		connect(action_wp_, SIGNAL(triggered()), this, SLOT(fixeWidget()));
		
		action_sb_ = new QAction("Status Bar (enabled)", this);
		action_sb_ -> setStatusTip("Enable/Disable Status bar");
		action_sb_ -> setVisible(true);
		OptionMenu -> addAction(action_sb_);
		connect(action_sb_, SIGNAL(triggered()), this, SLOT(toogleStatusBar()));
		
		action_sz_ = new QAction("Zoom status (enabled)", this);
		action_sz_ -> setStatusTip("show/hide zoom status");
		action_sz_ -> setVisible(true);
		OptionMenu -> addAction(action_sz_);
		connect(action_sz_, SIGNAL(triggered()), this, SLOT(toogleZoomStatus()));
		
		
		QStatusBar* statusBar = new QStatusBar(this);
		QLabel* leftStatusBar = new QLabel("Status bar is visible, go to option to Unshow");
		
		rightStatusBar_ = new QLabel("zoom : " + QString::number(cellWidget_->getZoom()) + "%");
		setStatusBar(statusBar);
		rightStatusBar_->setAlignment(Qt::AlignRight);
		statusBar->showMessage(leftStatusBar->text(),0);
		statusBar->addPermanentWidget(rightStatusBar_);
		
	}
	
	void CellViewer::toogleZoomStatus(){
		if (rightStatusBar_->isVisible()) {
			action_sz_->setText("Status Bar (disabled)");
			rightStatusBar_->hide();
			statusBar()->showMessage("Zoom status disabled");
		    
		} else {
			action_sz_->setText("Status Bar (enabled)");
			rightStatusBar_->show();
			statusBar()->showMessage("Zoom status enabled");
		}
	}
	
	void CellViewer::zoomIn(){
		cellWidget_->zoomIn();
		rightStatusBar_->setText("zoom : " + QString::number(cellWidget_->getZoom()) + "%");
		
	}
	
	void CellViewer::zoomOut(){
		cellWidget_->zoomOut();
		rightStatusBar_->setText("zoom : " + QString::number(cellWidget_->getZoom()) + "%");
	}
	
	void CellViewer::zoomDefault(){
		cellWidget_->zoomDefault();
		rightStatusBar_->setText("zoom : " + QString::number(cellWidget_->getZoom()) + "%");
	}
	
	void CellViewer::findCenter(){
		cellWidget_->findCenter();
	}
	void CellViewer::fixeWidget(){
		instancesWidget_->fixeWidget();
		cellsLib_->fixeWidget();
		if (action_wp_->text() == "Widget Priority (enabled)"){
			action_wp_->setText("Widget Priority (disabled)");
			statusBar()->showMessage("Widget Priority disabled");
		}else{
			action_wp_->setText("Widget Priority (enabled)");
			statusBar()->showMessage("Widget Priority disabled");	
		}
		
		//cout << "test : " << instancesWidget_->isVisible() << endl;
	}
	
	void CellViewer::toogleStatusBar(){
		
		if (statusBarVisible_) {
			action_sb_->setText("Status Bar (enabled)");
			statusBar()-> show();
		    statusBar()->showMessage("Status bar is visible, go to option to Unshow");
		    statusBarVisible_ = false;
		    
		} else {
			action_sb_->setText("Status Bar (disabled)");
		    statusBar()->hide();
		    statusBarVisible_ = true;
		}
	}
	
	void CellViewer::chooseColor(){
		color_->show();
	}
	
	CellViewer::~CellViewer(){}
	
	Cell* CellViewer::getCell() const{
		return cellWidget_->getCell();
	}
	
	void CellViewer::setCell(Cell* cell){
		cellWidget_->setCell(cell);
		instancesWidget_->setCell(cell);
		cellsLib_ ->getBaseModel()->setCell(cell);
	}
	
	
	void CellViewer::saveCell() {
		Cell* cell = getCell();
		if (cell == NULL) {
			cerr << "Nothing to save" << endl;
			return;
		}
		QString cellName = cell -> getName().c_str();
		
		if (saveCellDialog_ -> run(cellName)) {
			cell -> setName(cellName.toStdString());
			cell -> save(cellName.toStdString());
		}
	}
	
	void CellViewer::openCell(){
		Cell* cell;
		QString cellName;
		
		if(OpenCellDialog::run(cellName)){
			if((cell = Cell::find((cellName.toStdString()))) == NULL){
			
				if(cellName.toStdString() == "fulladder" and (Cell::find("halfadder") == NULL or Cell::find("or2") == NULL)){
					cerr << "[ERROR] unable to open file, check if halfadder or or2 is loaded before" << endl;
					return;
				}
				
				if(cellName.toStdString() == "halfadder" and (Cell::find("and2") == NULL or Cell::find("xor2") == NULL)){
					cerr << "[ERROR] unable to open file, check if and2 or xor2 is loaded before" << endl;
					return;
				}
				
				if((cellName.toStdString() == "or2" or cellName.toStdString() == "and2") 
					and (Cell::find("vdd") == NULL or Cell::find("gnd") == NULL 
					or Cell::find("TransistorN") == NULL or Cell::find("TransistorP") == NULL))
				{
					cerr << "[ERROR] unable to open file, check if vdd, gnd, TransistorP, TransistorN is loaded before" << endl;
					return;
				}
				
				cell = Cell::load(cellName.toStdString());
				if (cell == NULL) return;
				setCell(cell);
				emit cellLoaded();
				return;
			}else {
				if (cellWidget_->getCell() == NULL){
					setCell(cell);
					emit cellLoaded();
					return;
				}
				setCell(cell);
				emit cellLoaded();
				return;
			}
		}
	}
	
	void CellViewer::showInstancesWidget(){
		instancesWidget_ -> show();
	}

	void CellViewer::showCellsLib(){
		cellsLib_ -> show();
	}
	
}   // Netlist namespace.
