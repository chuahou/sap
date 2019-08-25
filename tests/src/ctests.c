#include <sap.h>

int main()
{
    SapConfig config =
    {
        .name = "ctests",
        .version_major = 1,
        .version_minor = 2,
        .version_patch = 3,
        .author = "Chua Hou",
        .about = "C language test for sap"
    };

    SapArgument helpOpt =
    {
        .shortopt = 'h',
        .longopt = "help",
        .type = SAP_ARG_OPTION,
        .required = 0,
        .help = "Prints this help message"
    };

    SapArgument posOpt =
    {
        .longopt = "POSITIONALARG",
        .type = SAP_ARG_POSITIONAL,
        .required = 1,
        .help = "A positional argument"
    };

    SapArgument valueOpt =
    {
        .shortopt = 'v',
        .longopt = "value",
        .type = SAP_ARG_OPTION_VALUE,
        .required = 1,
        .help = "A valued option"
    };

    SapArgument options[3] = { helpOpt, posOpt, valueOpt };

    config.arguments = options;
    config.argcount = 3;

    sap_print_help(config);

    return 0;
}
