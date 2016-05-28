/*

XChip - A chip8 lib and emulator.
Copyright (C) 2016  Rafael Moura

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

#ifndef _XCHIP_UTILITY_CLIOPTS_H_
#define _XCHIP_UTILITY_CLIOPTS_H_
#include <string>
#include <vector>

namespace xchip { namespace utility {



class CliOpts : public std::vector<std::string>
{
	using ArgVec = std::vector<std::string>;
public:
	CliOpts(int argc, char** argv);
	CliOpts(std::vector<std::string>&& cliArgs);
	std::string GetOpt(const std::string& match) const;
	bool RemoveOpt(const std::string& match);
	static std::string GetFullProcName();
	static std::string GetFullProcDir();
private:
	ArgVec::const_iterator GetOptItr(const std::string& match) const;
	mutable bool _isSub = false;
};








}}



#endif
