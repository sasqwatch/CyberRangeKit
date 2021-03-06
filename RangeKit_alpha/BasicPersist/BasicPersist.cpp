#include "stdafx.h"

BOOL RunTool();

void logo(void)
{
	xprintf(OPLOG_NORMAL, L"#  Cyber RangeKit \n");
	xprintf(OPLOG_NORMAL, L"#  -------------------------------------------    \n");
	xprintf(OPLOG_NORMAL, L"#  TOOL: BasicPersist %ws \n", VERSION);
	xprintf(OPLOG_NORMAL, L"#  Authors: Hoglund 2018 \n");
	xprintf(OPLOG_NORMAL, L"#  ------------------------------------------- \n");
}

void usage(void)
{
	wprintf(L"Usage: BasicPersist [-hnq] [-v|vv] [-NOCLEAN] [-f <mode>] [-f{RN}] [-p <mode>] [-p{RN}] \n\n");

	WCHAR help2[] =
		L"  examples                 \n"
		L"  -------------            \n"
		L"  'BasicPersist' // Run with defaults. Drop file in CWD and use Windows Run key, random names \n"
		L"  'BasicPersist -fN APD --filename myfile.bin' // Drop in AppData. Use filename 'myfile.bin' \n"
		L"  'BasicPersist -f WIN -p TSK' // Drop file in windows directory, use a task to persist, random names \n"
		L"  \n"
		L"  basic options            \n"
		L"  -------------            \n"
		L"  -h         Display this help message and exit \n"
		L"  -n         Display version number and exit \n"
		L"  -v         Verbose mode \n"
		L"  -vv        Extra-verbose mode (use for debug tracing) \n"
		L"  -q         Quiet mode. Consider using with -w as no prompt given when test is live \n"
		L"  -w <ms>    Wait time in ms.  Disables user input prompt to complete test.  Use with scripting or -q \n"
		L"  --pretty   Use pretty print. \n"
		L"  --NOCLEAN  Don't clean up.  Exit after applying changes. \n"
		L"  * warning DO NOT use --NOCLEAN unless you intend to leave artifacts behind! \n"
		L"  \n"
		L"  file options            \n"
		L"  ------------            \n"
		L"  -f <file drop mode> [-f{RN}] \n"
		L"  -f can be omitted and default will be used (equivalent to '-f CWD -fR') \n"
		L"  modes: \n"
		L"  -f CWD   Use Current Working Directory (DEFAULT) \n"
		L"  -f APD   Use AppData directory \n"
		L"  -f WIN   Use Windows directory \n"
		L"  extended options: \n"
		L"  -fN <filename>  Manually specify the name of the file on disk \n"
		L"  -fR <seed>      Seed the pseudo-random filename (use for reproducibility) \n"
		L"  -fR TIME        Seed with tickcount (DEFAULT) \n"
		L"  \n"
		L"  persistence options            \n"
		L"  -------------------            \n"
		L"  -p{RN} <persistence mode>      \n"
		L"  -p can be omitted and default will be used (equivalent to '-pR RUN') \n"
		L"  modes: \n"
		L"  -p RUN   Registry Run Key  (DEFAULT) \n"
		L"  -p MES   Modify Existing Service \n"
		L"  -p SVC   New Service \n"
		L"  -p TSK   Schedule New Task \n"
		L"  -p LPM   Local Port Monitor \n"
		L"  mode extensions: \n"
		L"  -p				Same as -pR \n"
		L"  -pR <seed>      Seed the pseudo-random key, task, or service name (use for reproducibility) \n"
		L"  -pR TIME        Seed with tickcount (DEFAULT) \n"
		L"  -pN	<name>		Manually specify the name of the key, task, or service (by context) \n"
		//L"  additional settings: \n"
        //L"  --keyname <name>       Specify manual keyname on commandline. Optional. \n"
		//L"  --taskname <name>      Specify manual taskname on commandline. Optional. \n"
		//L"  --servicename <name>   Specify manual servicename on commandline. Optional. \n"
		L"  \n"
		L"  decoy options            \n"
		L"  -------------            \n"
		L"  -d SIMPLE     Use Simple PE EXE  (DEFAULT) \n"
		L"  -d COPYDROP   Use CopyDrop (Makes 2nd copy and launches it) \n"
		L"  \n"
		L"  \n";

	wprintf(help2);
	wprintf(L"\n");
}

