#ifndef WAYPOINT_H
#define WAYPOINT_H

class Waypoint {
public:
    enum Type {TYPE_INVALID, TYPE_VORDME, TYPE_NDB, TYPE_AIRPORT, TYPE_WAYPOINT };
    Waypoint() {}
    Waypoint(const QString &name, const QString &fullName, Waypoint::Type type,
    const QPointF &coordinates)
    : name(name), fullName(fullName), type(type), coordinates(coordinates) { }
    QString name;
    QString fullName;
    Type type;
    QPointF coordinates;
};

#endif // WAYPOINT_H
