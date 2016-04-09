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
