#include <SDL2/SDL.h>
#include <Chip8/SDL/SdlInput.h>
#include <Chip8/Utility/log.h>
#include <Chip8/Utility/traits.h>

extern SDL_Event g_sdlEvent;
extern void UpdateSdlEvents();

SdlInput::SdlInput() 
	: m_keyboardState(nullptr),
	m_keyPairs
	{
		{ EmulatorKey::KEY_0, SDL_SCANCODE_KP_0}, { EmulatorKey::KEY_1, SDL_SCANCODE_KP_7}, { EmulatorKey::KEY_2, SDL_SCANCODE_KP_8}, 
		{ EmulatorKey::KEY_3, SDL_SCANCODE_KP_9}, { EmulatorKey::KEY_4, SDL_SCANCODE_KP_4}, { EmulatorKey::KEY_5, SDL_SCANCODE_KP_5}, 
		{ EmulatorKey::KEY_6, SDL_SCANCODE_KP_6}, { EmulatorKey::KEY_7, SDL_SCANCODE_KP_1}, { EmulatorKey::KEY_8, SDL_SCANCODE_KP_2}, 
		{ EmulatorKey::KEY_9, SDL_SCANCODE_KP_3}, { EmulatorKey::KEY_A, SDL_SCANCODE_KP_DIVIDE}, { EmulatorKey::KEY_B, SDL_SCANCODE_KP_MULTIPLY}, 
		{ EmulatorKey::KEY_C, SDL_SCANCODE_KP_MINUS}, { EmulatorKey::KEY_D, SDL_SCANCODE_KP_PLUS}, { EmulatorKey::KEY_E, SDL_SCANCODE_KP_PERIOD}, 
		{ EmulatorKey::KEY_F, SDL_SCANCODE_KP_ENTER}, { EmulatorKey::RESET, SDL_SCANCODE_RETURN}, { EmulatorKey::ESCAPE, SDL_SCANCODE_ESCAPE}
	},
	m_resetClbk(nullptr),
	m_escapeClbk(nullptr),
	m_resetClbkArg(nullptr),
	m_escapeClbkArg(nullptr)
{
	LOG("Creating SdlInput object...");
}

SdlInput::~SdlInput()
{
	if (m_keyboardState)
		this->Dispose();

	LOG("Destroying SdlInput object...");
}




bool SdlInput::Initialize()
{
	m_keyboardState = SDL_GetKeyboardState(NULL);

	if (!m_keyboardState) {
		LOGerr("Cannot get Keyboard State");
		return false;
	}

	return true;	
}



void SdlInput::Dispose() noexcept
{
	m_keyboardState = nullptr;
	m_resetClbk = nullptr;
	m_escapeClbk = nullptr;
	m_resetClbkArg = nullptr;
	m_escapeClbkArg = nullptr;	
}


bool SdlInput::UpdateKeys()
{
	UpdateSdlEvents();
	if (g_sdlEvent.type == SDL_KEYDOWN)
	{
		if (g_sdlEvent.key.keysym.sym == SDLK_RETURN) {
			if (m_resetClbk) m_resetClbk(m_resetClbkArg);
		}

		else if (g_sdlEvent.key.keysym.sym == SDLK_ESCAPE) {
			if (m_escapeClbk) m_escapeClbk(m_escapeClbkArg);
		}

		return true;
	}
	
	return false;
}


void SdlInput::SetKey(const EmulatorKey key, int value)
{
	m_keyPairs[toUType(key)].second = static_cast<SDL_Scancode>(value);
}



bool SdlInput::IsKeyPressed(const EmulatorKey key) const
{
	return m_keyboardState[m_keyPairs[toUType(key)].second] == SDL_TRUE;
}


EmulatorKey SdlInput::GetPressedKey() const
{
	for (const auto& keyPair : m_keyPairs) 
	{
		if (m_keyboardState[keyPair.second] == SDL_TRUE)
			return keyPair.first;
	}

	return EmulatorKey::NO_KEY_PRESSED;
}



EmulatorKey SdlInput::WaitKeyPress()
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
		};
	}

	return EmulatorKey::NO_KEY_PRESSED;
}


void SdlInput::SetWaitKeyPressCallback(void* arg, WaitKeyPressCallback callback) {
	m_waitClbkArg = arg;
	m_waitClbk = callback;
}


void SdlInput::SetResetCallback(void* arg, ResetCallback callback) {
	m_resetClbkArg = arg;
	m_resetClbk = callback;
	
}

void SdlInput::SetEscapeCallback(void* arg, EscapeCallback callback) {
	m_escapeClbkArg = arg;
	m_escapeClbk = callback;
}
