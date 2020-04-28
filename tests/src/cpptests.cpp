/**
 * @file cpptests.cpp
 * @brief Tests in C++ language
 */

#include <cassert>
#include <cstdarg>
#include <cstdio>
#include <cstring>

#include "sap.h"

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
	SapConfig config;
	config.name = "ctests";
	config.version_major = 1;
	config.version_minor = 2;
	config.version_patch = 3;
	config.author = "Chua Hou";
	config.about = "C language test for sap";

	SapArgument helpOpt;
	helpOpt.shortopt = 'h';
	helpOpt.longopt = "help";
	helpOpt.type = SAP_ARG_OPTION;
	helpOpt.required = 0;
	helpOpt.help = "Prints this help message";

	SapArgument posOpt1;
	posOpt1.longopt = "POSITIONALARG1";
	posOpt1.type = SAP_ARG_POSITIONAL;
	posOpt1.required = 1;
	posOpt1.help = "A positional argument";

	SapArgument valueOpt1;
	valueOpt1.shortopt = 'v';
	valueOpt1.longopt = "value";
	valueOpt1.type = SAP_ARG_OPTION_VALUE;
	valueOpt1.required = 1;
	valueOpt1.help = "A valued option";

	SapArgument flagOpt1;
	flagOpt1.shortopt = 'a';
	flagOpt1.longopt = "aflag";
	flagOpt1.type = SAP_ARG_OPTION;
	flagOpt1.required = 0;
	flagOpt1.help = "Flag A";

	SapArgument valueOpt2;
	valueOpt2.shortopt = 'c';
	valueOpt2.longopt = "cvalue";
	valueOpt2.type = SAP_ARG_OPTION_VALUE;
	valueOpt2.required = 0;
	valueOpt2.help = "Another valued option";

	SapArgument flagOpt2;
	flagOpt2.shortopt = 'b';
	flagOpt2.longopt = "bflag";
	flagOpt2.type = SAP_ARG_OPTION;
	flagOpt2.required = 0;
	flagOpt2.help = "Flag B";

	SapArgument posOpt2;
	posOpt2.longopt = "ANOTHERPOSARG";
	posOpt2.type = SAP_ARG_POSITIONAL;
	posOpt2.required = 1;
	posOpt2.help = "Positional another time";

	config.arguments = new SapArgument[7];

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
 * @brief Copies the contents of ..., an array of immutable strings, into argv,
 * another array of strings but mutable. Each element of argv must be freed
 * after use.
 *
 * @param argc length of argv and out
 * @param argv array to output to
 * @param ... array of immutable strings
 */
void copy_argv(int argc, char **argv, ...)
{
	va_list strings;
	va_start(strings, argv);
	for (int i = 0; i < argc; i++)
	{
		const char *next = va_arg(strings, const char *);
		argv[i] = new char[strlen(next) + 1]; // +1 for \0
		strcpy(argv[i], next);
	}
	va_end(strings);
}

#define FREE_ARGV(argc, argv) for (int i = 0; i < argc; i++) delete argv[i]

/**
 * @brief Tests valued arguments with provided config
 *
 * @param config config
 */
