arstudio
=====

Computer vision algorithm debugging tool.

Dependencies
=====

- Boost (system, filesystem)
- Qt4 (QtCore QtGui QtOpenGL QtXml QtTest)
- OpenGL
- OpenCV

This project is built with CMake. Features from C++11 are used here and there, so you'll need a compiler that supports it.

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
