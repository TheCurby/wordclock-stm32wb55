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

#include "util/timer.hpp"

class Key {
	public:
		Key();
		void setScrolling(bool bVal, uint16_t u16StartDelay_l = 500, uint16_t u16Delay_l = 80);
		bool getSingle(bool bKey);
		bool getToggle(bool bKey);
		bool getDouble(bool bKey);
		bool getLongPress(bool bKey, uint16_t u16Time = 1000);
		bool getValue() const;
		bool getLastStatus() const;

	protected:
		bool bLastStatus;
		bool bScrolling;
		bool bPressed;
		bool bToggle;
		bool bDebounced;
		uint16_t u16StartDelay;
		uint16_t u16Delay;
		Timer tEntprell;
		Timer tUtil;

		static constexpr uint16_t u16Prelltime = 100;
		static constexpr uint16_t u16Double = 800;
};
