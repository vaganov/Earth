#include <math.h>
#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/glu.h>
#include <QtGui>
#include <sphere.h>
#include "GeoView.h"
#include "Polygon.h"
#include "Route.h"
#include "Role.h"
#include "value.h"
#include "metatype.h"

#include "debug.h"

using namespace Sphere;

GeoView::GeoView (double phi, double psi, QWidget* parent) : QGLWidget(parent) {
    setAttribute(Qt::WA_AlwaysShowToolTips);
    setMouseTracking(true);
    scale = max_scale;
    model = 0;
    phiCenter = phi;
    psiCenter = psi;
    vecState = VS_Off;
    freeze = false;
#ifdef _DEBUG
    unsigned int i;
    for (i = 0; i < AC; ++i) {
        phiAC[i] = (drand48() - 0.5) * M_PI;
        psiAC[i] = (drand48() - 0.5) * 2 * M_PI;
    }
#endif
}

void GeoView::setModel (TreeModel* p) {
    model = p;
    connect(model, SIGNAL(checked(const QModelIndex&)),
            this, SLOT(check(const QModelIndex&)));
}

void GeoView::initializeGL (void) {
    glEnable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);	
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    listEarth = glGenLists(1);
    glNewList(listEarth, GL_COMPILE);
    
    GLUquadric* Q = gluNewQuadric();
    gluQuadricDrawStyle(Q, GLU_FILL);
    glColor4f(0.62, 0.62, 0.67, 1.0);
    gluSphere(Q, 1, 240, 180);
    gluDeleteQuadric(Q);

    glEndList();

    Route w1(10 / R); // 10 км
    w1.addPoint(0, 0);
    w1.addPoint(M_PI_2, 0);
    w1.addPoint(0, 0.1);
    Route w2(10 / R); // 10 км
    w2.addPoint(0, 0.2);
    w2.addPoint(0, 0.3);
    w2.addPoint(M_PI_2, 0);
    listDebug = glGenLists(1);
    glNewList(listDebug, GL_COMPILE);
    glColor3f(1.0, 0.0, 0.0);
    w1.draw();
    w2.draw();
    glEndList();

    center(phiCenter, psiCenter);

    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_DOUBLE, 0, vec);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glMultMatrixd(P);
    glTranslated(-X0, -Y0, -Z0);
}

void GeoView::resizeGL (int width, int height) {
    float r;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (width < height) {
        r = height;
        r /= width;
        glOrtho(-scale, scale, -scale * r, scale * r, -1, 1);
    }
    else {
        r = width;
        r /= height;
        glOrtho(-scale * r, scale * r, -scale, scale, -1, 1);
    }
    glViewport(0, 0, width, height);
}

void GeoView::paintGL (void) {

    if (freeze) return;

    glClear(GL_COLOR_BUFFER_BIT);
    glCallList(listEarth);
    drawLists(false);
/*
    drawLists(selectableFillLists);
    drawLists(unselectableFillLists);
    drawLists(selectableContourLists);
    drawLists(unselectableContourLists);
*/
    if ((vecState == VS_Draw) || (vecState == VS_Done)) {
        glColor3f(0.0, 0.0, 0.0);
        glLineWidth(2.0);
        drawVec();
    }
//    glCallList(listDebug);
#ifdef _DEBUG
    unsigned int i, j;
    double X, Y, Z;
    double x[16], y[16], z[16];
    glColor3f(1.0, 1.0, 0.0);
    for (i = 0; i < AC; ++i) {
        Sphere::XYZ(phiAC[i], psiAC[i], X, Y, Z);
        Sphere::circle(X, Y, Z, scale * 0.005, 16, x, y, z);
        glBegin(GL_TRIANGLE_FAN);
        glVertex3f(X, Y, Z);
        for (j = 0; j < 16; ++j) {
            glVertex3f(x[j], y[j], z[j]);
        }
        glVertex3f(x[0], y[0], z[0]);
        glEnd();
    }
#endif
}

