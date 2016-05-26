#ifndef _WXCHIP_BIN_CLIOPTS_H_
#define _WXCHIP_BIN_CLIOPTS_H_
#include <string>
#include <vector>



class CliOpts : public std::vector<std::string>
{
	using ArgVec = std::vector<std::string>;
public:
	CliOpts(int argc, char** argv);
	CliOpts(std::vector<std::string>&& cliArgs);
	std::string GetOpt(const std::string& match) const;
	bool RemoveOpt(const std::string& match);

private:
	ArgVec::const_iterator GetOptItr(const std::string& match) const;
	mutable bool _isSub = false;
};












#endif