#include <cstdint>
#include <XChip/Interfaces/iSound.h>
#include "SdlMedia.h"

// TODO: stop the sound clipping if possible
// TODO: precalculate the sin wave
// TODO: PLEASE ORGANIZE THIS CLASS

struct SDL_AudioSpec;
typedef uint32_t SDL_AudioDeviceID;
typedef uint8_t Uint8;
namespace xchip {



class SdlSound final : private SdlMedia, public iSound
{
public:
	SdlSound() noexcept;
	~SdlSound();
	bool Initialize() noexcept override;
	void Dispose() noexcept override;

	bool IsInitialized() const override { return _initialized; }
	bool IsPlaying() const override { return _playing;  };

	void SetCountdownFreq(const float hz) override;
	void Play(const uint8_t soundTimer) override;
	void Stop() override;
private:
	template<class T>
	static void audio_callback(void* sdlSound, Uint8* const stream, int len);

private:
	SDL_AudioSpec* _want; 
	SDL_AudioSpec* _have;
	SDL_AudioDeviceID _dev;
	bool _playing; 
	unsigned int _audioPos;  // which sample we are up to 
	float _audioLen;
	float _audioFreq;        // audio frequency in cycles per sample
	float _audioVol;         // audio volume, 0 - ~32000 */
	float _cycleTime;
	bool _initialized;

};
















}
