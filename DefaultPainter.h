#ifndef DEFAULTPAINTER_H
#define DEFAULTPAINTER_H

#include "AbstractPainter.h"

class DefaultPainter : public AbstractPainter {
public:
    DefaultPainter();
    DefaultPainter(TreeModel *model);
    virtual void country(QModelIndex &parentIndex, const QPolygonF polygon);
    virtual void reservoir(QModelIndex &parentIndex, const QPolygonF polygon);
    virtual void fir(QModelIndex &parentIndex, const QPolygonF polygon);
    virtual void route(QModelIndex &index, const QList<Waypoint> &route);
    virtual void restriction(QModelIndex &parentIndex, const QPolygonF polygon);
};

#endif // DEFAULTPAINTER_H
