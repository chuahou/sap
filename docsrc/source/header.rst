####################
Header Documentation
####################

.. contents::

Macros
======

+-------------------------+----------------------------------------+
| Macro                   | Remark                                 |
+=========================+========================================+
| ``SAP_H_MAJOR_VERSION`` | Current major version                  |
+-------------------------+----------------------------------------+
| ``SAP_H_MINOR_VERSION`` | Current minor version                  |
+-------------------------+----------------------------------------+
| ``SAP_H_REVISION``      | Current revision                       |
+-------------------------+----------------------------------------+
| ``SAP_H_VERSION``       | Full version string                    |
+-------------------------+----------------------------------------+
| ``SAP_H_VERSION_CHECK`` | Check compatible version of SAP        |
+-------------------------+----------------------------------------+

To check if the header is a compatible version of X.Y (i.e. the major
version is equal to X and the minor version is at least Y), use

::

	if (SAP_H_VERSION_CHECK(X, Y)) {
		// error
	}

Structs
=======

Main configuration
------------------

The main configuration is done using ``SapConfig``. Metadata for the
application is set in these fields, and a list of `argument
configurations <#argument-configuration>`_ is set in ``arguments``.

.. doxygenstruct:: SapConfig
	:members:

Argument configuration
----------------------

This struct will contain the configuration, and later the result, of
each argument the application expects / can handle.

.. doxygenstruct:: SapArgument
	:members:

Argument types
--------------

Argument types are specified using this enum:

.. doxygenenum:: SapArgumentType

Functions
---------

After configuration, these 2 functions are used for the actual
processing.

.. doxygenfunction:: sap_parse_args
.. doxygenfunction:: sap_print_help
