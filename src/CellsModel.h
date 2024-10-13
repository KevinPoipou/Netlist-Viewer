// -*- explicit-buffer-name: "CellsModel.h<M1-MOBJ/8-10>" -*-

#ifndef NETLIST_CELLS_MODEL_H
#define NETLIST_CELLS_MODEL_H
#include <QAbstractTableModel>
#include "Cell.h"

namespace Netlist {
	
    class CellsModel: public QAbstractTableModel {
        Q_OBJECT;
        public:
						CellsModel(QObject * parent = NULL);
						~CellsModel();
			void		setCell(Cell * );
			Cell* 		getModel(int row);
			int 		rowCount(const QModelIndex & parent = QModelIndex()) const;
			int 		columnCount(const QModelIndex & parent = QModelIndex()) const;
			QVariant 	data(const QModelIndex & index, int role = Qt::DisplayRole) const;
			QVariant 	headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
			
		public slots:
		
			void		updateDatas();
		
        private:
            Cell*	cell_;
    };
}  // Netlist namespace.

#endif  // NETLIST_CELLS_MODEL_H
