#include "GeoModel.h"

GeoModel::GeoModel(QObject *parent) : TreeModel(parent) {
}

void GeoModel::toggle (const QModelIndex &currentIndex) {

    emit freeze(true);

    QMap<int, QVariant> indexData = itemData(currentIndex);

    if (!indexData.contains(Qt::CheckStateRole)) {
        return;
    }

    int state0 = indexData[Qt::CheckStateRole].toInt();
    int state = (state0 == Qt::Unchecked) ? Qt::Checked : Qt::Unchecked;

    setData(currentIndex, state, Qt::CheckStateRole);

    QList<QModelIndex> stack;
    stack.append(currentIndex);

    while (!stack.isEmpty()) {
        QModelIndex childIndex = stack.last();
        stack.removeLast();
        if (data(childIndex, Qt::CheckStateRole).isValid()) {
            setData(childIndex, state, Qt::CheckStateRole);
        } else {
            break;
        }
        int rows = rowCount(childIndex);
        for (int row = 0; row < rows; row++) {
            stack.append(index(row, 0, childIndex));
        }
    }

    QModelIndex parentIndex = currentIndex.parent();
    while (parentIndex.isValid()) {
        QMap<int, QVariant> indexData = itemData(parentIndex);
        if (!indexData.contains(Qt::CheckStateRole)) {
            break;
        }
        bool all = (state == Qt::Checked);
        bool noOne = (state == Qt::Unchecked);
        int rows = rowCount(parentIndex);
        for (int row = 0; row < rows; row++) {
            QVariant varCheckState =
                    data(parentIndex.child(row, 0), Qt::CheckStateRole);
            if (varCheckState.isValid()) {
                int childrenState = varCheckState.toInt();
                all = all && childrenState == Qt::Checked;
                noOne = noOne && childrenState == Qt::Unchecked;
            }
        }
        setData(parentIndex, all ? Qt::Checked : (noOne ? Qt::Unchecked : Qt::PartiallyChecked), Qt::CheckStateRole);
        parentIndex = parentIndex.parent();
    }

    emit freeze(false);
}

