#ifndef _XCHIP_IMEDIAINTERFACE_H_
#define _XCHIP_IMEDIAINTERFACE_H_



namespace xchip {




class iMediaInterface
{
public:
	iMediaInterface() noexcept = default;
	virtual ~iMediaInterface() = default;
	virtual void Dispose() noexcept = 0;
	virtual bool IsInitialized() const noexcept = 0;
};





}






#endif
