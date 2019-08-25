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

    SapArgument posOpt;
    posOpt.longopt = "POSITIONALARG";
    posOpt.type = SAP_ARG_POSITIONAL;
    posOpt.required = 1;
    posOpt.help = "A positional argument";

    SapArgument valueOpt;
    valueOpt.shortopt = 'v';
    valueOpt.longopt = "value";
    valueOpt.type = SAP_ARG_OPTION_VALUE;
    valueOpt.required = 1;
    valueOpt.help = "A valued option";

    SapArgument options[3] = { helpOpt, posOpt, valueOpt };

    config.arguments = options;
    config.argcount = 3;

    sap_print_help(config);

    return 0;
}