bool GeoView::event (QEvent* evt) {
    if (evt->type() == QEvent::ToolTip) {
        toolTipEvent((QHelpEvent*) evt);
        return true;
    }
    else {
        return QGLWidget::event(evt);
    }
}

void GeoView::toolTipEvent (QHelpEvent* evt) {
    if (vecState != VS_Draw) {
        pick(evt);
    }
}

void GeoView::mousePressEvent (QMouseEvent* evt) {
    double x, y;
    double X, Y, Z;
    if (evt->button() == Qt::MidButton) {
        map(evt->x(), evt->y(), x, y);
        start_rot(x, y);
    }
    if (evt->button() == Qt::LeftButton) {
        switch (vecState) {
            case VS_Off:
                vecState = VS_Start;
                map(evt->x(), evt->y(), x, y);
                coord(x, y, X, Y, Z);
                vec[0] = X;
                vec[1] = Y;
                vec[2] = Z;
                break;
            case VS_Draw:
                vecState = VS_Done;
                break;
            case VS_Done:
                vecState = VS_Off;
                updateGL();
                break;
            default:
                break;
        }
    }
}

void GeoView::mouseMoveEvent (QMouseEvent* evt) {
    double x, y;
    double X, Y, Z;
    if (evt->buttons() & Qt::MidButton) {
        map(evt->x(), evt->y(), x, y);
        rot(x, y);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glMultMatrixd(P);
        glTranslated(-X0, -Y0, -Z0);
        updateGL();
    }
    else {
        if (vecState == VS_Start) {
            vecState = VS_Draw;
        }
        if (vecState == VS_Draw) {
            map(evt->x(), evt->y(), x, y);
            coord(x, y, X, Y, Z);
            geodesic(vec[0], vec[1], vec[2], X, Y, Z, vecSize, vec);
            vec[3 * vecSize] = X;
            vec[3 * vecSize + 1] = Y;
            vec[3 * vecSize + 2] = Z;
            vecHint = QString::number(R * rho(vec[0], vec[1], vec[2], X, Y, Z)) + " км";
            QToolTip::showText(evt->globalPos(), vecHint);
            updateGL();
        }
    }
}

void GeoView::mouseReleaseEvent (QMouseEvent*) {
/*
#ifndef _DEBUG
    north();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glMultMatrixd(P);
    glTranslated(-X0, -Y0, -Z0);
    updateGL();
#endif
*/
}

void GeoView::wheelEvent (QWheelEvent* evt) {
    int w = width();
    int h = height();
    int D = evt->delta();
    float r;
    const double factor = 1.1;
    if (D > 0) {
        scale /= factor;
        if (scale < min_scale) {
            scale = min_scale;
        }
    }
    if (D < 0) {
        scale *= factor;
        if (scale > max_scale) {
            scale = max_scale;
        }
    }
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (w < h) {
        r = h;
        r /= w;
        glOrtho(-scale, scale, -scale * r, scale * r, -1, 1);
    }
    else {
        r = w;
        r /= h;
        glOrtho(-scale * r, scale * r, -scale, scale, -1, 1);
    }
    updateGL();
}

void GeoView::drawLists (bool selectableOnly) {

    foreach (QList<int> list, hashLevelId) {
        foreach (int id, list) {
            if (hashIdEnabled[id] && (!selectableOnly || hashIdSelectable[id])) {
                glLoadName(id); // игнорируется в режиме GL_RENDER
                glCallList(id);
            }
        }
    }
}

void GeoView::drawVec (void) {

    glLoadName(0); // игнорируется в режиме GL_RENDER
    glPushName(1); // игнорируется в режиме GL_RENDER
    glDrawArrays(GL_LINE_STRIP, 0, vecSize + 1);
    glPopName(); // игнорируется в режиме GL_RENDER
}

