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
	SdlAudioDevice* _device = nullptr;
	bool _initialized = false;
};




inline bool SdlSound::IsInitialized() const { return _initialized; }









}















#endif
