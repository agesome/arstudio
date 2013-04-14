arstudio
=====

Computer vision algorithm debugging tool.

Repository structure
=====

- docs/ 			-- documentation;
- git-hooks/ 	-- useful git hooks to use when working on this project;
- src/ 				-- the source three;
- submodules/ -- contains KinectRecorder, a dependency to support .kinvideo files.

Dependencies
=====

- Boost (system, filesystem)
- Qt4 (QtCore QtGui QtOpenGL QtXml QtTest)
- OpenGL
- OpenCV

To generate documentation, optional:

- Doxygen
- Graphviz

This project is built with CMake. Features from C++11 are used here and there,
so you'll need a compiler that supports it.

Cloning & Compiling
=====

	$ git clone git://github.com/age/arstudio.git
	$ cd arstudio
	$ git submodule init
	$ git submodule update
	$ cd src
	$ mkdir build
	$ cd build
	$ cmake ..
	$ make [-jN]
	$ ./arstudio

Building documentation
=====

	$ cd docs
	$ doxygen
	$ xdg-open html/index.html