void GeoView::pick (QHelpEvent* evt) {
    double x, y;
    double X, Y, Z;
    double phi, psi;
    double eq[4];
    int i, hits;
    unsigned int j, names;
    unsigned int* p = selectBuf;
    map(evt->x(), evt->y(), x, y);
    QString hint;
    if (coord(x, y, X, Y, Z)) {
        S2(X, Y, Z, phi, psi);
        hint = deg(phi, psi);
        eq[3] = sens * scale;
        glSelectBuffer(selectBufSize, selectBuf);
        glRenderMode(GL_SELECT);
        glInitNames();
        glPushName(0);
        eq[0] = 0;
        eq[1] = -Z;
        eq[2] = Y;
        glClipPlane(GL_CLIP_PLANE0, eq);
        glEnable(GL_CLIP_PLANE0);
        eq[0] = 0;
        eq[1] = Z;
        eq[2] = -Y;
        glClipPlane(GL_CLIP_PLANE1, eq);
        glEnable(GL_CLIP_PLANE1);
        eq[0] = Z;
        eq[1] = 0;
        eq[2] = -X;
        glClipPlane(GL_CLIP_PLANE2, eq);
        glEnable(GL_CLIP_PLANE2);
        eq[0] = -Z;
        eq[1] = 0;
        eq[2] = X;
        glClipPlane(GL_CLIP_PLANE3, eq);
        glEnable(GL_CLIP_PLANE3);
        eq[0] = -Y;
        eq[1] = X;
        eq[2] = 0;
        glClipPlane(GL_CLIP_PLANE4, eq);
        glEnable(GL_CLIP_PLANE4);
        eq[0] = Y;
        eq[1] = -X;
        eq[2] = 0;
        glClipPlane(GL_CLIP_PLANE5, eq);
        glEnable(GL_CLIP_PLANE5);
//      drawLists(selectableFillLists);
        drawLists(true);
        if (vecState == VS_Done) {
            drawVec();
        }
        glDisable(GL_CLIP_PLANE0);
        glDisable(GL_CLIP_PLANE1);
        glDisable(GL_CLIP_PLANE2);
        glDisable(GL_CLIP_PLANE3);
        glDisable(GL_CLIP_PLANE4);
        glDisable(GL_CLIP_PLANE5);
        hits = glRenderMode(GL_RENDER);
#ifdef _DEBUG
        cout << "hits = " << hits << "\n";
#endif
        for (i = 0; i < hits; i++) {
            names = *p++;
#ifdef _DEBUG
            cout << "names = " << names << "\n";
#endif
            if (names == 2) {
                hint = vecHint;
                break;
            }
#ifdef _DEBUG
            cout << "z1 = " << (float) *p / 0x7FFFFFFF << "\n";
#endif
            p++;
#ifdef _DEBUG
            cout << "z2 = " << (float) *p / 0x7FFFFFFF << "\n";
#endif
            p++;
            hint += "\n";
//          hint += model->data(selectableFillLists[*p]).toString();
            hint += model->data(hashIdIndex[*p]).toString();
            for (j = 0; j < names; j++) {
#ifdef _DEBUG
                cout << "names[" << j << "] = " << *p << "\n";
#endif
                p++;
            }
        }
        QToolTip::showText(evt->globalPos(), hint);
    }
    else {
        QToolTip::hideText();
    }
}

void GeoView::map (int X, int Y, double& x, double& y) const {
    int w = width();
    int h = height();
    double m = qMin(w, h) / 2.;
    double w2 = w / 2.;
    double h2 = h / 2.;
    x = scale * (X - w2) / m;
    y = scale * (h2 - Y) / m;
}

