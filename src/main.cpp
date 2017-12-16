/*
 * Copyright (C) 2015 Mountassir El Hafi, (mountassirbillah1@gmail.com)
 *
 * gmonitor.cpp: Part of gmonitor
 *
 * gmonitor is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 3 as
 * published by the Free Software Foundation.
 *
 * gmonitor is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with gmonitor.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <stdlib.h>
#include <cstring>

#include "manager.h"
#include "utils.h"
#include "constants.h"

using namespace std;

int main(int argc, char **argv)
{
	char *dFlag = NULL; //display mode flag
	char *gFlag = NULL; //gpus to monitor flag
	char *rFlag = NULL; //refresh rate flag
	int oFlag = 0;      //optirun flag
	int sFlag = 0;      //overSsh flag
	int hFlag = 0;      //help flag

	int flag;

	opterr = 0;

	//parse arguments
	while ((flag = getopt(argc, argv, "d:g:r:hos")) != -1)
	{
		switch (flag)
		{
			case 'd': { dFlag = optarg; break;}
			case 'g': { gFlag = optarg; break;}
			case 'r': { rFlag = optarg; break;}
			case 'o': { oFlag = 1;      break;}
			case 's': { sFlag = 1;      break;}
			case 'h': { hFlag = 1;      break;}
			case '?':
			{
				if ((optopt == 'd') || (optopt == 'g') || (optopt == 'r'))
				{
					fprintf (stderr, "Option -%c requires an argument.\n", optopt);
				}
				else if (isprint (optopt))
				{
					fprintf (stderr, "Unknown option `-%c'.\n", optopt);
				}
				else
				{
					fprintf (stderr, "Unknown option character `\\x%x'.\n", optopt);
				}

				return 1;
			}
			default: abort ();
		}
	}

	//display the help menu
	if(hFlag)
	{
		displayHelpMenu();
		return 0;
	}

	DisplayMode mode = DEFAULT_DISPLAY_MODE;

	//set the mode (specified by the user)
	if(dFlag)
	{
		mode = modeSpecifiedByUser(string(dFlag));
	}

	int refreshRate = DEFAULT_REFRESH_RATE;

	//set the refresh rate (specified by the user)
	if(rFlag)
	{
		refreshRate = refreshRateSpecifiedByUser(string(rFlag));
	}

	vector<int> gpusToMonitor;

	//set the gpus to monitor (specified by the user)
	if(gFlag)
	{
		gpusToMoniterSpecifiedByUser(&gpusToMonitor, string(gFlag));
	}

	bool optirun = false;

	//enable optirun support
	if(oFlag)
	{
		optirun = oFlag;
	}

	bool overSsh = false;

	//enable overSsh command
	if(sFlag)
	{
		overSsh = sFlag;
	}

	Manager manager(mode, refreshRate, overSsh, optirun);

	manager.startMonitoring(gpusToMonitor);

    return 0;
}


