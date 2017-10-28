#include "AbstractLoader.h"

AbstractLoader::AbstractLoader(TreeModel *model, AbstractPainter *painter,
        QObject *parent)
: QObject(parent), model(model), painter(painter) {
}

AbstractLoader::~AbstractLoader() {
}
