extern "C"
{
#include <sap.h>
}

int main()
{
    SapConfig config;
    config.name = "cpptests";
    config.version_major = 1;
    config.version_minor = 2;
    config.version_patch = 3;
    config.author = "Chua Hou";
    config.about = "C++ language test for sap";

    SapArgument helpOpt;
    helpOpt.shortopt = 'h';
    helpOpt.longopt = "help";
    helpOpt.type = SAP_ARG_OPTION;
    helpOpt.required = 0;
    helpOpt.help = "Prints this help message";

    SapArgument options[1] = { helpOpt };

    config.arguments = options;
    config.argcount = 1;

    sap_print_help(config);

    return 0;
}
