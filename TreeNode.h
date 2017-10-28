#ifndef _TREENODE_H
#define _TREENODE_H

#include <QList>
#include <QMap>
#include <QVariant>

struct TreeNode {
    TreeNode* parent;
    QList<TreeNode*> children;
    QMap<int, QVariant> roles;
};

Q_DECLARE_METATYPE (TreeNode)

#endif