QString GeoView::deg (double phi, double psi) {
    QString res;
    int num;
    if (phi == M_PI / 2) {
        return "С 90°0'0''";
    }
    if (phi == -M_PI / 2) {
        return "Ю 90°0'0''";
    }
    if (phi < 0) {
        res = "Ю ";
        phi = -phi;
    }
    else {
        res = "С ";
    }
    phi *= 180 / M_PI;
    num = (int) floor(phi);
    res += QString::number(num);
    res += "°";
    phi -= num;
    phi *= 60;
    num = (int) floor(phi);
    res += QString::number(num);
    res += "'";
    phi -= num;
    phi *= 60;
/*
    num = (int) floor(phi);
    res += QString::number(num);
*/
    res += QString::number(phi);
    res += "''";
    res += ", ";
    if (psi < 0) {
        res += "З ";
        psi = -psi;
    }
    else {
        res += "В ";
    }
    psi *= 180 / M_PI;
    num = (int) floor(psi);
    res += QString::number(num);
    res += "°";
    psi -= num;
    psi *= 60;
    num = (int) floor(psi);
    res += QString::number(num);
    res += "'";
    psi -= num;
    psi *= 60;
/*
    num = (int) floor(psi);
    res += QString::number(num);
*/
    res += QString::number(psi);
    res += "''";
    return res;
}

void GeoView::appendList(int level, int id, const QModelIndex &index, int role,
        bool selectable, bool enabled) {

//    qDebug() << "GeoView::appendList";

    hashLevelId[level].append(id);
    hashIdSelectable[id] = selectable;
    hashIdEnabled[id] = enabled;
    hashIndexRoleId[index][role] = id;
    hashIdIndex[id] = index;
}

void GeoView::deleteList (const QModelIndex &index) {

    if (!hashIndexRoleId.contains(index)) {
        return;
    }

    foreach (int role, hashIndexRoleId[index].keys()) {
        qDebug() << role;
        deleteList(index, role);
    }
}
void GeoView::deleteList (const QModelIndex &index, int role) {

    if (!hashIndexRoleId.contains(index) || !hashIndexRoleId[index].contains(role)) {
        return;
    }

    int id = hashIndexRoleId[index][role];
    foreach(QList<int> list, hashLevelId) {
        list.removeOne(id);
    }

    hashIdSelectable.remove(id);
    hashIdEnabled.remove(id);
    hashIndexRoleId[index].remove(role);
    hashIdIndex.remove(id);

    glDeleteLists(id, 1);
}

void GeoView::enableList (const QModelIndex &index, bool enabled) {

    if (!hashIndexRoleId.contains(index)) {
//        qWarning() << "GeoView::enableList: invalid index";
        return;
    }

//    qDebug() << "GeoView::enableList";

    foreach(int id, hashIndexRoleId[index]) {
        hashIdEnabled[id] = enabled;
    }
}

void GeoView::check (const QModelIndex&) {
    updateGL();
}

