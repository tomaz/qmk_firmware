// Copyright 2023-24 Tomaz Kragelj
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H
#include "keymap_extras/keymap_slovenian.h"

// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
// TAP DANCE
// https://docs.qmk.fm/features/tap_dance
// https://docs.qmk.fm/tap_hold

// Comment to disable tap dance code (must also delete TD from rules.mk)
#define IS_TAP_DANCE_ENABLED

#ifdef IS_TAP_DANCE_ENABLED

// Definitions for simpler layout
#define TDC TD(TD_C)
#define TDS TD(TD_S)
#define TDZ TD(TD_Z)

// All tap dances
enum {
	TD_C,
	TD_S,
	TD_Z
};

// Tap dance states
typedef enum {
	TD_NONE,

	TD_SINGLE_TAP,
	TD_SINGLE_HOLD,

	TD_DOUBLE_TAP,
	TD_DOUBLE_HOLD,
	TD_DOUBLE_SINGLE_TAP,	// Send 2 single taps

	TD_TRIPLE_TAP,
	TD_TRIPLE_HOLD,
	TD_TRIPLE_SINGLE_TAP,	// Send 3 single taps
} td_state_t;

typedef enum {
	TD_FN_FINISHED,
	TD_FN_RESET
} td_fn_state_t;

// Tap dance state handler function.
typedef void (*td_state_fn_t)(td_state_t state, td_fn_state_t fn_state);

// Tap dance data for each td.
typedef struct {
	td_state_t state;
    bool is_shift;
    uint16_t kc;
    const char *uc;
    const char *uc_shift;
} td_data_t;

// Tap dance handlers
void td_finished_handler(tap_dance_state_t *state, void *user_data);
void td_reset_handler(tap_dance_state_t *state, void *user_data);

// Macro for creating each tap dance key
#define ACTION_TAP_DANCE_STATE_FN(kc, uc, ucs) { \
	.fn = {NULL, td_finished_handler, td_reset_handler, NULL}, \
	.user_data = (void *)&((td_data_t){false, TD_NONE, kc, uc, ucs}) \
}

// Setup all tap dance actions
tap_dance_action_t tap_dance_actions[] = {
	[TD_C] = ACTION_TAP_DANCE_STATE_FN(KC_C, "č", "Č"),
	[TD_S] = ACTION_TAP_DANCE_STATE_FN(KC_S, "š", "Š"),
	[TD_Z] = ACTION_TAP_DANCE_STATE_FN(KC_Z, "ž", "Ž")
};

// Helpers

td_state_t td_determine_state(tap_dance_state_t *state) {
	if (state->count == 1) {
		if (state->interrupted || !state->pressed) {
			return TD_SINGLE_TAP;
		} else {
			return TD_SINGLE_HOLD;
		}
	} else if (state->count == 2) {
		if (state->interrupted) {
			return TD_DOUBLE_SINGLE_TAP;
		} else if (state->pressed) {
			return TD_DOUBLE_HOLD;
		} else {
			return TD_DOUBLE_TAP;
		}
	} else if (state->count == 3) {
		if (state->interrupted) {
			return TD_TRIPLE_SINGLE_TAP;
		} else if (state->pressed) {
			return TD_TRIPLE_TAP;
		} else {
			return TD_TRIPLE_HOLD;
		}
	} else {
		return TD_NONE;
	}
}

bool td_is_shift_pressed(void) {
    if (get_mods() == MOD_BIT(KC_LSFT)) return true;
    if (get_mods() == MOD_BIT(KC_RSFT)) return true;
    return false;
}

void td_handle(td_data_t *data, td_fn_state_t state) {
    switch (state) {
        case TD_FN_FINISHED: {
            switch (data->state) {
                case TD_SINGLE_TAP: register_code(data->kc); break;
                case TD_SINGLE_HOLD: register_code(data->kc); break;
                case TD_TRIPLE_TAP: send_unicode_string(data->is_shift ? data->uc_shift : data->uc); break;
                default: break;
            }
            break;
        }
        case TD_FN_RESET: {
            switch (data->state) {
                case TD_SINGLE_TAP: unregister_code(data->kc); break;
                default: break;
            }
        }
        default: {
            break;
        }
    }
}

// Tap dance handlers

