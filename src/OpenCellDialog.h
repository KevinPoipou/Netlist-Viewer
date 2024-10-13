// -*- explicit-buffer-name: "OpenCellDialog.h<M1-MOBJ/8-10>" -*-

#ifndef NETLIST_OPEN_CELL_DIALOG_H
#define NETLIST_OPEN_CELL_DIALOG_H
#include <QDialog>
#include <QLineEdit>

namespace Netlist {

	class OpenCellDialog : public QDialog {
		Q_OBJECT ;
		
		public :
						OpenCellDialog ( QWidget * parent = NULL );
		static	bool	run (QString & name);
				const 	QString getCellName () const ;
			
		protected :
			QLineEdit*	lineEdit_ ;
	};

}  // Netlist namespace.

#endif  // NETLIST_OPEN_CELL_DIALOG_H
