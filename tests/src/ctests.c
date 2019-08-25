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

    SapArgument options[1] = { helpOpt };

    config.arguments = options;
    config.argcount = 1;

    sap_print_help(config);

    return 0;
}
