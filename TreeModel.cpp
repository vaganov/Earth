#include "TreeModel.h"
#include "metatype.h"
#include "debug.h"

TreeModel::TreeModel (QObject* parent) : QAbstractItemModel(parent) {
    root = new TreeNode;
    root->parent = 0;
}

TreeModel::~TreeModel (void) {
#ifdef _DEBUG
    cout << "TreeModel::~TreeModel(): removing tree...\n";
#endif
    free(root);
#ifdef _DEBUG
    cout << "TreeModel::~TreeModel(): tree removed\n";
#endif
}

int TreeModel::rowCount (const QModelIndex& idx) const {
    TreeNode* node;
    if (idx == QModelIndex()) {
        node = root;
    }
    else {
        node = (TreeNode*) idx.internalPointer();
        node = node->children[idx.row()];
    }
    return node->children.size();
}

int TreeModel::columnCount (const QModelIndex&) const {
    return 1;
}

bool TreeModel::insertRows(int row, int count, const QModelIndex& parent) {
    int i;
    TreeNode* node;
    TreeNode* child;
    if ((count > 0) && (row >= 0)) {
        if (parent == QModelIndex()) {
            node = root;
        }
        else {
            node = (TreeNode*) parent.internalPointer();
            node = node->children[parent.row()];
        }
        if (row > node->children.size()) {
            return false;
        }
        else {
            beginInsertRows(parent, row, row + count - 1);
            for (i = 0; i < count; i++) {
                child = new TreeNode;
                child->parent = node;
                node->children.insert(row, child);
            }
            endInsertRows();
            return true;
        }
    }
    else {
        return false;
    }
}

bool TreeModel::removeRows(int row, int count, const QModelIndex& parent) {
    int i;
    TreeNode* node;
    TreeNode* child;
    if ((count > 0) && (row >= 0)) {
        if (parent == QModelIndex()) {
            node = root;
            qDebug() << "TreeModel::removeRows: node = root";
        }
        else {
            node = (TreeNode*) parent.internalPointer();
            node = node->children[parent.row()];
            qDebug() << "TreeModel::removeRows: node =" << node->roles[Qt::DisplayRole].toString();
        }
        if (row + count > node->children.size()) {
            qDebug() << "TreeModel::removeRows: row + count > node->children.size()";
            return false;
        }
        else {
            qDebug() << "TreeModel::removeRows: parent" << (parent.isValid() ? data(parent).toString(): "invalid");
            beginRemoveRows(parent, row, row + count - 1);
            endRemoveRows();
            qDebug() << "!!!";
            for (i = 0; i < count; i++) {
                child = node->children[row];
                node->children.removeAt(row);
                free(child);
            }
            return true;
        }
    }
    else {
        return false;
    }
}

QModelIndex TreeModel::index (int row, int column,
        const QModelIndex& parent) const {
    TreeNode* node;
    if (parent == QModelIndex()) {
        return createIndex(row, column, root);
    }
    else {
        node = (TreeNode*) parent.internalPointer();
        return createIndex(row, column, node->children[parent.row()]);
    }
}

QModelIndex TreeModel::parent (const QModelIndex& idx) const {
    TreeNode* node = (TreeNode*) idx.internalPointer();
    TreeNode* parent;
    int row;
    if (node == root) {
        return QModelIndex();
    }
    else {
        parent = node->parent;
        row = parent->children.indexOf(node);
        return createIndex(row, 0, parent);
    }
}

QVariant TreeModel::data (const QModelIndex& idx, int role) const {
    TreeNode* node;
    if (idx.isValid()) {
        node = (TreeNode*) idx.internalPointer();
    } else {
        node = root;
    }
    TreeNode* child = node->children[idx.row()];
    if (child->roles.contains(role)) {
        return child->roles[role];
    }
    else {
        return QVariant();
    }
}

QMap<int, QVariant> TreeModel::itemData (const QModelIndex &idx) const {
    TreeNode* node = (TreeNode*) idx.internalPointer();
    TreeNode* child = node->children[idx.row()];
    return child->roles;
}

bool TreeModel::setItemData (const QModelIndex& idx, const QMap<int,
        QVariant>& new_roles) {
    TreeNode* node = (TreeNode*) idx.internalPointer();
    TreeNode* child = node->children[idx.row()];
    QMap<int, QVariant>::const_iterator i, end = new_roles.end();
    int role;
    QList<int> roles;
    for (i = new_roles.begin(); i != end; ++i) {
        role = i.key();
        if (child->roles.contains(role)) {
            child->roles[role] = i.value();
        }
        else {
            child->roles.insert(role, i.value());
        }
        roles.append(role);
        emit dataRoleChanged(idx, role);
    }
    emit dataChanged(idx, idx);
    emit dataRolesChanged(idx, roles);
    return true;
}

bool TreeModel::setData (const QModelIndex& idx, const QVariant& val,
        int role) {
    TreeNode* node = (TreeNode*) idx.internalPointer();
    TreeNode* child = node->children[idx.row()];
    if (child->roles.contains(role)) {
        child->roles[role] = val;
    }
    else {
        child->roles.insert(role, val);
    }
    emit dataChanged(idx, idx);
    emit dataRoleChanged(idx, role);
    QList<int> roles;
    roles << role;
    emit dataRolesChanged(idx, roles);
    return true;
}

void TreeModel::free (TreeNode* node) {
    QList<TreeNode*>::const_iterator i, end = node->children.end();
    for (i = node->children.begin(); i != end; ++i) {
        free(*i);
    }
    delete node;
}

QModelIndex TreeModel::addRow(const QModelIndex& parent) {
    TreeNode* node;
    if (parent == QModelIndex()) {
        node = root;
    }
    else {
        node = (TreeNode*) parent.internalPointer();
        node = node->children[parent.row()];
    }
    int row = node->children.size();
    beginInsertRows(parent, row, row);
    TreeNode* child = new TreeNode;
    child->parent = node;
    node->children.append(child);
    endInsertRows();
    return createIndex(row, 0, node);
}

QModelIndex TreeModel::find(const QVariant &value,
        const QModelIndex &parentIndex, int role, bool recursive) {

    QStack<TreeNode *> stack;

    if (parentIndex.isValid()) {
        stack.push(((TreeNode *) parentIndex.internalPointer())->
                children[parentIndex.row()]);
    } else {
        stack.push(root);
    }
    while (!stack.isEmpty()) {
        TreeNode *node = stack.pop();
        foreach (TreeNode *child, node->children) {
            if (child->roles[role] == value) {
                return createIndex(node->children.indexOf(child), 0, node);
            }
            if (recursive) {
                stack.push(child);
            }
        }
    }    

    return QModelIndex();
}

