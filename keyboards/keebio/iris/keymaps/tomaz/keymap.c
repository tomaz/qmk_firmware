// Copyright 2023-24 Tomaz Kragelj
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

enum {
	LAYER_QWERTY,
	LAYER_QWERTY_MAC,
	LAYER_CURSORS,
	LAYER_CURSORS_MAC,
	LAYER_MOUSE,
	LAYER_MOUSE_MAC
};

// Shortcuts for various platform-specific key combos
#define PL_HOME	LGUI(KC_LEFT)
#define PL_END	LGUI(KC_RIGHT)

// Momentary layer switch
#define LTCW(key) LT(LAYER_CURSORS, key)
#define LTCM(key) LT(LAYER_CURSORS_MAC, key)
#define LTMW(key) LT(LAYER_MOUSE, key)
#define LTMM(key) LT(LAYER_MOUSE_MAC, key)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Tap dance adopted from https://github.com/qmk/qmk_firmware/blob/9baafef968ea4b1439ed9cb58479e66eec1f80d7/docs/feature_tap_dance.md
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

enum {
	TD_TAB_BOOT,
	TD_RSFT_BOOT,
	TD_LSFT_MAC_TOGGLE,
};

void td_mac_win_layer_toggle(tap_dance_state_t *state, void *user_data) {
	switch (state->count) {
		case 1: {
			register_code(KC_LSFT);
			break;
		}
		case 2: {
			if (layer_state_is(LAYER_MOUSE)) {
				set_single_persistent_default_layer(LAYER_QWERTY_MAC);
			} else {
				set_single_persistent_default_layer(LAYER_QWERTY);
			}
			break;
		}
		default: {
			break;
		}
	}
}

// Associate our tap dance key with its functionality
tap_dance_action_t tap_dance_actions[] = {
	[TD_TAB_BOOT] = ACTION_TAP_DANCE_DOUBLE(KC_TAB, QK_BOOT),
	[TD_RSFT_BOOT] = ACTION_TAP_DANCE_DOUBLE(KC_RSFT, QK_BOOT),
	[TD_LSFT_MAC_TOGGLE] = ACTION_TAP_DANCE_FN(td_mac_win_layer_toggle)
};

#define TD_TBBT		TD(TD_TAB_BOOT)
#define TD_RSBT		TD(TD_RSFT_BOOT)
#define TD_LSTO		TD(TD_LSFT_MAC_TOGGLE)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Tap dance end
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