void test_value(SapConfig config)
{
	printf("Testing valued options...\n");

	printf("Testing simple value\n");
	char *argv1[5];
	copy_argv(5, argv1, "ctests", "-v", "value", "posarg", "posarg2");
	assert(sap_parse_args(config, 5, argv1) == 0);
	assert(config.arguments[2].set == 1);
	assert(strcmp(config.arguments[2].value, "value") == 0);
	FREE_ARGV(5, argv1);

	printf("Testing multiple values\n");
	char *argv2[7];
	copy_argv(7, argv2, "ctests", "-v", "value", "-c", "cvalue", "posarg",
		"posarg2");
	assert(sap_parse_args(config, 7, argv2) == 0);
	assert(config.arguments[2].set == 1);
	assert(config.arguments[4].set == 1);
	assert(strcmp(config.arguments[2].value, "value") == 0);
	assert(strcmp(config.arguments[4].value, "cvalue") == 0);
	FREE_ARGV(7, argv2);

	printf("Testing incorrect multiple shortopt\n");
	char *argv3[5];
	copy_argv(5, argv3, "ctests", "-vc", "value", "posarg", "posarg2");
	assert(sap_parse_args(config, 5, argv3) != 0);
	FREE_ARGV(5, argv3);

	printf("Testing value option followed by more options values\n");
	char *argv4[5];
	copy_argv(5, argv4, "ctests", "-v", "--cvalue", "posarg", "posarg2");
	assert(sap_parse_args(config, 5, argv4) != 0);
	FREE_ARGV(5, argv4);

	printf("Testing longopt value\n");
	char *argv5[5];
	copy_argv(5, argv5, "ctests", "--value", "value", "posarg", "posarg2");
	assert(sap_parse_args(config, 5, argv5) == 0);
	assert(config.arguments[2].set == 1);
	assert(strcmp(config.arguments[2].value, "value") == 0);
	FREE_ARGV(5, argv5);

	printf("Testing lack of required value\n");
	char *argv6[5];
	copy_argv(5, argv6, "ctests", "posarg", "--cvalue", "abc", "posarg2");
	assert(sap_parse_args(config, 5, argv6) != 0);
	FREE_ARGV(5, argv6);

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
	char *argv1[6];
	copy_argv(6, argv1, "ctests", "-v", "value", "posarg", "posarg2", "-a");
	assert(sap_parse_args(config, 6, argv1) == 0);
	assert(config.arguments[0].set == 0);
	assert(config.arguments[1].set == 1);
	assert(config.arguments[2].set == 1);
	assert(config.arguments[3].set == 1);
	assert(config.arguments[4].set == 0);
	assert(config.arguments[5].set == 0);
	assert(config.arguments[6].set == 1);
	FREE_ARGV(6, argv1);

	printf("Testing multiple options\n");
	char *argv2[6];
	copy_argv(6, argv2, "ctests", "-v", "value", "posarg", "posarg2", "-abh");
	assert(sap_parse_args(config, 6, argv2) == 0);
	assert(config.arguments[0].set == 1);
	assert(config.arguments[1].set == 1);
	assert(config.arguments[2].set == 1);
	assert(config.arguments[3].set == 1);
	assert(config.arguments[4].set == 0);
	assert(config.arguments[5].set == 1);
	assert(config.arguments[6].set == 1);
	FREE_ARGV(6, argv2);

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
	char *argv1[5];
	copy_argv(5, argv1, "ctests", "-v", "value", "posarg", "posarg2");
	assert(sap_parse_args(config, 5, argv1) == 0);
	assert(config.arguments[2].set == 1);
	assert(strcmp(config.arguments[2].value, "value") == 0);
	assert(strcmp(config.arguments[1].value, "posarg") == 0);
	assert(strcmp(config.arguments[6].value, "posarg2") == 0);
	FREE_ARGV(5, argv1);

	printf("Testing order 2\n");
	char *argv2[5];
	copy_argv(5, argv2, "ctests", "posarg", "-v", "value", "posarg2");
	assert(sap_parse_args(config, 5, argv2) == 0);
	assert(config.arguments[2].set == 1);
	assert(strcmp(config.arguments[2].value, "value") == 0);
	assert(strcmp(config.arguments[1].value, "posarg") == 0);
	assert(strcmp(config.arguments[6].value, "posarg2") == 0);
	FREE_ARGV(5, argv2);

	printf("Testing order 3\n");
	char *argv3[5];
	copy_argv(5, argv3, "ctests", "posarg", "posarg2", "-v", "value");
	assert(sap_parse_args(config, 5, argv3) == 0);
	assert(config.arguments[2].set == 1);
	assert(strcmp(config.arguments[2].value, "value") == 0);
	assert(strcmp(config.arguments[1].value, "posarg") == 0);
	assert(strcmp(config.arguments[6].value, "posarg2") == 0);
	FREE_ARGV(5, argv3);

	printf("Positional arguments testing passed\n\n");
}

void test_nonalpha(SapConfig config)
{
	printf("Testing nonalpha positional and valued arguments...\n");

	printf("Testing positional arguments\n");
	char *argv1[5];
	copy_argv(5, argv1, "ctests", "123abc", "abcdefg", "-v", "value");
	assert(sap_parse_args(config, 5, argv1) == 0);
	assert(config.arguments[2].set == 1);
	assert(strcmp(config.arguments[2].value, "value") == 0);
	assert(strcmp(config.arguments[1].value, "123abc") == 0);
	assert(strcmp(config.arguments[6].value, "abcdefg") == 0);
	FREE_ARGV(5, argv1);

	printf("Testing valued option\n");
	char *argv2[5];
	copy_argv(5, argv2, "ctests", "123abc", "abcdefg", "-v", "123");
	assert(sap_parse_args(config, 5, argv2) == 0);
	assert(config.arguments[2].set == 1);
	assert(strcmp(config.arguments[2].value, "123") == 0);
	assert(strcmp(config.arguments[1].value, "123abc") == 0);
	assert(strcmp(config.arguments[6].value, "abcdefg") == 0);
	FREE_ARGV(5, argv2);
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
	delete config.arguments;

	return 0;
}
