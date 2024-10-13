// -*- explicit-buffer-name: "CellViewer.h<M1-MOBJ/8-10>" -*-

#ifndef NETLIST_CELL_VIEWER_H
#define NETLIST_CELL_VIEWER_H
#include <QMainWindow>
#include "CellWidget.h"
#include "Cell.h"
#include "InstancesWidget.h"
#include "InstancesModel.h"
#include "OpenCellDialog.h"
#include "SaveCellDialog.h"
#include "CellsLib.h"
#include "Color.h"


namespace Netlist {
class InstancesWidget;
class CellsLib;
class Color;
	class CellViewer: public QMainWindow {
		Q_OBJECT;
		public:
								CellViewer(QWidget* parent = NULL);
			virtual				~CellViewer();
					Cell* 		getCell() const;
			inline 	CellsLib* 	getCellsLib(){return cellsLib_;} // TME9+.
		public slots:
		
					void 		setCell(Cell*);
					void 		saveCell();
					void 		openCell();
					void 		showCellsLib(); // TME9+.
					void 		showInstancesWidget(); // TME9+.
					void		chooseColor();
					void 		toogleStatusBar();
					void		fixeWidget();
					void		findCenter();
					void		zoomIn();
					void		zoomOut();
					void		zoomDefault();
					void		toogleZoomStatus();
		signals :
					void		cellLoaded();
					
		
		private:
			CellWidget*			cellWidget_;
			CellsLib* 			cellsLib_; // TME9+.
			InstancesWidget* 	instancesWidget_; // TME9+.
			SaveCellDialog* 	saveCellDialog_;
			Color*				color_;
			bool				statusBarVisible_;
			QAction*			action_wp_;
			QAction*			action_sb_;
			QAction*			action_sz_;
			QLabel*				rightStatusBar_;
			
	};

}  // Netlist namespace.

#endif  // NETLIST_CELL_VIEWER_H
