#ifndef TIMER_H
#define TIMER_H
#include <chrono>



namespace xchip { namespace utility {

namespace literals {
	inline std::chrono::microseconds operator""_sec(unsigned long long x) { return std::chrono::seconds(x); }
	inline std::chrono::microseconds operator""_milli(unsigned long long x) { return std::chrono::milliseconds(x); }
	inline std::chrono::microseconds operator""_micro(unsigned long long x) { return std::chrono::microseconds(x); }
	inline std::chrono::nanoseconds operator""_nano(unsigned long long x) { return std::chrono::nanoseconds(x); }
	inline std::chrono::microseconds operator""_hz(unsigned long long x) { return 1_sec / x; }
}


class Timer
{
public:
	using Micro = std::chrono::microseconds;
	using Nano = std::chrono::nanoseconds;
	using Duration = std::chrono::duration<long long, std::nano>;

public:
	Timer() noexcept = default;
	Timer(const Micro& target) noexcept;

	const Micro GetTarget() const;
	Duration GetRemain() const;
	bool Finished() const;
	void SetTargetTime(Micro target);
	void Start();
	static void Halt(const Nano& nano);
private:
	std::chrono::steady_clock::time_point m_startPoint = std::chrono::steady_clock::now();
	Micro m_target;
};


inline Timer::Timer(const Micro& target) noexcept : 
	m_target(target) {}

inline const Timer::Micro Timer::GetTarget() const { return m_target; }
inline void Timer::SetTargetTime(Micro target) { m_target = target; }
inline void Timer::Start() { m_startPoint = std::chrono::steady_clock::now(); }



}}


#endif
