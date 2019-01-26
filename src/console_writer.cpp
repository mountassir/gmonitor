/*
 * Copyright (C) 2015 Mountassir El Hafi, (mountassirbillah1@gmail.com)
 *
 * Writer.cpp: Part of gmonitor
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

#include <stdio.h>
#include <iostream>
#include <sstream>
#include <sys/ioctl.h>
#include "console_writer.h"
#include <ctime>
#include <chrono>

using namespace std;

//text in colour
string ConsoleWriter::colourText(Color color, const string &text)
{
	switch(color)
	{
		case BLUE:   { return inBlue(text);     }
		case YELLOW: { return inYellow(text);   }
		case GREEN:  { return inGreen(text);    }
		case RED:    { return inRed(text);      }
		case LBLUE:  { return inLightBlue(text);}
		default:     { return text;             }
	}
}

//light blue coloured text
string ConsoleWriter::inLightBlue(const string &text)
{
	ostringstream oss;

	oss << "\033[1;36m" << text << "\033[0m";

	return oss.str();
}

//red coloured text
string ConsoleWriter::inRed(const string &text)
{
	ostringstream oss;

	oss << "\033[1;31m" << text << "\033[0m";

	return oss.str();
}

//green coloured text
string ConsoleWriter::inGreen(const string &text)
{
	ostringstream oss;

	oss << "\033[1;32m" << text << "\033[0m";

	return oss.str();
}

//yellow coloured text
string ConsoleWriter::inYellow(const string &text)
{
	ostringstream oss;

	oss << "\033[1;33m" << text << "\033[0m";

	return oss.str();
}

//blue coloured text
string ConsoleWriter::inBlue(const string &text)
{
	ostringstream oss;

	oss << "\033[1;34m" << text << "\033[0m";

	return oss.str();
}

//return the correct colour for a value (0 - 100)
Color ConsoleWriter::getColorByValue(unsigned int value)
{
	if(value < 25 - resolution){ return BLUE;}
	else
	if(value < 50 - resolution){ return GREEN;}
	else
	if(value < 75 - resolution){ return YELLOW;}
	else
	{ return RED;}

	return NO_COLOUR;
}

//return the correct colour for a state
Color ConsoleWriter::getColorByState(States toPrint)
{
	switch(toPrint)
	{
		case GPU:  { return BLUE;      }
		case VRAM: { return YELLOW;    }
		case MBUS: { return GREEN;     }
		case PBUS: { return LBLUE;     }
		case TEMP: { return RED;       }
		default:   { return NO_COLOUR; }
	}
}

//get the pre and post labels for each state;
//GPU for gpu usage, VRAM for video RAM ...
//% for percentage value and c for temperature
void ConsoleWriter::getLabelsByState(States state,
							  string *preLabel,
		                      string *postLabel)
{
	switch(state)
	{
		case GPU:  { *preLabel = "GPU:  "; *postLabel = "%"; break;}
		case VRAM: { *preLabel = "VRAM: "; *postLabel = "%"; break;}
		case MBUS: { *preLabel = "MBUS: "; *postLabel = "%"; break;}
		case PBUS: { *preLabel = "PBUS: "; *postLabel = "%"; break;}
		case TEMP: { *preLabel = "TEMP: "; *postLabel = "c"; break;}
		default:   { *preLabel = "";       *postLabel = "";  break;}
	}
}

//print a line of '='
string ConsoleWriter::printSeperator(int length /* = 0 */)
{
	ostringstream oss;

	if(length != 0)
	{
		oss << "\n" << string((length), '=');
	}
	else
	{
		oss << "\n" << string((terminalColms), '=');
	}

	return oss.str();
}

//refresh the available terminal size to print on
void ConsoleWriter::refreshTerminalDimensions()
{
	int columns, rows;

	getTerminalDim(&columns, &rows);
	updateTerminalDimensions(columns, rows);
}

//get the current window size
void ConsoleWriter::getTerminalDim(int *xDim, int *yDim)
{
	struct winsize w;
	ioctl(0, TIOCGWINSZ, &w);

	*yDim = w.ws_row;
	*xDim = w.ws_col;
}

//update the size of the terminal window
void ConsoleWriter::updateTerminalDimensions(int colms, int rows)
{
	terminalRows = rows;
	terminalColms = colms;

	combinedBarSize = (terminalColms - NUMBER_OF_STATES) / NUMBER_OF_STATES;
	resolution = 100 / (terminalColms - 11) + 1;
}

//clear the terminal
void ConsoleWriter::clearConsole()
{
	cout << "\x1B[2J\x1B[H";
        // On the first run, the dimensions are wrong, so let's check them
        refreshTerminalDimensions();
}

