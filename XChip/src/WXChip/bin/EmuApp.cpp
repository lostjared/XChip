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





	if(argc >= 3)
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

	std::cout << "\n\n\tconfigure_emulator: \n\n";


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



	const auto begin = arguments.cbegin();
	const auto end = arguments.cend();

	for(auto arg = begin; arg != end; ++arg)
	{
		bool validArg = std::any_of(std::begin(configTable), std::end(configTable),
						[&arg](const ConfigPair& cpair) 
						{
							const auto argSize = (*arg).size();
							const auto cmdSize = strlen(cpair.first);

							if(argSize == cmdSize)
							{
								if(*arg == cpair.first) 
								{
									cpair.second(*++arg);
									return true;
								}
							}

							else if(argSize > cmdSize)
							{

								if((*arg).compare(0, cmdSize, cpair.first) == 0)
								{
									cpair.second((*arg).substr(cmdSize, argSize - cmdSize));
									return true;
								}

							}

							return false;
						});

		if(!validArg)
			std::cout << "Unkown argument: " << *arg << std::endl;
	}


	std::cout << "\n\n\tconfigure_emulator done.\n\n";
}







void res_config(const std::string& arg)
{

	try
	{
		std::cout << "Setting Resolution..." << std::endl;
		const auto separatorIndex = arg.find('x');
		
		if(separatorIndex == std::string::npos)
			throw std::invalid_argument("missing the \'x\' separator for widthxheight");
	
		const auto w = std::stoul(arg.substr(0, separatorIndex));
		const auto h = std::stoul(arg.substr(separatorIndex+1, arg.size()));

		if(!g_emulator.GetRender())
			throw std::runtime_error("null Render");

		if(!g_emulator.GetRender()->SetResolution(w, h))
			throw std::runtime_error("iRender internal error");

		std::cout << "Render Resolution: " << "W: " << w << " H: " << h << std::endl; 
		std::cout << "Done." << std::endl;

	}
	catch(std::exception& e)
	{
		std::cerr << "Failed to set Render resolution: " << e.what() << std::endl;
	}


}

void cfq_config(const std::string& arg)
{

	try
	{
		std::cout << "Setting Cpu Frequency..." << std::endl;		
		const auto cfq = std::stoul(arg);
		g_emulator.SetCpuFreq(cfq);
		std::cout << "Cpu Frequency: " << +cfq << std::endl;
		std::cout << "Done." << std::endl;
	}
	catch(std::exception& e)
	{
		std::cerr << "Failed to set Cpu Frequency: " << e.what() << std::endl;
	}
}



void sfq_config(const std::string& arg)
{
	try
	{
		std::cout << "Setting Sound Tone..." << std::endl;
		const auto sfq = std::stof(arg);

		if(!g_emulator.GetSound())
			throw std::runtime_error("null Sound");

		g_emulator.GetSound()->SetSoundFreq(sfq);
		std::cout << "Sound Freq: " << sfq << std::endl;
		std::cout << "Done." << std::endl;
	}
	catch(std::exception& e)
	{
		std::cerr << "Failed to set Sound Tone: " << e.what() << std::endl;
	}
	
}






void col_config(const std::string& arg)
{
	try
	{
		std::cout << "Setting Render Color..." << std::endl;
		const auto firstSeparator = arg.find('x');

		if(firstSeparator == std::string::npos)
			throw std::invalid_argument("missing the \'x\' separator");

		const auto secondSeparator = arg.find('x', firstSeparator+1);

		if(secondSeparator == std::string::npos)
			throw std::invalid_argument("missing the second \'x\' separator");

		decltype(std::stoul("1")) rgb[3] = 
		{
			std::stoul(arg.substr(0, firstSeparator)),
			std::stoul(arg.substr(firstSeparator+1, secondSeparator)),
			std::stoul(arg.substr(secondSeparator+1, arg.size()))
		};

		for(auto& col : rgb)
		{
			if(col > 255)
				col = 255;
		}

		xchip::utility::Color color(rgb[0], rgb[1], rgb[2]);

		if(!g_emulator.GetRender())
			throw std::runtime_error("null Render");

		if(!g_emulator.GetRender()->SetColorFilter(color))
			throw std::runtime_error("iRender internal error");

		std::cout << "Render Color: " << color << std::endl;
		std::cout << "Done." << std::endl;
		
	}
	catch(std::exception& e)
	{
		std::cerr << "Failed to set Render Color: " << e.what() << std::endl;
	}
}





void fps_config(const std::string& arg)
{
	try
	{
		std::cout << "Setting Emulator FPS..." << std::endl;
		const auto fps = std::stoul(arg);
		g_emulator.SetFps(fps);
		std::cout << "Emulator FPS: " << +fps << std::endl;
		std::cout << "Done." << std::endl;
	}
	catch(std::exception& e)
	{
		std::cerr << "Failed to set Emulator FPS: " << e.what() << std::endl;
	}
}





