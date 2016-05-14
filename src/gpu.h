/*
 * Copyright (C) 2015 Mountassir El Hafi, (mountassirbillah1@gmail.com)
 *
 * Gpu.h: Part of gMonitor
 *
 * gMonitor is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 3 as
 * published by the Free Software Foundation.
 *
 * gMonitor is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with gMonitor.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef GPU_H_
#define GPU_H_

#include <iostream>
#include <vector>
#include "constants.h"

using namespace std;

/*
 * This class represents a single graphics card,
 * stores, updates and exports all available statistics;
 * gpu usage, memory usage ...
 */
class Gpu
{
private:
	Graph       graph;       //a list of previous status of this gpu
	std::string gpuId;       //gpu id (from Nvidia drivers)
	std::string gpuName;     //gpu name (from Nvidia drivers)
	double totalMemory;      //total memory available on this gpu
	double usedMemory;       //memory used on this gpu
	double memoryUsage;      //memory usage (%)
	double gpuUsage;         //gpu usage
	double memoryBandwidth;  //memory bus usage
	double pciBandwidth;     //pci-e bus usage
	double temperature;      //gpu temperature

public:
	Gpu(const std::string &id, const std::string &name);

	//getters
	string getGpuId()           { return gpuId;          };
	string getGpuName()         { return gpuName;        };
	double getTotalMemory()     { return totalMemory;    };
	double getUsedMemory()      { return usedMemory;     };
	double getMemoryUsage()     { return memoryUsage;    };
	double getGpuUsage()        { return gpuUsage;       };
	double getMemoryBandwidth() { return memoryBandwidth;};
	double getPciBandwidth()    { return pciBandwidth;   };
	double getTemperature()     { return temperature;    };

	//setters
	void setTotalMemory    (double value) { totalMemory     = value;};
	void setUsedMemory     (double value) { usedMemory      = value;};
	void setMemoryUsage    (double value) { memoryUsage     = value;};
	void setGpuUsage       (double value) { gpuUsage        = value;};
	void setMemoryBandwidth(double value) { memoryBandwidth = value;};
	void setPciBandwidth   (double value) { pciBandwidth    = value;};
	void setTemperature    (double value) { temperature     = value;};

	//update this gpu's states
	void updateAllStates(const GpuStates &state);

	//export this gpu's all states
	//(current one and all stored in -graph- list)
	void exportAllStates(vector<GpuStates> *states) const;

	//export this gpu's current states
	void exportCurrentState(GpuStates *state) const;

	//create an empty list of states
	void createGraph();

	//print all previous states
	void dumpGraph() const;
};

ostream & operator<<(ostream &os, Gpu &gpu);

#endif
