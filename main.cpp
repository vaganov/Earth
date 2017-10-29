#include <QtWidgets>

#include "GeoModel.h"
#include "DefaultPainter.h"
#include "ShapeLoader.h"
#include "GeoView.h"

#include "debug.h"

#ifdef _DEBUG
#include <QTimer>
#endif

int main (int argc, char* argv[]) {
    QApplication app(argc, argv);

    srand48(time(0));

    GeoModel* model = new GeoModel();

    const double phi = M_PI / 3; // 60° Санкт-Петербург
    const double psi = M_PI / 6; // 30°
    GeoView* earth = new GeoView(phi, psi);
    earth->setWindowTitle("OpenGL");
    earth->setModel(model);

//  QObject::connect(model, SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&)),
//          earth, SLOT(onDataChanged(const QModelIndex&, const QModelIndex&)));
//  QObject::connect(model, SIGNAL(dataRoleChanged(const QModelIndex&, int)),
//          earth, SLOT(onDataRoleChanged(const QModelIndex&, int)));
    QObject::connect(model, SIGNAL(dataRolesChanged(const QModelIndex&, const QList<int> &)),
            earth, SLOT(onDataRolesChanged(const QModelIndex&, const QList<int> &)));
    QObject::connect(model, SIGNAL(freeze(bool)),
            earth, SLOT(setFreeze(bool)));
    QObject::connect(model, SIGNAL(rowsRemoved(const QModelIndex &, int, int)),
            earth, SLOT(onRowsRemoved(const QModelIndex &, int, int)));

    QTreeView* tree = new QTreeView();
    tree->setWindowTitle("QTreeView");
    tree->setModel(model);

    QObject::connect(tree, SIGNAL(clicked(const QModelIndex&)),
            model, SLOT(toggle(const QModelIndex&)));

    AbstractPainter *painter = new DefaultPainter(model);

    ShapeLoader shapeLoader("bigworld.csv", model, painter);
    shapeLoader.load();

#ifdef _DEBUG
    QTimer timer;
    timer.start(100);
    QObject::connect(&timer, SIGNAL(timeout(void)), earth, SLOT(moveAC(void)));
#endif

    earth->show();
    tree->show();

    int res = app.exec();

    delete painter;
    delete tree;
    delete earth;
    delete model;

    return res;
}
