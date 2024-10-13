// -*- explicit-buffer-name: "SaveCellDialog.h<M1-MOBJ/8-10>" -*-

#ifndef NETLIST_SAVE_CELL_DIALOG_H
#define NETLIST_SAVE_CELL_DIALOG_H
#include <QDialog>
#include <QLineEdit>

namespace Netlist {
	
	class SaveCellDialog : public QDialog {
		Q_OBJECT ;
		
		public :
					SaveCellDialog ( QWidget * parent = NULL );
			bool	run ( QString & name );
			const 	QString getCellName () const ;
			void 	setCellName ( const QString & );
			
		protected :
			QLineEdit*	lineEdit_ ;
	};

}  // Netlist namespace.

#endif  // NETLIST_SAVE_CELL_DIALOG_H
