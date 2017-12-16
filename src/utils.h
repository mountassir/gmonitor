/*
 * Copyright (C) 2015 Mountassir El Hafi, (mountassirbillah1@gmail.com)
 *
 * Utils.h: Part of gmonitor
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

#include "constants.h"

//display the help menu
void displayHelpMenu()
{
	cout << "====================\n"
		 << "       Usage\n"
		 << "====================\n"
	     << "-d [displayMode] 0 (default) Monitor both current and previous GPU states.\n"
		 << "                 1 Monitor most recent GPU state only.\n"
		 << "                 2 Monitor previous GPU states only.\n"
		 << "                 3 Same as 0, print current states for all GPU then print history.\n"
		 << "\n"
		 << "-g [gpuNumber]   GPU numbers to monitor, primary GPU's number is 0...\n"
		 << "\n"
		 << "-r [refreshRate] Monitoring refresh rate (default is 2 seconds).\n"
		 << "\n"
		 << "-o               Enable support for Optirun.\n"
		 << "\n"
		 << "-s               Needed if you are running over ssh.\n"
		 << "\n"
		 << "-h               Display this menu.\n"
		 << "====================\n"
		 << "      Examples\n"
		 << "====================\n"
		 << "-d 1 -g 02    Monitor the most recent state only for the first and third gpu.\n"
		 << "-d 0 -g 0123  Monitor current and history states for 4 GPUs.\n"
	     << "-d 0 -r 3     Monitor both current and previous states for all GPUs, refresh every 3 seconds.\n";
}

//parse the display mode arguments passed in
//by the user, will only consider the first
//character in the argument string passed to
//the d flag
DisplayMode modeSpecifiedByUser(const string &arg)
{
	//default mode
	DisplayMode mode = DEFAULT_DISPLAY_MODE;

	char c = arg[0];

	int value = c - '0';

	if( (value >= 0) && (value < 10) ) //if numeric
	{
		switch(value)
		{
			case CURRENT_NEXT_TO_HISTORY: { return CURRENT_NEXT_TO_HISTORY;}
			case CURRENT_STATE_ONLY:      { return CURRENT_STATE_ONLY;}
			case HISTORY_STATES_ONLY:     { return HISTORY_STATES_ONLY;}
			case CURRENT_THEN_HISTORY:    { return CURRENT_THEN_HISTORY;}
		}
	}

	return mode;
}

//parse the refresh arguments passed in
//by the user, will only consider the first
//character in the argument string passed to
//the r flag 1-9 seconds
int refreshRateSpecifiedByUser(const string &arg)
{
	char c = arg[0];

	int value = c - '0';

	if( (value > 0) && (value < 10) ) //if numeric
	{
		return value;
	}
	else
	{
		return DEFAULT_REFRESH_RATE;
	}
}

//parse the display mode arguments passed in
//by the user, will save all numeric values
//in the argument string passed in to the g flag
void gpusToMoniterSpecifiedByUser(vector<int> *gpus,
								  const string &arg)
{
	for(size_t i = 0, l = arg.length(); i < l; ++i)
	{
		char c = arg[i];

		int value = c - '0';

		if( (value >= 0) && (value < 10) ) //if numeric
		{
			gpus->push_back(value);
		}
	}
}
