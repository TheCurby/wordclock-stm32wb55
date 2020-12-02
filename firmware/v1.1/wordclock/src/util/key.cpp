////////////////////////////////////////////////////////////////////////////////
// Wordclock V1.0
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
#include "util/key.hpp"

Key::Key() {
	bPressed = false;
	bScrolling = false;
	bLastStatus = false;
	bToggle = false;
	u16StartDelay = 0;
	u16Delay = 0;
}

bool Key::getSingle(bool bKey) {
	bool bKlick = false;

	if (bPressed != bKey) {
		if (tEntprell.Ready()) {
			if (bKey && !bPressed) {
				bKlick = true;
				bPressed = true;
				tUtil.StartMs(u16StartDelay);
			}
			else if (!bKey && bPressed) {
				bPressed = false;
			}
			tEntprell.StartMs(u16Prelltime);
		}
	}
	else if (bPressed && tUtil.Ready() && bScrolling) {
		tUtil.StartMs(u16Delay);
		bKlick = true;
	}

	bLastStatus = bKlick;
	return bKlick;
}

bool Key::getToggle(bool bKey) {
	bToggle = false;

	if (bPressed != bKey) {
		if (tEntprell.Ready()) {
			if (bKey && !bPressed) {
				bToggle = true;
				bPressed = true;
				tUtil.StartMs(u16StartDelay);
			}
			else if (!bKey && bPressed) {
				bPressed = false;
				bToggle = true;
			}
			tEntprell.StartMs(u16Prelltime);
		}
	}
	else if (bPressed && tUtil.Ready() && bScrolling) {
		tUtil.StartMs(u16Delay);
		bToggle = true;
	}

	bLastStatus = bToggle;
	return bToggle;
}

bool Key::getDouble(bool bKey) {
	bool bDoubleKlick = false;

	if (bPressed != bKey) {
		if (tEntprell.Ready()) {
			if (bKey && !bPressed) {
				if (!tUtil.Ready()) {
					bDoubleKlick = true;
				}

				tUtil.StartMs(u16Double);
				bPressed = true;
			}
			else if (!bKey && bPressed) {
				bPressed = false;
			}
			tEntprell.StartMs(u16Prelltime);
		}
	}

	bLastStatus = bDoubleKlick;
	return bDoubleKlick;
}

bool Key::getLongPress(bool bKey, uint16_t u16Time) {
	bool bResult = false;

	if (!bKey) {
		tUtil.StartMs(u16Time);
		bPressed = false;
	}
	else if (tUtil.Ready() && !bPressed) {
		bResult = true;
		bPressed = true;
	}

	bLastStatus = bResult;
	return bResult;
}

void Key::setScrolling(bool bVal, uint16_t u16StartDelay_l, uint16_t u16Delay_l) {
	bScrolling = bVal;
	u16StartDelay = u16StartDelay_l;
	u16Delay = u16Delay_l;
}

bool Key::getValue() const {
	return bPressed;
}

bool Key::getLastStatus() const {
	return bLastStatus;
}
