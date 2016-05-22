/*

XChip - A chip8 lib and emulator.
Copyright (C) 2016  Rafael Moura

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see http://www.gnu.org/licenses/gpl-3.0.html.

*/

#ifndef _XCHIP_ISOUND_H_
#define _XCHIP_ISOUND_H_
#include "iPlugin.h"
#include "../Utility/StdintDef.h"

 
namespace xchip {



class iSound : public iPlugin
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
