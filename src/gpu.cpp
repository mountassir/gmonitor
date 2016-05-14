/*
 * Copyright (C) 2015 Mountassir El Hafi, (mountassirbillah1@gmail.com)
 *
 * Gpu.cpp: Part of gmonitor
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

#include "gpu.h"

//constructor, set the gpu id, name and create
//an empty list of states
Gpu::Gpu(const std::string &id, const std::string &name)
{
	gpuId = id;
	gpuName = name;

	createGraph();
}

//create an empty list of states (set all states to 0)
void Gpu::createGraph()
{
	int count = 0;

	while(count < 10)
	{
		GpuStates state{0,0,0,0,0,0,0};

		graph.insert(graph.begin(), state);

		++count;
	}
}

//update this gpu's states
void Gpu::updateAllStates(const GpuStates &state)
{
	usedMemory      = state.memoryUsage;
	gpuUsage        = state.gpuUsage;
	memoryBandwidth = state.memoryBandwidth;
	pciBandwidth    = state.pciBandwidth;
	temperature     = state.coreTemp;
	totalMemory     = state.totalMemory;
	memoryUsage     = state.memoryUsage;

	graph.insert(graph.begin(), state);  //save the new state
	graph.pop_back();                    //remove the oldest state
}

//export this gpu's current states
void Gpu::exportCurrentState(GpuStates *state) const
{
	state->memoryUsage     = usedMemory;
	state->gpuUsage        = gpuUsage;
	state->memoryBandwidth = memoryBandwidth;
	state->pciBandwidth    = pciBandwidth;
	state->coreTemp        = temperature;
	state->totalMemory     = totalMemory;
	state->memoryUsage     = memoryUsage;
}

//export this gpu's all states
//(current one and all stored in -graph- list)
void Gpu::exportAllStates(vector<GpuStates> *states) const
{
	Graph::const_iterator iter = graph.begin();

	while(iter != graph.end())
	{
		states->push_back(*iter);

		++iter;
	}
}

//print all previous states
void Gpu::dumpGraph() const
{
	Graph::const_iterator iter = graph.begin();

	while(iter != graph.end())
	{
		cout << iter->gpuUsage << endl;
		++iter;
	}
}

ostream & operator<<(ostream &os, Gpu &gpu)
{
	return os << "GPU id:           " << gpu.getGpuId()            << "\n"
			  << "GPU name:         " << gpu.getGpuName()          << "\n"
			  << "Gpu usage:        " << gpu.getGpuUsage()         << "%\n"
			  << "Memory usage:     " << gpu.getMemoryUsage()      << "%\n"
			  << "Total memory:     " << gpu.getTotalMemory()      << "MB\n"
			  << "Used memory:      " << gpu.getUsedMemory()       << "MB\n"
			  << "GPU temp:         " << gpu.getTemperature()      << "c\n"
			  << "Memory bandwidth: " << gpu.getMemoryBandwidth()  << "%\n"
			  << "PCIe bandwidth:   " << gpu.getPciBandwidth()     << "%\n";
}
