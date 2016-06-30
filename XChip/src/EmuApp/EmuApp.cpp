/*

WXChip - chip8 emulator using XChip library and a wxWidgets gui.
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


#if defined(__linux__) || defined(__APPLE__)
#include <signal.h>
#include <unistd.h>
#elif defined( _WIN32 )
#include <windows.h>
#include <stdlib.h>
#endif


#include <stdexcept>
#include <algorithm>
#include <utility>
#include <fstream>


#include <Utix/Log.h>
#include <Utix/CliOpts.h>
#include <Utix/Common.h>


#include <XChip/Core/Emulator.h>



/*******************************************************************************************
 *	-ROM  game rom path
 *	-REN  render plugin path
 *	-INP  input plugin path
 *	-SND  sound plugin path
 *	-RES  window size: WidthxHeight ex: -RES 200x300 and -RES FULLSCREEN for fullscreen
 *	-CHZ  Cpu Frequency in hz ex: -CHZ 600
 *	-SHZ  Sound Tone in hz ex: -SHZ 400
 *	-COL  Color in RGB ex: -COL 100x200x255
 *	-BKG  Background color in RGB ex: -BKG 255x0x0
 *	-FPS  Frame Rate ex: -FPS 30
 *******************************************************************************************/

/*********************************************************
 * SIGNALS:
 * SIGINT - set g_emulator exitflag
 * CTRL_EVENT: windows ConsoleCtrlEvents...
 *********************************************************/




static xchip::Emulator g_emulator;


void load_plugins(const utix::CliOpts& opts);
void configure_emulator(const utix::CliOpts& opts);

#if defined(__linux__) || defined(__APPLE__)
void signals_sigint(const int signum);
#elif defined(_WIN32)
bool _stdcall ctrl_handler(DWORD ctrlType);
#endif





int main(int argc, char **argv)
{

	using xchip::Emulator;
	using xchip::UniqueRender;
	using xchip::UniqueInput;
	using xchip::UniqueSound;
	using namespace utix;
	using namespace utix::literals;
	
	if (argc < 2) 
	{
		std::cerr << "Usage: " << argv[0] << " -ROM <rompath>\n";
		return EXIT_FAILURE;
	}
	

#if defined(__linux__) || defined(__APPLE__) 

	if (signal(SIGINT, signals_sigint) == SIG_ERR)
	{
		LogError("Could not install signal handler");
		return EXIT_FAILURE;
	}

#elif defined(_WIN32)

	if (!SetConsoleCtrlHandler((PHANDLER_ROUTINE)ctrl_handler, true))
	{
		LogError("Could not install Console Ctrl Handler. Error Code: %d", GetLastError());
		return EXIT_FAILURE;
	}

#endif



	try
	{
		// initialize with no plugins.
		if(!g_emulator.Initialize())
			throw std::runtime_error(utix::GetLastLogError());

		const CliOpts opts(argc-1, argv+1);
		auto romPath = opts.GetOpt("-ROM");

		if (romPath.empty())
			throw std::runtime_error("Missing -ROM argument");
		
		else if (!g_emulator.LoadRom(romPath))
			throw std::runtime_error(utix::GetLastLogError());
	

		load_plugins(opts);
		configure_emulator(opts);

		if(!g_emulator.Good())
			throw std::runtime_error("Could not initialize emulator!");
		
	}
	catch(std::exception& err)
	{
		std::cerr << "Exception: " << err.what() << '\n';
		return EXIT_FAILURE;
	}
	


	while (!g_emulator.GetExitFlag())
	{
		g_emulator.UpdateSystems(); 
		g_emulator.HaltForNextFlag();		
		if (g_emulator.GetInstrFlag()) 			
			g_emulator.ExecuteInstr();
		if (g_emulator.GetDrawFlag())
			g_emulator.Draw();
	}



	return EXIT_SUCCESS;
}



