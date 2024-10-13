// -*- explicit-buffer-name: "Box.cpp<M1-MOBJ/7>" -*-

#include "Color.h"
#include <QStringList>
#include <QWidget>
#include <QHeaderView>
#include <QVBoxLayout>
#include <QPushButton>
#include <QStandardItemModel>
#include <QColorDialog>
#include <iostream>


namespace Netlist {
	using namespace std;

	Color::Color (QWidget* parent, CellWidget* cellw )
	: QDialog (parent)
	, cellWidget_(cellw)
	{
		setMinimumSize(440, 660);
		Widget_ = new QTableView(this);
		Widget_->setModel(new QStandardItemModel(this));
		//Widget_->setFixedHeight(570);
		//Widget_->setFixedWidth(300);
		//Widget_->setSortingEnabled(true);
		QStringList columnHeaders;
		columnHeaders << "Objet" << "Color" << "setColor" << "show/hide" ;

		QStandardItemModel* model = qobject_cast<QStandardItemModel*>(Widget_->model());
   		model->setHorizontalHeaderLabels(columnHeaders);
   		
		QHeaderView* horizontalHeader = Widget_->horizontalHeader();
		
		horizontalHeader->resizeSection(0, 130);
		horizontalHeader->resizeSection(1, 50);
		horizontalHeader->resizeSection(2, 150);
		horizontalHeader->setStretchLastSection(true);
		horizontalHeader -> setMinimumSectionSize(50);
		
		
		//horizontalHeader->setSortIndicator(0, Qt::AscendingOrder);
	
		QVBoxLayout* layout = new QVBoxLayout(this);
        layout->addWidget(Widget_);
        
        QHeaderView * verticalHeader = Widget_ -> verticalHeader();
		verticalHeader -> setVisible(false);
        
        addRow("Lines");
        addRow("Lines Node");
        addRow("BoxShape");
        addRow("LineShape");
        addRow("EllipseShape");
        addRow("ArcShape");
        addRow("TermShape");
        addRow("TermShape Name");
        addRow("Term");
        addRow("Term Name");
        addRow("Instance Name");
        addRow("TransistorP");
        addRow("TransistorN");
        addRow("vdd");
        addRow("gnd");
        addRow("Title");
        addRow("Title Background");
        addRow("Title frame");
        addRow("Background");
        layout->addWidget(Widget_);
		
		
		
		QPushButton* resetColor = new QPushButton(this);
		resetColor -> setText("Reset color to default");
		layout->addWidget(resetColor);	
		connect(resetColor, SIGNAL(clicked()), this, SLOT(defaultColor()));
		setLayout(layout);
	}
	
	void Color::addRow(const QString& name){
	
		QStandardItemModel* model = qobject_cast<QStandardItemModel*>(Widget_->model());

   		int row = model->rowCount();

		QStandardItem* nameItem = new QStandardItem(name);
		nameItem->setEditable(false);
		model->setItem(row, 0, nameItem);

		lineColor_ = new QStandardItem();
  		model->setItem(row, 1, lineColor_);
  		lineColor_->setEditable(false);
		lineColor_->setData(cellWidget_->getColor(name), Qt::BackgroundRole);
		
		
		QPushButton* colorButton = new QPushButton("Press to SelectColor");
		colorButton->setProperty("row", row); 
		colorButton->setProperty("name", name);
		connect(colorButton, SIGNAL(clicked()), this, SLOT(selectColor()));
		Widget_->setIndexWidget(model->index(row, 2), colorButton);
		
		
		checkbox_ = new QCheckBox();
		checkbox_->setProperty("row", row);
		checkbox_->setProperty("name", name);
		
		
		checkbox_->setFocusPolicy(Qt::NoFocus);
		connect(checkbox_, SIGNAL(clicked()), this, SLOT(showhide()));
		
		if (name.toStdString() == "Background") {
			checkbox_->setEnabled(false);
			checkbox_->setText("Disabled (useless to hide)");
		}
		Widget_->setIndexWidget(model->index(row, 3), checkbox_);

	}
	
	void Color::showhide(){
		QObject* senderObject = QObject::sender();
    	QCheckBox* senderCheckbox = qobject_cast<QCheckBox*>(senderObject);
    	if(senderCheckbox){
    		QString name = senderCheckbox->property("name").toString();
    		QColor color = cellWidget_->getColor(name);
    		color.setAlpha(255);
    		if(senderCheckbox->isChecked()) color.setAlpha(0);
    		cellWidget_->setColor(color, name);
    	}
	}
	
