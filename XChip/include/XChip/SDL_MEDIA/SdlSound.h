#ifndef _XCHIP_SDLSOUND_H_
#define _XCHIP_SDLSOUND_H_

#include <cstdint>
#include <XChip/Interfaces/iSound.h>
#include "SdlAudioDevice.h"
#include "SdlMedia.h"

// TODO: stop the sound clipping if possible by smoothing the ( better volume controll )
// TODO: precalculate the sin wave
// TODO: PLEASE ORGANIZE THIS CLASS

namespace xchip {

class SdlSound final : private SdlMedia, public iSound
{
public:
	SdlSound() noexcept;
	~SdlSound();
	bool Initialize() noexcept override;
	void Dispose() noexcept override;

	bool IsInitialized() const override { return _initialized; }
	bool IsPlaying() const override { return _device.IsRunning(); };

	void SetCountdownFreq(const float hz) override;
	void Play(const uint8_t soundTimer) override;
	void Stop() override;

private:
	template<class T>
	static void audio_callback(void* sdlSound, Uint8* const stream, int len);

private:
	SdlAudioDevice _device;
	unsigned int _audioPos;  // which sample we are up to 
	float _tone;             // beep tone, default to 350hz
	float _audioLen;         // timeleft ( calculated from soundTimer )
	float _audioFreq;        // audio frequency in cycles per sample
	float _audioVol;         // audio volume, 0 - ~32000 */
	float _cycleTime;        // countdown freq, default to 60hz
	bool _initialized;
};
















}















#endif
