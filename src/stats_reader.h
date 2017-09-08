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

/*
 * StatsReader class: probes Nvidia drivers and get the appropriate
 * gpu states; gpu usage, memory usage ...
 */
class StatsReader
{
private:
	//execute a bash command and return the output as double
	double getDoubleFromSystemCall(const string &command);

	//functions getting the appropriate command and executing it
	double getGpuUsage(const string &gpuId);
	double getGpuMemoryBandwidth(const string &gpuId);
	double getGpuPciBandwidth(const string &gpuId);
	double getGpuTemp(const string &gpuId);
	double getGpuTotalMemory(const string &gpuId);
	double getGpuUsedMemory(const string &gpuId);

	//functions returning bash commands to be executed
	//these commands will call Nvidia drivers with the
	//corresponding arguments, these commands may even
	//perform some sort of stream processing (grep, sed...)
	void gpuListCommand(string *gpuList);
	void gpuCoreUsageCommand(const string &gpuId, string *command);
	void gpuMemoryBandwidthCommand(const string &gpuId, string *command);
	void gpuPciBandwidthCommand(const string &gpuId, string *command);
	void gpuTempCommand(const string &gpuId, string *command);
	void gpuUsedMemoryCommand(const string &gpuId, string *command);
	void gpuTotalMemoryCommand(const string &gpuId, string *command);

	bool _optirun;

public:
	//get the list of available gpus on this machine
	bool getGpuList(vector<string> *gpuList);

	//get the current states of a particular gpu
	void getGpuStates(GpuStates *gpuStates,
            		  const string &gpuId);

	void useOptirun(const bool withOptirun);

	StatsReader(bool withOptirun = false) : _optirun(withOptirun) {};
};

#endif
