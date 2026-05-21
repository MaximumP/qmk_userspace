/*
 * Copyright 2021 Quentin LEBASTARD <qlebastard@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdint.h>
#include <stdbool.h>
#include QMK_KEYBOARD_H
#include "keymap_german.h"
#include "sendstring_german.h"
#include "raw_hid.h"
#include "rgb_matrix.h"

struct led_color {
    bool active;
    uint8_t red;
    uint8_t green;
    uint8_t blue;
} led_color = {false, 0, 0, 0};

enum {
    TD_Q_RUN,
    TD_UML,
};

enum send_string_codes {
    EN = SAFE_RANGE,
    DSC,
    ISC
};

const int _SCROLL_CPI_STEP = 10;
static int _SCROLL_CPI = 50;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case EN:
            if (record->event.pressed) {
                SEND_STRING("maxregister@online.de");
            }

            return false;
        case DSC:
            if (record->event.pressed && _SCROLL_CPI > _SCROLL_CPI_STEP) {
                _SCROLL_CPI -= _SCROLL_CPI_STEP;
            }

            return false;
        case ISC:
            if (record->event.pressed) {
                _SCROLL_CPI += _SCROLL_CPI_STEP;
            }
        default:
            return true;
    }
    return true;
}

tap_dance_action_t tap_dance_actions[] = {
    [TD_Q_RUN] = ACTION_TAP_DANCE_DOUBLE(KC_Q, A(KC_F2)),
};

static bool scrolling_mode = false;
const int _CPI = 900;

layer_state_t layer_state_set_user(layer_state_t state) {
    switch (get_highest_layer(state)) {
        case 4:  // If we're on the _RAISE layer enable scrolling mode
            scrolling_mode = true;
            pointing_device_set_cpi(_SCROLL_CPI);
            break;
        default:
            if (scrolling_mode) {  // check if we were scrolling before and set disable if so
                scrolling_mode = false;
                pointing_device_set_cpi(_CPI);
            }
            break;
    }
    return state;
}

report_mouse_t pointing_device_task_user(report_mouse_t mouse_report) {
    if (scrolling_mode) {
        mouse_report.h = mouse_report.x;
        mouse_report.v = mouse_report.y * -1;
        mouse_report.x = 0;
        mouse_report.y = 0;
    }
    return mouse_report;
}

bool caps_word_press_user(uint16_t keycode) {
    switch (keycode) {
        // Keycodes that continue Caps Word, with shift applied.
        case KC_A ... KC_Z:
        case DE_MINS:
            add_weak_mods(MOD_BIT(KC_LSFT));  // Apply shift to next key.
            return true;

        // Keycodes that continue Caps Word, without shifting.
        case KC_1 ... KC_0:
        case KC_BSPC:
        case KC_DEL:
        case KC_UNDS:
        case KC_SEMICOLON:
            return true;

        default:
            return false;  // Deactivate Caps Word.
    }
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  // BASE
  [0] = LAYOUT_split_3x5_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
     TD(TD_Q_RUN),    KC_W,    KC_E,    KC_R,    KC_T,                         KC_Y,    KC_U,    KC_I,    KC_O,   KC_P,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
     GUI_T(KC_A),     ALT_T(KC_S),    CTL_T(KC_D),    SFT_T(KC_F),    KC_G,    KC_H,    RSFT_T(KC_J),    CTL_T(KC_K),    ALT_T(KC_L), GUI_T(DE_ADIA),
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
     LT(2, KC_Z),     RALT_T(KC_X),   KC_C,    KC_V,    KC_B,              KC_N,    KC_M, KC_COMM,  RALT_T(KC_DOT), DE_MINS,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                       LT(4, KC_ESC), LT(2, KC_SPC),  LT(3, KC_TAB),  LT(6, KC_ENT), LT(5, KC_BSPC), LT(7, KC_DEL)
                                      //`--------------------------'  `--------------------------'
  ),
  // EXTRA
  [1] = LAYOUT_split_3x5_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
     KC_Q,     KC_W,    KC_I,    KC_R,    KC_T,                                 KC_F1,    KC_F2,   KC_F3,  KC_F4,   KC_F5,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
     GUI_T(KC_A),     ALT_T(KC_S),    CTL_T(KC_D),    SFT_T(KC_F),    KC_G,     DF(0),   KC_BTN1, KC_BTN3, KC_BTN2, KC_F6,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
     KC_1,    KC_2,     KC_3,    KC_4,    KC_O,                                 KC_F7, LSFT(KC_1), LSFT(KC_2), LSFT(KC_3), LSFT(KC_4),
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+----CW_TOGG,  ----+--------+--------+--------+--------+--------|
                       LT(4, KC_ESC), LT(2, KC_SPC),  LT(3, KC_TAB),  LT(6, KC_ENT), LT(5, KC_BSPC), LT(7, KC_DEL)
                                      //`--------------------------'  `--------------------------'
  ),
  // NAV
  [2] = LAYOUT_split_3x5_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
      KC_EXLM,   KC_AT, KC_HASH,  KC_DLR, KC_PERC,                              KC_CAPS, LSFT(KC_INS), LCTL(KC_INS), LSFT(KC_DEL), KC_RPRN,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_LGUI, KC_LALT, KC_LCTL, KC_LSFT, DF(1),                              KC_LEFT, KC_DOWN, KC_UP, KC_RGHT, CW_TOGG,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      XXXXXXX, QK_BOOT, KC_KB_POWER, QK_RBT, XXXXXXX,                              KC_INS, KC_HOME, KC_PGDN, KC_PGUP, KC_END,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          XXXXXXX,  XXXXXXX,  XXXXXXX,     XXXXXXX, XXXXXXX, XXXXXXX
                                      //`--------------------------'  `--------------------------'
  ),
  // MOUSE
  [3] = LAYOUT_split_3x5_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
              XXXXXXX, KC_A,    KC_S,    KC_W,    KC_D,                        RGB_TOG, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_LGUI, KC_LALT, KC_LCTL, KC_LSFT, RGB_M_R,                              RGB_RMOD, KC_BTN1, KC_BTN3, KC_BTN2, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      XXXXXXX, XXXXXXX, RGB_TOG, RGB_RMOD, RGB_MOD,                             RGB_MOD, XXXXXXX, KC_WH_D, KC_WH_U, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          XXXXXXX,  XXXXXXX, XXXXXXX,     EN, XXXXXXX, XXXXXXX
                                      //`--------------------------'  `--------------------------'
  ),
  // MEDIA
  [4] = LAYOUT_split_3x5_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
      DSC, ISC, XXXXXXX, XXXXXXX, LALT(KC_INS),                         XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_LGUI, KC_LALT, KC_LCTL, KC_LSFT, XXXXXXX,                              LALT(KC_F1), KC_MPRV, KC_VOLD, KC_VOLU, KC_MNXT,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                              LALT(KC_F12), XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          XXXXXXX,  XXXXXXX, XXXXXXX,     KC_MSTP, KC_MPLY, KC_MUTE
                                      //`--------------------------'  `--------------------------'
  ),
  // NUM
  [5] = LAYOUT_split_3x5_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
      DE_UDIA, KC_7, KC_8, KC_9, KC_RBRC,                                       LGUI(KC_PGUP), XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      DE_ODIA, KC_4, KC_5, KC_6, KC_EQL,                                        LGUI(KC_PGDN), KC_LSFT, KC_LCTL, KC_LALT, KC_LGUI,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_GRV, KC_1, KC_2, KC_3, KC_BSLS,                                        LGUI(KC_LCTL), XXXXXXX, KC_COMM,  RALT_T(KC_DOT), XXXXXXX,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          KC_DOT,  KC_0, KC_MINS,     XXXXXXX, XXXXXXX, XXXXXXX
                                      //`--------------------------'  `--------------------------'
  ),
  // SYMBOL
  [6] = LAYOUT_split_3x5_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
      KC_LBRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RCBR,                              LALT(KC_KP_4), XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      DE_LABK, DE_RABK, DE_PIPE, KC_CIRC, KC_PLUS,                     XXXXXXX, KC_LSFT, KC_LCTL, KC_LALT, KC_LGUI,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      DE_GRV, KC_EXLM, KC_AT, KC_HASH, KC_PIPE,                                XXXXXXX, XXXXXXX, XXXXXXX, KC_RALT, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          KC_LPRN,  KC_RPRN, KC_UNDS,     XXXXXXX, XXXXXXX, XXXXXXX
                                      //`--------------------------'  `--------------------------'
  ),
  // FUN
  [7] = LAYOUT_split_3x5_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
      KC_F12, KC_F7, KC_F8, KC_F9, KC_PSCR,                                     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_F11, KC_F4, KC_F5, KC_F6, KC_SCRL,                                     XXXXXXX, KC_LSFT, KC_LCTL, KC_LALT, KC_LGUI,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_F10, KC_F1, KC_F2, KC_F3, KC_PAUS,                                     XXXXXXX, XXXXXXX, KC_WH_D, KC_WH_U, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          KC_APP,  KC_SPC, KC_TAB,     XXXXXXX, XXXXXXX, XXXXXXX
                                      //`--------------------------'  `--------------------------'
  ),
};

bool rgb_matrix_indicators_user(void) {
    if (led_color.active) {
        rgb_matrix_set_color_all(led_color.red, led_color.green, led_color.blue);
        return false;
    }
    return false;
}

void raw_hid_receive(uint8_t *data, uint8_t length) {
    rgb_matrix_enable();
    led_color.active = data[0];
    led_color.red = data[1];
    led_color.green = data[2];
    led_color.blue = data[3];
    raw_hid_send(data, length);
}
