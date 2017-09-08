/*
 * Copyright (C) 2015 Mountassir El Hafi, (mountassirbillah1@gmail.com)
 *
 * Manager.cpp: Part of gmonitor
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

#include <cstdlib>
#include <unistd.h>

#include <stdio.h>
#include "manager.h"

//get all available gpus on the system
void Manager::buildGpus()
{
	vector<string> gpuList;

	statsReader.getGpuList(&gpuList);

	for(size_t i = 0, l = gpuList.size(); i < l; ++i)
	{
		string name, id;

		decomposeGpuIdentifier(gpuList[i], &id, &name);

		allGpus.push_back(Gpu (id, name));
	}
}

//update gpu states manually
void Manager::updateGpuStates(Gpu *gpu, const GpuStates &states)
{
	gpu->setTotalMemory(states.totalMemory);
	gpu->setUsedMemory(states.memoryUsage);
	gpu->setGpuUsage(states.gpuUsage);
	gpu->setMemoryBandwidth(states.memoryBandwidth);
	gpu->setPciBandwidth(states.pciBandwidth);
	gpu->setTemperature(states.coreTemp);
}

//get gpu identifier and name from Nvidia output;
//[0] mountassir-workstation:0[gpu:0] (GeForce GTX 770)
//to gpuId = [gpu:0] and gpuName = GeForce GTX 770
void Manager::decomposeGpuIdentifier(const string &gpuIdentifier,
								  	 string *gpuId,
									 string *gpuName)
{
	size_t idStart = gpuIdentifier.find_last_of("[");
	size_t idEnd = gpuIdentifier.find_last_of("]");

	*gpuId = gpuIdentifier.substr(idStart, (idEnd - idStart) + 1);

	size_t nameStart = gpuIdentifier.find_last_of("(");
	size_t nameEnd = gpuIdentifier.find_last_of(")");

	*gpuName = gpuIdentifier.substr(nameStart + 1, nameEnd - nameStart -1);
}

//generate random gpu state
//(used for testing when no gpu available)
void Manager::generateState(GpuStates *state)
{
	state->gpuUsage        = rand() % 100 + 1;
	state->memoryUsage     = rand() % 100 + 1;
	state->pciBandwidth    = rand() % 100 + 1;
	state->memoryBandwidth = rand() % 100 + 1;
	state->coreTemp        = rand() % 100 + 1;
}

//print all available gpus
void Manager::dumpAllGpus()
{
	for(size_t i = 0, l = allGpus.size(); i < l; ++i)
	{
		cout << allGpus[i];
	}
}

//probe Nvidia drivers for current gpu states
void Manager::refreshGpus()
{
	for(size_t i = 0, l = allGpus.size(); i < l; ++i)
	{
		GpuStates states;

		statsReader.getGpuStates(&states, allGpus[i].getGpuId());

		allGpus[i].updateAllStates(states);
	}
}

//start monitoring the given gpus
void Manager::startMonitoring(const vector<int> &gpusToMonitor)
{
	buildGpus();

	if(noGpusToMonitor())
	{
		return;
	}

	if(gpusToMonitor.size() == 0)
	{
		if(displayMode == CURRENT_THEN_HISTORY)
		{
			monitorCurrentThenPreviousStates();
		}
		else
		{
			monitorCurrentAndPreviousStates();
		}
	}
	else
	{
		if(displayMode == CURRENT_THEN_HISTORY)
		{
			monitorCurrentThenPreviousStates(gpusToMonitor);
		}
		else
		{
			monitorCurrentAndPreviousStates(gpusToMonitor);
		}
	}
}

//monitor and display; current state only,
//history only or both for each gpu
void Manager::monitorCurrentAndPreviousStates()
{
	if(noGpusToMonitor())
	{
		return;
	}

	while(true)
	{
		refreshGpus();
		consoleWriter.clearConsole();

		for(size_t i = 0; i < allGpus.size(); ++i)
		{
			consoleWriter.printGpuIdentifier(allGpus[i].getGpuId(),
					                  allGpus[i].getGpuName(),
					                  allGpus[i].getTotalMemory());

			if( (displayMode == CURRENT_STATE_ONLY) ||
				(displayMode == CURRENT_NEXT_TO_HISTORY))
			{
				printCurrentGpuState(allGpus[i]);
			}

			if( (displayMode == HISTORY_STATES_ONLY) ||
				(displayMode == CURRENT_NEXT_TO_HISTORY))
			{
				printHistoryGpuState(allGpus[i]);
			}
		}

		sleep(refreshRate);
	}
}

//monitor and display; current state for all gpus
//then all of their previous states
void Manager::monitorCurrentThenPreviousStates()
{
	if(noGpusToMonitor())
	{
		return;
	}

	while(true)
	{
		refreshGpus();
		consoleWriter.clearConsole();

		for(size_t i = 0; i < allGpus.size(); ++i)
		{
			consoleWriter.printGpuIdentifier(allGpus[i].getGpuId(),
					                  allGpus[i].getGpuName(),
					                  allGpus[i].getTotalMemory());

			printCurrentGpuState(allGpus[i]);
		}

		for(size_t i = 0; i < allGpus.size(); ++i)
		{
			consoleWriter.printGpuIdentifier(allGpus[i].getGpuId(),
					                  allGpus[i].getGpuName(),
					                  allGpus[i].getTotalMemory());

			printHistoryGpuState(allGpus[i]);
		}

		sleep(refreshRate);
	}
}

//monitor and display; current state only,
//history only or both for the specified gpus only
void Manager::monitorCurrentAndPreviousStates(const vector<int> &gpusToMonitor)
{
	if(noGpusToMonitor())
	{
		return;
	}

	while(true)
	{
		refreshGpus();
		consoleWriter.clearConsole();

		for(size_t i = 0; i < gpusToMonitor.size(); ++i)
		{
			size_t gpuIndex = gpusToMonitor[i];

			if( (gpuIndex >= 0) && (gpuIndex < allGpus.size()) )
			{
				consoleWriter.printGpuIdentifier(allGpus[gpuIndex].getGpuId(),
						                  allGpus[gpuIndex].getGpuName(),
						                  allGpus[gpuIndex].getTotalMemory());

				if( (displayMode == CURRENT_STATE_ONLY) ||
					(displayMode == CURRENT_NEXT_TO_HISTORY))
				{
					printCurrentGpuState(allGpus[gpuIndex]);
				}

				if( (displayMode == HISTORY_STATES_ONLY) ||
					(displayMode == CURRENT_NEXT_TO_HISTORY))
				{
					printHistoryGpuState(allGpus[gpuIndex]);
				}
			}
		}

		sleep(refreshRate);
	}
}

//monitor and display; current state for the specified gpus only
//then all of their previous states
void Manager::monitorCurrentThenPreviousStates(const vector<int> &gpusToMonitor)
{
	if(noGpusToMonitor())
	{
		return;
	}

	while(true)
	{
		refreshGpus();
		consoleWriter.clearConsole();

		for(size_t i = 0; i < gpusToMonitor.size(); ++i)
		{
			size_t gpuIndex = gpusToMonitor[i];

			if( (gpuIndex >= 0) && (gpuIndex < allGpus.size()) )
			{
				consoleWriter.printGpuIdentifier(allGpus[gpuIndex].getGpuId(),
						                  allGpus[gpuIndex].getGpuName(),
						                  allGpus[gpuIndex].getTotalMemory());

				printCurrentGpuState(allGpus[gpuIndex]);
			}
		}

		for(size_t i = 0; i < gpusToMonitor.size(); ++i)
		{
			size_t gpuIndex = gpusToMonitor[i];

			if( (gpuIndex >= 0) && (gpuIndex < allGpus.size()) )
			{
				consoleWriter.printGpuIdentifier(allGpus[gpuIndex].getGpuId(),
						                  allGpus[gpuIndex].getGpuName(),
						                  allGpus[gpuIndex].getTotalMemory());

				printHistoryGpuState(allGpus[gpuIndex]);
			}
		}

		sleep(refreshRate);
	}
}

//print the most recent state this gpu was at
void Manager::printCurrentGpuState(const Gpu &gpu)
{
	GpuStates state;

	gpu.exportCurrentState(&state);

	consoleWriter.printCurrentGpuState(state);
}

//print all previous gpu states
void Manager::printHistoryGpuState(const Gpu &gpu)
{
	vector<GpuStates> states;

	gpu.exportAllStates(&states);

	consoleWriter.printHistoryStates(states);
}

//check if their is no gpu to monitor
bool Manager::noGpusToMonitor()
{
	if(allGpus.size() == 0)
	{
		cout << "No gpus to monitor, terminating." << endl;
		return true;
	}

	return false;
}
