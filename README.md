arstudio
=====

Computer vision algorithm debugging tool.

Repository structure
=====

	- docs/       -- documentation;
	- git-hooks/  -- useful git hooks to use when working on this project;
	- src/        -- the source three;
	- submodules/ -- contains KinectRecorder, a dependency to support .kinvideo files.
	- releases/   -- contains complete source archives of previous releases

Dependencies
=====

- Qt 5.1 (OpenGL, Quick; Widgets for deprecated 'widgets' branch)
- Qt 3D (forked for this project: https://gitorious.org/~evgeny/qt/qt3d-minor-fixes)
- OpenCV

To generate documentation, optional:

- Doxygen
- Graphviz

This project is built with CMake. Features from C++11 are used here and there,
so you'll need a compiler that supports it; arstudio has been built at least
with GCC 4.8.1 and Clang 3.3.

Build status: [![Build Status](https://travis-ci.org/age/arstudio.png?branch=master)](https://travis-ci.org/age/arstudio)

Cloning & Compiling
=====

	$ git clone git://github.com/age/arstudio.git
	$ cd arstudio
	$ git submodule init
	$ git submodule update
	$ cd src
	$ mkdir build
	$ cd build
	$ cmake [-D ALGORITHMS="algo1;algo2"] [-D CMAKE_BUILD_TYPE=Debug/Release] ..
	$ make [-jN]
	$ ./arstudio

Note: the ALGORITHMS CMake variable allows to selectively build algorithms,
avoiding automatic inclusion of everything in algorithms/.

Building documentation
=====

	$ cd docs
	$ doxygen
	$ xdg-open html/index.html

Coding style
=====

This project follows the GNU identation style. Uncrustify config for pre-commit
hook in git-hooks is made to format all code in this style.

As for actual coding, we try to follow [Google C++ style guide]
(http://google-styleguide.googlecode.com/svn/trunk/cppguide.xml) to some point,
but not strictly.

