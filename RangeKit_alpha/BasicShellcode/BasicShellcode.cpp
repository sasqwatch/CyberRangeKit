#include "stdafx.h"

BOOL RunTool();

void logo(void)
{
	xprintf(OPLOG_NORMAL, L"#  Cyber RangeKit \n");
	xprintf(OPLOG_NORMAL, L"#  -------------------------------------------    \n");
	xprintf(OPLOG_NORMAL, L"#  TOOL: BasicShellcode %ws \n", VERSION);
	xprintf(OPLOG_NORMAL, L"#  Authors: Hoglund 2018 \n");
	xprintf(OPLOG_NORMAL, L"#  ------------------------------------------- \n");
}

void usage(void)
{
	wprintf(L"Usage: BasicShellcode [-hnq] [-v|vv] [-NOCLEAN] [-f <mode>] [-f{RN}] [-p <mode>] [-p{RN}] \n\n");

	WCHAR help2[] =
		L"  basic options            \n"
		L"  -------------            \n"
		L"  -h         Display this help message and exit \n"
		L"  -n         Display version number and exit \n"
		L"  -v         Verbose mode \n"
		L"  -vv        Extra-verbose mode (use for debug tracing) \n"
		L"  -q         Quiet mode. Consider using with -w as no prompt given when test is live \n"
		L"  -w <ms>    Wait time in ms.  Disables user input prompt to complete test.  Use with scripting or -q \n"
		L"  --pretty   Use pretty print. \n"
		L"  \n"
		L"  \n";

	wprintf(help2);
	wprintf(L"\n");
}

void try_msg(void)
{
	wprintf(L"Try `%hs -h` for more information.\n",  "BasicShellcode");
}

// modify option string here
#define OPTION_STRING L"hnvqw:-"

// this is to pre-load all the verbosity settings for output logging
// and any other settings that should be pre-installed before full command-line parsing begins
// best practice is ONLY set console-output options here, all logic should be in the normal process_cmd_line
void pre_process_cmd_line(int argc, WCHAR **argv)
{
	WCHAR optw;
	
	// reset command line parsing
	optind = 0;

	while ((optw = wgetopt(argc, argv, OPTION_STRING)) != NULL)
	{
		switch (optw)
		{
		case L'?':
			xprintf(OPLOG_NORMAL, L"! Command line options were not understood\n");
			try_msg();
			exit(EXIT_FAILURE);

		case L'h':
			xprintf(OPLOG_DEBUG, L"~ Displaying usage and exit \n");
			usage();
			exit(EXIT_SUCCESS);

		case L'n':
			xprintf(OPLOG_DEBUG, L"~ Displaying version number and exit \n");
			wprintf(L"%ws\n", VERSION);
			exit(EXIT_SUCCESS);

		case L'v':
			if (NULL == optmod)
			{
				g_progopts.log_mode = PROG_LOGMODE::OPLOG_VERBOSE;
			}
			else if (0 == wcscmp(L"v", optmod))
			{
				g_progopts.log_mode = PROG_LOGMODE::OPLOG_DEBUG;
			}
			else
			{
				xprintf(OPLOG_NORMAL, L"! Did not recognize the supplied verbosity mode.\n");
				exit(EXIT_FAILURE);
			}

			break;

		case L'q':
			g_progopts.log_mode = PROG_LOGMODE::OPLOG_QUIET;
			break;

		case L'-':
			if (optmod)
			{
				if (0 == wcscmp(L"pretty", optmod))
				{
					g_progopts.pretty_print = true;
				}
			}
			break;

		default:
			break;
		}
	}
}

void process_cmd_line(int argc, WCHAR **argv)
{
	WCHAR optw;

	// reset command line parsing
	optind = 0;

	while ((optw = wgetopt(argc, argv, OPTION_STRING)) != NULL)
	{
		switch (optw)
		{
		case L'?':
			xprintf(OPLOG_NORMAL, L"! Command line options were not understood\n");
			try_msg();
			exit(EXIT_FAILURE);
		
		case L'v':
			break; //skip, this was handled in preprocessing

		case L'q':
			break; //skip, this was handled in preprocessing

		case L'n':
			break; //skip, this was handled in preprocessing

		case L'h':
			break; //skip, this was handled in preprocessing

		case L'w':
			if (optarg)
			{
				xprintf(OPLOG_DEBUG, L"~ parsing -w optarg %ws\n", optarg);
				DWORD wait_time = _wtoi(optarg);
				if (0 == wait_time)
				{
					xprintf(OPLOG_NORMAL, L"! Specified wait time was not understood? Time in ms must be an integer and non-zero\n");
					exit(EXIT_FAILURE);
				}
				g_progopts.use_wait_time = true;
				g_progopts.wait_time = wait_time;
			}
			else
			{
				// no wait time specified
				xprintf(OPLOG_NORMAL, L"! A wait time must be specified with -w\n");
				exit(EXIT_FAILURE);
			}
			break;
		
		/*
		case L'-':
			if (optmod)
			{
				if (0 == wcscmp(L"NOCLEAN", optmod))
				{
					xprintf(OPLOG_VERBOSE, L"> Setting NOCLEAN to TRUE!\n");
					g_progopts.no_clean = true;
				}
			}
			break; 
		*/

		default:
			try_msg();
			exit(EXIT_FAILURE);
		}
	}
}

int wmain(int argc, WCHAR* argv[])
{
	// note, the subroutine pre_process_cmd_line will call exit() for some cases like -V or -h or invalid options
	pre_process_cmd_line(argc, argv);
	
	if (g_progopts.log_mode > PROG_LOGMODE::OPLOG_QUIET) logo();

	// setup any initial values that are default, but may be overridden by the process_cmd_line below
	DWORD tick_count = GetTickCount();
	g_progopts.kst_seed = tick_count;
	g_progopts.filename_seed = tick_count;

	// note, the subroutine process_cmd_line will call exit() for some cases like -V or -h or invalid options
	process_cmd_line(argc, argv);
	
	// Note that even if something fails after this point it could be due to security controls
	// Thus, it is not considered a failure of the tool itself if an operation fails below

	xprintf(OPLOG_NORMAL, L"# Starting tool...\n");

	if (!RunTool())
	{
		xprintf(OPLOG_NORMAL, L"! The tool was not entirely successful. Refer to output for details.\n");
		xprintf(OPLOG_NORMAL, L"# The tool completed with errors.\n");
		return EXIT_FAILURE;
	}

	xprintf(OPLOG_NORMAL, L"# The tool completed successfully!\n");

    return EXIT_SUCCESS;
}
