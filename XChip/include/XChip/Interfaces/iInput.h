#ifndef _XCHIP_IINPUT_H_
#define _XCHIP_IINPUT_H_


namespace xchip {




class iInput
{
public:
	virtual ~iInput() = default;
	virtual bool Initialize() = 0;
	virtual bool Dispose() = 0;

	virtual bool UpdateKeys() = 0;










};















}

#endif