template<class P>
constexpr const char* DefaultPluginPath();
#ifdef _WIN32
template<> constexpr const char* DefaultPluginPath<xchip::UniqueRender>() { return "XChipSDLRender.dll";}
template<> constexpr const char* DefaultPluginPath<xchip::UniqueInput>() { return "XChipSDLInput.dll";}
template<> constexpr const char* DefaultPluginPath<xchip::UniqueSound>() { return "XChipSDLSound.dll"; }
#elif defined(__linux__) || defined(__APPLE__)
template<> constexpr const char* DefaultPluginPath<xchip::UniqueRender>() { return "/plugins/XChipSDLRender"; }
template<> constexpr const char* DefaultPluginPath<xchip::UniqueInput>() { return "/plugins/XChipSDLInput";}
template<> constexpr const char* DefaultPluginPath<xchip::UniqueSound>() { return "/plugins/XChipSDLSound";}
#endif

template<class PluginType>
void set_plugin(const std::string& path);


void load_plugins(const utix::CliOpts& opts)
{
#ifdef _WIN32
	// get current dll directory
	char buffer[256];
	GetDllDirectory(255, buffer);
	// set our plugins folder as a dll directory
	SetDllDirectory((utix::GetFullProcDir() + "\\plugins\\").c_str());
#endif

	using ConfigPair = std::pair<const char*, void(*)(const std::string&)>;
	
	const ConfigPair configPairs[] = 
	{
		{"-REN", set_plugin<xchip::UniqueRender>},
		{"-INP", set_plugin<xchip::UniqueInput>},
		{"-SND", set_plugin<xchip::UniqueSound>}
	};

	for(const auto& it : configPairs)
	{
		const auto opt = opts.GetOpt(it.first);
		it.second(opt);
	}

#ifdef _WIN32
	//set back 
	SetDllDirectory(buffer);
#endif
}


template<class PluginType>
void set_plugin(const std::string& path)
{
	using namespace utix::literals;
	using utix::CliOpts;

	PluginType plugin;

	if (path.empty())
	{

#ifdef _WIN32
		if (!plugin.Load(DefaultPluginPath<PluginType>()))			
#elif defined(__linux__) || defined(__APPLE__)
		if (!plugin.Load(utix::GetFullProcDir() + DefaultPluginPath<PluginType>()))
#endif

			throw std::runtime_error(utix::GetLastLogError());
	}
	else if (!plugin.Load(path))
	{
		throw std::runtime_error(utix::GetLastLogError());
	}

	if (!g_emulator.SetPlugin(std::move(plugin)))
		throw std::runtime_error(utix::GetLastLogError());

}



utix::Color get_arg_rgb(const std::string& arg);
void res_config(const std::string& arg);
void cfq_config(const std::string& arg);
void sfq_config(const std::string& arg);
void col_config(const std::string& arg);
void bkg_config(const std::string& arg);
void fps_config(const std::string& arg);

void configure_emulator(const utix::CliOpts& opts)
{
	std::cout << "\n*** setting up the emulator ***\n";


	using ConfigFunc = void(*)(const std::string&);
	using ConfigPair = std::pair<const char*, ConfigFunc>;

	ConfigPair configPairs[] = 
	{
		{"-RES", res_config},
		{"-CHZ", cfq_config},
		{"-SHZ", sfq_config},
		{"-COL", col_config},
		{"-BKG", bkg_config},
		{"-FPS", fps_config}
	};

	for(const auto& it : configPairs)
	{
		const auto opt = opts.GetOpt(it.first);
		
		if(!opt.empty())
			it.second(opt);
	}

	std::cout << "*** setting up done ***\n\n";
}











void res_config(const std::string& arg)
{
	using utix::Vec2i;

	try
	{
		std::cout << "setting window size...\n";

		if(!g_emulator.GetRender())
			throw std::runtime_error("null Render");


		if(arg == "FULLSCREEN")
		{
			if(!g_emulator.GetRender()->SetFullScreen(true))
				throw std::runtime_error(utix::GetLastLogError());
		}

		else
		{
			const auto separatorIndex = arg.find('x');
		
			if(separatorIndex == std::string::npos)
				throw std::invalid_argument("missing the \'x\' separator for widthxheight");

			const Vec2i res(std::stoi(arg.substr(0, separatorIndex)), 
                                        std::stoi(arg.substr(separatorIndex+1, arg.size())) );

			g_emulator.GetRender()->SetWindowSize(res);
		}

		std::cout << "render window size: " << g_emulator.GetRender()->GetWindowSize() << '\n'; 
		std::cout << "done.\n";

	}

	catch(std::exception& e)
	{
		std::cerr << "failed to set window size: " << e.what() << '\n';
	}


}




