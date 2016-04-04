#ifndef _XCHIP_TRAITS_H_
#define _XCHIP_TRAITS_H_
#include <type_traits>
#include <utility>
namespace xchip { namespace utility {





template<bool cond, class T>
using enable_if_t = typename std::enable_if<cond, T>::type;
	
template<class T>
using underlying_type_t = typename std::underlying_type<T>::type;


template<class T>
underlying_type_t<T> toUType(T t) noexcept
{
	return static_cast<underlying_type_t<T>>(t);
}



template<class T, size_t sz>
size_t static_arr_size(const T(&)[sz])
{
	return sz;
}



template<class F>
struct Scope_Exit
{
	Scope_Exit(F fun) noexcept : _fun(std::move(fun)) {}
	~Scope_Exit() { _fun(); }
	Scope_Exit(Scope_Exit&&) = default;
	Scope_Exit(const Scope_Exit&) = delete;
	Scope_Exit& operator=(const Scope_Exit&) = delete;
private:
	F _fun;
};

template<class T>
Scope_Exit<T> make_scope_exit(T&& t) {
	return Scope_Exit<T>(std::forward<T>(t));
}



}}
#endif
