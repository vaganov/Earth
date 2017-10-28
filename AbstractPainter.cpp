#include "AbstractPainter.h"

AbstractPainter::AbstractPainter() {

    m = 0;
}

AbstractPainter::AbstractPainter(TreeModel *model) {

    m = model;
}

AbstractPainter::AbstractPainter(const AbstractPainter &copy) {

    m = copy.m;
}

AbstractPainter::~AbstractPainter() {
}

TreeModel *AbstractPainter::model() {

    return m;
}

void AbstractPainter::setModel(TreeModel *model) {
    
//    if (m) {
//        delete m;
//    }
    m = model;
}
