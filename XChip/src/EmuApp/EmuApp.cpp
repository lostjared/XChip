/*

EmuApp - chip8 emulator implementation using XChip/Utix.
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

#include <SDL2/SDL_messagebox.h>
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

namespace {
void DisplayErrorMsg(const std::string& title, const std::string& errmsg);
void LoadPlugins(const utix::CliOpts& opts);
void ConfigureEmulator(const utix::CliOpts& opts);
}

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



	try {
		// initialize with no plugins.
		if(!g_emulator.Initialize())
			throw std::runtime_error(utix::GetLastLogError());

		const CliOpts opts(argc-1, argv+1);
		auto romPath = opts.GetOpt("-ROM");

		if (romPath.empty())
			throw std::runtime_error("Missing -ROM argument");
		
		else if (!g_emulator.LoadRom(romPath))
			throw std::runtime_error(utix::GetLastLogError());
	

		LoadPlugins(opts);
		ConfigureEmulator(opts);

		if(!g_emulator.Good())
			throw std::runtime_error("Could not initialize emulator!");
		
	}
	catch(std::exception& err) {
		DisplayErrorMsg("Fatal Exception", err.what());
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

// locals functions definitions
namespace {


#ifdef _WIN32
template<class P>
constexpr const char* DefaultPluginPath() {
	return utix::is_same<P, xchip::UniqueRender>::value ? "XChipSDLRender.dll" 
	: utix::is_same<P, xchip::UniqueInput>::value ?  "XChipSDLInput.dll" : "XChipSDLSound.dll";
}
#elif defined(__linux__) || defined(__APPLE__)
template<class P>
constexpr const char* DefaultPluginPath() {
	return utix::is_same<P, xchip::UniqueRender>::value ? "plugins/XChipSDLRender"
	: utix::is_same<P, xchip::UniqueInput>::value ?  "plugins/XChipSDLInput" : "plugins/XChipSDLSound";
}
#endif

void LoadPlugins(const utix::CliOpts& opts)
{
	using xchip::UniqueRender;
	using xchip::UniqueInput;
	using xchip::UniqueSound;

	const auto procDir = utix::GetFullProcDir();

	if(procDir.empty())
		DisplayErrorMsg("Load Plugins", "Couldn't get process directory!");

	auto ren_path = opts.GetOpt("-REN");
	auto inp_path = opts.GetOpt("-INP");
	auto snd_path = opts.GetOpt("-SND");
	const auto SetIfEmpty = [](const char* src, std::string& dest) { if(dest.empty()) dest = src; };
#ifdef _WIN32
	// setting the dll directory on windows
	// make possible to load the plugin's dependencies dlls
	// such as SDL.dll from plugins folder.
	char oldDir[256];
	GetDllDirectory(255, oldDir);
	SetDllDirectory((procDir + "\\plugins\\").c_str());
	const auto ddl_dir_cleanup = utix::MakeScopeExit([oldDir=&oldDir[0]]()noexcept{ SetDllDirectory(oldDir); });
	// since the dll directory is set, no need to use procDir here:
	SetIfEmpty(DefaultPluginPath<UniqueRender>(), ren_path);
	SetIfEmpty(DefaultPluginPath<UniqueInput>(), inp_path);
	SetIfEmpty(DefaultPluginPath<UniqueSound>(), snd_path);
#elif defined(__linux__) || defined(__APPLE__)
	SetIfEmpty((procDir + DefaultPluginPath<UniqueRender>()).c_str(), ren_path);
	SetIfEmpty((procDir + DefaultPluginPath<UniqueInput>()).c_str(), inp_path);
	SetIfEmpty((procDir + DefaultPluginPath<UniqueSound>()).c_str(), snd_path);
#else
	#error Unknown Plataform
#endif

	UniqueRender rend;
	UniqueInput input;
	UniqueSound sound;
	if(!rend.Load(ren_path))
		throw std::runtime_error("Failed to load Render Plugin: " + utix::GetLastLogError());
	if(!input.Load(inp_path))
		throw std::runtime_error("Failed to load Input Plugin: " + utix::GetLastLogError());
	if(!sound.Load(snd_path))
		throw std::runtime_error("Failed to load Sound Plugin: " + utix::GetLastLogError());

	g_emulator.SetPlugin(std::move(rend));
	g_emulator.SetPlugin(std::move(input));
	g_emulator.SetPlugin(std::move(sound));
}







utix::Color get_arg_rgb(const std::string& arg);
void res_config(const std::string& arg);
void chz_config(const std::string& arg);
void shz_config(const std::string& arg);
void col_config(const std::string& arg);
void bkg_config(const std::string& arg);
void fps_config(const std::string& arg);

void ConfigureEmulator(const utix::CliOpts& opts)
{
	std::cout << "\n*** setting up the emulator ***\n";


	using ConfigFunc = void(*)(const std::string&);
	using ConfigPair = std::pair<const char*, ConfigFunc>;

	ConfigPair configPairs[] = 
	{
		{"-RES", res_config},
		{"-CHZ", chz_config},
		{"-SHZ", shz_config},
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

	try {
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
	catch(std::exception& e) {
		DisplayErrorMsg("res_config", e.what());
	}


}




void chz_config(const std::string& arg)
{
	try {
		std::cout << "setting CPU frequency...\n";		
		const auto cfq = std::stoi(arg);
		g_emulator.SetCpuFreq(cfq);
		std::cout << "CPU frequency: " << g_emulator.GetCpuFreq() << '\n';
		std::cout << "done.\n";
	}
	catch(std::exception& e) {
		DisplayErrorMsg("chz_config", e.what());
	}

}



void shz_config(const std::string& arg)
{
	try {
		std::cout << "setting sound tone...\n";
		const auto sfq = std::stof(arg);

		if(!g_emulator.GetSound())
			throw std::runtime_error("null Sound");

		g_emulator.GetSound()->SetSoundFreq(sfq);
		std::cout << "sound tone frequency: " << g_emulator.GetSound()->GetSoundFreq() << '\n';
		std::cout << "done.\n";
	}
	catch(std::exception& e) {
		DisplayErrorMsg("shz_config", e.what());
	}
	
}






void col_config(const std::string& arg)
{
	try {
		std::cout << "setting render color...\n";
		const auto color = get_arg_rgb(arg);

		if(!g_emulator.GetRender())
			throw std::runtime_error("null Render");

		else if(!g_emulator.GetRender()->SetDrawColor(color))
			throw std::runtime_error(utix::GetLastLogError());

		std::cout << "render color: " << g_emulator.GetRender()->GetDrawColor() << '\n';
		std::cout << "done.\n";
		
	}
	catch(std::exception& e) {
		DisplayErrorMsg("col_config", e.what());
	}

}








void bkg_config(const std::string& arg)
{
	try {
		std::cout << "setting background color...\n";
		
		const auto color = get_arg_rgb(arg);

		if(!g_emulator.GetRender())
			throw std::runtime_error("null Render");

		if(!g_emulator.GetRender()->SetBackgroundColor(color))
			throw std::runtime_error(utix::GetLastLogError());

		std::cout << "background color: " << g_emulator.GetRender()->GetBackgroundColor() << '\n';
		std::cout << "done.\n";
		
	}
	catch(std::exception& e) {
		DisplayErrorMsg("bkg_config", e.what());
	}

}





void fps_config(const std::string& arg)
{
	try {
		std::cout << "setting emulator FPS...\n";
		const auto fps = std::stoi(arg);
		g_emulator.SetFps(fps);
		std::cout << "emulator FPS: " << g_emulator.GetFps() << '\n';
		std::cout << "done.\n";
	}
	catch(std::exception& e) {
		DisplayErrorMsg("fps_config", e.what());
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





void DisplayErrorMsg(const std::string& title, const std::string& errmsg)
{
	if(SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, title.c_str(), errmsg.c_str(), nullptr) != 0)
		std::cerr << title << ": " << errmsg << '\n';
}








}


// signals

#if defined(__linux__) || defined(__APPLE__)
void signals_sigint(const int signum)
{
	std::cout << "Received sigint! signum: " << signum << "\nClosing Application!\n";
	g_emulator.SetExitFlag(true);
}

#elif defined(_WIN32)
bool _stdcall ctrl_handler(DWORD ctrlType)
{
	std::cout << "Received ctrlType: " << ctrlType << "\nClosing Application!\n";
	g_emulator.SetExitFlag(true);
	return true;
}
#endif