//-------------------------------------------------------------------------------------------------------------------------------------------
// DEFAULT LAYER
//-------------------------------------------------------------------------------------------------------------------------------------------

	[LAYER_QWERTY] = LAYOUT(
// ALT and GUI keys are reversed here on Mac so that we maintain closer feel to Windows
// ┌───────┬───────┬───────┬───────┬───────┬───────┐							   ┌───────┬───────┬───────┬───────┬───────┬───────┐
	KC_ESC,	KC_1,	KC_2,	KC_3,	KC_4,	KC_5,									KC_6,	KC_7,	KC_8,	KC_9,	KC_0,	KC_MINS,
// ├───────┼───────┼───────┼───────┼───────┼───────┤							   ├───────┼───────┼───────┼───────┼───────┼───────┤
	KC_TAB,	KC_Q,	KC_W,	KC_E,	KC_R,	KC_T,									KC_Y,	KC_U,	KC_I,	KC_O,	KC_P,	LTMW(KC_EQL),
// ├───────┼───────┼───────┼───────┼───────┼───────┤							   ├───────┼───────┼───────┼───────┼───────┼───────┤
	KC_LCTL,KC_A,	KC_S,	KC_D,	KC_F,	KC_G,									KC_H,	KC_J,	KC_K,	KC_L,	KC_SCLN,LTCW(KC_QUOT),
// ├───────┼───────┼───────┼───────┼───────┼───────┤─────────────┐	   ┌───────────┼───────┼───────┼───────┼───────┼───────┼───────┤
	KC_LSFT,KC_Z,	KC_X,	KC_C,	KC_V,	KC_B,	LTMW(KC_GRV),		KC_BSLS,	KC_N,	KC_M,	KC_COMM,KC_DOT,	KC_SLSH,KC_RSFT,
// └───────┴───────┴───────┴────────┴──────┴───────┴─────────────┘	   └───────────┴───────┴───────┴───────┴───────┴───────┴───────┘
//				   ┌───────────────┬───────────────┬───────────────┐   ┌───────────────┬───────────────┬───────────────┐
					KC_LGUI,		LTCW(KC_SPC),	KC_LALT,			KC_BSPC,		KC_ENT,			KC_RCTL
//				   └───────────────┴───────────────┴───────────────┘   └───────────────┴───────────────┴───────────────┘
	),

	[LAYER_QWERTY_MAC] = LAYOUT(
// ALT and GUI keys are reversed here on Mac so that we maintain closer feel to Windows
// ┌───────┬───────┬───────┬───────┬───────┬───────┐							   ┌───────┬───────┬───────┬───────┬───────┬───────┐
	KC_ESC,	KC_1,	KC_2,	KC_3,	KC_4,	KC_5,									KC_6,	KC_7,	KC_8,	KC_9,	KC_0,	KC_MINS,
// ├───────┼───────┼───────┼───────┼───────┼───────┤							   ├───────┼───────┼───────┼───────┼───────┼───────┤
	KC_TAB,	KC_Q,	KC_W,	KC_E,	KC_R,	KC_T,									KC_Y,	KC_U,	KC_I,	KC_O,	KC_P,	LTMM(KC_EQL),
// ├───────┼───────┼───────┼───────┼───────┼───────┤							   ├───────┼───────┼───────┼───────┼───────┼───────┤
	KC_LCTL,KC_A,	KC_S,	KC_D,	KC_F,	KC_G,									KC_H,	KC_J,	KC_K,	KC_L,	KC_SCLN,LTCM(KC_QUOT),
// ├───────┼───────┼───────┼───────┼───────┼───────┤─────────────┐	   ┌───────────┼───────┼───────┼───────┼───────┼───────┼───────┤
	KC_LSFT,KC_Z,	KC_X,	KC_C,	KC_V,	KC_B,	LTMM(KC_GRV),		KC_BSLS,	KC_N,	KC_M,	KC_COMM,KC_DOT,	KC_SLSH,KC_RSFT,
// └───────┴───────┴───────┴────────┴──────┴───────┴─────────────┘	   └───────────┴───────┴───────┴───────┴───────┴───────┴───────┘
//				   ┌───────────────┬───────────────┬───────────────┐   ┌───────────────┬───────────────┬───────────────┐
					KC_LALT,		LTCM(KC_SPC),	KC_LGUI,			KC_BSPC,		KC_ENT,			KC_RCTL
//				   └───────────────┴───────────────┴───────────────┘   └───────────────┴───────────────┴───────────────┘
	),

