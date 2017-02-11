# 2pem-viewer

## Installation and Compilation Instructions

### OS X

1.  Install [Homebrew](http://brew.sh) (follow the instructions on the
Homebrew website).

2.  Use Homebrew to install the require packages:

    $ brew install armadillo boost cmake fftw brew install glfw3
    $ brew install liblo portaudio # optional

3.  In a reasonable location, clone this git repository:

    $ git clone https://github.com/sampotter/2pem-viewer.git

4.  Change directories into the repository, create a build directory
    (for an out-of-source build), run CMake, and build:

    $ cd 2pem-viewer
    $ mkdir build
    $ cd build
    $ cmake -DCMAKE_BUILD_TYPE=Release ..
    $ make

5.  The resulting binaries in the `build` directory are `./src/client`
    and `./src/server`.

    $ cd 2pem-viewer
    $ git submodule init
    $ git submodule update
    $ mkdir build
    $ cd build
    $ cmake -DCMAKE_BUILD_TYPE=Release ..
    $ make

6.  The resulting binaries in the `build` directory are `./src/client`
    and `./src/server`.

### Ubuntu Linux (and similar)

**TODO**.

### Windows (using MSYS2)

1.  Download and install the 64-bit version of
    [MSYS2](https://msys2.github.io/).
2.  Use Pacman to initially update the MSYS2 installation (follow the
    post-installation instructions at the
    [MSYS2](https://msys2.github.io/) website).
3.  When opening an MSYS2 terminal (e.g. from the Start menu),
    always open "MSYS2 MinGW 64-bit" (the other options are "MSYS2
    MSYS" and "MSYS2 MinGW 32-bit").
4.  Install the necessary packages:

    $ pacman -Sy git mingw-w64_x86-ninja mingw-w64-x86_64-armadillo \
        mingw-w64-x86_64-boost mingw-w64-x86_64-cmake \
        mingw-w64-x86_64-extra-cmake-modules mingw-w64-x86_64-fftw \
        mingw-w64-x86_64-gcc mingw-w64-x86_64-glew mingw-w64-x86_64-glfw
    $ pacman -Sy mingw-w64-x86_64-portua

5.  MSYS2 doesn't currently provide a package for
    [liblo](http://liblo.sourceforge.net/). To install it from source,
    follow the following instructions:

    $ pacman -S autoconf autoconf-archive automake make
    $ git clone git://liblo.git.sourceforge.net/gitroot/liblo/liblo
    $ cd liblo
    $ sh autogen.sh
    $ TODO

1.  Use `git` to clone this repository in an MSYS2 terminal (if you
    haven't downloaded the repository by some other means).
2.  Navigate to the root directory of the repository and build:

    $ cd 2pem-viewer
    $ git submodule init
    $ git submodule update
    $ mkdir -p build
    $ cd build
    $ cmake -GNinja -DCMAKE_BUILD_TYPE=Release ../..
    $ ninja

1.  The binaries are placed in `2pem-viewer/build/Release/src`.

## CMake Options

There are optional features which can be built (or not) by passing
arguments to CMake. For a complete list of these variables, run:

    $ cmake -L

To use these options, set them using `-DOPTION_NAME={ON,OFF}` when
calling `cmake` on the command line. For example:

    $ cmake -GNinja -DCMAKE_BUILD_TYPE=Release -DUSE_OSC=OFF <path-to-repo>