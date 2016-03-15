#ifndef IINPUT_H
#define IINPUT_H
#include <functional>

// system constants
#define MAX_KEYS 18
enum class EmulatorKey : uint8_t
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
	using WaitKeyPressCallback = bool(*)(void*);
	using ResetCallback = void(*)(void*);
	using EscapeCallback = void(*)(void*);


	virtual ~iInput() = default;
	virtual bool Initialize() = 0;
	virtual void Dispose() noexcept = 0;
	virtual bool IsKeyPressed(const EmulatorKey key) const = 0;
	
	// return the current EmulatorKey being Pressed, or NO_KEY_PRESSED..
	virtual EmulatorKey GetPressedKey() const = 0;
	
	// return true if there is a key press, store the key in m_currentKey
	virtual bool UpdateKeys() = 0;   

	// loop until there is a key press or until callback returns false
	virtual EmulatorKey WaitKeyPress() = 0;
	
	// set a new value to an emulator key
	virtual void SetKey(EmulatorKey key, int value) = 0;

	virtual void SetWaitKeyPressCallback(void* arg, WaitKeyPressCallback callback) = 0;
	virtual void SetResetCallback(void* arg, ResetCallback callback) = 0;
	virtual void SetEscapeCallback(void* arg, EscapeCallback callback) = 0;
};


#endif // IINPUT