//-------------------------------------------------------------------------------------------------------------------------------------------
// CURSORS LAYER
//-------------------------------------------------------------------------------------------------------------------------------------------

	[LAYER_CURSORS] = LAYOUT(
// ALT and GUI keys are always the same here since we always want them in the same position on all platforms
// ┌───────┬───────┬───────┬───────┬───────┬───────┐							   ┌───────┬───────┬───────┬───────┬───────┬───────┐
	_______,KC_F1,	KC_F2,	KC_F3,	KC_F4,	KC_F5,									KC_F6,	KC_F7,	KC_F8,	KC_F9,	KC_F10,	KC_F11,
// ├───────┼───────┼───────┼───────┼───────┼───────┤							   ├───────┼───────┼───────┼───────┼───────┼───────┤
	_______,_______,KC_HOME,KC_UP,	KC_END,	_______,								_______,_______,_______,_______,_______,KC_F12,
// ├───────┼───────┼───────┼───────┼───────┼───────┤							   ├───────┼───────┼───────┼───────┼───────┼───────┤
	KC_LCTL,_______,KC_LEFT,KC_DOWN,KC_RGHT,KC_PGUP,								KC_LCTL,KC_LALT,KC_LGUI,_______,_______,_______,
// ├───────┼───────┼───────┼───────┼───────┼───────┤─────────────┐	   ┌───────────┼───────┼───────┼───────┼───────┼───────┼───────┤
	KC_LSFT,KC_LALT,KC_LGUI,_______,_______,KC_PGDN,KC_INS,				KC_DEL,		KC_SPC,	KC_ENT,	_______,_______,_______,_______,
// └───────┴───────┴───────┴────────┴──────┴───────┴─────────────┘	   └───────────┴───────┴───────┴───────┴───────┴───────┴───────┘
//				   ┌───────────────┬───────────────┬───────────────┐   ┌───────────────┬───────────────┬───────────────┐
					_______,		_______,		KC_LALT,			KC_BSPC,		KC_LBRC,		KC_RBRC
//				   └───────────────┴───────────────┴───────────────┘   └───────────────┴───────────────┴───────────────┘
	),

	[LAYER_CURSORS_MAC] = LAYOUT(
// ALT and GUI keys are always the same here since we always want them in the same position on all platforms
// ┌───────┬───────┬───────┬───────┬───────┬───────┐							   ┌───────┬───────┬───────┬───────┬───────┬───────┐
	_______,KC_F1,	KC_F2,	KC_F3,	KC_F4,	KC_F5,									KC_F6,	KC_F7,	KC_F8,	KC_F9,	KC_F10,	KC_F11,
// ├───────┼───────┼───────┼───────┼───────┼───────┤							   ├───────┼───────┼───────┼───────┼───────┼───────┤
	_______,_______,PL_HOME,KC_UP,	PL_END,	_______,								_______,_______,_______,_______,_______,KC_F12,
// ├───────┼───────┼───────┼───────┼───────┼───────┤							   ├───────┼───────┼───────┼───────┼───────┼───────┤
	KC_LCTL,_______,KC_LEFT,KC_DOWN,KC_RGHT,KC_PGUP,								KC_LCTL,KC_LALT,KC_LGUI,_______,_______,_______,
// ├───────┼───────┼───────┼───────┼───────┼───────┤─────────────┐	   ┌───────────┼───────┼───────┼───────┼───────┼───────┼───────┤
	KC_LSFT,KC_LALT,KC_LGUI,_______,_______,KC_PGDN,KC_INS,				KC_DEL,		KC_SPC,	KC_ENT,	_______,_______,_______,_______,
// └───────┴───────┴───────┴────────┴──────┴───────┴─────────────┘	   └───────────┴───────┴───────┴───────┴───────┴───────┴───────┘
//				   ┌───────────────┬───────────────┬───────────────┐   ┌───────────────┬───────────────┬───────────────┐
					_______,		_______,		KC_LALT,			KC_BSPC,		KC_LBRC,		KC_RBRC
//				   └───────────────┴───────────────┴───────────────┘   └───────────────┴───────────────┴───────────────┘
	),