void cfq_config(const std::string& arg)
{
	try
	{
		std::cout << "setting CPU frequency...\n";		
		const auto cfq = std::stoi(arg);
		g_emulator.SetCpuFreq(cfq);
		std::cout << "CPU frequency: " << g_emulator.GetCpuFreq() << '\n';
		std::cout << "done.\n";
	}
	catch(std::exception& e)
	{
		std::cerr << "failed to set CPU frequency: " << e.what() << '\n';
	}

}



void sfq_config(const std::string& arg)
{
	try
	{
		std::cout << "setting sound tone...\n";
		const auto sfq = std::stof(arg);

		if(!g_emulator.GetSound())
			throw std::runtime_error("null Sound");

		g_emulator.GetSound()->SetSoundFreq(sfq);
		std::cout << "sound tone frequency: " << g_emulator.GetSound()->GetSoundFreq() << '\n';
		std::cout << "done.\n";
	}
	catch(std::exception& e)
	{
		std::cerr << "Failed to set sound tone: " << e.what() << '\n';
	}
	
}






void col_config(const std::string& arg)
{
	try
	{
		std::cout << "setting render color...\n";
		const auto color = get_arg_rgb(arg);

		if(!g_emulator.GetRender())
			throw std::runtime_error("null Render");

		else if(!g_emulator.GetRender()->SetDrawColor(color))
			throw std::runtime_error(utix::GetLastLogError());

		std::cout << "render color: " << g_emulator.GetRender()->GetDrawColor() << '\n';
		std::cout << "done.\n";
		
	}
	catch(std::exception& e)
	{
		std::cerr << "Failed to set render color: " << e.what() << '\n';
	}

}








void bkg_config(const std::string& arg)
{
	try
	{
		std::cout << "setting background color...\n";
		
		const auto color = get_arg_rgb(arg);

		if(!g_emulator.GetRender())
			throw std::runtime_error("null Render");

		if(!g_emulator.GetRender()->SetBackgroundColor(color))
			throw std::runtime_error(utix::GetLastLogError());

		std::cout << "background color: " << g_emulator.GetRender()->GetBackgroundColor() << '\n';
		std::cout << "done.\n";
		
	}
	catch(std::exception& e)
	{
		std::cerr << "Failed to set render background color: " << e.what() << '\n';
	}

}





void fps_config(const std::string& arg)
{
	try
	{
		std::cout << "setting emulator FPS...\n";
		const auto fps = std::stoi(arg);
		g_emulator.SetFps(fps);
		std::cout << "emulator FPS: " << g_emulator.GetFps() << '\n';
		std::cout << "done.\n";
	}
	catch(std::exception& e)
	{
		std::cerr << "Failed to set emulator FPS: " << e.what() << '\n';
	}

}


utix::Color get_arg_rgb(const std::string& arg)
{
	const auto firstSeparator = arg.find('x');

	
	const auto secondSeparator = arg.find('x', firstSeparator + 1);

	if (firstSeparator == std::string::npos || 
         secondSeparator == std::string::npos)
	{
		throw std::runtime_error("Bad color input format. Please use rgb format, example: 255x255x255");
	}

	unsigned long rgb[3];
	
	try  {
		rgb[0] = std::stoul(arg.substr(0, firstSeparator));
		rgb[1] = std::stoul(arg.substr(firstSeparator + 1, secondSeparator));
		rgb[2] = std::stoul(arg.substr(secondSeparator + 1, arg.size()));
	} 
	catch(...) {
		throw std::runtime_error("Bad color input values, Please use values between 0-255, example: 0x255x127");
	}

	for (auto& col : rgb)
	{
		if (col > 255)
			col = 255;
	}

	return { (uint8_t)rgb[0], (uint8_t)rgb[1], (uint8_t)rgb[2] };
}








#if defined(__linux__) || defined(__APPLE__)
void signals_sigint(const int signum)
{
	std::cout << "Received sigint! signum: " << signum << '\n';
	std::cout << "Closing Application!\n";
	g_emulator.SetExitFlag(true);
}

#elif defined(_WIN32)
bool _stdcall ctrl_handler(DWORD ctrlType)
{
	std::cout << "Received ctrlType: " << ctrlType << '\n';
	std::cout << "Closing Application!\n";
	g_emulator.SetExitFlag(true);
	return true;
}
#endif



