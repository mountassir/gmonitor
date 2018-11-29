/*
 * Copyright (C) 2015 Mountassir El Hafi, (mountassirbillah1@gmail.com)
 *
 * Reader.h: Part of gmonitor
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

#ifndef STATS_READER_H_
#define STATS_READER_H_

#include <iostream>
#include <vector>
#include <stdio.h>
#include "constants.h"

using namespace std;

namespace 
{
	const int NUMBER_OF_SUPPORTED_STATES = 7;

	enum StateIndex
	{
		GRAPHICS_USAGE_INDEX = 0,
		VRAM_BANDWIDTH_INDEX = 1,
		VIDEO_USAGE_INDEX    = 2,
		PCIE_BADWIDTH_INDEX  = 3,
		CORE_TEMP_INDEX      = 4,
		TOTAL_VRAM_INDEX     = 5,
		USED_VRAM_INDEX      = 6,
	};
}

/*
 * StatsReader class: probes Nvidia drivers and get the appropriate
 * gpu states; gpu usage, memory usage ...
 */
class StatsReader
{
private:
	//execute a bash command and return the output as an array doubles
	bool getDoubleFromSystemCall(string &command, std::vector<double> *values);

	//functions returning bash commands to be executed
	//these commands will call Nvidia drivers with the
	//corresponding arguments, these commands may even
	//perform some sort of stream processing (grep, sed...)
	void gpuListCommand(string *gpuList);
	void getAllGpuStatesCommand(const string &gpuId, string *command);

	string checkIfSshCommand(string &command);

	bool _overSsh;
	bool _optirun;

public:
	//get the list of available gpus on this machine
	bool getGpuList(vector<string> *gpuList);

	//get the current states of a particular gpu
	void getGpuStates(GpuStates *gpuStates,
            		  const string &gpuId);

	void useOptirun(const bool withOptirun);

	StatsReader(bool overSsh, bool withOptirun = false) :
		_overSsh(overSsh), _optirun(withOptirun) {};
};

#endif
