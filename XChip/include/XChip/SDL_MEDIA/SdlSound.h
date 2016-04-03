#ifndef _XCHIP_SDLSOUND_H_
#define _XCHIP_SDLSOUND_H_

#include <cstdint>
#include <XChip/Interfaces/iSound.h>
#include "SdlMedia.h"

// TODO: stop the sound clipping if possible by smoothing the ( better volume controll )
// TODO: precalculate the sin wave
// TODO: PLEASE ORGANIZE THIS CLASS

namespace xchip {
class SdlAudioDevice;

class SdlSound final : private SdlMedia, public iSound
{
public:
	SdlSound() noexcept;
	~SdlSound();
	bool Initialize() noexcept override;
	void Dispose() noexcept override;

	bool IsInitialized() const override { return _initialized; }
	bool IsPlaying() const override;

	void SetCountdownFreq(const float hz) override;
	void Play(const uint8_t soundTimer) override;
	void Stop() override;

private:
	template<class T>
	static void audio_callback(void* sdlSound, uint8_t* const stream, int len);

private:
	SdlAudioDevice *_device = nullptr;
	unsigned int _audioPos = 0;   // which sample we are up to 
	float _tone = 512;            // beep tone, default to 350hz
	float _audioLen = 0;          // timeleft ( calculated from soundTimer )
	float _audioFreq = 0;         // audio frequency in cycles per sample
	float _audioVol = 0;          // audio volume, 0 - ~32000 */
	float _cycleTime = 0;         // countdown freq, default to 60hz
	bool _initialized = false;
};
















}















#endif
