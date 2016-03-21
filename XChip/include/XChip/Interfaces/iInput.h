#ifndef _XCHIP_IINPUT_H_
#define _XCHIP_IINPUT_H_
#include <cstdint>

namespace xchip {

enum class Key : uint8_t
{
	//Chip8 keypad, which will be emulated in the computer's keypad
	KEY_0,
	KEY_1, KEY_2, KEY_3,
	KEY_4, KEY_5, KEY_6,
	KEY_7, KEY_8, KEY_9,
	KEY_A, KEY_B, KEY_C,
	KEY_D, KEY_E, KEY_F,


	// system keys
	RESET,
	ESCAPE,

	// does not count as a key, but is returned if none of the others are pressed
	NO_KEY_PRESSED
};


class iInput
{
public:
	using WaitKeyCallback = bool(*)(const void*);
	using ResetKeyCallback = void(*)(const void*);
	using EscapeKeyCallback = void(*)(const void*);
	
	virtual ~iInput() = default;
	virtual bool Initialize() noexcept = 0;
	virtual void Dispose() noexcept = 0;
	
	virtual Key WaitKeyPress() = 0;
	virtual Key GetPressedKey() const = 0;
	virtual bool IsKeyPressed(const Key key) const = 0;
	virtual bool UpdateKeys() = 0;
	
	virtual void SetWaitKeyCallback(WaitKeyCallback callback, const void* arg) = 0;
	virtual void SetResetKeyCallback(ResetKeyCallback callback, const void* arg) = 0;
	virtual void SetEscapeKeyCallback(EscapeKeyCallback callback, const void* arg) = 0;
};















}

#endif
