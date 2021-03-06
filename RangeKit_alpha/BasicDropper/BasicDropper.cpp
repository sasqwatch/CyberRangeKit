#include "stdafx.h"

#define OPTION_STRING L"hnvq:AW"

BOOL RunTool();

void logo(void)
{
	xprintf(OPLOG_NORMAL, L"#  Cyber RangeKit \n");
	xprintf(OPLOG_NORMAL, L"#  -------------------------------------------    \n");
	xprintf(OPLOG_NORMAL, L"#  TOOL: BasicDropper %ws \n", VERSION);
	xprintf(OPLOG_NORMAL, L"#  Authors: Hoglund 2018 \n");
	xprintf(OPLOG_NORMAL, L"#  ------------------------------------------- \n");
}

void usage(void)
{
	wprintf(L"Usage: BasicDropper [-hV] [-as] [-vq]\n\n");

	WCHAR help2[] =
		L"                     \n"
		L"  basic options            \n"
		L"  -------------            \n"
		L"  -h       Display this help message and exit  \n"
		L"  -n       Display version number and exit     \n"
		L"  -v       Verbose mode \n"
		L"  -q       Quiet mode. \n"
		L"  \n"
		L"  behavior options            \n"
		L"  ----------------            \n"
		L"  -A       Use AppData directory  \n"
		L"  -W       Use Windows directory  \n"
		L"  \n"
		L"  If no directory is specified, the tool will use the current directory.\n";

	wprintf(help2);
	wprintf(L"\n");
}

void try_msg(void)
{
	wprintf(L"Try `BasicDropper -h` for more information.\n");
}

void process_cmd_line(int argc, WCHAR **argv)
{
	//unsigned char tmp;
	WCHAR c;

	while ((c = wgetopt(argc, argv, OPTION_STRING)) != NULL)
	{
		switch (c)
		{

		case 'h':
			usage();
			exit(EXIT_SUCCESS);

		case 'n':
			wprintf(L"%ws\n", VERSION);
			exit(EXIT_SUCCESS);

		case 'v':
			g_progopts.log_mode = PROG_LOGMODE::OPLOG_VERBOSE;
			break;

		case 'q':
			g_progopts.log_mode = PROG_LOGMODE::OPLOG_QUIET;
			break;

		case 'A':
			g_progopts.file_mode = PROG_FILEMODE::OPF_APPDATA;
			break;

		case 'W':
			g_progopts.file_mode = PROG_FILEMODE::OPF_WINDOWS;
			break;

		//case 'u':
		//	if (optarg)
		//	{
		//		// parse the domain if provided
		//		LPWSTR slash = wcschr(optarg, L'\\');
		//		if (slash != NULL)
		//		{
		//			g_toolstate.username.assign(slash + 1);
		//			g_toolstate.domain.assign(optarg, slash - optarg);
		//		}
		//		else
		//		{
		//				g_toolstate.username.assign(optarg);
		//		}
		//	}
		//	break;

		//case 'p':
		//	if (optarg)
		//	{
		//		g_toolstate.password.assign(optarg);
		//	}
		//	break;

		default:
			try_msg();
			exit(EXIT_FAILURE);
		}
	}
}

int wmain(int argc, WCHAR* argv[])
{
	// note, the subroutine process_cmd_line will call exit() for some cases like -V or -h or invalid options
	process_cmd_line(argc, argv);

	if(g_progopts.log_mode > PROG_LOGMODE::OPLOG_QUIET) logo();

	RunTool();

    return 0;
}
