#ifndef _XCHIP_ISOUND_H_
#define _XCHIP_ISOUND_H_
#include "iMediaInterface.h"
#include "../Utility/StdintDef.h"

 
namespace xchip {



class iSound : public iMediaInterface
{
public:
	virtual bool Initialize() noexcept = 0;
	virtual bool IsPlaying() const noexcept = 0;
	virtual float GetCountdownFreq() const noexcept = 0;
	virtual float GetSoundFreq() const noexcept = 0;
	virtual void SetCountdownFreq(const float hz) noexcept = 0;
	virtual void SetSoundFreq(const float hz) noexcept = 0;
	virtual void Play(const uint8_t soundTimer) noexcept = 0;
	virtual void Stop() noexcept = 0;


};









}










#endif
