// -*- explicit-buffer-name: "Main.cpp<M1-MOBJ/7>" -*-

#include <string>
#include <sstream>
#include <iostream>
#include <memory>
#include <QApplication>
#include <QtGui>
#include "CellViewer.h"
#include "Cell.h"

using namespace std;
using namespace Netlist;

int main ( int argc , char * argv []) {
	
    cout << "Chargement des modeles..." << endl;
    Cell::load("vdd");
    Cell::load("gnd");
    Cell::load("TransistorN");
    Cell::load("TransistorP");
    Cell::load("and2");
    Cell::load("or2");
    Cell::load("xor2");
    Cell::load("halfadder");
	Cell* fulladder = Cell::load("fulladder");
	
	QApplication * qa = new QApplication ( argc , argv );
	CellViewer * viewer = new CellViewer ();
	viewer -> setCell ( fulladder );						//ligne a enlever si on ne charge rien
	viewer -> show ();
	int rvalue = qa -> exec ();
	delete qa ;
	return rvalue ;
	
}
