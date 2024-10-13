// -*- explicit-buffer-name: "CellsLib.h<M1-MOBJ/8-10>" -*-

#ifndef NETLIST_CELLS_LIB_H
#define NETLIST_CELLS_LIB_H

#include <QTableView>
#include <QHeaderView>
#include <QPushButton>
#include "CellViewer.h"
#include "CellsModel.h"

namespace Netlist {
class CellViewer;
	class CellsLib : public QWidget {
		Q_OBJECT;
		public:
					CellsLib    (QWidget* parent=NULL );
					
					void        setCellViewer  ( CellViewer* );
					int         getSelectedRow () const;
			inline 	CellsModel* getBaseModel   (){return baseModel_;}
			
		public slots:
					void        load           ();
					void		fixeWidget();
		private:
			CellViewer*  cellViewer_;
			CellsModel*  baseModel_;
			QTableView*  view_;
			QPushButton* load_;
	};

}  // Netlist namespace.

#endif  // NETLIST_CELLS_LIB_H
