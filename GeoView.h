#ifndef _GEOVIEW_H
#define _GEOVIEW_H

#include <QtOpenGL>
#include "TreeModel.h"

#include "debug.h"

class GeoView : public QGLWidget {
    Q_OBJECT
private:
    unsigned int listEarth;
    unsigned int listDebug;
    
    typedef enum {VS_Off, VS_Start, VS_Draw, VS_Done} VecState;

    QHash< int, QList<int> > hashLevelId;
    QHash<int, bool> hashIdSelectable;
    QHash<int, bool> hashIdEnabled;
    QHash<QModelIndex, QHash<int, int> > hashIndexRoleId;
    QHash<int, QModelIndex> hashIdIndex;

    static const double min_scale = 0.001;
    static const double max_scale = 0.5;
    double phiCenter;
    double psiCenter;
    double scale;
    static const double sens = 0.005;
    TreeModel* model;
    static const unsigned int selectBufSize = 1024;
    unsigned int selectBuf[selectBufSize];
    static const unsigned int vecSize = 128;
    double vec[3 * (vecSize + 1)];
    VecState vecState;
    QString vecHint;
    static const double R = 6371;
    bool freeze;
#ifdef _DEBUG
    static const unsigned int AC = 10000;
    double phiAC[AC];
    double psiAC[AC];
#endif
public:
    GeoView (double, double, QWidget* = 0);
    void setModel (TreeModel*);
protected:
    void initializeGL (void);
    void resizeGL (int, int);
    void paintGL (void);
    bool event (QEvent*);
    void toolTipEvent (QHelpEvent*);
    void mousePressEvent (QMouseEvent*);
    void mouseMoveEvent (QMouseEvent*);
    void mouseReleaseEvent (QMouseEvent*);
    void wheelEvent (QWheelEvent*);
private:
    void appendList (int, int, const QModelIndex &, int, bool, bool);
    void deleteList (const QModelIndex &);
    void deleteList (const QModelIndex &, int);
    void enableList (const QModelIndex &, bool);
    void drawLists (bool);
    void drawVec (void);
    void pick (QHelpEvent*);
    void map (int, int, double&, double&) const;
    static QString deg (double, double);
    void drawPolygon(const QPolygonF &, const QColor &, float, bool, int &);
#ifdef _DEBUG
public slots:
    void moveAC (void);
#endif
private slots:
    void check (const QModelIndex&);
//    void onDataChanged (const QModelIndex &index, const QModelIndex &);
//    void onDataRoleChanged (const QModelIndex &index, int role);
    void onDataRolesChanged (const QModelIndex &index, const QList<int> &role);
    void setFreeze (bool);
    void onRowsRemoved (const QModelIndex &, int, int);
};

#endif
