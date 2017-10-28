#include "DefaultPainter.h"
#include <QtCore>
#include "Role.h"
#include "metatype.h"

DefaultPainter::DefaultPainter() : AbstractPainter() {
}

DefaultPainter::DefaultPainter(TreeModel *model) : AbstractPainter(model) {
}

void DefaultPainter::country(QModelIndex &index, const QPolygonF polygon) {

    QMap<int, QVariant> roles;
    QVariant varPolygon;
    varPolygon.setValue<QPolygonF>(polygon);
    roles.insert(ContourPolygonRole, varPolygon);
    roles.insert(ContourColorRole, QColor(168, 135, 0, 255));
    roles.insert(ContourLevelRole, 2);
    roles.insert(LineWidthRole, 3.0);
    roles.insert(AreaPolygonRole, varPolygon);
    roles.insert(AreaColorRole, QColor(165, 165, 158, 255));
    roles.insert(AreaLevelRole, 1);   
    roles.insert(IsAreaSelectableRole, true);
    m->setItemData(index, roles);
}

void DefaultPainter::reservoir(QModelIndex &parentIndex, const QPolygonF polygon) {

    QMap<int, QVariant> roles;
    QVariant varPolygon;
    varPolygon.setValue<QPolygonF>(polygon);
    roles.insert(ContourPolygonRole, varPolygon);
    roles.insert(ContourColorRole, QColor(168, 135, 0, 255));
    roles.insert(ContourLevelRole, 4);
    roles.insert(LineWidthRole, 1.0);
    roles.insert(AreaPolygonRole, varPolygon);
    roles.insert(AreaColorRole, QColor(158, 158, 171, 255));
    roles.insert(IsAreaSelectableRole, true); 
    roles.insert(AreaLevelRole, 3);   
    m->setItemData(parentIndex, roles);
}

void DefaultPainter::fir(QModelIndex &parentIndex, const QPolygonF polygon) {

    QMap<int, QVariant> roles;
    QVariant varPolygon;
    varPolygon.setValue<QPolygonF>(polygon);
    roles.insert(ContourPolygonRole, varPolygon);
    roles.insert(ContourColorRole, QColor(0, 0, 0, 255));
    roles.insert(ContourLevelRole, 6);
    roles.insert(LineWidthRole, 1.0);
    roles.insert(AreaPolygonRole, varPolygon);
    roles.insert(AreaColorRole, QColor(181, 181, 181, 102));
    roles.insert(IsAreaSelectableRole, true); 
    roles.insert(AreaLevelRole, 5);   
    m->setItemData(parentIndex, roles);
}

void DefaultPainter::route(QModelIndex &index, const QList<Waypoint> &route) {

    QMap<int, QVariant> roles;
    QPolygonF polygon;
    polygon.reserve(route.size());

    foreach (Waypoint waypoint, route) {
        polygon.append(waypoint.coordinates);
    }
    
    QVariant v;
    v.setValue<QPolygonF>(polygon);
    roles.insert(RoutePolygonRole, v);
    QColor color(191, 191, 191, 255);
    roles.insert(RouteColorRole, color);
    roles.insert(RouteWidthRole, 10);
    roles.insert(IsRouteSelectableRole, true);
    roles.insert(RouteLevelRole, 7);
    m->setItemData(index, roles);

    QImage imgAirport = QImage(":/airport.png");
    QImage imgNdb = QImage(":/ndb.png");
    QImage imgVorDme = QImage(":/vordme.png");
    QImage imgWaypoint = QImage(":/waypoint.png");

    QList<Waypoint>::const_iterator end = route.end();
    int counter = m->rowCount(index);
    for (QList<Waypoint>::const_iterator i = route.begin(); i != end; ++i) {
        QMap<int, QVariant> roles;
        roles.insert(Qt::DisplayRole, i->name + "(" + QString::number(i->coordinates.x()) + ":" + QString::number(i->coordinates.y()) + ")");
        roles.insert(TextRole, i->name);
        switch (i->type) {
            case (Waypoint::TYPE_VORDME):
                roles.insert(ImageRole, imgVorDme);
                break;
            case (Waypoint::TYPE_NDB):
                roles.insert(ImageRole, imgNdb);
                break;
            case (Waypoint::TYPE_AIRPORT):
                roles.insert(ImageRole, imgAirport);
                break;
            case (Waypoint::TYPE_WAYPOINT):
                roles.insert(ImageRole, imgWaypoint);
                break;
            default:
                qWarning() << "DefaultPainter::route: Wrong waypoint type!";
        }
        roles.insert(ImageCoordinatesRole, i->coordinates);
        roles.insert(ImageLevelRole, 9);
        roles.insert(IsImageSelectableRole, true);
//        roles.insert(TextRole, i->name);
        roles.insert(TextCoordinatesRole, i->coordinates);
        roles.insert(TextLevelRole, 8);
        roles.insert(IsTextSelectableRole, true);
        roles.insert(Qt::CheckStateRole, Qt::Checked);
        m->insertRow(counter, index);
        QModelIndex childIndex = m->index(counter++, 0, index);
        m->setItemData(childIndex, roles);
    }
}

void DefaultPainter::restriction(QModelIndex &index, const QPolygonF polygon) {

    QMap<int, QVariant> roles;
    QVariant varPolygon;
    varPolygon.setValue<QPolygonF>(polygon);
    roles.insert(ContourPolygonRole, varPolygon);
    roles.insert(ContourColorRole, QColor(255, 0, 0, 255));
    roles.insert(ContourLevelRole, 11);
    roles.insert(LineWidthRole, 1.0);
    roles.insert(AreaPolygonRole, varPolygon);
    roles.insert(AreaColorRole, QColor(255, 0, 0, 127));
    roles.insert(AreaLevelRole, 10);   
    roles.insert(IsAreaSelectableRole, true);
    m->setItemData(index, roles);
}
