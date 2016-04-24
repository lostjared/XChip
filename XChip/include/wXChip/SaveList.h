#ifndef _WXCHIP_SAVELIST_H_
#define _WXCHIP_SAVELIST_H_

#include <iostream>
#include <fstream>
#include <string>

void saveDirectory(const std::string &text, const std::string &fps, const std::string &cpufreq);
std::string getDirectory(std::string &fps, std::string &cpufreq);



#endif