//-------------------------------------------------------------------------------------------------------------------------------------------
// MOUSE LAYER
//-------------------------------------------------------------------------------------------------------------------------------------------

	[LAYER_MOUSE] = LAYOUT(
// ┌───────┬───────┬───────┬───────┬───────┬───────┐							   ┌───────┬───────┬───────┬───────┬───────┬───────┐
	_______,KC_F1,	KC_F2,	KC_F3,	KC_F4,	KC_F5,									KC_F6,	KC_F7,	KC_F8,	KC_F9,	KC_F10,	KC_F11,
// ├───────┼───────┼───────┼───────┼───────┼───────┤							   ├───────┼───────┼───────┼───────┼───────┼───────┤
	TD_TBBT,_______,KC_BTN1,KC_MS_U,KC_BTN2,_______,								_______,KC_P7,	KC_P8,	KC_P9,	_______,KC_F12,
// ├───────┼───────┼───────┼───────┼───────┼───────┤							   ├───────┼───────┼───────┼───────┼───────┼───────┤
	_______,RGB_VAI,KC_MS_L,KC_MS_D,KC_MS_R,KC_WH_U,								KC_VOLU,KC_P4,	KC_P5,	KC_P6,	_______,_______,
// ├───────┼───────┼───────┼───────┼───────┼───────┤─────────────┐	   ┌───────────┼───────┼───────┼───────┼───────┼───────┼───────┤
	TD_LSTO,_______,_______,_______,_______,KC_WH_D,KC_INS,				KC_DEL,		KC_VOLD,KC_P1,	KC_P2,	KC_P3,	_______,TD_RSBT,
// └───────┴───────┴───────┴────────┴──────┴───────┴─────────────┘	   └───────────┴───────┴───────┴───────┴───────┴───────┴───────┘
//				   ┌───────────────┬───────────────┬───────────────┐   ┌───────────────┬───────────────┬───────────────┐
					_______,		_______,		_______,			KC_BSPC,		KC_P0,			 _______
//				   └───────────────┴───────────────┴───────────────┘   └───────────────┴───────────────┴───────────────┘
	),

	[LAYER_MOUSE_MAC] = LAYOUT(
// ┌───────┬───────┬───────┬───────┬───────┬───────┐							   ┌───────┬───────┬───────┬───────┬───────┬───────┐
	_______,KC_F1,	KC_F2,	KC_F3,	KC_F4,	KC_F5,									KC_F6,	KC_F7,	KC_F8,	KC_F9,	KC_F10,	KC_F11,
// ├───────┼───────┼───────┼───────┼───────┼───────┤							   ├───────┼───────┼───────┼───────┼───────┼───────┤
	TD_TBBT,_______,KC_BTN1,KC_MS_U,KC_BTN2,_______,								_______,KC_P7,	KC_P8,	KC_P9,	_______,KC_F12,
// ├───────┼───────┼───────┼───────┼───────┼───────┤							   ├───────┼───────┼───────┼───────┼───────┼───────┤
	_______,RGB_VAI,KC_MS_L,KC_MS_D,KC_MS_R,KC_WH_U,								KC_VOLU,KC_P4,	KC_P5,	KC_P6,	_______,_______,
// ├───────┼───────┼───────┼───────┼───────┼───────┤─────────────┐	   ┌───────────┼───────┼───────┼───────┼───────┼───────┼───────┤
	TD_LSTO,_______,_______,_______,_______,KC_WH_D,KC_INS,				KC_DEL,		KC_VOLD,KC_P1,	KC_P2,	KC_P3,	_______,TD_RSBT,
// └───────┴───────┴───────┴────────┴──────┴───────┴─────────────┘	   └───────────┴───────┴───────┴───────┴───────┴───────┴───────┘
//				   ┌───────────────┬───────────────┬───────────────┐   ┌───────────────┬───────────────┬───────────────┐
					_______,		_______,		_______,			KC_BSPC,		KC_P0,			 _______
//				   └───────────────┴───────────────┴───────────────┘   └───────────────┴───────────────┴───────────────┘
	)
};

#define RGB_DARK_BLUE       0x00, 0x39, 0x88
#define RGB_DARK_PURPLE     0x88, 0x00, 0x52
#define RGB_BLUE_PURPLE     0x46, 0x00, 0xFF
#define RGB_REDISH_ORANGE   0xff, 0x44, 0x00

void set_key_color(uint8_t layer, uint8_t index, uint8_t rgb[]) {
	rgb_matrix_set_color(index, rgb[layer * 3], rgb[layer * 3 + 1], rgb[layer * 3 + 2]);
}

bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
	uint8_t layer = get_highest_layer(layer_state);

	for (uint8_t row = 0; row < MATRIX_ROWS; ++row) {
		for (uint8_t col = 0; col < MATRIX_COLS; ++col) {
			uint8_t index = g_led_config.matrix_co[row][col];

			if (index < led_min) continue;
			if (index > led_max) continue;
			if (index == NO_LED) continue;

			uint16_t keymap = keymap_key_to_keycode(layer, (keypos_t){col,row});

			switch (keymap)
			{
				case KC_F1: case KC_F2: case KC_F3: case KC_F4:
				case KC_F5: case KC_F6: case KC_F7: case KC_F8:
				case KC_F9: case KC_F10: case KC_F11: case KC_F12:
					rgb_matrix_set_color(index, RGB_REDISH_ORANGE);
					break;

				case KC_0: case KC_1: case KC_2: case KC_3: case KC_4:
				case KC_5: case KC_6: case KC_7: case KC_8: case KC_9:
					rgb_matrix_set_color(index, RGB_ORANGE);
					break;

				case KC_P0: case KC_P1: case KC_P2: case KC_P3: case KC_P4:
				case KC_P5: case KC_P6: case KC_P7: case KC_P8: case KC_P9:
					rgb_matrix_set_color(index, RGB_RED);
					break;

				case KC_A: case KC_B: case KC_C: case KC_D: case KC_E: case KC_F: case KC_G: case KC_H: case KC_I: case KC_J:
				case KC_K: case KC_L: case KC_M: case KC_N: case KC_O: case KC_P: case KC_Q: case KC_R: case KC_S: case KC_T:
				case KC_U: case KC_V: case KC_W: case KC_X: case KC_Y: case KC_Z:
					rgb_matrix_set_color(index, RGB_RED);
					break;

				case KC_COMM: case KC_DOT: case KC_SLSH: case KC_MINS:
				case KC_BSLS: case KC_GRV: case KC_EQL: case KC_QUOT:
				case KC_LBRC: case KC_RBRC: case KC_SCLN:
				case LTCW(KC_QUOT): case LTMW(KC_EQL): case LTMW(KC_GRV):
				case LTCM(KC_QUOT): case LTMM(KC_EQL): case LTMM(KC_GRV):
					rgb_matrix_set_color(index, RGB_DARK_BLUE);
					break;

				case KC_DEL: case KC_BSPC:
					rgb_matrix_set_color(index, RGB_MAGENTA);
					break;

				case KC_LGUI: case KC_LCTL: case KC_RCTL: case KC_LALT: case KC_RALT:
					// modifiers
					rgb_matrix_set_color(index, RGB_DARK_PURPLE);
					break;

				case KC_SPC: case LTCW(KC_SPC):
					rgb_matrix_set_color(index, RGB_BLUE);
					break;

				case KC_LSFT: case KC_RSFT:
					rgb_matrix_set_color(index, RGB_BLUE);
					break;

				// --------------------------- CURSORS

				case KC_UP: case KC_DOWN: case KC_LEFT: case KC_RIGHT:
					rgb_matrix_set_color(index, RGB_RED);
					break;

				case KC_HOME: case KC_END: case KC_PGUP: case KC_PGDN:
				case PL_HOME: case PL_END:
					rgb_matrix_set_color(index, RGB_SPRINGGREEN);
					break;

				// --------------------------- MOUSE

				case KC_MS_U: case KC_MS_D: case KC_MS_L: case KC_MS_R:
					rgb_matrix_set_color(index, RGB_RED);
					break;

				case KC_BTN1: case KC_BTN2: case KC_WH_U: case KC_WH_D:
					rgb_matrix_set_color(index, RGB_SPRINGGREEN);
					break;

				// -------------------------- MEDIA & DEFAULTS

				case KC_VOLU: case KC_VOLD:
					rgb_matrix_set_color(index, RGB_YELLOW);
					break;

				case TD_TBBT:
				case TD_RSBT:
				case TD_LSTO:
					rgb_matrix_set_color(index, RGB_MAGENTA);
					break;

				case KC_TRNS:
					// transparent keys should be off
					rgb_matrix_set_color(index, RGB_OFF);
					break;

				default:
					switch (layer) {
						case LAYER_QWERTY:
							// every other key on main layer should be white (so we don't have to specify each)
							rgb_matrix_set_color(index, RGB_SPRINGGREEN);
							break;
						default:
							// every other key on other layers should be off, since we only want to show the ones that are programmed (thought KC_TRNS case above should take care of those)
							rgb_matrix_set_color(index, RGB_OFF);
							break;
					}
					break;
			}
		}
	}

	return false;
}
