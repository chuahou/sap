/**
 * @mainpage
 * @section Introduction
 *
 * A simple argument parser written for C/C++ CLI applications. It is written in
 * C such that both only-C applications and C++ applications can make use of it.
 *
 * It is inspired by some basic functionality from clap from Rust in terms of
 * the desired output, but is nowhere near as function-complete. I only aim to
 * properly implement in a reusable fashion the functionality I need
 * to use for my own projects.
 *
 * I chose not to use getopt or any other similar options as I would like my
 * projects to be portable across as many platforms as possible.
 */

/**
 * @file sap.h
 * @brief Primary header file for sap
 */

#ifndef __SAP_H_INCLUDED__
#define __SAP_H_INCLUDED__

/**
 * @brief Enum describing argument type
 */
typedef enum SapArgumentType
{
	/**
	 * @brief Option that does not take value
	 */
	SAP_ARG_OPTION,

	/**
	 * @brief Option that takes value
	 */
	SAP_ARG_OPTION_VALUE,

	/**
	 * @brief Positional argument
	 */
	SAP_ARG_POSITIONAL
} SapArgumentType;

/**
 * @brief Struct containing single argument configuration
 *
 * The application will set up shortopt, longopt, help and type, then set and
 * value will be set by sap_parse_args().
 */
typedef struct SapArgument
{
	/**
	 * @brief Short option
	 */
	char shortopt;

	/**
	 * @brief Long option name / argument name
	 */
	const char *longopt;

	/**
	 * @brief Short help message
	 */
	const char *help;

	/**
	 * @brief Type of argument
	 */
	SapArgumentType type;

	/**
	 * @brief 1 if argument is required, 0 otherwise
	 *
	 * All arguments of type SAP_ARG_POSITIONAL are considered required
	 * regardless of this value
	 */
	int required;

	/**
	 * @brief Will be set to 1 if argument has been provided, 0 otherwise
	 */
	int set;

	/**
	 * @brief Value set for this argument
	 */
	const char *value;
} SapArgument;

/**
 * @brief Struct containing configuration of application and arguments
 */
typedef struct SapConfig
{
	/**
	 * @brief Name of the application
	 */
	const char *name;

	/**
	 * @brief Major version
	 */
	unsigned int version_major;

	/**
	 * @brief Minor version
	 */
	unsigned int version_minor;

	/**
	 * @brief Patch version
	 */
	unsigned int version_patch;

	/**
	 * @brief Author's name
	 */
	const char *author;

	/**
	 * @brief Short description of program
	 */
	const char *about;

	/**
	 * @brief List of argument configurations
	 */
	SapArgument *arguments;

	/**
	 * @brief Number of argument configurations in arguments
	 */
	unsigned int argcount;
} SapConfig;

/**
 * @brief Parses arguments provided with the provided configuration, prints
 * help message if unsuccessful
 *
 * @param config The SapConfig to use
 * @param argc Argument count
 * @param argv Argument values
 * @return 0 If arguments parsed succesfully
 * @return 1 If arguments were invalid
 */
int sap_parse_args(SapConfig config, int argc, char **argv);

/**
 * @brief Prints help message based on configuration
 *
 * @param config The SapConfig to use
 */
void sap_print_help(SapConfig config);

#endif
