#ifndef _XCHIP_UTILITY_TRAITS_H_
#define _XCHIP_UTILITY_TRAITS_H_
#include <type_traits>
#include <utility>

 
namespace xchip { namespace utility {


template<bool cond, class T>
using enable_if_t = typename std::enable_if<cond, T>::type;
	
template<class T>
using underlying_type_t = typename std::underlying_type<T>::type;


template<class T>
constexpr underlying_type_t<T> toUType(T t) noexcept
{
	return static_cast<underlying_type_t<T>>(t);
}



template<class T, const size_t sz>
constexpr size_t static_arr_size(const T(&)[sz]) noexcept
{
	return sz;
}



template<class F>
struct ScopeExit
{
	constexpr ScopeExit(F fun) noexcept : _fun(std::move(fun)) {}
	~ScopeExit() noexcept { _fun(); }
	ScopeExit(ScopeExit&&) noexcept = default;
	ScopeExit(const ScopeExit&) = delete;
	ScopeExit& operator=(const ScopeExit&) = delete;
private:
	const F _fun;
};


template<class T>
constexpr ScopeExit<T> make_scope_exit(T&& t) noexcept {
	return ScopeExit<T>(std::forward<T>(t));
}










}}
#endif
