#ifndef _XCHIP_ISOUND_H_
#define _XCHIP_ISOUND_H_

namespace xchip {


class iSound
{
public:
	virtual ~iSound() = default;
	virtual bool Initialize() noexcept = 0;
	virtual void Dispose() noexcept = 0;
	virtual bool IsInitialized() = 0;
	virtual bool IsPlaying() = 0;


	virtual void Play(unsigned soundTimer) = 0;
	virtual void Stop() = 0;


};









}










#endif
