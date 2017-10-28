#include <QtCore>

#include "GeoModel.h"
#include "DefaultPainter.h"
#include "ShapeLoader.h"

int main (int argc, char* argv[]) {
    QCoreApplication app(argc, argv);

    GeoModel* model = new GeoModel();
    AbstractPainter *painter = new DefaultPainter(model);

    ShapeLoader shapeLoader("bigworld.csv", model, painter);
    shapeLoader.load();

    int res = app.exec();

    delete painter;
    delete model;

    return res;
}
