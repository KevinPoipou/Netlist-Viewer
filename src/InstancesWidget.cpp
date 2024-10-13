// -*- explicit-buffer-name: "InstancesWidget.cpp<M1-MOBJ/8-10>" -*-
#include <QStandardItemModel>
#include <QHeaderView>
#include "CellViewer.h"
#include "InstancesWidget.h"
#include "InstancesModel.h"
#include "CellWidget.h"
#include <QtGui>

namespace Netlist {
	using namespace std;
	
	InstancesWidget::InstancesWidget(QWidget * parent)
		: QWidget(parent), cellViewer_(NULL)
		, baseModel_(new InstancesModel(this))
		, view_(new QTableView(this))
		, load_(new QPushButton(this)) {
		
		setWindowTitle(tr("Instances (Peut passer en arriere plan)"));
		setAttribute(Qt::WA_QuitOnClose, false);
		setAttribute(Qt::WA_DeleteOnClose, false);
		setContextMenuPolicy(Qt::ActionsContextMenu);
		
		view_ -> setShowGrid(false);
		view_ -> setAlternatingRowColors(true);
		view_ -> setSelectionBehavior(QAbstractItemView::SelectRows);
		view_ -> setSelectionMode(QAbstractItemView::SingleSelection);
		view_ -> setModel(baseModel_); // On associe le modele .
		view_ -> setSortingEnabled(true);
		
		QHeaderView * horizontalHeader = view_ -> horizontalHeader();
		horizontalHeader -> setDefaultAlignment(Qt::AlignHCenter);
		horizontalHeader -> setMinimumSectionSize(100);
		horizontalHeader -> setStretchLastSection(true);
		horizontalHeader -> setSortIndicatorShown(true); //affiche le truc pour trier
		horizontalHeader -> setSortIndicator(0, Qt::AscendingOrder);
		horizontalHeader -> setClickable(true);
		
		QHeaderView * verticalHeader = view_ -> verticalHeader();
		verticalHeader -> setVisible(false);
		
		QHBoxLayout * hLayout = new QHBoxLayout ();
		hLayout -> addStretch ();
		hLayout -> addWidget ( load_ );
		hLayout -> addStretch ();
		
		QFrame * separator = new QFrame ();
		separator -> setFrameShape ( QFrame :: HLine );
		separator -> setFrameShadow ( QFrame :: Sunken );
		
		QVBoxLayout * vLayout = new QVBoxLayout ();
		//vLayout -> setSizeConstraint ( QLayout :: SetFixedSize );
		vLayout -> addWidget ( view_ );
		vLayout -> addLayout ( hLayout );
		setLayout ( vLayout );
		
		
		
		load_ -> setText(" Load ");
		connect(load_, SIGNAL(clicked()), this, SLOT(load()));
		
		//setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint); //Empeche le widget de passer en arriere plan

	}
	
	int InstancesWidget::getSelectedRow() const {
		QModelIndexList selecteds = view_ -> selectionModel() -> selection().indexes();
		if (selecteds.empty()) return -1;
		return selecteds.first().row();
	}
	
	void InstancesWidget::load() {
		int selectedRow = getSelectedRow();
		if (selectedRow < 0) return;
		
		cellViewer_ -> setCell(baseModel_ -> getModel(selectedRow));
		cout << "Chargement du modele \"" << cellViewer_->getCell()->getName() << "\"" <<endl;
	}
	
	void InstancesWidget::setCellViewer(CellViewer* viewer){
		cellViewer_ = viewer;
	}
	
	
	void InstancesWidget::fixeWidget(){
		if (windowFlags() & Qt::WindowStaysOnTopHint) {
			setWindowTitle(tr("Instances (NonPriority)"));
			if(isVisible()) {
				setWindowFlags(windowFlags() & ~Qt::WindowStaysOnTopHint);
				show();
			}else setWindowFlags(windowFlags() & ~Qt::WindowStaysOnTopHint );
		} else {
			setWindowTitle(tr("Instances (Priority)"));
			if(isVisible()) {
				setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
				show();
			}else setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
		}
	}
}   // Netlist namespace.