void try_msg(void)
{
	wprintf(L"Try `%hs -h` for more information.\n",  "BasicPersist");
}

// modify option string here
#define OPTION_STRING L"hnvqf:w:p:-"

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
		
		case L'f':
			if (optmod)
			{
				// its -f[mod]

				// with a mod, its how to generate the file name
				// or some other behavioral modifier

				if (0 == wcscmp(L"N", optmod))
				{
					// they are manually specifying the file name (-fN <filename>)
					if (optarg)
					{
						g_progopts.use_manual_filename = true;
						_snwprintf_s(g_progopts.file_name, MAX_PATH - 2, _TRUNCATE, L"%ws", optarg);
						// no auto-NULL truncate capability if source is too large --> wcscpy_s(g_progopts.file_name, MAX_PATH - 2, optarg);
					}
					else
					{
						xprintf(OPLOG_NORMAL, L"! You must supply a filename with the -fN flag.\n");
						exit(EXIT_FAILURE);
					}
				}
				else if (0 == wcscmp(L"R", optmod))
				{
					// they want a random file name (-fR)
					// this is default with a seed of zero
				
					if (optarg)
					{
						// they are explicitly setting the seed
						int res = _wtoi(optarg);
						if (0 == res && EINVAL == errno)
						{
							xprintf(OPLOG_NORMAL, L"! When setting the -fR seed you must use a valid integer value.\n");
							exit(EXIT_FAILURE);
						}
						g_progopts.filename_seed = res;
					}
				}
			}
			else if (optarg)
			{
				// its -f <arg>
				
				xprintf(OPLOG_DEBUG, L"~ parsing -f optarg %ws\n", optarg);
				if (0 == wcscmp(L"APD", optarg))
				{
					xprintf(OPLOG_VERBOSE, L"> Using AppData directory for file drop\n");
					g_progopts.file_mode = PROG_FILEMODE::OPF_APPDATA;
				}
				else if (0 == wcscmp(L"WIN", optarg))
				{
					xprintf(OPLOG_VERBOSE, L"> Using Windows directory for file drop\n");
					g_progopts.file_mode = PROG_FILEMODE::OPF_WINDOWS;
				}
				else if (0 == wcscmp(L"CWD", optarg))
				{
					xprintf(OPLOG_VERBOSE, L"> Using current working directory for file drop\n");
					g_progopts.file_mode = PROG_FILEMODE::OPF_CWD;
				}
				else
				{
					xprintf(OPLOG_NORMAL, L"! Did not recognize the supplied file placement mode.\n");
					exit(EXIT_FAILURE);
				}
			}
			else
			{
				xprintf(OPLOG_NORMAL, L"! You must supply a file placement mode with the -f flag.\n");
				exit(EXIT_FAILURE);
			}
			
			break;

		case L'p':
			if (optarg)
			{
				xprintf(OPLOG_DEBUG, L"~ parsing -p optarg %ws\n", optarg);

				if (0 == wcscmp(L"MES", optarg))
				{
					xprintf(OPLOG_VERBOSE, L"> Using existing service\n");
					g_progopts.persist_mode = PROG_PERSISTMODE::OPP_EXISTINGSERVICE;
				}
				else if (0 == wcscmp(L"SVC", optarg))
				{
					xprintf(OPLOG_VERBOSE, L"> Creating new service\n");
					g_progopts.persist_mode = PROG_PERSISTMODE::OPP_NEWSERVICE;
				}
				else if (0 == wcscmp(L"TSK", optarg))
				{
					xprintf(OPLOG_VERBOSE, L"> Creating new task\n");
					g_progopts.persist_mode = PROG_PERSISTMODE::OPP_NEWTASK;
				}
				else
				{
					xprintf(OPLOG_NORMAL, L"! Did not recognize the supplied persistence mode\n");
					exit(EXIT_FAILURE);
				}
			}
			else
			{
				xprintf(OPLOG_NORMAL, L"! You must supply a persistence mode with the -p flag.\n");
				exit(EXIT_FAILURE);
			}

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
		//			g_toolstate.username.assign(optarg);
		//		}
		//	}
		//	break;

		//case 'p':
		//	if (optarg)
		//	{
		//		g_toolstate.password.assign(optarg);
		//	}
		//	break;

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
