#ifndef ABSTRACTLOADER_H
#define ABSTRACTLOADER_H

#include <QtCore>
#include "TreeModel.h"
#include "AbstractPainter.h"

class AbstractLoader : public QObject {

public:
    AbstractLoader(TreeModel *model, AbstractPainter *painter, QObject *parent = 0);
    virtual ~AbstractLoader();
	virtual bool load() = 0;
	
protected:
    TreeModel *model;
    AbstractPainter *painter;
};

#endif // ABSTRACTLOADER_H
