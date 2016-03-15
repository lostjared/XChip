#ifndef TIMER_H
#define TIMER_H
#include <chrono>


#ifdef __linux__

#include <ctime>

#elif _WIN32

#include <Windows.h>

#endif

constexpr std::chrono::microseconds operator""_sec(unsigned long long x) {
	return std::chrono::seconds(x); 
}

constexpr std::chrono::microseconds operator""_milli(unsigned long long x) {
	return std::chrono::milliseconds(x);
}

constexpr std::chrono::microseconds operator""_micro(unsigned long long x) { 
	return std::chrono::microseconds(x); 
}

constexpr std::chrono::nanoseconds operator""_nano(unsigned long long x) {
	return std::chrono::nanoseconds(x);
}

constexpr std::chrono::microseconds operator""_hz(unsigned long long x) {
	return 1_sec / x;
}


class Timer
{
public:
	using Micro = std::chrono::microseconds;
	using Nano = std::chrono::nanoseconds;
	using Duration = std::chrono::duration<long long, std::nano>;

public:
	Timer() noexcept = default;
	Timer(Micro target) noexcept;

	const Micro GetTarget() const;
	Duration GetRemain() const;
	bool Finished() const;
	void SetTargetTime(Micro target);
	void Start();

	static void Halt(Nano nano);
private:
	std::chrono::steady_clock::time_point m_startPoint = std::chrono::steady_clock::now();
	Micro m_target;
};


inline Timer::Timer(Micro target) noexcept
	: m_target(target)
{

}

inline const Timer::Micro Timer::GetTarget() const
{
	return m_target;
}

inline Timer::Duration Timer::GetRemain() const
{
	using namespace std::chrono;
	auto passedTime = duration_cast<Duration>(steady_clock::now() - m_startPoint);
	return passedTime < m_target ? (m_target - passedTime) : Duration(0);
}


inline void Timer::SetTargetTime(Micro target)
{
	m_target = target;
}


inline bool Timer::Finished() const
{
	return (( std::chrono::steady_clock::now() - m_startPoint ) > m_target );
}


inline void Timer::Start()
{
	m_startPoint = std::chrono::steady_clock::now();
}




inline void Timer::Halt(Timer::Nano nano)
{
	using namespace std::chrono;
	/* high precision sleep on linux */

#ifdef __linux__
	static timespec _sleep{ 0, 0 };
		_sleep.tv_nsec = (nano - 65000_nano).count();
		nanosleep(&_sleep, NULL);

#elif _WIN32
	Sleep(static_cast<DWORD>(duration_cast<milliseconds>(nano).count()));

#endif
}












#endif
