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
#include "wordclock/animation.hpp"

static void drawHLine(Container& oContainer, uint8_t x) {
	Container::s_Point oPoint;
	oPoint.u8X = x;
	oPoint.oColors.setWhiteOnly(0x80);

	for (uint8_t i = 0; i < HEIGHT; i++) {
		oPoint.u8Y = i;
		oContainer.add(oPoint);
	}
}

static void drawVLine(Container& oContainer, uint8_t y) {
	Container::s_Point oPoint;
	oPoint.u8Y = y;
	oPoint.oColors.setWhiteOnly(0x80);

	for (uint8_t i = 0; i < WIDTH; i++) {
		oPoint.u8X = i;
		oContainer.add(oPoint);
	}
}

Animation::Animation() {
	bRunning = false;
	u8Dir = 0;
	u8DimmValue = 0xff;
	s16AnimationStep = 0;
	oContainerOutput = &oContainerDraw;
	CurrentAnimation = AnimationType::None;
}

Animation::Animation(Container& oContainerOld_l, Container& oContainerNew_l, AnimationType CurrentAnimation_l, Colors& oColors_l, DisplayMode eMode_l) {
	if (oContainerNew_l.size() > 0) {
		bRunning = true;
	}
	else {
		bRunning = false;
	}
	s16AnimationStep = 0;
	u8DimmValue = 0xff;
	CurrentAnimation = CurrentAnimation_l;

	oContainerOld = oContainerOld_l;
	oContainerNew = oContainerNew_l;
	oContainerMatrix.clear();
	oContainerDraw.clear();
	oContainerOutput = &oContainerDraw;

	oColors = oColors_l;
	eMode = eMode_l;

	switch (CurrentAnimation) {
		default:
		break;

		case AnimationType::Move:
			u8Dir = rand() % 4;
			oContainerDraw = oContainerNew;

			switch (u8Dir) {
				default:
				case 0:
					oContainerDraw.moveY(-(HEIGHT));
				break;

				case 1:
					oContainerDraw.moveX(WIDTH);
				break;

				case 2:
					oContainerDraw.moveY((HEIGHT));
				break;

				case 3:
					oContainerDraw.moveX(-WIDTH);
				break;
			}

			oContainerDraw.combine(oContainerOld);
		break;

		case AnimationType::Drop:
			oContainerDraw.clear();
			oContainerDraw.combine(oContainerNew);
			oContainerDraw.moveY(-(HEIGHT));
			oContainerDraw.combine(oContainerOld);
		break;

		case AnimationType::Slider:
			u8Dir = rand() % 4;
		break;

		case AnimationType::Hazard:
			oContainerDraw = oContainerNew;
		break;
	}
}

void Animation::stop() {
	bRunning = false;
}

bool Animation::running() {
	return (bRunning);
}

Container& Animation::get() {
	return *oContainerOutput;
}

uint8_t Animation::getDimmValue() {
	return u8DimmValue;
}

