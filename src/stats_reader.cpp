/*
 * Copyright (C) 2015 Mountassir El Hafi, (mountassirbillah1@gmail.com)
 *
 * StatsReader.cpp: Part of gmonitor
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

#include "stats_reader.h"
#include <sstream>
#include <string>
#include <cstdlib>

//get the current states of a particular gpu
void StatsReader::getGpuStates(GpuStates *gpuStates,
            		           const string &gpuId)
{
	string command;

	getAllGpuStatesCommand(gpuId, &command);

	std::vector<double> values;

	if(!getDoubleFromSystemCall(command, &values) || (values.size() != NUMBER_OF_SUPPORTED_STATES))
	{
		return;
	}

	gpuStates->gpuUsage        = values[GRAPHICS_USAGE_INDEX];
	gpuStates->memoryBandwidth = values[VRAM_BANDWIDTH_INDEX];
	gpuStates->pciBandwidth    = values[PCIE_BADWIDTH_INDEX];
	gpuStates->totalMemory     = values[TOTAL_VRAM_INDEX];
	gpuStates->usedMemory      = values[USED_VRAM_INDEX];
	gpuStates->coreTemp        = values[CORE_TEMP_INDEX];

	//calculate memory usage percentage
	gpuStates->memoryUsage     = (gpuStates->usedMemory / gpuStates->totalMemory) * 100;
}

//probe Nvidia drivers for gpu usage
double StatsReader::getGpuUsage(const string &gpuId)
{
	string command;

	gpuCoreUsageCommand(gpuId, &command);

	std::vector<double> values;

	if(!getDoubleFromSystemCall(command, &values) || values.empty())
	{
		return -1;
	}

	return values[0];
}

//probe Nvidia drivers for memory bus usage
double StatsReader::getGpuMemoryBandwidth(const string &gpuId)
{
	string command;

	gpuMemoryBandwidthCommand(gpuId, &command);

	std::vector<double> values;

	if(!getDoubleFromSystemCall(command, &values) || values.empty())
	{
		return -1;
	}

	return values[0];
}

//probe Nvidia drivers for pci-e usage
double StatsReader::getGpuPciBandwidth(const string &gpuId)
{
	string command;

	gpuPciBandwidthCommand(gpuId, &command);

	std::vector<double> values;

	if(!getDoubleFromSystemCall(command, &values) || values.empty())
	{
		return -1;
	}

	return values[0];
}

//probe Nvidia drivers for gpu temperature
double StatsReader::getGpuTemp(const string &gpuId)
{
	string command;

	gpuTempCommand(gpuId, &command);

	std::vector<double> values;

	if(!getDoubleFromSystemCall(command, &values) || values.empty())
	{
		return -1;
	}

	return values[0];
}

//probe Nvidia drivers for total available memory
double StatsReader::getGpuTotalMemory(const string &gpuId)
{
	string command;

	gpuTotalMemoryCommand(gpuId, &command);

	std::vector<double> values;

	if(!getDoubleFromSystemCall(command, &values) || values.empty())
	{
		return -1;
	}

	return values[0];
}

//probe Nvidia drivers for currently used memory
double StatsReader::getGpuUsedMemory(const string &gpuId)
{
	string command;

	gpuUsedMemoryCommand(gpuId, &command);

	std::vector<double> values;

	if(!getDoubleFromSystemCall(command, &values) || values.empty())
	{
		return -1;
	}

	return values[0];
}

//execute a bash command and return the output as double
bool StatsReader::getDoubleFromSystemCall(string &command, std::vector<double> *values)
{
	FILE *in;
	char buff[512];

	command = checkIfSshCommand(command);

	if(!(in = popen(command.c_str(), "r")))
	{
		return false;
	}

	while(fgets(buff, sizeof(buff), in) != NULL)
	{
		values->push_back(atoi(string(buff).c_str()));

	}

	pclose(in);

	return true;
}

//get the list of available gpus on this machine
bool StatsReader::getGpuList(vector<string> *gpuList)
{
	FILE *in;
	char buff[512];

	string command;

	//get the bash command
	gpuListCommand(&command);

	command = checkIfSshCommand(command);

	//execute bash command
	if(!(in = popen(command.c_str(), "r")))
	{
		return false;
	}

	//while the line starts with '[', push a new gpu
	//ie: "[0] mountassir-workstation:0[gpu:0] (GeForce GTX 770)"
	while(fgets(buff, sizeof(buff), in) != NULL)
	{
		if(buff[0] == '[')
		{
			gpuList->push_back(buff);
		}
	}

	pclose(in);

	return true;
}

//=======================================================================
//         functions returning the appropriate bash commands
//=======================================================================

//get all list of available gpus
//and trim any leading spaces (sed -e 's/^ *//')
void StatsReader::gpuListCommand(string *gpuList)
{
	stringstream oss;
	oss << "nvidia-settings ";
	if(_optirun)
		oss << "-c :8 ";
	oss << "-t -q gpus | sed -e 's/^ *//'";

	*gpuList = oss.str();
}