void td_finished_handler(tap_dance_state_t *state, void *user_data) {
    td_data_t *data = (td_data_t *)user_data;
    data->is_shift = td_is_shift_pressed();
    data->state = td_determine_state(state);
    td_handle(data, TD_FN_FINISHED);
}

void td_reset_handler(tap_dance_state_t *state, void *user_data) {
    td_data_t *data = (td_data_t *)user_data;
    data->is_shift = td_is_shift_pressed();
    data->state = td_determine_state(state);
    td_handle(data, TD_FN_RESET);
}

// Overrides

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    // This is to emit taps as soon as possible - on release of the TD key; have to use this function as there's no tap dance override for it.
	td_data_t *action;

	switch (keycode) {
		case TDC:
		case TDS:
		case TDZ:
			action = &tap_dance_actions[QK_TAP_DANCE_GET_INDEX(keycode)];
			if (!record->event.pressed && action->state.count && !action->state.finished) {
                td_data_t *data = (td_data_t *)action->user_data;
				tap_code16(data->kc);
			}
			break;
	}

	return true;
}

#else
#   define TDC KC_C
#   define TDS KC_S
#   define TDZ KC_Z
#endif

/*
// Data for each tap dance
typedef struct {
	uint16_t tap;
	uint16_t hold;
} tap_dance_tap_hold_t;

bool process_record_user(uint16_t keycode, keyrecord_t *record);
void tap_dance_tap_hold_finished(tap_dance_state_t *state, void *user_data);

#define ACTION_TAP_DANCE_TAP_HOLD(tap) \
	{ \
		.fn        = { NULL, tap_dance_tap_hold_finished, NULL }, \
		.user_data = (void *)&((tap_dance_tap_hold_t){ tap, tap }), \
	}

tap_dance_action_t tap_dance_actions[] = {
	[TD_C] = ACTION_TAP_DANCE_TAP_HOLD(KC_C),
	[TD_S] = ACTION_TAP_DANCE_TAP_HOLD(KC_S),
	[TD_Z] = ACTION_TAP_DANCE_TAP_HOLD(KC_Z),
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
	tap_dance_action_t *action;

	switch (keycode) {
		case TD(TD_C):
		case TD(TD_S):
		case TD(TD_Z):
			action = &tap_dance_actions[QK_TAP_DANCE_GET_INDEX(keycode)];
			if (!record->event.pressed && action->state.count && !action->state.finished) {
				tap_dance_tap_hold_t *tap_hold = (tap_dance_tap_hold_t *)action->user_data;
				tap_code16(tap_hold->tap);
			}
			break;
	}

	return true;
}

void tap_dance_tap_hold_finished(tap_dance_state_t *state, void *user_data) {
	tap_dance_tap_hold_t *tap_hold = (tap_dance_tap_hold_t *)user_data;

	if (state->pressed) {
		if (state->count == 1
#ifndef PERMISSIVE_HOLD
			&& !state->interrupted
#endif
		) {
			bool is_shift = get_mods() == MOD_BIT(KC_LSFT);
			switch (tap_hold->hold) {
				case KC_C:
					send_unicode_string(is_shift ? "Č" : "č");
					break;
				case KC_S:
					send_unicode_string(is_shift ? "Š" : "š");
					break;
				case KC_Z:
					send_unicode_string(is_shift ? "Ž" : "ž");
					break;
			}
		} else {
			register_code16(tap_hold->tap);
		}
	}
}
*/

// TAP DANCE
//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

//vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
// LAYOUT DEFINITION

#define LAYER_QWERTY 0
#define LAYER_CURSORS 1
#define LAYER_MOUSE 2

#ifdef IS_MAC
#	define PL_LGUI	KC_LALT		// on Mac ALT = CMD
#   define PL_RGUI  KC_RALT
#	define PL_LALT	KC_LGUI		// on Mac CMD = ALT
#   define PL_RALT  KC_RGUI
#	define PL_HOME	LGUI(KC_LEFT)
#	define PL_END	LGUI(KC_RGHT)
#else
#	define PL_LGUI	KC_LGUI
#	define PL_LALT	KC_LALT
#	define PL_HOME	KC_HOME
#	define PL_END	KC_END
#endif

