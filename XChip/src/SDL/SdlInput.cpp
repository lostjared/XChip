#include <SDL2/SDL.h>
#include <XChip/Utility/Log.h>
#include <XChip/Utility/Traits.h>
#include <XChip/SDL/SdlInput.h>


namespace xchip {

	
	
using namespace utility;
extern SDL_Event g_sdlEvent;

	

SdlInput::SdlInput()
	: SdlMedia(System::Input),
	m_keyboardState(nullptr),
	m_keyPairs
{
	{ Key::KEY_0, SDL_SCANCODE_KP_0 },{ Key::KEY_1, SDL_SCANCODE_KP_7 },{ Key::KEY_2, SDL_SCANCODE_KP_8 },
	{ Key::KEY_3, SDL_SCANCODE_KP_9 },{ Key::KEY_4, SDL_SCANCODE_KP_4 },{ Key::KEY_5, SDL_SCANCODE_KP_5 },
	{ Key::KEY_6, SDL_SCANCODE_KP_6 },{ Key::KEY_7, SDL_SCANCODE_KP_1 },{ Key::KEY_8, SDL_SCANCODE_KP_2 },
	{ Key::KEY_9, SDL_SCANCODE_KP_3 },{ Key::KEY_A, SDL_SCANCODE_KP_DIVIDE },{ Key::KEY_B, SDL_SCANCODE_KP_MULTIPLY },
	{ Key::KEY_C, SDL_SCANCODE_KP_MINUS },{ Key::KEY_D, SDL_SCANCODE_KP_PLUS },{ Key::KEY_E, SDL_SCANCODE_KP_PERIOD },
	{ Key::KEY_F, SDL_SCANCODE_KP_ENTER },{ Key::RESET, SDL_SCANCODE_RETURN }, { Key::ESCAPE, SDL_SCANCODE_ESCAPE   }
},
	m_resetClbk(nullptr),
	m_escapeClbk(nullptr),
	m_resetClbkArg(nullptr),
	m_escapeClbkArg(nullptr),
	m_initialized(false)
{
	LOG("Creating SdlInput object...");
}


SdlInput::~SdlInput()
{
	if (m_keyboardState)
		this->Dispose();

	LOG("Destroying SdlInput object...");
}




bool SdlInput::Initialize() noexcept
{
	if (m_initialized)
		this->Dispose();

	else if (!this->InitSubSystem())
		return false;

	m_keyboardState = SDL_GetKeyboardState(NULL);

	if (!m_keyboardState) 
	{
		LOGerr("Cannot get Keyboard State");
		return false;
	}

	m_initialized = true;
	return true;
}



void SdlInput::Dispose() noexcept
{
	m_keyboardState = nullptr;
	m_resetClbk = nullptr;
	m_escapeClbk = nullptr;
	m_resetClbkArg = nullptr;
	m_escapeClbkArg = nullptr;
	m_initialized = false;
}


bool SdlInput::UpdateKeys()
{
	SdlMedia::UpdateEvents();
	if (g_sdlEvent.type == SDL_KEYDOWN)
	{
		if (g_sdlEvent.key.keysym.sym == SDLK_RETURN) 
		{
			if (m_resetClbk) 
				m_resetClbk(m_resetClbkArg);
		}


		else if (g_sdlEvent.key.keysym.sym == SDLK_ESCAPE) 
		{
			if (m_escapeClbk) 
				m_escapeClbk(m_escapeClbkArg);
		}

		return true;
	}

	return false;
}



bool SdlInput::IsKeyPressed(const Key key) const
{
	return m_keyboardState[m_keyPairs[toUType(key)].second] == SDL_TRUE;
}


Key SdlInput::GetPressedKey() const
{
	for (const auto& keyPair : m_keyPairs)
	{
		if (m_keyboardState[keyPair.second] == SDL_TRUE)
			return keyPair.first;
	}

	return Key::NO_KEY_PRESSED;
}



Key SdlInput::WaitKeyPress()
{
	if (m_waitClbk != nullptr)
	{
		while (m_waitClbk(m_waitClbkArg))
		{
			if (UpdateKeys())
			{
				/*check for RESET & ESCAPE keys first */
				for (auto itr = m_keyPairs.crbegin(); itr != m_keyPairs.crend(); ++itr)
				{
					if (m_keyboardState[itr->second] == SDL_TRUE)
						return itr->first;
				}
			}
		}
	}

	return Key::NO_KEY_PRESSED;
}



void SdlInput::SetWaitKeyCallback(const void* arg, WaitKeyCallback callback) 
{
	m_waitClbkArg = arg;
	m_waitClbk = callback;
}


void SdlInput::SetResetKeyCallback(const void* arg, ResetKeyCallback callback) 
{
	m_resetClbkArg = arg;
	m_resetClbk = callback;
}


void SdlInput::SetEscapeKeyCallback(const void* arg, EscapeKeyCallback callback)
{
	m_escapeClbkArg = arg;
	m_escapeClbk = callback;
}








}











