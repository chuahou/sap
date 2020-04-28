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
#include <iostream> // std::cout, std::cerr
#include <string> // std::string, std::stoll, std::stod

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

	// --- DEFINING ARGUMENTS ---
	sap_config.argcount = 10; // number of possible arguments
	SapArgument sap_args[sap_config.argcount]; // create an array of arguments
	sap_config.arguments = sap_args; // add array to configuration

	// When defining your arguments, you should fill out shortopt, longopt, help
	// and type. After parsing, set and value will be written by SAP.

	// A simple optional option that prints the help message when present.
	sap_args[0].shortopt = 'h'; // short one letter version of option, -h
	sap_args[0].longopt  = "help"; // long option, --help
	sap_args[0].help     = "Print this help message"; // description of option
	sap_args[0].type     = SAP_ARG_OPTION; // simple option
	sap_args[0].required = 0; // optional option

	// Optional options for mathematics. Ideally we would want to set exactly 1
	// of these to be required, but this is not yet supported by SAP so we will
	// have to do this by ourselves. We will be setting NONE of them to be
	// required and validating afterwards.

	// add
	sap_args[1].shortopt = 'a';
	sap_args[1].longopt  = "add";
	sap_args[1].help     = "Add x and y";
	sap_args[1].type     = SAP_ARG_OPTION;
	sap_args[1].required = 0;

	// subtract
	sap_args[2].shortopt = 's';
	sap_args[2].longopt  = "subtract";
	sap_args[2].help     = "Subtract y from x";
	sap_args[2].type     = SAP_ARG_OPTION;
	sap_args[2].required = 0;

	// multiply
	sap_args[3].shortopt = 'm';
	sap_args[3].longopt  = "multiply";
	sap_args[3].help     = "Multiply x and y";
	sap_args[3].type     = SAP_ARG_OPTION;
	sap_args[3].required = 0;

	// divide
	sap_args[4].shortopt = 'd';
	sap_args[4].longopt  = "divide";
	sap_args[4].help     = "Divide x by y";
	sap_args[4].type     = SAP_ARG_OPTION;
	sap_args[4].required = 0;

	// modulo
	sap_args[5].shortopt = 'M';
	sap_args[5].longopt  = "modulo";
	sap_args[5].help     = "Calculate x mod y";
	sap_args[5].type     = SAP_ARG_OPTION;
	sap_args[5].required = 0;

	// exponent
	sap_args[6].shortopt = 'e';
	sap_args[6].longopt  = "exponent";
	sap_args[6].help     = "Calculate x to the power of y";
	sap_args[6].type     = SAP_ARG_OPTION;
	sap_args[6].required = 0;

	// A required option with a value to determine what type of values we are
	// using. -t int for integers and -t float for floating-point.
	sap_args[7].shortopt = 't';
	sap_args[7].longopt  = "type";
	sap_args[7].help     = "[ int | float ] type of numbers";
	sap_args[7].type     = SAP_ARG_OPTION_VALUE; // option with value
	sap_args[7].required = 1; // make this compulsory

	// Two compulsory positional arguments for our numbers
	// sap_args[8].shortopt = ''; Note: No need for shortopt for positionals
	sap_args[8].longopt  = "x"; // longopt only used in usage message
	sap_args[8].help     = "First argument for calculator";
	sap_args[8].type     = SAP_ARG_POSITIONAL; // positional argument
	sap_args[8].required = 1; // make this compulsory
	sap_args[9].longopt  = "y";
	sap_args[9].help     = "Second argument for calculator";
	sap_args[9].type     = SAP_ARG_POSITIONAL;
	sap_args[9].required = 1;

	// --- PARSING ARGUMENTS ---
	// Simply run sap_parse_args() to perform the parsing. It will return 1 if
	// unsuccessful (e.g. if a required argument is missing). sap_args[0].set
	// checks if -h asking for help has been set.
	if (sap_parse_args(sap_config, argc, argv) || sap_args[0].set)
	{
		sap_print_help(sap_config); // print help message if wrong
		return 1;
	}

	// Next let's check that exactly one of the operators has been chosen. Every
	// option / positional present will have .set set to 1, and will be 0
	// otherwise.
	unsigned int operators_selected = 0;
	for (size_t i = 1; i <= 6; i++) // iterate through operators
		operators_selected += sap_args[i].set;
	if (operators_selected != 1)
	{
		std::cerr << "Use only one operator" << std::endl;
		sap_print_help(sap_config);
		return 1;
	}

	// Next, we check that the -t option is set to int or float and parse x
	// and y, printing the help message if anything is invalid.
	long long i_x, i_y; // ints for int mode
	double lf_x, lf_y; // doubles for float mode
	int type; // store which type we are using
	const int TYPE_INT = 0;
	const int TYPE_FLOAT = 1;
	std::string type_opt = sap_args[7].value;

	try
	{
		// check for int
		if (type_opt == "int") // thanks std::string
		{
			// int mode
			type = TYPE_INT;

			// convert x and y to long long
			i_x = std::stoll(sap_args[8].value);
			i_y = std::stoll(sap_args[9].value);
		}
		// check for float
		else if (type_opt == "float")
		{
			// float mode
			type = TYPE_FLOAT;

			// convert x and y to double
			lf_x = std::stod(sap_args[8].value);
			lf_y = std::stod(sap_args[9].value);
		}
		// invalid -t value
		else
		{
			std::cerr << "Invalid type, use int or float" << std::endl;
			sap_print_help(sap_config);
			return 1;
		}
	}
	catch (const std::invalid_argument& e) // failed to parse x or y
	{
		std::cerr << "Invalid numbers" << std::endl;
		sap_print_help(sap_config);
		return 1;
	}
}
