/*
 * Copyright (C) 2015 Mountassir El Hafi, (mountassirbillah1@gmail.com)
 *
 * Constants.h: Part of gmonitor
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

#ifndef CONSTANTS_H_
#define CONSTANTS_H_

#include <list>

using namespace std;

typedef struct GpuStates
{
	double gpuUsage;         //gpu usage (%)
	double memoryUsage;      //memory usage (%)
	double usedMemory;       //used memory (MB)
	double totalMemory;      //total available memory (MB)
	double coreTemp;         //gpu temperature (c)
	double pciBandwidth;     //pci-e bus bandwidth usage (%)
	double memoryBandwidth;  //memory bus bandwidth usage (%)
}GpuStates;

typedef list<GpuStates> Graph;

const int NUMBER_OF_STATES = 5;    //number of states to be displayed

enum States
{
	GPU  = 0,  //gpu usage
	VRAM = 1,  //memory usage
	MBUS = 2,  //memory bus usage
	PBUS = 3,  //pci-e bus usage
	TEMP = 4,  //gpu temperature
};

enum Color
{
	NO_COLOUR = 0,
	BLUE   = 1,
	RED    = 2,
	YELLOW = 3,
	GREEN  = 4,
	LBLUE  = 5,
};

enum DisplayMode
{
	CURRENT_NEXT_TO_HISTORY  = 0,   //print the most recent state only
	CURRENT_STATE_ONLY       = 1,   //print the previous states only
	HISTORY_STATES_ONLY      = 2,   //print current followed by history for each gpu
	CURRENT_THEN_HISTORY     = 3,   //print current for all gpus, then history for all gpus
};

 //default display mode (will display current then previous states
// for each gpu)
const DisplayMode DEFAULT_DISPLAY_MODE = CURRENT_NEXT_TO_HISTORY;

const int DEFAULT_REFRESH_RATE = 2; //default refresh rate, 2 seconds

const int BUFFER_LENGTH = 512;

#endif
