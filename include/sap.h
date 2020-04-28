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

#include <ctype.h>
#include <stdio.h>
#include <string.h>

#ifndef DOXYGEN_IGNORE // exclude from documentation
	#define ARG_SHORTOPT 0
	#define ARG_LONGOPT 1
	#define ARG_NORMAL 2
	#define ARG_ERROR 3
#endif

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

// checks argument type
/** @private */
int _sap_check_arg_type(char *arg)
{
	// flag
	if (strlen(arg) >= 2 && arg[0] == '-')
	{
		// long option
		if (arg[1] == '-') return ARG_LONGOPT;

		// short options
		if (isalpha(arg[1])) return ARG_SHORTOPT;

		// unknown
		else return ARG_ERROR;
	}
	// normal string
	else
		return ARG_NORMAL;
}

int sap_parse_args(SapConfig config, int argc, char **argv)
{
	// keep track of which tokens have been parsed
	// we only update this for ARG_NORMAL arguments (since we want to keep track
	// of which are positional and which are valued options
	int parsed[argc]; // 0 = not parsed, 1 = parsed
	for (int i = 0; i < argc; i++) parsed[i] = 0;

	// set all arguments to not set
	for (unsigned int i = 0; i < config.argcount; i++)
	{
		config.arguments[i].set = 0;
	}

	// positionals handled so far
	unsigned int positionalsHandled = 0;

	// iterate through the arguments we are looking for
	for (unsigned int i = 0; i < config.argcount; i++)
	{
		SapArgument* arg = config.arguments + i;

		// check for flags
		if (arg->type == SAP_ARG_OPTION || arg->type == SAP_ARG_OPTION_VALUE)
		{
			// iterate through arguments
			for (int j = 1; j < argc; j++)
			{
				// check if option
				int k, found = 0;
				switch (_sap_check_arg_type(argv[j]))
				{
				case ARG_SHORTOPT: // short options
					k = 1;
					while (isalpha(argv[j][k]) && !found)
					{
						// found option
						if (argv[j][k] == arg->shortopt)
						{
							arg->set = 1;
							found = 1;

							// check for value if necessary
							if (arg->type == SAP_ARG_OPTION_VALUE)
							{
								if (j < argc - 1)
								{
									// too many options set for valued option
									if (strlen(argv[j]) > 2)
									{
										return 1;
									}

									// get value and set next value to NIL
									else if
										(_sap_check_arg_type(argv[j + 1])
											== ARG_NORMAL)
									{
										arg->value = argv[j + 1];
										parsed[j + 1] = 1;
									}
									else // no value given
									{
										return 1;
									}
								}
								else // no value given
								{
									return 1;
								}
							}
						}

						k++;
					}
					break;
				case ARG_LONGOPT: // long option
					if (strcmp(argv[j] + 2, arg->longopt) == 0)
					{
						arg->set = 1;
						found = 1;

						// check for value if necessary
						if (arg->type == SAP_ARG_OPTION_VALUE)
						{
							if (j < argc - 1)
							{
								if (_sap_check_arg_type(argv[j + 1])
									== ARG_NORMAL)
								{
									arg->value = argv[j + 1];
									parsed[j + 1] = 1;
								}
							}
							else
							{
								return 1; // no value given
							}
						}
					}
					break;
				case ARG_ERROR: // error
					return 1;
				}
			}
		}
	}

	for (unsigned int i = 0; i < config.argcount; i++)
	{
		SapArgument* arg = config.arguments + i;

		// check for positionals
		if (arg->type == SAP_ARG_POSITIONAL)
		{
			// positionals hit so far
			unsigned int positionalsHit = 0;

			// iterate through arguments
			for (int j = 1; j < argc && arg->set == 0; j++)
			{
				// check if positional
				switch (_sap_check_arg_type(argv[j]))
				{
				case ARG_NORMAL:
					if (parsed[j]) break; // this was a valued option
					if (positionalsHit == positionalsHandled)
					{
						arg->value = argv[j];
						arg->set = 1;
						positionalsHandled++;
					}
					else
					{
						positionalsHit++;
					}
					break;
				case ARG_ERROR:
					return 1;
				}
			}
		}
	}

	// check all required arguments are fulfilled
	for (unsigned int i = 0; i < config.argcount; i++)
	{
		SapArgument* arg = config.arguments + i;

		// positional or required
		if (arg->required || arg->type == SAP_ARG_POSITIONAL)
		{
			// not set
			if (!arg->set)
				return 1;
		}
	}

	return 0;
}

void sap_print_help(SapConfig config)
{
	// prints metadata
	printf("%s %d.%d.%d\n", config.name, config.version_major,
		config.version_minor, config.version_patch);
	printf("%s\n%s\n\n", config.author, config.about);

	// prints usage
	printf("USAGE:\n\t%s [FLAGS] ", config.name);

	// iterate through arguments to print usage
	for (unsigned int i = 0; i < config.argcount; i++)
	{
		SapArgument arg = config.arguments[i];
		if (arg.type == SAP_ARG_POSITIONAL)
		{
			if (arg.required)
			{
				printf("%s ", arg.longopt);
			}
			else
			{
				printf("[%s] ", arg.longopt);
			}
		}
	}
	printf("\n\n");

	// prints flags
	printf("FLAGS:\n");
	for (unsigned int i = 0; i < config.argcount; i++)
	{
		SapArgument arg = config.arguments[i];
		if (arg.type != SAP_ARG_POSITIONAL)
		{
			printf("\t-%c, --%s %s\n", arg.shortopt, arg.longopt, arg.help);
		}
	}
	printf("\n");

	// prints arguments
	printf("ARGUMENTS:\n");
	for (unsigned int i = 0; i < config.argcount; i++)
	{
		SapArgument arg = config.arguments[i];
		if (arg.type == SAP_ARG_POSITIONAL)
		{
			printf("\t%s %s\n", arg.longopt, arg.help);
		}
	}
}

#endif
