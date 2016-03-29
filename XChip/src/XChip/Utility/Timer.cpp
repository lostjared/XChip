#include <XChip/Utility/Timer.h>


#ifdef __linux__

#include <ctime>

#elif _WIN32

#include <Windows.h>

#endif

namespace xchip { namespace utility {


Timer::Duration Timer::GetRemain() const
{
	using namespace std::chrono;
	auto passedTime = duration_cast<Duration>(steady_clock::now() - m_startPoint);
	return passedTime < m_target ? (m_target - passedTime) : Duration(0);
}


bool Timer::Finished() const
{
	return ((std::chrono::steady_clock::now() - m_startPoint) > m_target);
}




void Timer::Halt(const Timer::Nano& nano)
{
	using namespace std::chrono;
	using namespace literals;
	/* high precision sleep on linux */

#ifdef __linux__
	static timespec _sleep{ 0, 0 };
	_sleep.tv_nsec = (nano - 65000_nano).count();
	nanosleep(&_sleep, NULL);

#elif _WIN32
	Sleep(static_cast<DWORD>(duration_cast<milliseconds>(nano).count()));

#endif
}








}}
