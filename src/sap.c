/**
 * @file sap.c
 * @brief Implements sap.h
 */

#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include "sap.h"

#define ARG_SHORTOPT 0
#define ARG_LONGOPT 1
#define ARG_NORMAL 2
#define ARG_ERROR 3
#define ARG_EMPTY 4

// checks argument type
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
    // not flag
    else
    {
        // empty
        if (arg[0] == '\0') return ARG_EMPTY;

        // normal string
        if (isalpha(arg[0])) return ARG_NORMAL;

        // unknown
        else return ARG_ERROR;
    }
}

int sap_parse_args(SapConfig config, int argc, char **argv)
{
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
                                        argv[j + 1] = "";
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
                                    argv[j + 1] = "";
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
                        // all values for options should be ARG_EMPTY now
                    break;
                case ARG_ERROR:
                    return 1;
                }
            }
        }
    }

    // TODO: check all required arguments are fulfilled

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
