Project to use a 2 axis plasma cutter

All XML handling is done on the pugixml lib. Works like a charm
This software includes pugixml library (http://pugixml.org).
pugixml is Copyright (C) 2006-2014 Arseny Kapoulkine. It is included in this project as a submodule.
During make install this submodule is retrieved and compiled.

The polygon getSurface algorithm is from http://geomalgorithms.com/a01-_area.html.

The GUI uses QT 5.5. It is necessary that qmake is available in the path

The arduino sketch is made using a package arduino-mk 1.3.1-1. This package will bring the header/compiler for the avr etc.

