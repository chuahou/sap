/**
 * @file example.cpp
 * @brief example application for SAP
 */

// version macros (do them however your project does them, as long as they can
// be accessed later
#define SAP_EXAMPLE_MAJOR_VERSION 0
#define SAP_EXAMPLE_MINOR_VERSION 0
#define SAP_EXAMPLE_REVISION      1

// application metadata (as above, do them however your project does)
#define SAP_EXAMPLE_NAME   "example"
#define SAP_EXAMPLE_AUTHOR "Chua Hou"
#define SAP_EXAMPLE_ABOUT  "Example application for SAP"

#include <cmath> // pow
#include <cstddef> // size_t
#include <iostream> // std::cout

#include "sap.h" // include single header file

int main(int argc, char **argv) // get arguments the normal way
{
	// --- CONFIGURATION ---
	SapConfig sap_config; // create a config struct to configure information
	                      // about your application as well as the arguments you
	                      // expect

	// application information
	sap_config.name = SAP_EXAMPLE_NAME; // name of application used when running
	                                    // it, usually the name of the
	                                    // executable (e.g. git, tmux, vim)
	sap_config.version_major = SAP_EXAMPLE_MAJOR_VERSION; // major version
	sap_config.version_minor = SAP_EXAMPLE_MINOR_VERSION; // minor version
	sap_config.version_patch = SAP_EXAMPLE_REVISION; // patch version
		// version is printed as MAJOR.MINOR.PATCH
	sap_config.author = SAP_EXAMPLE_AUTHOR; // author name
	sap_config.about = SAP_EXAMPLE_ABOUT; // short description of application
}
