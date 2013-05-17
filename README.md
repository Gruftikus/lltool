# lltool

![panorama](https://lh6.googleusercontent.com/-Q4p5qHEUWps/UXVzhYw-YgI/AAAAAAAAAdw/MTd5Gd1_TuM/s971/lltool.jpg)

## Landscape and Heightmap Editing Tool and Library

[Full manual](https://github.com/Gruftikus/lltool/wiki/) under construction.

### Overview

lltool provides several methods and algorithms for heightmap manipulation and terrain triangulation. Steering of the process is done via a batch script with a small and dedicated scripting language. The terrain trinagulation is done in two steps: in a first step vertices are placed, either based on vertex placing methods, or via (a combination) of density algorithms. In second step the vertices are triangulated and the resulting mesh can be further manipulated. lltool provides multi-level LODs, break lines and polygon stencils.

### Features

...some are not yet done!

* Map creation and manipulation: importers/exporters and terraforming
* Support for cells and quads (for game LODs)
* Multiple algorithms for random Monte-Carlo vertex placement, e.g. based on first and second order, height, peak finding, and general purpose quations
* Triangle manipulation: stencils and break lines
* Output formats: obj- and VRML-files

### Installation

Make a directory "externals". lltool needs:

* [Triangle](http://www.cs.cmu.edu/~quake/triangle.html) by J. R. Shewchuk, download the code and put it into externals/triangle/
* [MTParser](http://www.codeproject.com/Articles/7335/An-extensible-math-expression-parser-with-plug-ins) by Mathieu Jacques, download the code and put it into externals/mtparser (Windows only!!!)
 
Windows:

* Open lltool.sln and build the project.

Linux:

* Run "make"

### Syntax

...w.i.p

### Licence

Copyright (C) 2011-2013

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see http://www.gnu.org/licenses/.








