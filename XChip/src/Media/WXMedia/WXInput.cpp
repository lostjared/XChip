
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <XChip/Media/WXMedia/WXInput.h>
#include <XChip/Utility/Log.h>
#include <XChip/Utility/ScopeExit.h>
#include <XChip/Utility/Assert.h>


namespace xchip {
using namespace utility;


WXInput::WXInput() noexcept
	: _keyPairs
	{
		{ Key::KEY_0, WXK_NUMPAD0},{ Key::KEY_1, WXK_NUMPAD1 },{ Key::KEY_2, WXK_NUMPAD2 },
		{ Key::KEY_3, WXK_NUMPAD3 },{ Key::KEY_4, WXK_NUMPAD4},{ Key::KEY_5, WXK_NUMPAD5 },
		{ Key::KEY_6, WXK_NUMPAD6 },{ Key::KEY_7, WXK_NUMPAD7 },{ Key::KEY_8, WXK_NUMPAD8 },
		{ Key::KEY_9, WXK_NUMPAD9 },{ Key::KEY_A, WXK_DIVIDE },{ Key::KEY_B, WXK_MULTIPLY },
		{ Key::KEY_C, WXK_SUBTRACT },{ Key::KEY_D, WXK_ADD },{ Key::KEY_E, WXK_DECIMAL},
		{ Key::KEY_F, WXK_NUMPAD_ENTER },{ Key::RESET, WXK_RETURN }, { Key::ESCAPE, WXK_ESCAPE}
	}

{
	
	
	
	LOG("Creating WXInput object...");
}


WXInput::~WXInput()
{
	//if (_keyboardState)
	//	this->Dispose();
	
	LOG("Destroying WXInput object...");
}




bool WXInput::Initialize() noexcept
{
	if (_initialized)
		this->Dispose();
	
		
		
		_initialized = true;
	return true;
}



void WXInput::Dispose() noexcept
{
	//_keyboardState = nullptr;
	_resetClbk = nullptr;
	_escapeClbk = nullptr;
	_waitClbk = nullptr;
	_initialized = false;
}


bool WXInput::UpdateKeys() noexcept
{
	if (wxGetKeyState(WXK_RETURN))
	{
		if (_resetClbk)
			_resetClbk(_resetClbkArg);
	}
	else if (wxGetKeyState(WXK_ESCAPE))
	{
		if (_escapeClbk)
			_escapeClbk(_escapeClbkArg);
			
	}
	return false;
}


bool WXInput::IsInitialized() const noexcept
{
	return _initialized;
}

bool WXInput::IsKeyPressed(const Key key) const noexcept
{
	return wxGetKeyState(_keyPairs[static_cast<size_t>(key)].second);
}


Key WXInput::GetPressedKey() const noexcept
{
	for (const auto& keyPair : _keyPairs)
	{
		if (wxGetKeyState(keyPair.second))
			return keyPair.first;
	}
	
	return Key::NO_KEY_PRESSED;
}



Key WXInput::WaitKeyPress() noexcept
{
	
	
	
	return Key::NO_KEY_PRESSED;
}



void WXInput::SetWaitKeyCallback(const void* arg, WaitKeyCallback callback) noexcept
{
	_waitClbkArg = arg;
	_waitClbk = callback;
}


void WXInput::SetResetKeyCallback(const void* arg, ResetKeyCallback callback) noexcept
{
	_resetClbkArg = arg;
	_resetClbk = callback;
}


void WXInput::SetEscapeKeyCallback(const void* arg, EscapeKeyCallback callback) noexcept
{
	_escapeClbkArg = arg;
	_escapeClbk = callback;
}














}