//get current gpu states, gpuId = [gpu:0], [gpu:1]...
//split results by ',' (tr ',' '\n')
//grep only numbers in the line (sed 's/[^0-9]//g')
void StatsReader::getAllGpuStatesCommand(const string &gpuId, string *command)
{
	stringstream oss;

	oss << "nvidia-settings ";
	if(_optirun)
		oss << "-c :8 ";
	oss << "-t -q " << gpuId << "/GPUUtilization -q "
	    << gpuId << "/GPUCoreTemp -q " 
	    << gpuId << "/TotalDedicatedGPUMemory -q "
	    << gpuId << "/UsedDedicatedGPUMemory "
	    << " | tr ',' '\n' | sed 's/[^0-9]//g'";

	*command = oss.str();
}

//get current gpu states, gpuId = [gpu:0], [gpu:1]...
//split results by ',' (tr ',' '\n')
//grep gpu usage 'graphics' (grep graphics)
//grep only numbers in the line (sed 's/[^0-9]//g')
void StatsReader::gpuCoreUsageCommand(const string &gpuId, string *command)
{
	stringstream oss;

	oss << "nvidia-settings ";
	if(_optirun)
		oss << "-c :8 ";
	oss << "-t -q " << gpuId << "/GPUUtilization | tr ',' '\n' |grep graphics|sed 's/[^0-9]//g'";

	*command = oss.str();
}

//get current gpu states, gpuId = [gpu:0], [gpu:1]...
//split results by ',' (tr ',' '\n')
//grep memory usage 'memory' (grep memory)
//grep only numbers in the line (sed 's/[^0-9]//g')
void StatsReader::gpuMemoryBandwidthCommand(const string &gpuId, string *command)
{
	stringstream oss;

	oss << "nvidia-settings ";
	if(_optirun)
		oss << "-c :8 ";
	oss << "-t -q " << gpuId << "/GPUUtilization | tr ',' '\n' |grep memory|sed 's/[^0-9]//g'";

	*command = oss.str();
}

//get current gpu states, gpuId = [gpu:0], [gpu:1]...
//split results by ',' (tr ',' '\n')
//grep pci-e usage 'PCIe' (grep PCIe)
//grep only numbers in the line (sed 's/[^0-9]//g')
void StatsReader::gpuPciBandwidthCommand(const string &gpuId, string *command)
{
	stringstream oss;

	oss << "nvidia-settings ";
	if(_optirun)
		oss << "-c :8 ";
	oss << "-t -q " << gpuId << "/GPUUtilization | tr ',' '\n' |grep PCIe|sed 's/[^0-9]//g'";

	*command = oss.str();
}

//get current gpu temperature, gpuId = [gpu:0], [gpu:1]...
void StatsReader::gpuTempCommand(const string &gpuId, string *command)
{
	stringstream oss;

	oss << "nvidia-settings ";
	if(_optirun)
		oss << "-c :8 ";
	oss << "-t -q " << gpuId << "/GPUCoreTemp";

	*command = oss.str();
}

//get the current used memory, gpuId = [gpu:0], [gpu:1]...
void StatsReader::gpuUsedMemoryCommand(const string &gpuId, string *command)
{
	stringstream oss;

	oss << "nvidia-settings ";
	if(_optirun)
		oss << "-c :8 ";
	oss << "-t -q " << gpuId << "/UsedDedicatedGPUMemory";

	*command = oss.str();
}

//get the total dedicated memory on the card, gpuId = [gpu:0], [gpu:1]...
void StatsReader::gpuTotalMemoryCommand(const string &gpuId, string *command)
{
	stringstream oss;

	oss << "nvidia-settings ";
	if(_optirun)
		oss << "-c :8 ";
	oss << "-t -q " << gpuId << "/TotalDedicatedGPUMemory";

	*command = oss.str();
}

void StatsReader::useOptirun(const bool withOptirun)
{
	_optirun = withOptirun;
}

string StatsReader::checkIfSshCommand(string &command)
{
	if(_overSsh)
	{
		return "DISPLAY=:0 XAUTHORITY=/var/run/lightdm/root/:0 " + command;
	}

	return command;
}
