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

#ifndef XCHIP_PLUGINS_SDLSOUND_H_
#define XCHIP_PLUGINS_SDLSOUND_H_

#include <SDL2/SDL.h>
#include <Utix/Ints.h>
#include <XChip/Plugins/iSound.h>




namespace xchip {


class SdlSound final : public iSound
{
	static constexpr const char* const PLUGIN_NAME = "SdlSound";
	static constexpr const char* const PLUGIN_VER = "1.0 using SDL2";
	static constexpr float DEFAULT_FREQ = 450;
public:
	SdlSound() noexcept;
	~SdlSound();

	bool Initialize() noexcept override;
	void Dispose() noexcept override;
	bool IsInitialized() const noexcept override;
	const char* GetPluginName() const noexcept override;
	const char* GetPluginVersion() const noexcept override;
	PluginDeleter GetPluginDeleter() const noexcept override;
	bool IsPlaying() const noexcept override;
	float GetCountdownFreq() const noexcept override;	
	float GetSoundFreq() const noexcept override;
	void SetCountdownFreq(const float hertz) noexcept override;
	void SetSoundFreq(const float hz) noexcept override;
	void Play(const uint8_t soundTimer) noexcept override;
	void Stop() noexcept override;



private:
	float GetPlayFreq() const;
	float GetCurFreq() const;
	void SetPlayFreq(const float hz);
	void SetCurFreq(const float hz);
	void SetCycleTime(const float hz);
	void SetLenght(const unsigned int len);

	bool OpenAudioDevice();
	void CloseAudioDevice();
	template<class T>
	static void audio_callback(void* userdata, uint8_t* stream, int len) noexcept;


	SDL_AudioSpec* m_specs = nullptr;
	SDL_AudioDeviceID m_dev = 0;
	float m_cycleTime;
	float m_playFreq;
	float m_curFreq;
	float m_len;
	unsigned int m_pos;
	int m_amplitude;
	bool m_initialized = false;
	enum SpecsID { WANT, HAVE };
};

























}




#endif // XCHIP_PLUGINS_SDLSOUND_H_
