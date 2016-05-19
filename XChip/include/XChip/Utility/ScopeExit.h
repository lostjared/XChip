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

#ifndef _XCHIP_SCOPE_EXIT_H_
#define _XCHIP_SCOPE_EXIT_H_
#include <utility>


namespace xchip { namespace utility {




template<class F>
struct ScopeExit
{
	constexpr ScopeExit(F fun) noexcept : _fun(std::move(fun)) {
		static_assert(noexcept(fun()) == true, "ScopeExit functor must be noexcept!");
	}
	~ScopeExit() noexcept { _fun(); }
	ScopeExit(ScopeExit&& rhs) noexcept = default;
	ScopeExit(const ScopeExit&) = delete;
	ScopeExit& operator=(const ScopeExit&) = delete;
private:
	F _fun;
};


template<class T>
constexpr inline ScopeExit<T> make_scope_exit(T&& t) noexcept {
	return ScopeExit<T>(std::forward<T>(t));
}






}}









#endif
