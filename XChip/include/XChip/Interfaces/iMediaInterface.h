#ifndef _XCHIP_IMEDIAINTERFACE_H_
#define _XCHIP_IMEDIAINTERFACE_H_


namespace xchip {



class iMediaInterface
{
public:
	virtual ~iMediaInterface() = default;
	iMediaInterface& operator=(const iMediaInterface&) = delete;
	virtual void Dispose() noexcept = 0;
	virtual bool IsInitialized() const = 0;
};




}






#endif
