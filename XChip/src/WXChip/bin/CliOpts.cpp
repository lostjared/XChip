#include "CliOpts.h"






CliOpts::CliOpts(int argc, char** argv)
	: ArgVec(argv, argv+argc)
{

}


CliOpts::CliOpts(std::vector<std::string>&& cliArgs)
	: ArgVec(std::move(cliArgs))
{

}



std::string CliOpts::GetOpt(const std::string& match) const
{

	const auto itr = GetOptItr(match);
	if(itr != this->end())
	{
		if(_isSub)
			return itr->substr(match.size(), itr->size());
		else
			return *itr;
	}

	return "";

}


bool CliOpts::RemoveOpt(const std::string& match)
{
	auto itr = GetOptItr(match);
	if(itr != this->end())
	{
		if(_isSub)
		{
			this->erase(itr);
		}

		else
		{
			itr = this->erase(itr-1);
			this->erase(itr);
		}
		
		return true;
	}

	return false;
}



CliOpts::ArgVec::const_iterator CliOpts::GetOptItr(const std::string& match) const
{
	const auto matchSize = match.size();
	const auto begin = this->begin();
	const auto end = this->end();
	for(auto itr = begin; itr != end; ++itr)
	{
		const auto argSize = itr->size();
		if(argSize == matchSize)
		{
			if(*itr == match)
			{
				_isSub = false;
				return ++itr;
			}

		}
		else if(argSize > matchSize)
		{
			const auto substr = itr->substr(0, matchSize);
			if(substr == match)
			{
				_isSub = true;
				return itr;
			}
		}
	}

	return end;
}
