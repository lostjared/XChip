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




#include <cstdlib>
#include <iostream>
#include <Utix/RWrap.h>

using namespace utix;


struct X
{
	int x, y;
};

int main()
{
	auto x = make_rwrap<X*>((X*)std::malloc(sizeof(X) * 5), [](X* x)noexcept { std::free(x); });


	if(nullptr == x)
	{
		std::cerr << "FAILED TO ALLOCATE MEMORY\n";
		return EXIT_FAILURE;
	}


	x->x = 20;
	x->y = -14;

	std::cout << "x->x: " << x->x << '\n';
	std::cout << "x->y: " << x->y << '\n';

	const auto& _cp = x;

	// _cp->x = 10; error
	// _cp->y = -10; error

	// (*_cp).x = 10; error
	// (*_cp).y = -10; error

	std::cout << "_cp->x: " << _cp->x << '\n';
	std::cout << "_cp->y: " << _cp->y << '\n';

	(x+1)->x = 20;
	(x+1)->y = -19;

	std::cout << "(_cp+1)->x: " << (_cp+1)->x << '\n';
	std::cout << "(_cp+1)->y: " << (_cp+1)->y << '\n';





	const auto moved2 = move(x);

}











#if 0
int main(int argc, char** argv)
{
	if( argc < 2 )
	{
		std::cerr << "USAGE: " << argv[0] << " <filename> \n";
		return EXIT_FAILURE;
	}


	auto file = make_rwrap<std::FILE*>(nullptr, [](std::FILE** pp) noexcept { if((*pp)!=nullptr) std::fclose((*pp)); });

	file = std::fopen(argv[1], "r");

	if( !file )
	{
		std::cerr << "COULD NOT OPEN FILE \'" << argv[1] << '\'' << '\n';
		return EXIT_FAILURE;
	}


	std::fseek(file, 0, SEEK_END);
	const auto size = std::ftell(file);
	std::fseek(file, 0, SEEK_SET);


	auto buffer = make_rwrap<char*>(nullptr, [](char** pp) noexcept { if((*pp)!=nullptr) std::free((*pp)); });


	buffer = (char*) std::malloc(sizeof(char) * (size+1));


	if( buffer == nullptr )
	{
		std::cerr << "COULD NOT ALLOCATE BUFFER\n";
		return EXIT_FAILURE;
	}


	const auto readSize = std::fread(buffer, 1, size, file);
	buffer[readSize] = '\0';
	std::cout << buffer << std::endl;
	return EXIT_SUCCESS;
}




#endif



#if 0



#include <iostream>
#include <XChip/Utils/CliOpts.h>







int main(int argc, char **argv)
{
	using namespace xchip;
	using namespace xchip::utils;

	CliOpts opts(argc-1, argv+1);


	opts.RemoveOpt("OPTION_B");

	for(const auto& arg : opts)
		std::cout << arg << '\n';
}






#endif


#if 0

// TESTING Vector, it's a version of std::vector for POD types only
#include <iostream>
#include <vector>
#include <XChip/Utils/Vector.h>

struct Pod
{
	int key;
	char value;
	uint64_t x[1000];
};

std::ostream& operator<<(std::ostream& os, const Pod& pod)
{
	os << "{ " << pod.key << ", " << pod.value << " } " << std::endl;
	return os;
}

void VecTest()
{
	using namespace xchip;
	using namespace xchip::utils;
	const auto print = [](const Vector<int>& vec) {
		for (size_t i = 0; i < vec.size(); ++i)
			std::cout << "vec[" << i << "] = " << vec[i] << std::endl;
	};

	Vector<int> x;
	x.resize(8);
	x.push_back(555);

	std::cout << "x size: " << x.size() << std::endl;

	x.push_back(666);

	std::cout << "x size: " << x.size() << std::endl;

	print(x);
}

int main()
{


	const auto print = [](const std::vector<int>& vec) {
		for (size_t i = 0; i < vec.size(); ++i)
			std::cout << "vec[" << i << "] = " << vec[i] << std::endl;
	};

	std::vector<int> x;

	x.resize(8);

	x.push_back(555);
	std::cout << "x size: " << x.size() << std::endl;

	x.push_back(666);
	std::cout << "x size: " << x.size() << std::endl;

	print(x);

	std::cout << "\t VEC TEST: " << std::endl;
	VecTest();


	return 0;
}





#endif




#if 0

#include <csignal>
#include <iostream>
#ifdef _WIN32
#include <windows.h>
#elif defined(__linux__) || defined(__APPLE__)
#include <dlfcn.h>
#endif
#include <XChip/Plugins/iPlugin.h>

int main(int argc, char **argv)
{
	using xchip::PluginLoader;
	using xchip::PluginDeleter;

	// need to make sure that the functions that the 
	// dynamic library used from Utils / Core are
	// linked into the caller binary. Cuz 
	// Utils and Core are not compiled with -fPIC
	// so they can't be linked to a shared library.
	// but if needed, the -fPIC flag can be added easily.

	// removing these lines causes segmentation fault
	//volatile auto x=&xchip::utils::LogError;
	//volatile auto y=&xchip::utils::Log;

#ifdef _WIN32
	auto handle = LoadLibrary("XChipSFMLInput.dll");
	
	auto loadFun = (PluginLoader)GetProcAddress(handle, XCHIP_LOAD_PLUGIN_SYM);

	auto freeFun = (PluginDeleter)GetProcAddress(handle, XCHIP_FREE_PLUGIN_SYM);


	auto plugin = loadFun();
	std::cout << plugin->GetPluginName() << std::endl;
	std::cout << plugin->GetPluginVersion() << std::endl;

	freeFun(plugin);
	FreeLibrary(handle);

#elif defined(__linux__) || defined(__APPLE__)
	auto handle = dlopen("./XChipSFMLInput.so", RTLD_LAZY);

	auto loadFun = (LoadPtr)dlsym(handle, "XCHIP_LoadPlugin");

	auto plugin = loadFun();

	std::cout << plugin->GetPluginName() << std::endl;
	std::cout << plugin->GetPluginVersion() << std::endl;

	auto freeFun = (FreePtr)dlsym(handle, "XCHIP_FreePlugin");

	freeFun(plugin);

	dlclose(handle);
#endif
	return EXIT_SUCCESS;
}













#endif
