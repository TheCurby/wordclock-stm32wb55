#include "wordclock/Animation.hpp"

Animation::Animation() {
	bRunning = false;
	u8Dir = 0;
	u8DimmValue = 0xff;
	s16AnimationStep = 0;
	oContainerOutput = &oContainerDraw;
	CurrentAnimation = AnimationType::None;
}

Animation::Animation(Container& oContainerOld_l, Container& oContainerNew_l, AnimationType CurrentAnimation_l) {
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

	switch (CurrentAnimation) {
		default:
		break;

		case AnimationType::Move:
			u8Dir = rand() % 4;
			oContainerDraw = oContainerNew;

			switch (u8Dir) {
				default:
				case 0:
					oContainerDraw.moveY(-(HEIGHT + 1));
				break;

				case 1:
					oContainerDraw.moveX(WIDTH);
				break;

				case 2:
					oContainerDraw.moveY((HEIGHT + 1));
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
			oContainerDraw.moveY(-(HEIGHT + 1));
			oContainerDraw.combine(oContainerOld);
		break;

		case AnimationType::Snake:
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
			if (s16AnimationStep >= (HEIGHT + 1) && (u8Dir == 0 || u8Dir == 2)) {
				bRunning = false;
			}
		break;

		case AnimationType::Fade:
			if (oContainerOld.empty() && s16AnimationStep < 110) {
				s16AnimationStep = 110;
			}

			if (s16AnimationStep < 110) {
				oContainerDraw = oContainerOld;
				if (s16AnimationStep < 100) {
					u8DimmValue = 0xff - ((uint16_t) s16AnimationStep * 0xff / 99);
				}
				else {
					u8DimmValue = 0;
				}
			}
			else {
				oContainerDraw = oContainerNew;
				u8DimmValue = ((uint16_t) (s16AnimationStep - 110) * 0xff / 99);
			}

			oContainerOutput = &oContainerDraw;
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

				oContainerDraw.add(oContainerNew.remove(u8Rand));
				oContainerOutput = &oContainerDraw;
			}

			u16Time = 100;
			if (oContainerNew.empty()) {
				bRunning = false;
			}
		break;

		case AnimationType::Drop:
			for (int8_t i = oContainerDraw.size() - 1; i >= 0; i--) {
				Container::s_Point& oPoint = oContainerDraw.getRef(i);

				if (s16AnimationStep < (HEIGHT + 1)) {
					if (oPoint.u8X >= WIDTH - 1 - s16AnimationStep) {
						oPoint.u8Y++;
					}
				}
				else {
					if (oPoint.u8X < WIDTH - 1 - (s16AnimationStep - (HEIGHT + 1))) {
						oPoint.u8Y++;
					}
				}
			}

			if (++s16AnimationStep >= (HEIGHT + 1) + (WIDTH - 1)) {
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

				for (uint16_t j = 0; j < oContainerOld.size(); j++) {
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
			for (uint16_t j = 0; j < oContainerOld.size(); j++) {
				oContainerDraw.add(oContainerOld.get(j));
			}

			for (uint16_t i = oContainerMatrix.size(); i-- > 0;) {
				Container::s_Point oPointTmp = oContainerMatrix.get(i);

				for (uint8_t j = 0; j < 7; j++) {
					static const uint8_t MatrixFarben[2][7] = { {0x60, 0x30, 0x10, 0x05, 0x03, 0x02, 0x01}, {0x20, 0x10, 0x05, 0}};

					oPointTmp.oColors.set(0, MatrixFarben[0][j], 0, MatrixFarben[1][j]);
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

				if (oPointTmp.u8Y * WIDTH + oPointTmp.u8X > s16AnimationStep) {
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
			if(s16AnimationStep >= WIDTH * HEIGHT){
				oPoint.u8X += 3;
			}
			oContainerDraw.add(oPoint);

			oContainerOutput = &oContainerDraw;
			u16Time = 50;
			if (++s16AnimationStep >= LEDS) {
				bRunning = false;
			}
		}
		break;

		case AnimationType::Snake:
		break;

		case AnimationType::None:
		default:
			bRunning = false;
		break;
	}

	return u16Time;
}
