#ifndef GEOMODEL_H
#define GEOMODEL_H

#include "TreeModel.h"

class GeoModel : public TreeModel {
    Q_OBJECT
    
public:
    GeoModel (QObject *parent = 0);
        
public slots:
//    void check (const QModelIndex &idx);
    void toggle (const QModelIndex& idx);
    
signals:
    void checked (const QModelIndex &idx);
    void freeze(bool);
};

#endif // GEOMODEL_H

