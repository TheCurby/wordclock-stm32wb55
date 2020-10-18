#pragma once

#include <stdlib.h>
#include "wordclock/Container.hpp"
#include "wordclock/Colors.hpp"

class Animation {
	public:
		enum AnimationType : uint8_t {
			None,
			Fade,
			Plop,
			Move,
			Drop,
			Matrix,
			Teletype,
			Random,
			Snake,
		};

		Animation();
		Animation(Container& oContainerOld_l, Container& oContainerNew_l, AnimationType CurrentAnimation_l);
		void stop();
		bool running();
		uint16_t run();
		Container& get();
		uint8_t getDimmValue();

	private:
		bool bRunning;
		uint8_t u8Dir;
		uint8_t u8DimmValue;
		int16_t s16AnimationStep;

		AnimationType CurrentAnimation;
		Container oContainerOld;
		Container oContainerNew;
		Container oContainerDraw;
		Container oContainerMatrix;
		Container* oContainerOutput;
};
