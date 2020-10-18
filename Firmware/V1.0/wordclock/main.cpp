#include "wordclock/wordclock.hpp"

extern "C" void TIM2_IRQHandler() {
	TIM2->SR &= ~TIM_SR_UIF;

	Timer::incOverflow();
}

extern "C" int main(void) {
	static Wordclock wordclock;

	while (true) {
		wordclock.loop();
	}

	return 0;
}
