#include <csignal>
#include <algorithm>
#include <utility>
#include <iostream>

#include <XChip/Core/Emulator.h> 
#include <XChip/Media/SDLMedia.h>
#include <XChip/Utility/Memory.h>
#include <XChip/Utility/Color.h>
#include <XChip/Utility/Log.h>

static xchip::Emulator g_emulator;
void configure_emulator(const std::vector<std::string>& arguments);




/*********************************************************
 *	-RES  WidthxHeight ex: -RES 200x300
 *	-CFQ  Cpu Frequency in hz ex: -CFQ 600
 *	-SFQ  Sound Tone in hz ex: -SFQ 400
 *	-COL  Color in RGB ex: -COL 100x200x400
 *	-FPS  Frame Rate ex: -FPS 30
 *********************************************************/




int main(int argc, char **argv)
{
	using xchip::Emulator;
	using xchip::SdlRender;
	using xchip::SdlInput;
	using xchip::SdlSound;
	using xchip::UniqueRender;
	using xchip::UniqueInput;
	using xchip::UniqueSound;
	using xchip::utility::make_unique;

	


	if (argc < 2) {
		std::cout << "No game to load..." << std::endl;
		return EXIT_SUCCESS;
	}


	UniqueRender render;
	UniqueInput input;
	UniqueSound sound;


	try {
		render = make_unique<SdlRender>();
		input = make_unique<SdlInput>();
		sound = make_unique<SdlSound>();
	}
	catch (std::exception& e) {
		std::cout << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	
	if (!g_emulator.Initialize(std::move(render), std::move(input), std::move(sound)))
		return EXIT_FAILURE;

	if (!g_emulator.LoadRom(argv[1]))
		return EXIT_FAILURE;



	if(signal(SIGINT, [](int signum)
	{
		std::cout << "Received signal: " << signum << std::endl;
		std::cout << "Closing Application!" << std::endl;
		g_emulator.SetExitFlag(true);

	}) == SIG_ERR )
	{
		std::cout << "Could not install signal handler!" << std::endl;
		return EXIT_FAILURE;
	}





	if( argc >= 3)
		configure_emulator(std::vector<std::string>(argv+2, argv+argc));

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






void res_config(const std::string& arg);
void cfq_config(const std::string& arg);
void sfq_config(const std::string& arg);
void col_config(const std::string& arg);
void fps_config(const std::string& arg);

void configure_emulator(const std::vector<std::string>& arguments)
{
	using xchip::utility::make_unique;

	std::cout << "arguments to configure_emulator: " << std::endl;
	for( auto& it : arguments)
		std::cout << it << std::endl;


	using ConfigFunc = void(*)(const std::string&);
	using ConfigPair = std::pair<const char*, ConfigFunc>;

	ConfigPair configTable[5] = 
	{
		{"-RES", res_config},
		{"-CFQ", cfq_config},
		{"-SFQ", sfq_config},
		{"-COL", col_config},
		{"-FPS", fps_config}
	};



	const auto begin = arguments.cbegin(), end = arguments.cend();
	for(auto arg = begin; arg != end; ++arg)
	{
		bool validArg = std::any_of(std::begin(configTable), std::end(configTable),
						[&arg](const ConfigPair& cpair) 
						{
							if(*arg == cpair.first) {
								cpair.second(*++arg);
								return true;
							}
							else {
								return false;
							}
						});

		if(!validArg)
			std::cout << "Unkown argument: " << *arg << std::endl;
	}

}







void res_config(const std::string& arg)
{
	std::cout << "Setting Resolution... to: " << arg << std::endl;
	const auto separatorIndex = arg.find('x');
	
	const auto wStr = arg.substr(0, separatorIndex);
	const auto hStr = arg.substr(separatorIndex+1, arg.size()); 

	try
	{
		const auto w = std::stoul(wStr);
		const auto h = std::stoul(hStr);
	
		g_emulator.GetRender()->SetResolution(w, h);
	
		std::cout << "W: " << w << std::endl;
		std::cout << "H: " << h << std::endl; 
		std::cout << "Done." << std::endl;

	}
	catch(std::invalid_argument& e)
	{
		std::cerr << "Invalid resolution syntax: " << arg << std::endl;		
	}

}

void cfq_config(const std::string& arg)
{
	std::cout << "Setting Cpu Frequency... to: " << arg << std::endl;
}

void sfq_config(const std::string& arg)
{
	std::cout << "Setting Sound Tone... to: " << arg << std::endl;
}

void col_config(const std::string& arg)
{
	std::cout << "Setting Color... to: " << arg << std::endl;
}

void fps_config(const std::string& arg)
{
	std::cout << "Setting FPS... to: " << arg << std::endl;
}





