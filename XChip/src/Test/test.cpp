#include <csignal>
#include <iostream>

#include <XChip/Core/Emulator.h> 
#include <XChip/Media/SDLMedia.h>
#include <XChip/Utility/Memory.h>



static xchip::Emulator g_emulator;


int main(void)
{
	using xchip::Emulator;
	using xchip::SdlRender;
	using xchip::SdlInput;
	using xchip::SdlSound;
	using xchip::UniqueRender;
	using xchip::UniqueInput;
	using xchip::UniqueSound;
	using xchip::utility::make_unique;




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


	
	uint32_t* buff = const_cast<uint32_t*>(g_emulator.GetRender()->GetBuffer());



	buff[0] = ~0;
	buff[63 * 1] = ~0;

	int y = 0;

	while (!g_emulator.GetExitFlag())
	{
		g_emulator.UpdateSystems(); 
		g_emulator.HaltForNextFlag();
		if (g_emulator.GetDrawFlag())
		{
			g_emulator.Draw();

			if( y == 32 ) 
				y = 0;
		}
	}





	return EXIT_SUCCESS;


}














