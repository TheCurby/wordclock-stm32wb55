////////////////////////////////////////////////////////////////////////////////
// Wordclock V1.1
// 
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
////////////////////////////////////////////////////////////////////////////////
#pragma once

#include <stdlib.h>
#include "wordclock/container.hpp"
#include "wordclock/colors.hpp"

class Animation {
	public:
		enum AnimationType : uint8_t {
			None,
#ifdef ENABLE_ANIMATION_FADE
			Fade,
#endif
#ifdef ENABLE_ANIMATION_PLOP
			Plop,
#endif
#ifdef ENABLE_ANIMATION_MOVE
			Move,
#endif
#ifdef ENABLE_ANIMATION_DROP
			Drop,
#endif
#ifdef ENABLE_ANIMATION_MATRIX
			Matrix,
#endif
#ifdef ENABLE_ANIMATION_TELETYPE
			Teletype,
#endif
#ifdef ENABLE_ANIMATION_SLIDER
			Slider,
#endif
#ifdef ENABLE_ANIMATION_HAZARD
			Hazard,
#endif
#ifdef ENABLE_ANIMATION_CLOCK
			Clock,
#endif
			Random,
#ifndef ENABLE_ANIMATION_FADE
			Fade,
#endif
#ifndef ENABLE_ANIMATION_PLOP
			Plop,
#endif
#ifndef ENABLE_ANIMATION_MOVE
			Move,
#endif
#ifndef ENABLE_ANIMATION_DROP
			Drop,
#endif
#ifndef ENABLE_ANIMATION_MATRIX
			Matrix,
#endif
#ifndef ENABLE_ANIMATION_TELETYPE
			Teletype,
#endif
#ifndef ENABLE_ANIMATION_SLIDER
			Slider,
#endif
#ifndef ENABLE_ANIMATION_HAZARD
			Hazard,
#endif
#ifndef ENABLE_ANIMATION_CLOCK
			Clock,
#endif
		};

		Animation();
		Animation(Container& oContainerOld_l, Container& oContainerNew_l, AnimationType CurrentAnimation_l, Colors & oColors_l, DisplayMode eMode_l = DisplayMode::Normal);
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
		Colors oColors;
		DisplayMode eMode;

		Container oContainerOld;
		Container oContainerNew;
		Container oContainerDraw;
		Container oContainerMatrix;
		Container* oContainerOutput;

		static constexpr int16_t u16Height = HEIGHT + 2;
		static constexpr int16_t u16Width = WIDTH + 2;
};
