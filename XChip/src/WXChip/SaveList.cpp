/*

XChip - A chip8 lib and emulator.
Copyright (C) 2016  Jared Bruni.


This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see http://www.gnu.org/licenses/gpl-3.0.html.

*/

#include <string>
#include <XChip/Utility/Log.h>
#include <WXChip/SaveList.h>

void saveDirectory(const std::string &text, const std::string &fps, const std::string &cpufreq)
{
	std::fstream file;
	file.open("romlist.dat", std::ios::out);
	if(!file.is_open())
	{
		xchip::utility::LOGerr("Error could not open file");
		return;
	}
	
	if(text.length()==0)
		file << "nopath";
	else
		file << text << "\n";
	
	file << fps << "\n";
	file << cpufreq << "\n";
	file.close();
}

std::string getDirectory(std::string &fps, std::string &cpufreq)
{
	std::fstream file;
	file.open("romlist.dat", std::ios::in);
	if(!file.is_open())
	{
		return "nolist";
	}

	std::string value;
	std::getline(file, value);
	
	if(!file)
		fps = "60";
	else
	std::getline(file, fps);
	
	if(!file)
		cpufreq = "60";
	else
		std::getline(file, cpufreq);
	
	file.close();
	return value;
}
