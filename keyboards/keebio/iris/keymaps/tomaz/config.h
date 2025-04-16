/*
Copyright 2023 Tomaz Kragelj <tkragelj@gmail.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

// Comment for windows implementation
#define IS_MAC

#define SPLIT_LAYER_STATE_ENABLE
#define SPLIT_LED_STATE_ENABLE

// How fast tap action should end for next tap to be considered double tap
// See examples on https://docs.qmk.fm/tap_hold
#define TAPPING_TERM 175

// How fast mouse movement changes to maximum speed
#define MOUSEKEY_TIME_TO_MAX 100

#ifdef IS_MAC
#   define UNICODE_SELECTED_MODES UNICODE_MODE_MACOS
#else
#   define UNICODE_SELECTED_MODES UNICODE_MODE_WINDOWS
#endif
