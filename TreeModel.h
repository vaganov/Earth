#ifndef _TREEMODEL_H
#define _TREEMODEL_H

#include <QtCore>
#include "TreeNode.h"

class TreeModel : public QAbstractItemModel {
    Q_OBJECT
    TreeNode* root;
public:
    TreeModel (QObject* = 0);
    ~TreeModel (void);
    int rowCount (const QModelIndex& = QModelIndex()) const;
    int columnCount (const QModelIndex& = QModelIndex()) const;
    bool insertRows(int, int, const QModelIndex& = QModelIndex());
    bool removeRows(int, int, const QModelIndex& = QModelIndex());
//    bool removeRow (int, const QModelIndex & = QModelIndex());
    QModelIndex index (int, int, const QModelIndex& = QModelIndex()) const;
    QModelIndex parent (const QModelIndex&) const;
    QVariant data (const QModelIndex&, int = Qt::DisplayRole) const;
    QMap<int, QVariant> itemData (const QModelIndex &index) const;
    bool setItemData (const QModelIndex&, const QMap<int, QVariant>&);
    bool setData (const QModelIndex&, const QVariant&, int = Qt::DisplayRole);
    QModelIndex find(const QVariant &, const QModelIndex & = QModelIndex(), int = Qt::DisplayRole, bool = false);
private:
    void free (TreeNode*);
signals:
    void dataRoleChanged (const QModelIndex &, int);
    void dataRolesChanged (const QModelIndex &, const QList<int> &);
};

#endif

