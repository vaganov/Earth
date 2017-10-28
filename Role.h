#ifndef _ROLE_H
#define _ROLE_H

#include <Qt>

enum { ContourPolygonRole = Qt::UserRole, ContourColorRole,
       ContourLevelRole, LineWidthRole, IsContourSelectableRole,
       RoutePolygonRole, RouteColorRole, RouteLevelRole, IsRouteSelectableRole, RouteWidthRole,
       AreaPolygonRole, AreaColorRole, AreaLevelRole, IsAreaSelectableRole,
       ImageRole, ImageLevelRole, IsImageSelectableRole, ImageCoordinatesRole,
       TextRole, TextLevelRole, IsTextSelectableRole, TextCoordinatesRole };

#endif