	void Color::selectColor(){
		QPushButton* senderButton = qobject_cast<QPushButton*>(QObject::sender());
		if (senderButton)
		{
		    int row = senderButton->property("row").toInt();
		    //cout << "row : " << row << endl;
		    QString name = senderButton->property("name").toString();
		    QColor selectedColor = QColorDialog::getColor(Qt::red, Widget_);
		    if (selectedColor.isValid())
		    {
		    	
		    	cellWidget_->setColor(selectedColor, name);
		    	QStandardItemModel* model = qobject_cast<QStandardItemModel*>(Widget_->model());
		        model->item(row, 1)->setData(selectedColor, Qt::BackgroundRole);
		       
		        
		    }
		}
	
	}
	
	
	//Fonction qui marche avec le tri
	/*void Color::selectColor() {
		QPushButton* senderButton = qobject_cast<QPushButton*>(QObject::sender());
		if (senderButton) {
		    int row = senderButton->property("row").toInt();
		    QString name = senderButton->property("name").toString();
		    QColor selectedColor = QColorDialog::getColor(Qt::red, Widget_);
		    if (selectedColor.isValid()) {
		        cellWidget_->setColor(selectedColor, name);
		        QStandardItemModel* model = qobject_cast<QStandardItemModel*>(Widget_->model());
		        
		        QPoint buttonPosition = senderButton->mapTo(Widget_, QPoint(0, 0));
		        
		        QModelIndex index = Widget_->indexAt(buttonPosition);
		        if (index.isValid()) {
		            int newRow = index.row();
		            
		            model->item(newRow, 1)->setData(selectedColor, Qt::BackgroundRole);
		            
		            if (newRow !=  row) {
		                model->item( row, 1)->setData(QColor(), Qt::BackgroundRole);
		            }
		        }
		    }
		}
	}*/
	
	void Color::defaultColor(){
		QStandardItemModel* model = qobject_cast<QStandardItemModel*>(Widget_->model());
		cellWidget_->setColor(Qt::cyan, QString::fromStdString("Lines"));
		model->item(0, 1)->setData(Qt::cyan, Qt::BackgroundRole);
    	cellWidget_->setColor(Qt::cyan, QString::fromStdString("Lines Node"));
    	model->item(1, 1)->setData(Qt::cyan, Qt::BackgroundRole);	
    	cellWidget_->setColor(Qt::darkGreen, QString::fromStdString("BoxShape"));
    	model->item(2, 1)->setData(Qt::darkGreen, Qt::BackgroundRole);	
    	cellWidget_->setColor(Qt::darkGreen, QString::fromStdString("LineShape"));
    	model->item(3, 1)->setData(Qt::darkGreen, Qt::BackgroundRole);	
    	cellWidget_->setColor(Qt::darkGreen, QString::fromStdString("EllipseShape"));
    	model->item(4, 1)->setData(Qt::darkGreen, Qt::BackgroundRole); 	
    	cellWidget_->setColor(Qt::darkGreen, QString::fromStdString("ArcShape"));
    	model->item(5, 1)->setData(Qt::darkGreen, Qt::BackgroundRole); 	
    	cellWidget_->setColor(Qt::darkRed, QString::fromStdString("TermShape"));
    	model->item(6, 1)->setData(Qt::darkRed, Qt::BackgroundRole);   	
    	cellWidget_->setColor(Qt::darkRed, QString::fromStdString("TermShape Name"));
    	model->item(7, 1)->setData(Qt::darkRed, Qt::BackgroundRole);
    	cellWidget_->setColor(Qt::darkRed, QString::fromStdString("Term"));
    	model->item(8, 1)->setData(Qt::darkRed, Qt::BackgroundRole);
    	cellWidget_->setColor(Qt::darkRed, QString::fromStdString("Term Name"));
    	model->item(9, 1)->setData(Qt::darkRed, Qt::BackgroundRole);	
    	cellWidget_->setColor(Qt::darkGreen, QString::fromStdString("Instance Name"));
    	model->item(10, 1)->setData(Qt::darkGreen, Qt::BackgroundRole);   	
    	cellWidget_->setColor(Qt::darkYellow, QString::fromStdString("TransistorP"));
    	model->item(11, 1)->setData(Qt::darkYellow, Qt::BackgroundRole);   	
    	cellWidget_->setColor(Qt::magenta, QString::fromStdString("TransistorN"));
    	model->item(12, 1)->setData(Qt::magenta, Qt::BackgroundRole);    	
    	cellWidget_->setColor(Qt::blue, QString::fromStdString("vdd"));
    	model->item(13, 1)->setData(Qt::blue, Qt::BackgroundRole);
    	cellWidget_->setColor(Qt::darkGray, QString::fromStdString("gnd"));
    	model->item(14, 1)->setData(Qt::darkGray, Qt::BackgroundRole);
    	cellWidget_->setColor(Qt::white, QString::fromStdString("Title"));
    	model->item(15, 1)->setData(Qt::white, Qt::BackgroundRole);
    	cellWidget_->setColor(Qt::black, QString::fromStdString("Title Background"));
    	model->item(16, 1)->setData(Qt::black, Qt::BackgroundRole);
    	cellWidget_->setColor(Qt::white, QString::fromStdString("Title frame"));
    	model->item(17, 1)->setData(Qt::white, Qt::BackgroundRole);
    	cellWidget_->setColor(Qt::black, QString::fromStdString("Background"));
		model->item(18, 1)->setData(Qt::black, Qt::BackgroundRole);

		
		for(int row = 0; row < model->rowCount(); row++){
			QModelIndex index = model->index(row, 3);
			QCheckBox* checkbox = qobject_cast<QCheckBox*>(Widget_->indexWidget(index));
			checkbox->setChecked(false);
		}
	}
}   // Netlist namespace.
