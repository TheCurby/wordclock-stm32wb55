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
#include "wordclock/wordclock.hpp"

const uint32_t Wordclock::u32AnimationTime = 3500;
const uint16_t Wordclock::u16TimeoutTime = 30000;
const uint16_t Wordclock::u16BlinkTime = 200;
const uint16_t Wordclock::u16TestTime = 500;
const uint16_t Wordclock::u16RedrawTime = 30000;

//@formatter:off
const uint8_t Wordclock::au8Mapping[LEDS] = {
	102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112,
	100, 99, 98, 97, 96, 95, 94, 93, 92, 91, 90,
	79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89,
	78, 77, 76, 75, 74, 73, 72, 71, 70, 69, 68,
	57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67,
	56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46,
	35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45,
	34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24,
	13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23,
	11, 10,  9,  8,  7,  6,  5,  4,  3,  2,  1,
	101, 113, 0, 12
};

const Image Wordclock::Up = {
	.u8Size = 14,
	.u8Points = { {3, 0}, {2, 1}, {4, 1}, {1, 2}, {5, 2}, {0, 3}, {6, 3}, {3, 1}, {3, 2}, {3, 3}, {3, 4}, {3, 5}, {3, 6},
		  {3, 7}}
};

const Image Wordclock::Down = {
	.u8Size = 14,
	.u8Points = { {3, 7}, {2, 6}, {4, 6}, {1, 5}, {5, 5}, {0, 4}, {6, 4}, {3, 6}, {3, 5}, {3, 4}, {3, 3}, {3, 2}, {3, 1},
		  {3, 0}}
};

const Image Wordclock::ok = {
	.u8Size = 6,
	.u8Points = { {8, 2}, {7, 3}, {6, 4}, {5, 5}, {4, 4}, {3, 3}}
};

const Image Wordclock::error = {
	.u8Size = 9,
	.u8Points = { {3, 2}, {7, 2}, {4, 3}, {6, 3}, {5, 4}, {4, 5}, {6, 5}, {3, 6}, {7, 6}}
};

const Image Wordclock::antenna = {
	.u8Size = 12,
	.u8Points = { {5, 2}, {5, 3}, {5, 4}, {5, 5}, {5, 6}, {5, 7}, {6, 4}, {7, 3}, {8, 2}, {4, 4}, {3, 3}, {2, 2}}
};

