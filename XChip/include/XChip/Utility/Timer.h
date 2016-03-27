#ifndef TIMER_H
#define TIMER_H
#include <chrono>


namespace xchip { namespace utility {


constexpr std::chrono::microseconds operator""_sec(unsigned long long x) { return std::chrono::seconds(x); }

constexpr std::chrono::microseconds operator""_milli(unsigned long long x) { return std::chrono::milliseconds(x); }

constexpr std::chrono::microseconds operator""_micro(unsigned long long x) { return std::chrono::microseconds(x); }

constexpr std::chrono::nanoseconds operator""_nano(unsigned long long x) { return std::chrono::nanoseconds(x); }

constexpr std::chrono::microseconds operator""_hz(unsigned long long x) { return 1_sec / x; }


class Timer
{
public:
	using Micro = std::chrono::microseconds;
	using Nano = std::chrono::nanoseconds;
	using Duration = std::chrono::duration<long long, std::nano>;

public:
	Timer() noexcept = default;
	Timer(Micro target) noexcept : m_target(target) {}

	const Micro GetTarget() const { return m_target; }
	Duration GetRemain() const;
	bool Finished() const;
	void SetTargetTime(Micro target) { m_target = target; }
	void Start() { m_startPoint = std::chrono::steady_clock::now(); }

	static void Halt(const Nano& nano);
private:
	std::chrono::steady_clock::time_point m_startPoint = std::chrono::steady_clock::now();
	Micro m_target;
};








} }


#endif
