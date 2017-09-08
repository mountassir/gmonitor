/*
 * Copyright (C) 2015 Mountassir El Hafi, (mountassirbillah1@gmail.com)
 *
 * Manager.h: Part of gmonitor
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

#ifndef MANAGER_H_
#define MANAGER_H_

#include <vector>
#include "stats_reader.h"
#include "console_writer.h"
#include "constants.h"
#include "gpu.h"

using namespace std;

/*
 * Manager class: set up list of gpus, probe Nvidia drivers
 * for state updates and print results for the user
 */
class Manager
{
private:
	StatsReader    statsReader;    //get gpu states
	ConsoleWriter  consoleWriter;  //print gpu states on screen
	vector<Gpu>    allGpus;        //list of all gpus available on the system
	DisplayMode    displayMode;    //what to print on screen for each gpu
	int            refreshRate;    //monitoring refresh rate

	//get all available gpus on the system
	void buildGpus();

	//update gpu states manually
	void updateGpuStates(Gpu *gpu, const GpuStates &states);

	//get gpu identifier and name from Nvidia output;
	//[0] mountassir-workstation:0[gpu:0] (GeForce GTX 770)
	//to gpuId = [gpu:0] and gpuName = GeForce GTX 770
	void decomposeGpuIdentifier(const string &gpuIdentifier,
								string *gpuId,
								string *gpuName);

	//generate random gpu state
	//(used for testing when no gpu available)
	void generateState(GpuStates *state);

	//print all available gpus
	void dumpAllGpus();

	//check if their is no gpu to monitor
	bool noGpusToMonitor();

public:
	Manager(DisplayMode mode,
			int refresh,
			bool optirun = false) :
				displayMode(mode),
				refreshRate(refresh),
				statsReader(optirun) {};

	Manager(bool optirun = false) :
				displayMode(CURRENT_NEXT_TO_HISTORY),
				refreshRate(DEFAULT_REFRESH_RATE),
				statsReader(optirun) {};

	//start monitoring the given gpus
	void startMonitoring(const vector<int> &gpusToMonitor);

	//probe Nvidia drivers for current gpu states
	void refreshGpus();

	//monitor and display; current state only,
	//history only or both for each gpu
	void monitorCurrentAndPreviousStates();
	void monitorCurrentAndPreviousStates(const vector<int> &gpusToMonitor);

	//monitor and display; current state for all gpus
	//then all of their previous states
	void monitorCurrentThenPreviousStates();
	void monitorCurrentThenPreviousStates(const vector<int> &gpusToMonitor);

	//print the most recent state this gpu was at
	void printCurrentGpuState(const Gpu &gpu);

	//print all previous gpu states
	void printHistoryGpuState(const Gpu &gpu);

	//change the monitoring refresh rate
	void setRefreshRate(int refresh) { refreshRate = refresh;};
};

#endif
