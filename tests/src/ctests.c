#include <assert.h>
#include <malloc.h>
#include <sap.h>

SapConfig setup_test_config()
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

    SapArgument posOpt1 =
    {
        .longopt = "POSITIONALARG1",
        .type = SAP_ARG_POSITIONAL,
        .required = 1,
        .help = "A positional argument"
    };

    SapArgument valueOpt1 =
    {
        .shortopt = 'v',
        .longopt = "value",
        .type = SAP_ARG_OPTION_VALUE,
        .required = 1,
        .help = "A valued option"
    };

    SapArgument flagOpt1 =
    {
        .shortopt = 'a',
        .longopt = "aflag",
        .type = SAP_ARG_OPTION,
        .required = 0,
        .help = "Flag A"
    };

    SapArgument valueOpt2 =
    {
        .shortopt = 'c',
        .longopt = "cvalue",
        .type = SAP_ARG_OPTION_VALUE,
        .required = 0,
        .help = "Another valued option"
    };

    SapArgument flagOpt2 =
    {
        .shortopt = 'b',
        .longopt = "bflag",
        .type = SAP_ARG_OPTION,
        .required = 0,
        .help = "Flag B"
    };

    SapArgument posOpt2 =
    {
        .longopt = "ANOTHERPOSARG",
        .type = SAP_ARG_POSITIONAL,
        .required = 1,
        .help = "Positional another time"
    };

    config.arguments = malloc(sizeof(SapArgument) * 7);

    config.arguments[0] = helpOpt;
    config.arguments[1] = posOpt1;
    config.arguments[2] = valueOpt1;
    config.arguments[3] = flagOpt1;
    config.arguments[4] = valueOpt2;
    config.arguments[5] = flagOpt2;
    config.arguments[6] = posOpt2;
    config.argcount = 7;

    return config;
}

void test_value(SapConfig config)
{
    printf("Testing valued options...\n");

    printf("Testing simple value\n");
    const char *argv1[] = { "ctests", "-v", "value", "posarg" };
    assert(sap_parse_args(config, 4, argv1) == 0);
    assert(config.arguments[2].set == 1);
    assert(strcmp(config.arguments[2].value, "value") == 0);

    printf("Testing multiple values\n");
    const char *argv2[] = { "ctests", "-v", "value", "-c", "cvalue", "posarg" };
    assert(sap_parse_args(config, 6, argv2) == 0);
    assert(config.arguments[2].set == 1);
    assert(config.arguments[4].set == 1);
    assert(strcmp(config.arguments[2].value, "value") == 0);
    assert(strcmp(config.arguments[4].value, "cvalue") == 0);

    printf("Testing incorrect multiple shortopt\n");
    const char *argv3[] = { "ctests", "-vc", "value", "posarg" };
    assert(sap_parse_args(config, 4, argv3) != 0);

    printf("Testing value option followed by more options values\n");
    const char *argv4[] = { "ctests", "-v", "--cvalue", "posarg" };
    assert(sap_parse_args(config, 4, argv4) != 0);

    printf("Testing longopt value\n");
    const char *argv5[] = { "ctests", "--value", "value", "posarg" };
    assert(sap_parse_args(config, 4, argv5) == 0);
    assert(config.arguments[2].set == 1);
    assert(strcmp(config.arguments[2].value, "value") == 0);

    printf("Testing lack of required value\n");
    const char *argv6[] = { "ctests", "posarg", "--cvalue", "abc" };
    assert(sap_parse_args(config, 4, argv6) != 0);

    printf("Value testing passed\n\n");
}

int main()
{
    // create config
    SapConfig config = setup_test_config();

    // run tests
    test_value(config);

    // free config memory
    free(config.arguments);

    return 0;
}
