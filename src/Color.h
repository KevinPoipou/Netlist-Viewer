// -*- explicit-buffer-name: "Box.h<M1-MOBJ/7>" -*-

#ifndef NETLIST_COLOR_H
#define NETLIST_COLOR_H
#include <QDialog>
#include <QTableWidget>
#include <QStandardItemModel>
#include "CellWidget.h"
#include <QCheckBox>

namespace Netlist {
class CellWidget;
	class Color: public QDialog{
	Q_OBJECT;
	public :
		Color(QWidget* parent = NULL, CellWidget* cellw = NULL);
		void	addRow(const QString& name);
		
	public slots :
		void	selectColor();
		void	defaultColor();
		void	showhide();
		
	private :
	QStandardItem*		lineColor_;
	QStandardItem*		boxShapeColor_;
	CellWidget* 		cellWidget_;
	QTableView* 		Widget_;
	QCheckBox*			checkbox_;
	QStandardItem*		checkboxItem_;
	};

} // Netlist namespace.

#endif  // NETLIST_BOX_H