const Image Wordclock::question = {
	.u8Size = 27,
	.u8Points = { {1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, {0, 1}, {1, 1}, {5, 1}, {6, 1},
				  {0, 2}, {6, 2}, {5, 3}, {6, 3}, {4, 4}, {5, 4}, {2, 5}, {3, 5}, {4, 5},
				  {2, 6}, {3, 6}, {4, 6}, {2, 8}, {3, 8}, {4, 8}, {2, 9}, {3, 9}, {4, 9}}
};

const Image Wordclock::Night = {
	.u8Size = 13,
	.u8Points = {
		{3, 2}, {3, 3}, {3, 4}, {3, 5}, {3, 6},
		{7, 2}, {7, 3}, {7, 4}, {7, 5}, {7, 6},
		{4, 3}, {5, 4}, {6, 5}
	}
};

static const Image z0 = {
	.u8Size = 16,
	.u8Points = { {1, 0}, {2, 0}, {3, 0}, {0, 1}, {4, 1}, {0, 2}, {4, 2}, {0, 3}, {4, 3}, {0, 4}, {4, 4}, {0, 5}, {4, 5}, {1, 6}, {2, 6}, {3, 6}}
};
static const Image z1 = {
	.u8Size = 13,
	.u8Points = { {2, 0}, {1, 1}, {2, 1}, {0, 2}, {2, 2}, {2, 3}, {2, 4}, {2, 5}, {0, 6}, {1, 6}, {2, 6}, {3, 6}, {4, 6}}
};
static const Image z2 = {
	.u8Size = 15,
	.u8Points = { {1, 0}, {2, 0}, {3, 0}, {0, 1}, {4, 1}, {0, 2}, {4, 2}, {3, 3}, {2, 4}, {1, 5}, {0, 6}, {1, 6}, {2, 6}, {3, 6}, {4, 6}}
};
static const Image z3 = {
	.u8Size = 14,
	.u8Points = { {1, 0}, {2, 0}, {3, 0}, {0, 1}, {4, 1}, {4, 2}, {3, 3}, {2, 3}, {4, 4}, {0, 5}, {4, 5}, {1, 6}, {2, 6}, {3, 6}}
};
static const Image z4 = {
	.u8Size = 14,
	.u8Points = { {3, 0}, {3, 1}, {3, 2}, {3, 3}, {3, 4}, {3, 5}, {3, 6}, {0, 4}, {1, 4}, {2, 4}, {2, 1}, {1, 2}, {0, 3}, {4, 4}}
};
static const Image z5 = {
	.u8Size = 18,
	.u8Points = { {0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0}, {0, 1}, {0, 2}, {1, 2}, {2, 2}, {3, 2}, {4, 3}, {0, 4}, {4, 4}, {0, 5}, {4, 5}, {1, 6}, {2, 6}, {3, 6}}
};
static const Image z6 = {
	.u8Size = 17,
	.u8Points = { {1, 0}, {2, 0}, {3, 0}, {0, 1}, {4, 1}, {0, 2}, {0, 3}, {1, 3}, {2, 3}, {3, 3}, {0, 4}, {4, 4}, {0, 5}, {4, 5}, {1, 6}, {2, 6}, {3, 6}}
};
static const Image z7 = {
	.u8Size = 11,
	.u8Points = { {0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0}, {4, 1}, {3, 2}, {2, 3}, {2, 4}, {2, 5}, {2, 6}}
};
static const Image z8 = {
	.u8Size = 17,
	.u8Points = { {1, 0}, {2, 0}, {3, 0}, {0, 1}, {4, 1}, {0, 2}, {4, 2}, {1, 3}, {2, 3}, {3, 3}, {0, 4}, {4, 4}, {0, 5}, {4, 5}, {1, 6}, {2, 6}, {3, 6}}
};
static const Image z9 = {
	.u8Size = 17,
	.u8Points = { {1, 0}, {2, 0}, {3, 0}, {0, 1}, {4, 1}, {0, 2}, {4, 2}, {1, 3}, {2, 3}, {3, 3}, {4, 3}, {4, 4}, {0, 5}, {4, 5}, {1, 6}, {2, 6}, {3, 6}}
};

const Image* Wordclock::aoZiffern[10] = {&z0, &z1, &z2, &z3, &z4, &z5, &z6, &z7, &z8, &z9};

static const Image D = {
	.u8Size = 18,
	.u8Points = {{0, 0}, {1, 0}, {2, 0}, {3, 1}, {0, 7}, {1, 7}, {2, 7}, {3, 6}, {4, 2}, {4, 3}, {4, 4}, {4, 5}, {0, 1}, {0, 2}, {0, 3}, {0, 4}, {0, 5},{0, 6}}
};

static const Image E = {
	.u8Size = 20,
	.u8Points = {{1, 0}, {2, 0}, {3, 0}, {4, 0}, {1, 3}, {2, 3}, {3, 3}, {4, 3}, {1, 7}, {2, 7}, {3, 7}, {4, 7}, {0, 0}, {0, 1}, {0, 2}, {0, 3}, {0, 4}, {0, 5}, {0, 6}, {0, 7}}
};

static const Image F = {
	.u8Size = 16,
	.u8Points = {{1, 0}, {2, 0}, {3, 0}, {4, 0}, {1, 3}, {2, 3}, {3, 3}, {4, 3}, {0, 0}, {0, 1}, {0, 2}, {0, 3}, {0, 4}, {0, 5}, {0, 6}, {0, 7}}
};

static const Image L = {
	.u8Size = 12,
	.u8Points = {{1, 7}, {2, 7}, {3, 7}, {4, 7}, {0, 0}, {0, 1}, {0, 2}, {0, 3}, {0, 4}, {0, 5}, {0, 6}, {0, 7}}
};

static const Image N = {
	.u8Size = 20,
	.u8Points = {{0, 0}, {0, 1}, {0, 2}, {0, 3}, {0, 4}, {0, 5}, {0, 6}, {0, 7}, {4, 0}, {4, 1}, {4, 2}, {4, 3}, {4, 4}, {4, 5}, {4, 6}, {4, 7}, {1, 2}, {2, 3}, {2, 4}, {3, 5}}
};

static const Image R = {
	.u8Size = 20,
	.u8Points = {{0, 0}, {0, 1}, {0, 2}, {0, 3}, {0, 4}, {0, 5}, {0, 6}, {0, 7}, {2, 5}, {3, 6}, {4, 7}, {1, 0}, {2, 0}, {3, 0}, {4, 1}, {4, 2}, {4, 3}, {1, 4}, {2, 4}, {3, 4}}
};

static const Image I = {
	.u8Size = 16,
	.u8Points = {{0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0}, {0, 7}, {1, 7}, {2, 7}, {3, 7}, {4, 7}, {2, 1}, {2, 2}, {2, 3}, {2, 4}, {2, 5}, {2, 6}}
};

static const Image T = {
	.u8Size = 12,
	.u8Points = {{0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0}, {2, 7}, {2, 1}, {2, 2}, {2, 3}, {2, 4}, {2, 5}, {2, 6}}
};

const Image* Wordclock::aoBuchstaben[24] = {nullptr, nullptr, nullptr, &D, &E, &F, nullptr, nullptr, &I, nullptr, nullptr, &L, nullptr, &N, nullptr, nullptr, nullptr, &R, nullptr, &T};
//@formatter:on
