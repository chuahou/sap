############################
Simple Argument Parser (SAP)
############################

|Build Status|
|Total alerts|
|Language grade: C/C++|
|Maintenance|
|License: MIT|

A simple argument parser written for C/C++ CLI applications. It is
written in C such that both only-C applications and C++ applications can
make use of it.

It is inspired by some basic functionality from `clap from Rust
<https://clap.rs/>`_ in terms of the desired output, but is nowhere
near as function-complete. I only aim to properly implement in a
reusable fashion the functionality I need to use for my own projects.

I chose not to use getopt or any other similar options as I would like
my projects to be portable across as many platforms as possible.

Usage
=====

SAP is now written as a single header file. To use it, simply copy
``include/sap.h`` to your project’s header directories, or add
``include`` to your project’s include directories, then add to your
code:

.. code-block:: cpp

	#include "sap.h"

Doxygen documentation
=====================

To generate documentation, enter the repository directory and run:

::

	doxygen Doxyfile

.. |Build Status| image:: https://travis-ci.org/chuahou/sap.svg?branch=master
	:target: https://travis-ci.org/chuahou/sap
.. |Total alerts| image:: https://img.shields.io/lgtm/alerts/g/chuahou/sap.svg?logo=lgtm&logoWidth=18
	:target: https://lgtm.com/projects/g/chuahou/sap/alerts/
.. |Language grade: C/C++| image:: https://img.shields.io/lgtm/grade/cpp/g/chuahou/sap.svg?logo=lgtm&logoWidth=18
	:target: https://lgtm.com/projects/g/chuahou/sap/context:cpp
.. |Maintenance| image:: https://img.shields.io/badge/Maintained%3F-yes-green.svg
.. |License: MIT| image:: https://img.shields.io/badge/License-MIT-yellow.svg
	:target: https://opensource.org/licenses/MIT
