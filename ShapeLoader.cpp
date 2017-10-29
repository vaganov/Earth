#include "ShapeLoader.h"
#include "Role.h"
#include "metatype.h"

//Q_REGISTER_METATYPE(QPolygonF)

ShapeLoader::ShapeLoader(const QString &fileName, GeoModel *model,
               AbstractPainter *painter, QObject *parent)
: AbstractLoader(model, painter, parent), fileName(fileName) {
}

bool ShapeLoader::load() {

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
//        myWarning() << "can't open file" << fileName;
        return false;
    }

    QTextStream stream(&file);
    stream.setCodec(QTextCodec::codecForName("UTF-8"));
//    stream.readLine();

    QMap<QString, QMap<int, QMap<int, QPolygonF> > > borders;
    QMap<QString, QMap<int, QMap<int, QPolygonF> > > internalBorders;
    QMap<QString, QMap<int, QPolygonF> > water;
    QMap<QString, QMap<int, QMap<int, QPolygonF> > > islands;

    {
        double lat, lon, lat0 = 0, lon0 = 0;
        int id, id2, id0 = 0, id20 = 0;
        QString name, name0;
        int repeatCounter = 0;
        bool loop = false;

        while (!stream.atEnd()) {
            if (!readRecord(stream, lat, lon, id, id2, name)) {
//                myWarning() << "readRecord returned false";
                continue;
            }
            if (lat == lat0 && lon == lon0) {
                loop = true;
            } else if (loop) {
                repeatCounter++;
                lat0 = lat;
                lon0 = lon;
                loop = false;
            }
            if (id != id0 || id2 != id20) {
                repeatCounter = 0;
                id0 = id;
                id20 = id2;
                lat0 = lat;
                lon0 = lon;
                loop = false;
            }

            if (fabs(lat) == 90 || (fabs(lon) == 180 && lat < 0)) {
                continue;
            }

            QPointF point(M_PI * lat / 180, M_PI * lon / 180);
            borders[name][id][repeatCounter].append(point);
/*            if (id2 == 99999) {
                if (repeatCounter) {
                    //qDebug() << "repeatCounter:" << repeatCounter << "count:" << islands[name][id][repeatCounter].count();
                    islands[name][id][repeatCounter].append(point);
                } else {
                    water[name][id].append(point);
                }
            } else {
                if (repeatCounter) {
                    internalBorders[name][id][repeatCounter].append(point);
                } else {
                    borders[name][id].append(point);
                }
            }*/
        }
    }

    qRegisterMetaType<QPolygonF>();
    qRegisterMetaType<QList<QPolygonF> >();

    QModelIndex idx1 = model->addRow();
    model->setData(idx1, "Страны");
    model->setData(idx1, Qt::Checked, Qt::CheckStateRole);

    foreach (QString name, borders.keys()) {
        QModelIndex idx2 = model->addRow(idx1);
        model->setData(idx2, name);
        model->setData(idx2, Qt::Checked, Qt::CheckStateRole);
        QList<QPolygonF> list;
        foreach (int id, borders[name].keys()) {
            foreach (int counter, borders[name][id].keys()) {
//                qDebug() << name << id << polygon.count() << polygon.isClosed();
                list.append(borders[name][id][counter]);
            }
        }
        QVariant variant;
        variant.setValue< QList<QPolygonF> >(list);
        model->setData(idx2, variant, AreaPolygonRole);
    }
/*
    QModelIndex idx2 = model->addRow(idx1);
    model->setData(idx2, "Внутр. водоемы");
    model->setData(idx2, Qt::Checked, Qt::CheckStateRole);

    foreach (QString name, water.keys()) {
        QModelIndex idx3 = model->addRow(idx2);
        model->setData(idx3, name);
        model->setData(idx3, Qt::Checked, Qt::CheckStateRole);
        QList<QPolygonF> list;
        foreach (int id, water[name].keys()) {
            list.append(water[name][id]);
        }
        QVariant variant;
        variant.setValue< QList<QPolygonF> >(list);
        model->setData(idx3, variant, ReservoirRole);
    }

    foreach (QString name, islands.keys()) {
        QModelIndex idx3;
        idx3 = model->find(name, idx2);
//        idx3 = model->addRow(idx2);
//        model->setData(idx3, name);
//        model->setData(idx3, Qt::Checked, Qt::CheckStateRole);
        if (idx3.isValid()) {
            QList<QPolygonF> list;
            foreach (int id, islands[name].keys()) {
                foreach (int counter, islands[name][id].keys()) {
                    list.append(islands[name][id][counter]);
                }
            }
            QVariant variant;
            variant.setValue< QList<QPolygonF> >(list);
            model->setData(idx3, variant, IslandRole);
        }
    }

//    idx2 = model->addRow(idx1);
//    model->setData(idx2, "Острова");
//    model->setData(idx2, Qt::Checked, Qt::CheckStateRole);
//
//    QList<QPolygonF> list;
//    foreach (QString name, water.keys()) {
//        foreach (int id, islands[name].keys()) {
//            foreach (int counter, islands[name][id].keys()) {
//                QPolygonF plygon = islands[name][id][counter];
//                list.append(plygon);
//            }
//        }
//    }
//    QVariant variant;
//    variant.setValue< QList<QPolygonF> >(list);
//    model->setData(idx2, variant, IslandRole);
*/
    return true;
}

bool ShapeLoader::readRecord(QTextStream &stream, double &lat, double &lon,
        int &id, int &idAux, QString& name) {

    QStringList list = stream.readLine().split('\t'/*, QString::KeepEmptyParts*/);

    bool ok;
    lon = list[1].toDouble(&ok);
    if (!ok) {
//        myWarning() << "Error parsing longitude" << list[1];
        return false;
    }
    lat = list[2].toDouble(&ok);
    if (!ok) {
//        myWarning() << "Error parsing lattitude" << list[2];
        return false;
    }
    id = list[5].toUInt(&ok);
    if (!ok) {
//        myWarning() << "Error parsing id" << list[7];
        return false;
    }
    idAux = id;
    name = list[6];
    return true;
}
