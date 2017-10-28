#ifndef SHAPE_LOADER_H
#define SHAPE_LOADER_H

#include "AbstractLoader.h"
#include <QtCore>
#include "GeoModel.h"

class ShapeLoader : public AbstractLoader {

public:
	ShapeLoader(const QString &fileName, GeoModel *model,
	           AbstractPainter *painter, QObject *parent = 0);
	virtual bool load();
	
private:
    struct ShapePolygon {
        QString name;
        QPolygonF polygon;
        bool reservoir;
    };

	QString fileName;
	bool readRecord(QTextStream &stream, double &lat, double &lon, int &id,
	       int &idAux, QString& name);
};

#endif // SHAPE_LOADER_H
