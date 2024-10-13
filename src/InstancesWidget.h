// -*- explicit-buffer-name: "InstancesWidget.h<M1-MOBJ/8-10>" -*-

#ifndef NETLIST_INSTANCES_WIDGET_H
#define NETLIST_INSTANCES_WIDGET_H
#include <QTableView>
#include <QPushButton>
#include <QDialog>
#include <QLineEdit>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "CellViewer.h"
#include "InstancesModel.h"
#include "CellWidget.h"
#include "Cell.h"

namespace Netlist {
class CellViewer;


	class InstancesWidget: public QWidget {
	
		Q_OBJECT;
		public:
							InstancesWidget(QWidget* parent = NULL);
					void 	setCellViewer(CellViewer*);
					int 	getSelectedRow() const;
			inline 	void 	setCell(Cell* cell){baseModel_->setCell(cell);}
			
			
			
		public slots:
					void 	load();
					void	fixeWidget();
					
		private:
			CellViewer* 	cellViewer_;
			InstancesModel* baseModel_;
			QTableView* 	view_;
			QPushButton* 	load_;
	};

}  // Netlist namespace.

#endif  // NETLIST_INSTANCES_WIDGET_H
