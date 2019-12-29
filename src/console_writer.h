/*
 * Copyright (C) 2015 Mountassir El Hafi, (mountassirbillah1@gmail.com)
 *
 * Writer.h: Part of gmonitor
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

#ifndef CONSOLE_WRITER_H_
#define CONSOLE_WRITER_H_

#include <vector>
#include "constants.h"

using namespace std;

const int DEFAULT_ROW_N = 100;  //default terminal window vertical size
const int DEFAULT_COL_N = 100;  //default terminal window horizontal size
const int DEFAULT_RES   = 1;    //default resolution, '|' represents one % / c

/*
 * consoleWriter class: prints gpu states on the terminal
 */
class ConsoleWriter
{
private:
	unsigned int    terminalRows;     //total rows of the terminal
	unsigned int    terminalColms;    //total columns of the terminal
	unsigned int    resolution;       //how many units a column represents
	unsigned int    combinedBarSize;

	//return a coloured text
	string colourText(Color color, const string &text);
	string inRed(const string &text);
	string inGreen(const string &text);
	string inYellow(const string &text);
	string inBlue(const string &text);
	string inLightBlue(const string &text);

	//return the correct color for a value (0 - 100)
	Color getColorByValue(unsigned int value);

	//return the correct color for a state
	Color getColorByState(States toPrint);

	//get the pre and post labels for each state;
	//GPU for gpu usage, VRAM for video RAM ...
	//% for percentage value and c for temperature
	void getLabelsByState(States state,
						  string *preLabel,
						  string *postLabel);

	//print a line of '='
	string printSeperator(int length = 0);

	//refresh the available terminal size to print on
	void refreshTerminalDimensions();

	//get the current window size
	void getTerminalDim(int *xDim, int *yDim);

	//update the size of the terminal window
	void updateTerminalDimensions(int colms, int rows);

	//print one state, value is represented by a list
	//of '|', 10 -> |||||||||| (for resolution = 1)
	string printOneStateInOneLine(States state,
                                                    unsigned int value);

	//print one state, value is represented by a list
	//of '|', resolution depends on the line size
	string printOneHistoryStateInOneLine(States state,
			                             int value,
			                             double lineSize);

	//print heading for history view
	//GPU~~~ VRAM~~~ ...
	string printHistoryStatesHeading(States state, int barSize);

public:
	//constructors
	ConsoleWriter(int rows, int colms, int res)
		: terminalRows(rows),
		  terminalColms(colms),
		  resolution(res),
		  combinedBarSize(0) {};
	ConsoleWriter()
		: terminalRows(DEFAULT_ROW_N),
		  terminalColms(DEFAULT_COL_N),
		  resolution(DEFAULT_RES),
		  combinedBarSize(0) { };

	//clear the terminal
	void clearConsole();

	//print gpu identifiers
	void printGpuIdentifier(const string &gpuId,
					        const string &gpuName,
					        int totalAvailableMemory);

	//print the current state of the gpu
	void printCurrentGpuState(const GpuStates &state);

	//print all previous states of the gpu
	void printHistoryStates(const vector<GpuStates> &states);
};

#endif