uint16_t Animation::run() {
	uint16_t u16Time = 0;

	switch (CurrentAnimation) {
		case AnimationType::Move:
			switch (u8Dir) {
				default:
				case 0:
					oContainerDraw.moveY(1);
				break;

				case 1:
					oContainerDraw.moveX(-1);
				break;

				case 2:
					oContainerDraw.moveY(-1);
				break;

				case 3:
					oContainerDraw.moveX(1);
				break;
			}

			oContainerOutput = &oContainerDraw;
			u16Time = 100;

			++s16AnimationStep;
			if (s16AnimationStep >= WIDTH && (u8Dir == 1 || u8Dir == 3)) {
				bRunning = false;
			}
			if (s16AnimationStep >= (HEIGHT) && (u8Dir == 0 || u8Dir == 2)) {
				bRunning = false;
			}
		break;

		case AnimationType::Fade:
			if (oContainerOld.empty() && s16AnimationStep < 110) {
				s16AnimationStep = 110;
			}

			if (s16AnimationStep < 110) {
				oContainerOutput = &oContainerOld;
				if (s16AnimationStep < 100) {
					u8DimmValue = 0xff - ((uint16_t) s16AnimationStep * 0xff / 99);
				}
				else {
					u8DimmValue = 0;
				}
			}
			else {
				oContainerOutput = &oContainerNew;
				u8DimmValue = ((uint16_t) (s16AnimationStep - 110) * 0xff / 99);
			}

			u16Time = 10;

			if (++s16AnimationStep >= 210) {
				bRunning = false;
			}
		break;

		case AnimationType::Plop:
			if (!oContainerOld.empty()) {
				uint8_t u8Rand = rand() % oContainerOld.size();

				oContainerOld.remove(u8Rand);
				oContainerOutput = &oContainerOld;
			}
			else {
				uint8_t u8Rand = rand() % oContainerNew.size();

				Container::s_Point oPoint = oContainerNew.remove(u8Rand);
				oContainerDraw.add(oPoint);
				oContainerOutput = &oContainerDraw;
			}

			u16Time = 100;
			if (oContainerNew.empty()) {
				bRunning = false;
			}
		break;

		case AnimationType::Drop:
			for (uint8_t i = oContainerDraw.size(); i-- > 0;) {
				Container::s_Point& oPoint = oContainerDraw.getRef(i);

				if (s16AnimationStep < (HEIGHT)) {
					if (oPoint.u8X >= WIDTH - 1 - s16AnimationStep) {
						oPoint.u8Y++;
					}
				}
				else {
					if (oPoint.u8X < WIDTH - 1 - (s16AnimationStep - (HEIGHT))) {
						oPoint.u8Y++;
					}
				}
			}

			if (++s16AnimationStep >= (HEIGHT) + (WIDTH - 1)) {
				bRunning = false;
			}

			oContainerOutput = &oContainerDraw;
			u16Time = 100;
		break;

		case AnimationType::Matrix: {
			oContainerMatrix.moveY(1);

			if (s16AnimationStep != 0b11111111111 && rand() % 2 == 0) {
				Container::s_Point oPoint;
				oPoint.u8Y = 0;
				oPoint.u8X = rand() % WIDTH;
				oPoint.oColors = {0, 0x20, 0, 0x20};
				oContainerMatrix.add(oPoint);
			}

			for (uint16_t i = 0; i < oContainerMatrix.size(); i++) {
				Container::s_Point& oPointTmp1 = oContainerMatrix.getRef(i);

				for (uint16_t j = oContainerOld.size(); j-- > 0;) {
					Container::s_Point& oPointTmp2 = oContainerOld.getRef(j);
					if (oPointTmp1.u8X == oPointTmp2.u8X && oPointTmp1.u8Y == oPointTmp2.u8Y) {
						oContainerOld.remove(j);
					}
				}

				for (uint16_t j = 0; j < oContainerNew.size(); j++) {
					Container::s_Point& oPointTmp2 = oContainerNew.getRef(j);
					if (oPointTmp1.u8X == oPointTmp2.u8X && oPointTmp1.u8Y == oPointTmp2.u8Y) {
						oContainerOld.add(oPointTmp2);
					}
				}
			}

			oContainerDraw.clear();
			oContainerDraw.combine(oContainerOld);

			for (uint16_t i = oContainerMatrix.size(); i-- > 0;) {
				Container::s_Point oPointTmp = oContainerMatrix.get(i);

				for (uint8_t j = 0; j < 7; j++) {
#ifndef _SIMULATOR_
					static const uint8_t MatrixColors[2][7] = { {0x60, 0x30, 0x10, 0x05, 0x03, 0x02, 0x01}, {0x20, 0x10, 0x05, 0}};
#else
					static const uint8_t MatrixColors[2][7] = { {0xff, 0xa0, 0x60, 0x30, 0x20, 0x10, 0x05}, {0x80, 0x50, 0x20, 0}};
#endif
					oPointTmp.oColors.set(0, MatrixColors[0][j], 0, MatrixColors[1][j]);
					oContainerDraw.add(oPointTmp);
					oPointTmp.u8Y--;
				}

				if (oPointTmp.u8Y >= HEIGHT + 7) {
					oContainerMatrix.remove(i);
					s16AnimationStep |= (1 << oPointTmp.u8X);
				}
			}

			oContainerOutput = &oContainerDraw;

			u16Time = 80;
			if (s16AnimationStep == 0b11111111111 && oContainerMatrix.empty()) {
				bRunning = false;
			}
		}
		break;

		case AnimationType::Teletype: {
			oContainerDraw.clear();

			for (uint16_t j = 0; j < oContainerOld.size(); j++) {
				Container::s_Point& oPointTmp = oContainerOld.getRef(j);

				if (oPointTmp.u8Y * WIDTH + oPointTmp.u8X > s16AnimationStep || (oPointTmp.u8Y < 0)) {
					oContainerDraw.add(oPointTmp);
				}
			}

			for (uint16_t j = 0; j < oContainerNew.size(); j++) {
				Container::s_Point& oPointTmp = oContainerNew.getRef(j);

				if (oPointTmp.u8Y * WIDTH + oPointTmp.u8X < s16AnimationStep) {
					oContainerDraw.add(oPointTmp);
				}
			}

			Container::s_Point oPoint;
			oPoint.oColors.setWhiteOnly(0x40);
			oPoint.u8Y = s16AnimationStep / WIDTH;
			oPoint.u8X = s16AnimationStep % WIDTH;
			oContainerDraw.add(oPoint);

			oContainerOutput = &oContainerDraw;
			u16Time = 50;
			if (++s16AnimationStep >= WIDTH * HEIGHT) {
				bRunning = false;
			}
		}
		break;

		case AnimationType::Slider:
			oContainerDraw.clear();

			for (uint16_t j = 0; j < oContainerNew.size(); j++) {
				Container::s_Point& oPointTmp = oContainerNew.getRef(j);

				if ((oPointTmp.u8X < s16AnimationStep && u8Dir == 0) || (oPointTmp.u8X > (WIDTH - s16AnimationStep - 1) && u8Dir == 1)
					|| (oPointTmp.u8Y < s16AnimationStep && u8Dir == 2) || (oPointTmp.u8Y > (HEIGHT - s16AnimationStep - 1) && u8Dir == 3)) {
					oContainerDraw.add(oPointTmp);
				}
			}

			for (uint16_t j = 0; j < oContainerOld.size(); j++) {
				Container::s_Point& oPointTmp = oContainerOld.getRef(j);

				if ((oPointTmp.u8X > s16AnimationStep && u8Dir == 0) || (oPointTmp.u8X < (WIDTH - s16AnimationStep - 1) && u8Dir == 1)
					|| (oPointTmp.u8Y > s16AnimationStep && u8Dir == 2) || (oPointTmp.u8Y < (HEIGHT - s16AnimationStep - 1) && u8Dir == 3)) {
					oContainerDraw.add(oPointTmp);
				}
			}

			switch (u8Dir) {
				default:
				case 0:
					drawHLine(oContainerDraw, s16AnimationStep);
				break;
				case 1:
					drawHLine(oContainerDraw, WIDTH - s16AnimationStep - 1);
				break;
				case 2:
					drawVLine(oContainerDraw, s16AnimationStep);
				break;
				case 3:
					drawVLine(oContainerDraw, HEIGHT - s16AnimationStep - 1);
				break;
			}

			oContainerOutput = &oContainerDraw;
			u16Time = 100;
			++s16AnimationStep;
			if (s16AnimationStep >= WIDTH && (u8Dir == 0 || u8Dir == 1)) {
				bRunning = false;
			}
			if (s16AnimationStep >= HEIGHT && (u8Dir == 2 || u8Dir == 3)) {
				bRunning = false;
			}
		break;

		case AnimationType::Hazard: {
			uint8_t u8Mode = rand() % 5;
			Container::s_Point oPointTmp;

			if (oContainerNew.empty() || oContainerOld.size() >= WIDTH * HEIGHT) {
				u8Mode = 0;
			}
			else if (oContainerOld.size() < LEDS) {
				u8Mode = 1 + rand() % 4;
			}

			switch (u8Mode) {
				default:
				case 0: {
					uint16_t u16Remove;
					int16_t u16Counter = 0;
					do {
						u16Remove = rand() % oContainerOld.size();
						oPointTmp = oContainerOld.get(u16Remove);

						if (++u16Counter > 400) {
							u16Remove = 0xffff;
							break;
						}
					} while (oContainerDraw.contains(oPointTmp, true) != -1);
					oContainerOld.remove(u16Remove);
				}
				break;

				case 1:
				case 2:
				case 3:
					oPointTmp.oColors = oColors;
					oPointTmp.oColors.setRandom(eMode);
					do {
						oPointTmp.u8X = rand() % WIDTH;
						oPointTmp.u8Y = rand() % HEIGHT;
					} while (oContainerOld.contains(oPointTmp) != -1);
					oContainerOld.add(oPointTmp);
				break;

				case 4: {
					int8_t s8Pos;
					oPointTmp = oContainerNew.remove(rand() % oContainerNew.size());
					s8Pos = oContainerOld.contains(oPointTmp);
					if (s8Pos > -1) {
						oContainerOld.remove(s8Pos);
					}
					oContainerOld.add(oPointTmp);
				}
				break;
			}

			oContainerOutput = &oContainerOld;
			u16Time = 40;
			if (oContainerNew.empty() && oContainerOld.size() == oContainerDraw.size()) {
				bRunning = false;
			}
		}
		break;

		case AnimationType::Clock:
			oContainerOutput = &oContainerDraw;
			u16Time = 100;
			++s16AnimationStep;
		break;

		case AnimationType::None:
		default:
			bRunning = false;
		break;
	}

	return u16Time;
}
