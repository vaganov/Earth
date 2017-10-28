#include <QtWidgets>

#include "GeoModel.h"
#include "DefaultPainter.h"
#include "ShapeLoader.h"

int main (int argc, char* argv[]) {
    QApplication app(argc, argv);

    GeoModel* model = new GeoModel();

    QTreeView* tree = new QTreeView();
    tree->setWindowTitle("QTreeView");
    tree->setModel(model);

    AbstractPainter *painter = new DefaultPainter(model);

    ShapeLoader shapeLoader("bigworld.csv", model, painter);
    shapeLoader.load();

    tree->show();

    int res = app.exec();

    delete painter;
    delete tree;
    delete model;

    return res;
}
