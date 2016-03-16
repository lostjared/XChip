#ifndef RESOLUTION_T_H
#define RESOLUTION_T_H



struct resolution_t
{

	inline resolution_t() noexcept;
	inline resolution_t(const int new_x, const int new_y) noexcept;

	inline void set(const int newX, const int newY) noexcept;
	inline operator size_t() const noexcept;


	int x, y;
	size_t total;

};


inline resolution_t::resolution_t() noexcept 
	: x(0), y(0), total(0)
{

}

inline resolution_t::resolution_t(const int new_x, const int new_y) noexcept 
	: x(new_x), y(new_y), total(x*y)
{

}


inline void resolution_t::set(const int new_x, const int new_y) noexcept
{
	x = new_x;
	y = new_y;
	total = x * y;
}

inline resolution_t::operator size_t() const noexcept
{
	return total;
}






#endif