void GeoView::onDataRolesChanged(const QModelIndex &index, const QList<int> &roles) {

    if (!index.isValid()) {
        qWarning() << "GeoView::onDataChanged: index is not valid";
        return;
    }
    makeCurrent();
    QMap<int, QVariant> itemData = model->itemData(index);
    bool enabled = value<int>(itemData[Qt::CheckStateRole], Qt::Checked);
    bool hasContour = roles.contains(ContourPolygonRole);
    bool hasArea = roles.contains(AreaPolygonRole);
    bool hasRoute = roles.contains(RoutePolygonRole);
    bool hasImage = roles.contains(ImageRole);
    bool hasText = roles.contains(TextRole);
    if (hasContour) {
        deleteList(index, ContourPolygonRole);
        QList<QPolygonF> polygons
            = itemData[ContourPolygonRole].value<QList<QPolygonF> >();
        foreach (QPolygonF polygon, polygons) {
            if (polygon.size() < 2) {
                qWarning() << "GeoView::onDataChanged: contour size < 2";
            } else {
                QColor color = itemData[ContourColorRole].value<QColor>();
                float lineWidth = value<float>(itemData[LineWidthRole], 1.0);
                int level = itemData[ContourLevelRole].toInt();
                bool isSelectable = itemData[IsContourSelectableRole].toBool();
                int listNum;
                drawPolygon(polygon, color, lineWidth, false, listNum);
                appendList(level, listNum, index, ContourColorRole, isSelectable, enabled);
//              updateGL();
            }
        }
    }
    if (hasArea) {
        deleteList(index, AreaPolygonRole);
        QList<QPolygonF> polygons
            = itemData[AreaPolygonRole].value<QList<QPolygonF> >();
        foreach (QPolygonF polygon, polygons) {
            if (polygon.size() < 3) {
                qWarning() << "GeoView::onDataChanged: area size < 3";
            } else {
                QColor color = itemData[AreaColorRole].value<QColor>();
                int level = itemData[AreaLevelRole].toInt();
                bool isSelectable = itemData[IsAreaSelectableRole].toBool();
                int listNum;
                drawPolygon(polygon, color, 0, true, listNum);
                appendList(level, listNum, index, AreaPolygonRole, isSelectable, enabled);
//              updateGL();
            }
        }
    }
    if (hasRoute) {
        deleteList(index, RoutePolygonRole);
        QPolygonF polygon = itemData[RoutePolygonRole].value<QPolygonF>();
        if (polygon.size() < 2) {
            qWarning() << "GeoView::onDataChanged: route size < 2";
        } else {
            QColor color = itemData[RouteColorRole].value<QColor>();
            float r = color.redF();
            float g = color.greenF();
            float b = color.blueF();
            float a = color.alphaF();
            int level = itemData[RouteLevelRole].toInt();
            bool isSelectable = itemData[IsRouteSelectableRole].toBool();
            int width = itemData[RouteWidthRole].toInt();
            Route route(width / R);
            foreach (QPointF point, polygon) {
                route.addPoint(point.x(), point.y());
            }
            int listNum = glGenLists(1);
            if (!listNum) {
                qDebug() << "GeoView::drawPolygon: glGenLists returned 0";
            }
            glNewList(listNum, GL_COMPILE);
            glColor4f(r, g, b, a);
            route.draw();
            glEndList();
            appendList(level, listNum, index, RoutePolygonRole, isSelectable, enabled);
        }
    }
    if (hasText) {
        deleteList(index, TextRole);
        QString text = itemData[TextRole].toString();
        if (text.isEmpty()) {
            qWarning() << "GeoView::onDataChanged: text is empty";
        } else {
            QPointF coordinates = itemData[TextCoordinatesRole].value<QPointF>();
            double x, y, z;
            XYZ(coordinates.x(), coordinates.y(), x, y, z);
            int level = value<int>(itemData[TextLevelRole], 0);
            bool isSelectable = itemData[IsTextSelectableRole].toBool();

            QPainter pr;
            QFont font;
            font.setPixelSize(24);
            QFontMetrics fm(font);
            QRect rect = fm.boundingRect(text);
                    
            int xOffset = 10, yOffset = 5;

            QSize imgSize = QSize(rect.size().width() + 1 + xOffset,
                    rect.size().height() + 1 + yOffset);
            QImage img(imgSize, QImage::Format_ARGB32);
            img.fill(0);
            QPoint textPoint = QPoint(0 + xOffset, fm.ascent() + yOffset);
                        
            pr.begin(&img);
            pr.setPen(QColor(0, 0, 0));
            pr.setFont(font);
            pr.drawText(textPoint, text);
            pr.end();                        

            img = img.mirrored(false, true);

            int listNum = glGenLists(1);
            if (!listNum) {
                qDebug() << "GeoView::drawPolygon: glGenLists returned 0";
            }
            glNewList(listNum, GL_COMPILE);
            glRasterPos3f(x, y, z);
            glDrawPixels(img.width(), img.height(), GL_BGRA, GL_UNSIGNED_BYTE,
                    (GLvoid *) img.bits());
            glEndList();
            appendList(level, listNum, index, TextRole, isSelectable, enabled);
//            updateGL();
        }
    }
    if (hasImage) {
        deleteList(index, ImageRole);
        QImage image = itemData[ImageRole].value<QImage>();
        if (image.isNull()) {
            qWarning() << "GeoView::onDataChanged: image is null";
        } else {
            image = image.convertToFormat(QImage::Format_Mono,
                    Qt::MonoOnly | Qt::ThresholdDither
                    | Qt::ThresholdAlphaDither | Qt::PreferDither)
                    .transformed(QMatrix(1, 0, 0, -1, 0, 0));
            QPointF coordinates = itemData[ImageCoordinatesRole].value<QPointF>();
            double x, y, z;
            XYZ(coordinates.x(), coordinates.y(), x, y, z);
            int level = value<int>(itemData[ImageLevelRole], 0);
            bool isSelectable = itemData[IsImageSelectableRole].toBool();
            int w = image.width();
            int h = image.height();
            GLubyte *bits = (GLubyte *)image.bits();

            int listNum = glGenLists(1);
            if (!listNum) {
                qDebug() << "GeoView::drawPolygon: glGenLists returned 0";
            }
            glNewList(listNum, GL_COMPILE);
            glRasterPos3f(x, y, z);
            glColor4f(0, 0, 0, 1);
            glBitmap(w, h, w / 2, h / 2, 0.0, 0.0, bits);
            glEndList();
            appendList(level, listNum, index, ImageRole, isSelectable, enabled);
//            updateGL();
        }
    }
    if (roles.contains(Qt::CheckStateRole) && roles.size() == 1) {
        enableList(index, enabled);
//        updateGL();
    }
}

