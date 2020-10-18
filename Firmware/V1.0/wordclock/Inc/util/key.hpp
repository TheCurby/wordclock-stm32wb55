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
		uint16_t u16StartDelay;
		uint16_t u16Delay;
		Timer tEntprell;
		Timer tUtil;

		static constexpr uint16_t u16Prelltime = 200;
		static constexpr uint16_t u16Double = 800;
};
