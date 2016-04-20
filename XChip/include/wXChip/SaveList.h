#ifndef __SAVELIST__H__
#define __SAVELIST__H__

#include <iostream>
#include <fstream>
#include <string>

void saveDirectory(const std::string &text, const std::string &fps, const std::string &cpufreq);
std::string getDirectory(std::string &fps, std::string &cpufreq);



#endif
