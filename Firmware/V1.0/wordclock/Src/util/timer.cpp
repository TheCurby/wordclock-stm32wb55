#include "util/timer.hpp"

std::atomic_uint32_t Timer::u32OverFlow {0};

void Timer::incOverflow() {
	u32OverFlow += 1;
}

uint64_t Timer::getTimestamp() {
	return TIM2->CNT + ((uint64_t) u32OverFlow << 32);
}

Timer::Timer() {
	Stop();
}

Timer::Timer(uint64_t u32Ms) {
	StartMs(u32Ms);
}

void Timer::StartUs(uint64_t u32Ms) {
	bRunning = true;
	u64Start = getTimestamp();
	u64Time = u32Ms;
}

void Timer::StartMs(uint64_t u32Ms) {
	StartUs(u32Ms * 1000);
}

void Timer::StartS(uint64_t u32S) {
	StartUs(u32S * 1000000);
}

void Timer::Restart() {
	StartUs(u64Time);
}

void Timer::Stop() {
	bRunning = false;
}

bool Timer::Ready() {
	if (!bRunning) {
		return true;
	}
	else if (getTimestamp() - u64Start >= u64Time) {
		bRunning = false;
		return true;
	}
	else {
		return false;
	}
}

bool Timer::ReadyEdge() {
	if (getTimestamp() - u64Start >= u64Time && bRunning) {
		bRunning = false;
		return true;
	}
	else {
		return false;
	}
}

uint64_t Timer::ElapsedMs() {
	return ElapsedUs() / 1000;
}

uint64_t Timer::ElapsedUs() {
	Ready();

	if (!bRunning)
		return u64Time;
	else {
		return getTimestamp() - u64Start;
	}
}

uint64_t Timer::RestMs() {
	return RestUs() / 1000;
}

uint64_t Timer::RestUs() {
	if (!bRunning)
		return 0;
	else {
		return u64Time - ElapsedUs();
	}
}
