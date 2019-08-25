/**
 * @file sap.c
 * @brief Implements sap.h
 */

#include <stdio.h>
#include "sap.h"

int sap_parse_args(SapConfig config, int argc, char **argv)
{
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
    for (int i = 0; i < config.argcount; i++)
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
    for (int i = 0; i < config.argcount; i++)
    {
        SapArgument arg = config.arguments[i];
        if (arg.type != SAP_ARG_POSITIONAL)
        {
            printf("\t-%c, --%s %s\n", arg.shortopt, arg.longopt, arg.help);
        }
    }
}