// Momentary layer switch
#define LTC(key) LT(LAYER_CURSORS, key)
#define LT2(key) LT(LAYER_MOUSE, key)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

	[LAYER_QWERTY] = LAYOUT(
		KC_ESC,	KC_1,	KC_2,	KC_3,	KC_4,	KC_5,						    	KC_6,	KC_7,	KC_8,	KC_9,	KC_0,	KC_MINS,
		KC_TAB,	KC_Q,	KC_W,	KC_E,	KC_R,	KC_T,							    KC_Y,	KC_U,	KC_I,	KC_O,	KC_P,	LT2(KC_EQL),
		KC_LCTL,KC_A,	TDS,    KC_D,	KC_F,	KC_G,							    KC_H,	KC_J,	KC_K,	KC_L,	KC_SCLN,LTC(KC_QUOT),
		KC_LSFT,TDZ,    KC_X,	TDC,   KC_V,	KC_B,	LT2(KC_GRV),	KC_BSLS,    KC_N,	KC_M,	KC_COMM,KC_DOT,	KC_SLSH,KC_RSFT,
								PL_LGUI,LTC(KC_SPC),	PL_LALT,		KC_BSPC,    KC_ENT,	PL_RALT
	),

	[LAYER_CURSORS] = LAYOUT(
		_______,KC_F1,	KC_F2,	KC_F3,	KC_F4,	KC_F5,							    KC_F6,	KC_F7,	KC_F8,	KC_F9,	KC_F10,	KC_F11,
		_______,KC_HOME,PL_HOME,KC_UP,	PL_END,	KC_END,							    _______,_______,_______,_______,_______,KC_F12,
		_______,_______,KC_LEFT,KC_DOWN,KC_RGHT,KC_PGUP,						    KC_LCTL,KC_LALT,KC_LGUI,_______,_______,_______,
		_______,KC_LALT,KC_LGUI,_______,_______,KC_PGDN,KC_INS,			KC_DEL,	    KC_SPC,	KC_ENT,	_______,_______,_______,KC_BRK,
								_______,_______,		PL_LALT,		KC_BSPC,    KC_LBRC,	KC_RBRC
	),

	[LAYER_MOUSE] = LAYOUT(
		_______,KC_F1,  KC_F2,  KC_F3,  KC_F4,  KC_F5,                              KC_F6,  KC_F7,  KC_F8,  KC_F9,  KC_F10, KC_F11,
		QK_BOOT,_______,KC_BTN1,KC_MS_U,KC_BTN2,_______,                            _______,KC_P7,  KC_P8,  KC_P9,  _______,KC_F12,
		_______,RGB_VAI,KC_MS_L,KC_MS_D,KC_MS_R,KC_WH_U,                            KC_VOLU,KC_P4,  KC_P5,  KC_P6,  _______,_______,
		_______,_______,_______,_______,_______,KC_WH_D,KC_INS,         KC_DEL,     KC_VOLD,KC_P1,  KC_P2,  KC_P3,  _______,QK_BOOT,
								_______,_______,        _______,        KC_BSPC,    KC_P0,_______
	)
};

// LAYOUT DEFINITION
//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

//vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
// COLORS

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
            #ifdef IS_TD_ON
				case TDC: case TDS: case TDZ:
            #endif
					rgb_matrix_set_color(index, RGB_RED);
					break;

				case KC_COMM: case KC_DOT: case KC_SLSH: case KC_MINS:
				case KC_BSLS: case KC_GRV: case KC_EQL: case KC_QUOT:
				case KC_LBRC: case KC_RBRC: case KC_SCLN:
				case LTC(KC_QUOT): case LT2(KC_EQL): case LT2(KC_GRV):
					rgb_matrix_set_color(index, RGB_DARK_BLUE);
					break;

				case KC_INS: case KC_BRK:
					rgb_matrix_set_color(index, RGB_RED);
					break;

				case KC_DEL: case KC_BSPC:
					rgb_matrix_set_color(index, RGB_MAGENTA);
					break;

				case KC_LGUI: case KC_LCTL: case KC_RCTL: case KC_LALT: case KC_RALT:
					// modifiers
					rgb_matrix_set_color(index, RGB_DARK_PURPLE);
					break;

				case KC_SPC: case LTC(KC_SPC):
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

				case QK_BOOT:
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

// COLORS
//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
