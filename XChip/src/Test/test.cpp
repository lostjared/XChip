#include <XChip/Cpu.h>
#include <XChip/Instructions.h>
#include <XChip/CpuManager.h>
#include <XChip/Utility/Log.h>



int main()
{	
	std::cout << std::is_pod<xchip::Cpu>::value;
}
