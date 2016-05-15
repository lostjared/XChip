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
