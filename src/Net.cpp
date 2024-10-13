// -*- explicit-buffer-name: "Net.cpp<M1-MOBJ/4-5>" -*-
#include  <limits>
#include <libxml/xmlreader.h>
#include  "Net.h"

namespace Netlist {

    using namespace std;

    Net::Net(Cell * cell,const string & str, Term::Type type)
		: owner_(cell)
		, name_(str)
		, id_(cell -> newNetId())
		, type_(type)
		, nodes_()
		, lines_()
	{
        cell -> add(this);
    }
	
    Net::~Net() {
        getCell() -> remove(this);
    }

    Cell * Net::getCell() const {
        return owner_;
    }

    const string & Net::getName() const {
        return name_;
    }

    unsigned int Net::getId() const {
        return id_;
    }

    Term::Type Net::getType() const {
        return type_;
    }

    const vector<Node*>& Net::getNodes() const {
        return nodes_;
    }
	
	Node* Net::getNode(size_t id) const{
		if (id < nodes_.size()){				//--ajout securité
			// vector<Node*> tmp = getNodes();  //inutile car on a deja nodes_
			// return tmp[id];
			return nodes_[id];
		}
		cerr << "Erreur, return NULL dans Net.cpp, Node* Net::getNode(size_t id)const" << endl;	//Message d'erreur
		return NULL;
	}

    size_t Net::getFreeNodeId() const {
        for (vector <Node*>::size_type i = 0; i != nodes_.size(); i++) {
            if (nodes_[i] == NULL) {
                return i;
            } //else return nodes_.size(); //inutile
        }
        return nodes_.size();
    }

    void Net::add(Node * n) {
        size_t i = getFreeNodeId();
		
        if (n -> getId() == n -> noid) { //Si la node n est egale à noid
            nodes_.push_back(n);
            nodes_[i] -> setId(i);
        } else{
			if(n->getId() >= nodes_.size()){
				nodes_.resize(n->getId()+1);
				
			}
			nodes_[n->getId()] = n;
        }
    }

    bool Net::remove(Node * n) {
        if (n -> getId() == n -> noid) return false;
        else {
            nodes_[n->getId()] = NULL;
			return true;
        }
        return false;
    }

    //Methode Line 
    void Net::add(Line * line) {
        if (line) lines_.push_back(line);
    }

    bool Net::remove(Line * line) {
        if (line) {
            for (vector < Line * > ::iterator il = lines_.begin(); il != lines_.end(); ++il) {
                if ( * il == line) {
                    lines_.erase(il);
                    return true;
                }
            }
        }
        return false;
    }

    // toXml :
    void Net::toXml(ostream & stream) {
        stream << indent++ << "<net name=\"" << name_ << "\" type=\"" << Term::toString(type_) << "\">\n";
        for (vector < Node * > ::iterator inode = nodes_.begin(); inode != nodes_.end(); ++inode) {
			if( (*inode) != NULL){
				( * inode) -> toXml(stream);
			}
        }
        for (vector < Line * > ::iterator iline = lines_.begin(); iline != lines_.end(); ++iline) {
			if( (*iline) != NULL){
				( * iline) -> toXml(stream);
			}
        }
        stream << --indent << "</net>\n";

    }

    // FromXml :
    Net * Net::fromXml(Cell * cell, xmlTextReaderPtr reader) {

        const xmlChar * netTag = xmlTextReaderConstString(reader, (const xmlChar * ) "net");
		const xmlChar * nodeTag = xmlTextReaderConstString(reader, (const xmlChar * ) "node");
		const xmlChar * lineTag = xmlTextReaderConstString(reader, (const xmlChar * ) "line");

        Net * net = NULL;
        string NetName = xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar * ) "name"));
        string NetType = xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar * ) "type"));

        if (NetType == "External") {
            net = new Net(cell, NetName, Term::External);
        }
        if (NetType == "Internal") {
            net = new Net(cell, NetName, Term::Internal);
        }
        
        while (true) {
            int status = xmlTextReaderRead(reader);
            if (status != 1) {
                if (status != 0) {
                    cerr << "[ERROR] Cell::fromXml(): Unexpected termination of the XML parser." << endl;
                }
                break;
            }
            switch (xmlTextReaderNodeType(reader)) {
            case XML_READER_TYPE_COMMENT:
            case XML_READER_TYPE_WHITESPACE:
            case XML_READER_TYPE_SIGNIFICANT_WHITESPACE:
                continue;
            }

            const xmlChar * nodeName = xmlTextReaderConstLocalName(reader);

            /*if ((nodeName == netTag or nodeName == nodeTag or nodeName == lineTag ) and(xmlTextReaderNodeType(reader) == XML_READER_TYPE_END_ELEMENT)) {
                return net;
            }
			if (nodeName == nodeTag){
				Node::fromXml(net, reader);	
			}
			if ((nodeName == lineTag)){
				Line::fromXml(net, reader);
			}*/
			if (nodeName == netTag and(xmlTextReaderNodeType(reader) == XML_READER_TYPE_END_ELEMENT)) {
                return net;
            }
			if (nodeName == nodeTag){
				Node::fromXml(net, reader);	
			}
			if ((nodeName == lineTag)){
				Line::fromXml(net, reader);
			}
        }
        return net;
    }
}
