#ifndef ABSTRACTPAINTER_H
#define ABSTRACTPAINTER_H

#include "TreeModel.h"
#include <QtGui>
#include "Waypoint.h"

class AbstractPainter {
public:
    AbstractPainter();
    AbstractPainter(TreeModel *model);
    AbstractPainter(const AbstractPainter &copy);
    TreeModel *model();
    void setModel(TreeModel *model);
    virtual void country(QModelIndex &parentIndex, QPolygonF polygon) = 0;
    virtual void reservoir(QModelIndex &parentIndex, const QPolygonF polygon) = 0;
    virtual void fir(QModelIndex &parentIndex, const QPolygonF polygon) = 0;
    virtual void route(QModelIndex &index, const QList<Waypoint> &route) = 0;
    virtual void restriction(QModelIndex &parentIndex, const QPolygonF polygon) = 0;
protected:
    TreeModel *m;
};

#endif // ABSTRACTPAINTER_H
