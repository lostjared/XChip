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


#include <Utix/Log.h>
#include <Utix/ScopeExit.h>
#include <Utix/Assert.h>

#include <XChip/Core/CpuManager.h>
#include <XChip/Plugins/iRender.h>
#include <XChip/Plugins/iInput.h>
#include <XChip/Plugins/iSound.h>

 
namespace xchip {

using namespace utix;

inline void set_plugin_flag(Cpu::Flags flag, const iPlugin* plugin, CpuManager& man);
template<class T>
inline bool alloc_cpu_arr(const size_t size, T*&);
template<class T>
inline bool realloc_cpu_arr(const size_t size, T*&);
template<class T>
inline void free_cpu_arr(T*& arr);




CpuManager::CpuManager() noexcept
{
	Log("Creating CpuManager object...");
	// init all members to 0
	memset(&_cpu, 0, sizeof(Cpu));
	SetFlags( Cpu::BAD_RENDER | Cpu::BAD_INPUT | Cpu::BAD_SOUND ); 
}



CpuManager::~CpuManager()
{
	this->Dispose();
	Log("Destroying CpuManager object...");
}




void CpuManager::Dispose() noexcept
{
	free_cpu_arr(_cpu.gfx);
	free_cpu_arr(_cpu.stack);
	free_cpu_arr(_cpu.registers);
	free_cpu_arr(_cpu.memory);
}


bool CpuManager::SetMemory(const size_t size)
{
	if ( !alloc_cpu_arr(size, _cpu.memory) ) 
	{
		LogError("Cannot allocate Cpu memory size: %zu", size);
		return false;
	}

	return true;
}



bool CpuManager::SetRegisters(const size_t size)
{
	if ( !alloc_cpu_arr(size, _cpu.registers) )  
	{
		LogError("Cannot allocate Cpu registers size: %zu", size);
		return false;
	}

	return true;
}


bool CpuManager::SetStack(const size_t size)
{
	if ( !alloc_cpu_arr(size, _cpu.stack) )
	{
		LogError("Cannot allocate Cpu stack size: %zu", size);
		return false;
	}

	return true;
}


bool CpuManager::SetGfxRes(const Vec2i& res)
{
	if ( !alloc_cpu_arr(res.x * res.y, _cpu.gfx) )
	{
		LogError("Cannot allocate Cpu memory size: %d", + res.x*res.y);
		_gfxRes = 0;
		return false;
	}

	_gfxRes = res;

	return true;
}



bool CpuManager::SetGfxRes(const int w, const int h)
{
	if ( !alloc_cpu_arr(w * h, _cpu.gfx) )
	{
		LogError("Cannot allocate Cpu memory size: %d", w*h);
		_gfxRes = 0;
		return false;
	}

	_gfxRes.x = w;
	_gfxRes.y = h;

	return true;
}





bool CpuManager::ResizeMemory(const std::size_t size)
{

	if ( !realloc_cpu_arr(size, _cpu.memory) ) 
	{
		LogError("Cannot reallocate Cpu memory to size: %zu", size);
		return false;
	}


	return true;
}



bool CpuManager::ResizeRegisters(const size_t size)
{
	if (!realloc_cpu_arr(size, _cpu.registers) )
	{
		LogError("Cannot reallocate Cpu registers to size: %zu",  size);
		return false;
	}

	return true;
}




bool CpuManager::ResizeStack(const size_t size)
{
	if (!realloc_cpu_arr(size, _cpu.stack) )
	{
		LogError("Cannot reallocate Cpu stack to size: %zu", size);
		return false;
	}

	return true;
}


void CpuManager::LoadDefaultFont()
{
	using namespace xchip::fonts;

	// default font is loaded right in the first memory bytes
	// default font : [0] -> [DEFAULT_FONT_SIZE - 1] 

	ASSERT_MSG(_cpu.memory != nullptr, "null Cpu::memory");
	ASSERT_MSG((arr_size(_cpu.memory) >= arr_size(chip8DefaultFont)), "Memory size is too low");

	memcpy(_cpu.memory, chip8DefaultFont, arr_size(chip8DefaultFont));
}

void CpuManager::LoadHiResFont()
{
	using namespace xchip::fonts;

	// hi res font is loaded right after the default font
	// default font : [0] -> [DEFAULT_FONT_SIZE - 1]
	// hi res font : [ DEFAULT_FONT_SIZE ] -> [ HI_RES_FONT_SIZE - 1 ]

	constexpr const auto at = arr_size(chip8DefaultFont); 
	
	ASSERT_MSG(_cpu.memory != nullptr, "null Cpu::memory");
	ASSERT_MSG( arr_size(_cpu.memory) >= ( at + arr_size(chip8HiResFont)), "Memory size is too low");
	ASSERT_MSG((at + arr_size(chip8HiResFont)) < 0x200, "Hi res font is over 0x200 memory area");

	memcpy(_cpu.memory + at, chip8HiResFont, arr_size(chip8HiResFont));
}


bool CpuManager::LoadRom(const char* fileName, const size_t at)
{
	// if the parameter 'at' is greater than _cpu.memory array,
	// or the _cpu.memory ptr is null. It is the caller's error,
	// so here's an assert for that purpose.

	ASSERT_MSG(_cpu.memory != nullptr, "null Cpu::memory");
	ASSERT_MSG(arr_size(_cpu.memory) > at, "parameter 'at' greater than Cpu::memory size");

	Log("Loading %s", fileName);

	auto *const file = std::fopen(fileName, "rb");

	if (!file)
	{
		LogError("Error opening ROM file \'%s\'", fileName);
		return false;
	}

	const auto fileClose = make_scope_exit([file]() noexcept { std::fclose(file); });

	// get file size
	std::fseek(file, 0, SEEK_END);
	const auto fileSize = static_cast<size_t>(std::ftell(file));
	std::fseek(file, 0, SEEK_SET);


	
	
	// check if file size will not overflow emulated memory size
	if ( (arr_size(_cpu.memory) - at) <= fileSize)
	{
		LogError("Error, size of \'%s\' does not fit in memory at %zu! memory size: %zu, file size: %zu", 
                           fileName, at, arr_size(_cpu.memory), fileSize);

		return false;
	}

	const auto readSize = std::fread(_cpu.memory + at, 1, fileSize, file);

	if( readSize != fileSize ) 
	{
		LogError("Could not read the file \'%s\' properly. bytes asked %zu , bytes read %zu", 
                          fileName, fileSize, readSize);
		return false;
	}

	Log("Load Done!");
	return true;
}




void CpuManager::SetRender(iRender* render) 
{
	set_plugin_flag(Cpu::BAD_RENDER, render, *this);
	_cpu.render = render; 
}


void CpuManager::SetInput(iInput* input) 
{
	set_plugin_flag(Cpu::BAD_INPUT, input, *this);
	_cpu.input = input; 
}


void CpuManager::SetSound(iSound* sound) 
{
	set_plugin_flag(Cpu::BAD_SOUND, sound, *this);
	_cpu.sound = sound; 
}



iRender* CpuManager::SwapRender(iRender* render)
{
	ASSERT_MSG(render != _cpu.render, "trying to swap the same addresses");
	auto* const ret = _cpu.render;
	SetRender(render);
	return ret;
}



iInput* CpuManager::SwapInput(iInput* input)
{
	ASSERT_MSG(input != _cpu.input, "trying to swap the same addresses");
	auto* const ret = _cpu.input;
	SetInput(input);
	return ret;
}



iSound* CpuManager::SwapSound(iSound* sound)
{
	ASSERT_MSG(sound != _cpu.sound, "trying to swap the same addresses");
	auto* const ret = _cpu.sound;
	SetSound(sound);
	return ret;
}












// local static functions
inline bool __alloc_arr(const size_t size, void*& arr);
inline bool __realloc_arr(const size_t size, void*& arr);

inline void set_plugin_flag(Cpu::Flags flag, const iPlugin* plugin, CpuManager& man)
{
	if (!plugin)
		man.SetFlags(flag);
	else if (!plugin->IsInitialized())
		man.SetFlags(flag);
	else
		man.UnsetFlags(flag);
}





template<class T>
inline bool alloc_cpu_arr(const size_t size, T*& arr)
{
	if (arr != nullptr)
	{
		if (size == arr_size(arr))
			return true;
		
		else
			free_arr(arr);
	}

	return __alloc_arr(sizeof(T) * size, reinterpret_cast<void*&>(arr));
}



template<class T>
inline bool realloc_cpu_arr(const size_t size, T*& arr)
{
	if (arr != nullptr)
	{
		if(size == arr_size(arr))
			return true;
		
		else
			return __realloc_arr(sizeof(T) * size, reinterpret_cast<void*&>(arr));
	}	

	return __alloc_arr(sizeof(T) * size, reinterpret_cast<void*&>(arr));	
}



template<class T>
inline void free_cpu_arr(T*& arr)
{
	if(arr != nullptr)
	{
		free_arr(arr);
		arr = nullptr;
	}

}



inline bool __alloc_arr(const size_t size, void*& arr)
{
	arr = alloc_arr(size);
	
	if (arr)
	{
		arr_zero(arr, size);
		return true;
	}

	return false;
}




inline bool __realloc_arr(const size_t size, void*& arr)
{
	arr = realloc_arr(arr, size);
	return arr != nullptr;
}

























}
