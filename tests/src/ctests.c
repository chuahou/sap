/**
 * @file ctests.c
 * @brief Tests in C language
 */

#include <assert.h>
#include <malloc.h>
#include <sap.h>
#include <string.h>

/**
 * @brief Set the up test configuration
 * 
 * The test configuration emulates an application ctests v1.2.3 by Chua Hou
 * with description "C language test for sap".
 * 
 * It supports the following options:
 * 
 * -h --help    print help message
 * 
 * -v --value   a valued option
 * 
 * -c --cvalue  another valued option
 * 
 * -a --aflag   flag A
 * 
 * -b --bflag   flag B
 * 
 * and takes 2 positional arguments.
 * 
 * @return SapConfig test configuration
 */
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

/**
 * @brief Tests valued arguments with provided config
 * 
 * @param config config
 */
void test_value(SapConfig config)
{
    printf("Testing valued options...\n");

    printf("Testing simple value\n");
    char *argv1[] = { "ctests", "-v", "value", "posarg", "posarg2" };
    assert(sap_parse_args(config, 5, argv1) == 0);
    assert(config.arguments[2].set == 1);
    assert(strcmp(config.arguments[2].value, "value") == 0);

    printf("Testing multiple values\n");
    char *argv2[] = { "ctests", "-v", "value", "-c", "cvalue", "posarg", "posarg2" };
    assert(sap_parse_args(config, 7, argv2) == 0);
    assert(config.arguments[2].set == 1);
    assert(config.arguments[4].set == 1);
    assert(strcmp(config.arguments[2].value, "value") == 0);
    assert(strcmp(config.arguments[4].value, "cvalue") == 0);

    printf("Testing incorrect multiple shortopt\n");
    char *argv3[] = { "ctests", "-vc", "value", "posarg", "posarg2" };
    assert(sap_parse_args(config, 5, argv3) != 0);

    printf("Testing value option followed by more options values\n");
    char *argv4[] = { "ctests", "-v", "--cvalue", "posarg", "posarg2" };
    assert(sap_parse_args(config, 5, argv4) != 0);

    printf("Testing longopt value\n");
    char *argv5[] = { "ctests", "--value", "value", "posarg", "posarg2" };
    assert(sap_parse_args(config, 5, argv5) == 0);
    assert(config.arguments[2].set == 1);
    assert(strcmp(config.arguments[2].value, "value") == 0);

    printf("Testing lack of required value\n");
    const char *argv6[] = { "ctests", "posarg", "--cvalue", "abc", "posarg2" };
    assert(sap_parse_args(config, 5, argv6) != 0);

    printf("Value testing passed\n\n");
}

/**
 * @brief Test options with provided config
 * 
 * @param config config
 */
void test_opt(SapConfig config)
{
    printf("Testing options...\n");

    printf("Testing setting of option\n");
    char *argv1[] = { "ctests", "-v", "value", "posarg", "posarg2", "-a" };
    assert(sap_parse_args(config, 6, argv1) == 0);
    assert(config.arguments[0].set == 0);
    assert(config.arguments[1].set == 1);
    assert(config.arguments[2].set == 1);
    assert(config.arguments[3].set == 1);
    assert(config.arguments[4].set == 0);
    assert(config.arguments[5].set == 0);
    assert(config.arguments[6].set == 1);

    printf("Testing multiple options\n");
    char *argv2[] = { "ctests", "-v", "value", "posarg", "posarg2", "-abh" };
    assert(sap_parse_args(config, 6, argv2) == 0);
    assert(config.arguments[0].set == 1);
    assert(config.arguments[1].set == 1);
    assert(config.arguments[2].set == 1);
    assert(config.arguments[3].set == 1);
    assert(config.arguments[4].set == 0);
    assert(config.arguments[5].set == 1);
    assert(config.arguments[6].set == 1);

    printf("Options testing passed\n\n");
}

/**
 * @brief Test positional arguments with specified config
 * 
 * @param config config
 */
void test_pos(SapConfig config)
{
    printf("Testing positional arguments...\n");

    printf("Testing order 1\n");
    char *argv1[] = { "ctests", "-v", "value", "posarg", "posarg2" };
    assert(sap_parse_args(config, 5, argv1) == 0);
    assert(config.arguments[2].set == 1);
    assert(strcmp(config.arguments[2].value, "value") == 0);
    assert(strcmp(config.arguments[1].value, "posarg") == 0);
    assert(strcmp(config.arguments[6].value, "posarg2") == 0);

    printf("Testing order 2\n");
    char *argv2[] = { "ctests", "posarg", "-v", "value", "posarg2" };
    assert(sap_parse_args(config, 5, argv2) == 0);
    assert(config.arguments[2].set == 1);
    assert(strcmp(config.arguments[2].value, "value") == 0);
    assert(strcmp(config.arguments[1].value, "posarg") == 0);
    assert(strcmp(config.arguments[6].value, "posarg2") == 0);

    printf("Testing order 3\n");
    char *argv3[] = { "ctests", "posarg", "posarg2", "-v", "value" };
    assert(sap_parse_args(config, 5, argv3) == 0);
    assert(config.arguments[2].set == 1);
    assert(strcmp(config.arguments[2].value, "value") == 0);
    assert(strcmp(config.arguments[1].value, "posarg") == 0);
    assert(strcmp(config.arguments[6].value, "posarg2") == 0);

    printf("Positional arguments testing passed\n\n");
}

int main()
{
    // create config
    SapConfig config = setup_test_config();

    // run tests
    test_value(config);
    test_opt(config);
    test_pos(config);

    // free config memory
    free(config.arguments);

    return 0;
}