//print gpu identifiers
void ConsoleWriter::printGpuIdentifier(const string &gpuId,
				                const string &gpuName,
				                int totalAvailableMemory)
{
        char outline[BUFFER_LENGTH + 1];
        size_t len;
        chrono::system_clock::time_point today = chrono::system_clock::now();
        time_t tt = chrono::system_clock::to_time_t(today);

        cout << endl;
        len = snprintf(outline, BUFFER_LENGTH, "ID: %s", gpuId.c_str());
        if ((len < terminalColms) && (len < BUFFER_LENGTH)) {
            len += snprintf(&outline[len], BUFFER_LENGTH - len, ", Name: %s",
                gpuName.c_str());
            if ((len < terminalColms) && (len < BUFFER_LENGTH)) {
                len += snprintf(&outline[len], BUFFER_LENGTH - len, " (%d MB)",
                    totalAvailableMemory);
                if ((len < terminalColms) && (len < BUFFER_LENGTH)) {
                    len += snprintf(&outline[len], BUFFER_LENGTH - len, "%*s",
                        terminalColms - len, ctime(&tt));
                }
            }
        }
        outline[len] = 0; // make sure it's terminated
        cout << outline;
}

//print the current state of the gpu
void ConsoleWriter::printCurrentGpuState(const GpuStates &state)
{
	refreshTerminalDimensions();

	int line = 0;

	ostringstream oss;

	oss << printSeperator();

	oss << "\n      |";

	while(line < NUMBER_OF_STATES)
	{
		switch(line)
		{
			case 0: { oss << printOneStateInOneLine(GPU,  state.gpuUsage);        break;}
			case 1: { oss << printOneStateInOneLine(VRAM, state.memoryUsage);     break;}
			case 2: { oss << printOneStateInOneLine(MBUS, state.memoryBandwidth); break;}
			case 3: { oss << printOneStateInOneLine(PBUS, state.pciBandwidth);    break;}
			case 4: { oss << printOneStateInOneLine(TEMP, state.coreTemp);        break;}
		}

		++line;
	}

	oss << "\n      |";

	cout << oss.str();
}

//print one state, value is represented by a list
//of '|', 10 -> |||||||||| (for resolution = 1)
string ConsoleWriter::printOneStateInOneLine(States state, unsigned int value)
{
	string preLabel, postLabel;

	getLabelsByState(state, &preLabel, &postLabel);

	ostringstream oss;

	unsigned int character = 0;

	while(character <= (terminalColms ) * resolution)
	{
		if(character == 0)
		{
			oss << "\n" << preLabel << "|";
		}

		if(character < value)
		{
			oss << colourText(getColorByValue(character), "|");
		}

		character += resolution;
	}

	oss << value << postLabel;

	return oss.str();
}

//print all previous states of the gpu
void ConsoleWriter::printHistoryStates(const vector<GpuStates> &states)
{
	refreshTerminalDimensions();

	ostringstream oss;

	oss << printSeperator();

	oss << printHistoryStatesHeading(GPU,  combinedBarSize) << " "
	    << printHistoryStatesHeading(VRAM, combinedBarSize) << " "
	    << printHistoryStatesHeading(MBUS, combinedBarSize) << " "
	    << printHistoryStatesHeading(PBUS, combinedBarSize) << " "
	    << printHistoryStatesHeading(TEMP, combinedBarSize);

	for(size_t i = 0; i < states.size(); ++i)
	{
		oss << "\n"
		    << printOneHistoryStateInOneLine(GPU,  states[i].gpuUsage,        combinedBarSize) << " "
		    << printOneHistoryStateInOneLine(VRAM, states[i].memoryUsage,     combinedBarSize) << " "
		    << printOneHistoryStateInOneLine(MBUS, states[i].memoryBandwidth, combinedBarSize) << " "
		    << printOneHistoryStateInOneLine(PBUS, states[i].pciBandwidth,    combinedBarSize) << " "
		    << printOneHistoryStateInOneLine(TEMP, states[i].coreTemp,        combinedBarSize);
	}

	cout << oss.str();
}

//print one state, value is represented by a list
//of '|', resolution depends on the line size
string ConsoleWriter::printOneHistoryStateInOneLine(States state, int value, double lineSize)
{
	Color color = getColorByState(state);

	ostringstream oss;

	double character = 1;

	while(character <= lineSize)
	{
		double percentage = (character / lineSize) * 100;

		if( percentage < value)
		{
			oss << colourText(color, "|");
		}
		else
		{
			oss << "|";
		}

		++character;
	}

	return oss.str();
}

//print heading for history view
//GPU~~~ VRAM~~~ ...
string ConsoleWriter::printHistoryStatesHeading(States state, int barSize)
{
	ostringstream oss;

	Color color = getColorByState(state);

	switch(state)
	{
		case GPU:  { oss << "GPU";  break;}
		case VRAM: { oss << "VRAM"; break;}
		case MBUS: { oss << "MBUS"; break;}
		case PBUS: { oss << "PBUS"; break;}
		case TEMP: { oss << "TEMP"; break;}
		default:   { oss << "";     break;}
	}

	int textSize = oss.str().length();

	if(textSize > barSize)
	{
		return "";
	}

	for(int i = 0; i < (barSize - textSize); ++i)
	{
		oss << "~";
	}

	return colourText(color, oss.str());
}