void GeoView::drawPolygon(const QPolygonF &polygon, const QColor &color,
        float lineWidth, bool filled, int &listNum) {

    bool  isClosed = polygon.isClosed();
    float r = color.redF();
    float g = color.greenF();
    float b = color.blueF();
    float a = color.alphaF();
            
    Polygon spherePolygon;
    QPolygonF::const_iterator i   = polygon.begin();
    QPolygonF::const_iterator end = polygon.end() - (isClosed ? 1 : 0);
    spherePolygon.start(i->x(), i->y());
    for (++i; i != end; ++i) {
        spherePolygon.geodesic(i->x(), i->y());
    }
    if (isClosed) {
        if (!spherePolygon.geodesic()) {
            qDebug() << "GeoView::drawPolygon: geodesic returned false";
        }
    }
    listNum = glGenLists(1);
    if (!listNum) {
        qDebug() << "GeoView::drawPolygon: glGenLists returned 0";
    }
    glNewList(listNum, GL_COMPILE);
    glColor4f(r, g, b, a);
    if (filled) {
        spherePolygon.drawFill();
    } else {
        glLineWidth(lineWidth);
        spherePolygon.drawContour();
    }
    glEndList();
}

void GeoView::setFreeze(bool value) {

    freeze = value;
    if (!freeze) {
        updateGL();
    }
}

#ifdef _DEBUG
void GeoView::moveAC (void) {
    unsigned int i;
    for (i = 0; i < AC; ++i) {
        phiAC[i] += (drand48() - 0.5) * 0.005;
        psiAC[i] += drand48() * 0.005;
    }
    updateGL();
}
#endif

void GeoView::onRowsRemoved (const QModelIndex &parent, int start, int end) {

    qDebug() << "GeoView::onRowsRemoved: parent:" << (parent.isValid() ? model->data(parent).toString() : "invalid");

    for (int row = start; row <= end; ++row) {
        QModelIndex index = parent.child(row, 0);
        qDebug() << "index:" << (index.isValid() ? model->data(index).toString() : "invalid");
        deleteList(index);
//        deleteList(parent.child(row, 0));
    }
    updateGL();
}

