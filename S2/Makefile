CXX = g++
CXXFLAGS = -I.
LDFLAGS = -mimpure-text -shared
AR = ar

obj = rot_M.o rot.o north.o center.o tr.o coord.o dot.o cross.o normalize.o between.o rho.o arc.o geodesic.o circle.o triangle.o fractal_aux.o fractal.o inside.o polygon.o loxodrome.o route_aux.o xtern.o

default: libsphere.a
rot_M.o: rot_M.cpp sphere.h sphere_private.h
	$(CXX) $(CXXFLAGS) -c rot_M.cpp -o rot_M.o
rot.o: rot.cpp sphere.h sphere_private.h debug.h
	$(CXX) $(CXXFLAGS) -c rot.cpp -o rot.o
north.o: north.cpp sphere.h sphere_private.h
	$(CXX) $(CXXFLAGS) -c north.cpp -o north.o
center.o: center.cpp sphere.h sphere_private.h
	$(CXX) $(CXXFLAGS) -c center.cpp -o center.o
tr.o: tr.cpp sphere.h sphere_private.h
	$(CXX) $(CXXFLAGS) -c tr.cpp -o tr.o
coord.o: coord.cpp sphere.h sphere_private.h
	$(CXX) $(CXXFLAGS) -c coord.cpp -o coord.o
dot.o: dot.cpp sphere.h sphere_private.h
	$(CXX) $(CXXFLAGS) -c dot.cpp -o dot.o
cross.o: cross.cpp sphere.h sphere_private.h
	$(CXX) $(CXXFLAGS) -c cross.cpp -o cross.o
normalize.o: normalize.cpp sphere.h sphere_private.h
	$(CXX) $(CXXFLAGS) -c normalize.cpp -o normalize.o
between.o: between.cpp sphere.h sphere_private.h
	$(CXX) $(CXXFLAGS) -c between.cpp -o between.o
rho.o: rho.cpp sphere.h sphere_private.h
	$(CXX) $(CXXFLAGS) -c rho.cpp -o rho.o
arc.o: arc.cpp sphere.h sphere_private.h debug.h
	$(CXX) $(CXXFLAGS) -c arc.cpp -o arc.o
geodesic.o: geodesic.cpp sphere.h sphere_private.h
	$(CXX) $(CXXFLAGS) -c geodesic.cpp -o geodesic.o
circle.o: circle.cpp sphere.h sphere_private.h
	$(CXX) $(CXXFLAGS) -c circle.cpp -o circle.o
triangle.o: triangle.cpp sphere.h sphere_private.h
	$(CXX) $(CXXFLAGS) -c triangle.cpp -o triangle.o
inside.o: inside.cpp sphere.h sphere_private.h
	$(CXX) $(CXXFLAGS) -c inside.cpp -o inside.o
polygon.o: polygon.cpp sphere.h sphere_private.h debug.h
	$(CXX) $(CXXFLAGS) -c polygon.cpp -o polygon.o
fractal_aux.o: fractal_aux.cpp fractal_aux.h
	$(CXX) $(CXXFLAGS) -c fractal_aux.cpp -o fractal_aux.o
fractal.o: fractal.cpp sphere.h sphere_private.h fractal_aux.h
	$(CXX) $(CXXFLAGS) -c fractal.cpp -o fractal.o
loxodrome.o: loxodrome.cpp sphere.h sphere_private.h
	$(CXX) $(CXXFLAGS) -c loxodrome.cpp -o loxodrome.o
route_aux.o: route_aux.cpp sphere.h sphere_private.h
	$(CXX) $(CXXFLAGS) -c route_aux.cpp -o route_aux.o
xtern.o: xtern.cpp sphere.h sphere_private.h
	$(CXX) $(CXXFLAGS) -c xtern.cpp -o xtern.o
libsphere.a: $(obj)
	$(AR) -rc libsphere.a $(obj)
clean:
	rm -f *~
	rm -f $(obj)
