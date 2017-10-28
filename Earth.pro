QT = core gui widgets opengl
TEMPLATE = app
TARGET = map-bin
INCLUDEPATH += S2

SOURCES += main.cpp
HEADERS += metatype.h
HEADERS += debug.h
HEADERS += TreeNode.h
HEADERS += TreeModel.h
SOURCES += TreeModel.cpp
HEADERS += GeoModel.h
SOURCES += GeoModel.cpp
HEADERS += AbstractPainter.h
SOURCES += AbstractPainter.cpp
HEADERS += AbstractLoader.h
SOURCES += AbstractLoader.cpp
HEADERS += ShapeLoader.h
SOURCES += ShapeLoader.cpp
HEADERS += DefaultPainter.h
SOURCES += DefaultPainter.cpp
HEADERS += Polygon.h
SOURCES += Polygon.cpp
HEADERS += Route.h
SOURCES += Route.cpp

LIBS += S2/libsphere.a
