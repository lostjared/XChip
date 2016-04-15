#include <string>
#include <wXChip/SaveList.h>
#include <XChip/Utility/Log.h>

void saveDirectory(const std::string &text) 
{
    std::fstream file;
    file.open("romlist.dat", std::ios::out);
    if(!file.is_open()) {
        xchip::utility::LOGerr("Error could not open file");
        return;
    }

	file << text << "\n";
    file.close();
}

std::string getDirectory() 
{
    std::fstream file;
    file.open("romlist.dat", std::ios::in);
    if(!file.is_open()) {
        return "nolist";
    }
    std::string value;
    std::getline(file, value);
    file.close();
    return value;
}
