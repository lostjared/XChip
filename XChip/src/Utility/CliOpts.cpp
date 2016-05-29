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


#ifdef _WIN32
#include <stdlib.h>
#include <windows.h>
#elif defined(__linux__) 
#include <string.h>
#include <unistd.h>
#elif defined(__APPLE__)
<<<<<<< HEAD
#include <string.h>
#include <unistd.h>
#include <libproc.h>
=======
>>>>>>> upstream/master
#include <mach-o/dyld.h>
#endif



#include <XChip/Utility/StdintDef.h>
#include <XChip/Utility/Log.h>
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

	return std::string();

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







std::string CliOpts::GetFullProcName()
{
	using namespace literals;
	
#ifdef _WIN32



	HMODULE handle = GetModuleHandleW(NULL);
	WCHAR buffer[MAX_PATH];
	auto writeSize = GetModuleFileNameW(handle, buffer, MAX_PATH);
	
	if (writeSize == MAX_PATH)
	{
		const auto errorCode = GetLastError();
		LOGerr("Error in GetModuleFileName ErrorCode: "_s + std::to_string(errorCode));
		return std::string();

	}

	std::string ret;
	ret.resize(writeSize);
	std::wcstombs(&ret[0], buffer, writeSize);
	
	return ret;


#elif defined(__linux__)


	constexpr const size_t BUFF_LEN = 256;
	char buffer[BUFF_LEN];
	auto writeSize = readlink("/proc/self/exe", buffer, BUFF_LEN);

	if (writeSize == -1)
	{
		const auto errorCode = errno;
		LOGerr("Error in readlink: "_s + strerror(errorCode));
		return std::string();
	}

	buffer[writeSize] = 0;

	return buffer;

#elif defined(__APPLE__)
	constexpr const uint32_t BUFF_LEN = 400;
	
	char buffer[BUFF_LEN];
	uint32_t size = BUFF_LEN;

	if (_NSGetExecutablePath(path, &size) != 0)
	{
		LOGerr("_NSGetExecutablePath failed. output size: "_s std::to_string(size));
		return std::string();
	}
	
	return buffer;

#endif
	
}


std::string CliOpts::GetFullProcDir()
{
#ifdef _WIN32
	constexpr const char dirSlash = '\\';
#elif defined(__linux__) || defined(__APPLE__)
	constexpr const char dirSlash = '/';
#endif

	auto fullPath = GetFullProcName();
	fullPath.erase(fullPath.find_last_of(dirSlash), fullPath.size());
	
	return fullPath;
}

















}}
