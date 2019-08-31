# Simple Argument Parser (SAP)

A simple argument parser written for C/C++ CLI applications. It is written in C
such that both only-C applications and C++ applications can make use of it.

It is inspired by some basic functionality from [clap from Rust](https://clap.rs/)
in terms of the desired output, but is nowhere near as function-complete. I
only aim to properly implement in a reusable fashion the functionality I need
to use for my own projects.

I chose not to use getopt or any other similar options as I would like my projects
to be portable across as many platforms as possible.

## Compilation

Compilation is done using CMake. To do so, run:

    mkdir build
    cd build
    cmake ..
    make

## Usage

Currently, there is no way to install the library and headers. Instead, just compile sap and copy the `libsap.a`
or `libsap.lib` file from your build to a folder of your choice, and do likewise with `include/sap.h`. During
compilation, ensure your compiler and linker are pointed to these folders containing `libsap` and `sap.h`.

After doing so, add

    #include <sap.h>

to your application and use it. Actual usage will be documented in the future.

## Doxygen documentation

To generate documentation, enter the repository directory and run:

    doxygen Doxyfile
