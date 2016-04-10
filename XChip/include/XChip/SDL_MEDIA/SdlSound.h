#ifndef _XCHIP_SDLSOUND_H_
#define _XCHIP_SDLSOUND_H_
 
#include <cstdint>
#include <XChip/Interfaces/iSound.h>
#include "SdlSystem.h"


struct SDL_AudioSpec;
typedef uint32_t SDL_AudioDeviceID;

namespace xchip {


class SdlSound final : private SdlSystem, public iSound
{
	static constexpr float defaultFreq = 450;
public:
	SdlSound() noexcept;
	~SdlSound();

	bool Initialize() noexcept override;
	void Dispose() noexcept override;

	bool IsInitialized() const override;
	bool IsPlaying() const override;

	void SetCountdownFreq(const float hertz) override;
	void Play(const uint8_t soundTimer) override;
	void Stop() override;



private:
	bool InitDevice(SDL_AudioSpec& want, SDL_AudioSpec& have);
	float GetFreq() const;
	void SetFreq(const float hz);
	void SetCycleTime(const float hz);
	void SetLenght(const unsigned int len);
	template<class T>
	static void audio_callback(void* userdata, uint8_t* stream, int len);


	SDL_AudioSpec* _specs = nullptr;
	SDL_AudioDeviceID _dev = 0;
	float _cycleTime;
	float _freq;
	float _len;
	unsigned int _pos;
	int _amplitude;
	bool _initialized = false;
	enum SpecsID { Want, Have };
};


inline bool SdlSound::IsInitialized() const { return _initialized; }























}
#endif
