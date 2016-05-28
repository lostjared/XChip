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

#include <XChip/Utility/CliOpts.h>


namespace xchip { namespace utility {



CliOpts::CliOpts(int argc, char** argv)
	: ArgVec(argv, argv + argc)
{

}


CliOpts::CliOpts(std::vector<std::string>&& cliArgs)
	: ArgVec(std::move(cliArgs))
{

}


std::string CliOpts::GetOpt(const std::string& match) const
{

	const auto itr = GetOptItr(match);
	if (itr != this->end())
	{
		if (_isSub)
			return itr->substr(match.size(), itr->size());
		else
			return *itr;
	}

	return "";

}







CliOpts::ArgVec::const_iterator CliOpts::GetOptItr(const std::string& match) const
{
	const auto matchSize = match.size();
	const auto begin = this->begin();
	const auto end = this->end();
	for (auto itr = begin; itr != end; ++itr)
	{
		const auto argSize = itr->size();
		if (argSize == matchSize)
		{
			if (*itr == match)
			{
				_isSub = false;
				return ++itr;
			}

		}
		else if (argSize > matchSize)
		{
			const auto substr = itr->substr(0, matchSize);
			if (substr == match)
			{
				_isSub = true;
				return itr;
			}
		}
	}

	return end;
}






bool CliOpts::RemoveOpt(const std::string& match)
{
	auto itr = GetOptItr(match);
	if (itr != this->end())
	{
		if (_isSub)
		{
			this->erase(itr);
		}

		else
		{
			itr = this->erase(itr - 1);
			this->erase(itr);
		}

		return true;
	}

	return false;
}







}}
