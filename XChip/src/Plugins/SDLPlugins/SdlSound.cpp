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

// needed for M_PI 
#define _USE_MATH_DEFINES 
#include <cmath>
#include <cstdlib>
#include <cstring>

#include <Utix/Log.h>
#include <Utix/Timer.h>
#include <Utix/ScopeExit.h>
#include <Utix/Assert.h>

#include <XChip/Plugins/SDLPlugins/SdlSound.h>

 
#define _SDLSOUND_INITIALIZED_ASSERT_() ASSERT_MSG(m_initialized == true, "SdlSound is not initialized")

namespace xchip {

using namespace utix;

extern "C" XCHIP_EXPORT void XCHIP_FreePlugin(const iPlugin*);






constexpr const char* const SdlSound::PLUGIN_NAME;
constexpr const char* const SdlSound::PLUGIN_VER;
constexpr float SdlSound::DEFAULT_FREQ;





inline float SdlSound::GetCurFreq() const { return m_curFreq * m_specs[HAVE].freq; }
inline float SdlSound::GetPlayFreq() const { return m_playFreq * m_specs[HAVE].freq; }
inline void SdlSound::SetCycleTime(const float hz) { m_cycleTime = m_specs[HAVE].freq / hz; }
inline void SdlSound::SetCurFreq(const float hz) { m_curFreq = hz / m_specs[HAVE].freq; }
inline void SdlSound::SetPlayFreq(const float hz) { m_playFreq = hz / m_specs[HAVE].freq; }
inline void SdlSound::SetLenght(const unsigned int len) { m_len = m_cycleTime * len; }







SdlSound::SdlSound() noexcept
{
	Log("Creating SdlSound object...");
}



SdlSound::~SdlSound()
{
	if (m_initialized)
		this->Dispose();

	Log("Destroying SdlSound object...");
}



bool SdlSound::Initialize() noexcept
{
	using namespace utix::literals;

	if (m_initialized)
		this->Dispose();


	if( SDL_InitSubSystem(SDL_INIT_AUDIO) )
		return false;

	const auto cleanup = MakeScopeExit([this]() noexcept {
		if (!this->m_initialized)
			this->Dispose();
	});
	
	if( !OpenAudioDevice() )
		return false;

	m_len = 0.f;
	m_pos = 0u;
	m_amplitude = 16000;
	m_cycleTime = m_specs[HAVE].freq / 60.f;
	this->SetCurFreq(DEFAULT_FREQ);

	m_initialized = true;
	return true;
}



void SdlSound::Dispose() noexcept
{
	CloseAudioDevice();
	SDL_QuitSubSystem( SDL_INIT_AUDIO );
	m_initialized = false;
}


bool SdlSound::IsInitialized() const noexcept
{
	return m_initialized;
}



const char* SdlSound::GetPluginName() const noexcept
{
	return PLUGIN_NAME;
}



const char* SdlSound::GetPluginVersion() const noexcept
{
	return PLUGIN_VER;
}

PluginDeleter SdlSound::GetPluginDeleter() const noexcept
{
	return XCHIP_FreePlugin;
}



float SdlSound::GetSoundFreq() const noexcept 
{ 
	return this->GetCurFreq(); 
}


float SdlSound::GetCountdownFreq() const noexcept
{
	_SDLSOUND_INITIALIZED_ASSERT_();
	return m_specs[HAVE].freq / m_cycleTime;
}

bool SdlSound::IsPlaying() const  noexcept
{
	_SDLSOUND_INITIALIZED_ASSERT_();
	return SDL_GetAudioDeviceStatus(m_dev) == SDL_AUDIO_PLAYING;
}


void SdlSound::SetCountdownFreq(const float hertz) noexcept 
{ 
	_SDLSOUND_INITIALIZED_ASSERT_(); 
	m_cycleTime = m_specs[HAVE].freq / hertz; 
}


void SdlSound::SetSoundFreq(const float hz) noexcept 
{ 
	_SDLSOUND_INITIALIZED_ASSERT_();
	this->SetCurFreq(hz); 
}



void SdlSound::Play(const uint8_t soundTimer) noexcept
{
	_SDLSOUND_INITIALIZED_ASSERT_();
	if (!this->IsPlaying()) 
	{
		SetPlayFreq(GetCurFreq() + 2 * soundTimer);
		SetLenght(soundTimer);
	}
	
	else
	{
		SDL_LockAudioDevice(m_dev);
		SetPlayFreq(GetCurFreq() + 2 * soundTimer);
		SetLenght(soundTimer);
		SDL_UnlockAudioDevice(m_dev);
	}

	SDL_PauseAudioDevice(m_dev, 0);
}





void SdlSound::Stop() noexcept
{
 	_SDLSOUND_INITIALIZED_ASSERT_();
	if (this->IsPlaying())
	{
		SDL_LockAudioDevice(m_dev);
		m_len = 0;
		SDL_UnlockAudioDevice(m_dev);
	}
}














// private methods


bool SdlSound::OpenAudioDevice()
{

	m_specs = (SDL_AudioSpec*) std::malloc( sizeof(SDL_AudioSpec) * 2 );

	if (!m_specs) {
		LogError("Could not allocate memory for SDL_AudioSpecs");
		return false;
	}

	auto& want = m_specs[WANT];
	auto& have = m_specs[HAVE];

	memset(&want, 0, sizeof(SDL_AudioSpec));
	want.freq = 44100;
	want.format = AUDIO_S16;
	want.channels = 1;
	want.samples = 1024;
	want.callback = SdlSound::audio_callback<Sint16>;
	want.userdata = this;

	m_dev = SDL_OpenAudioDevice(NULL, 0, &want, &have, 0);

	if ( m_dev < 2 ) 
	{
		std::free(m_specs);
		m_specs = nullptr;
		m_dev = 0;
		LogError("SdlSound: Failed to open audio device: %s", SDL_GetError());
		return false;
	}

	return true;
}


void SdlSound::CloseAudioDevice() 
{
	if(m_specs) {
		std::free(m_specs);
		m_specs = nullptr;
	}

	if( m_dev != 0 ) {
		SDL_CloseAudioDevice(m_dev);
		m_dev = 0;
	}
}















template<class T>
void SdlSound::audio_callback(void* userdata, uint8_t* const stream, const int len) noexcept
{
	auto *const _this = reinterpret_cast<SdlSound*>(userdata);
	auto *const buff = reinterpret_cast<T*>(stream);
	const size_t bufflen = len / sizeof(T);


	constexpr auto _2pi = static_cast<float>(2 * M_PI);
	const auto ampl = _this->m_amplitude;
	const auto freq = _this->m_playFreq;
	auto pos = _this->m_pos;


	if (_this->m_len > 0)
	{
		for (size_t i = 0; i < bufflen; ++i, ++pos)
			buff[i] = static_cast<T>(ampl * std::sin(_2pi * freq * pos));

		_this->m_pos = pos;
		_this->m_len -= bufflen;
	}

	else
	{	// need a better workaround to stop clipping.
		auto downAmpl = ampl;
		for (size_t i = 0; i < bufflen; ++i, ++pos)
		{
			buff[i] = static_cast<T>(downAmpl * std::sin(_2pi * freq * pos));
			if (downAmpl > 100)
				downAmpl -= 60;
		}

		// pause the device from the callback function.
		// is this ok ?
		_this->m_pos = 0;
		SDL_PauseAudioDevice(_this->m_dev, 1);
		
	}

}

















// export

extern "C" XCHIP_EXPORT iPlugin* XCHIP_LoadPlugin()
{
	return new(std::nothrow) SdlSound();
}




extern "C" XCHIP_EXPORT void XCHIP_FreePlugin(const iPlugin* plugin)
{
	const auto* sdlsound = dynamic_cast<const SdlSound*>(plugin);

	if(!sdlsound)
	{
		LogError("XCHIP_FreePlugin: dynamic_cast from iPlugin* to SdlSound* Failed");
		std::exit(EXIT_FAILURE);
	}

	delete sdlsound;
}

















}
