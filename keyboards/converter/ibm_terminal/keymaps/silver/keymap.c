/*
Copyright 2012 Jun Wako <wakojun@gmail.com>

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

#include <stdint.h>
#include <stdbool.h>
#include "keycode.h"
#include "print.h"
#include "debug.h"
#include "util.h"
#include "ibm_terminal.h"
#include <hd44780.h>
#include "timer.h"

#define KC_MCOPY LCTL(KC_C)        // Copy
#define KC_MPSTE LCTL(KC_V)        // Copy
#define KC_MCUT  LCTL(KC_X)        // Copy

uint32_t last_timer = 0;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    // Display key click count
    if (event.pressed) {
        lcd_goto(0x40);
        sprintf(lcd2, "%16d", (int)++click_count);
        lcd_puts(lcd2); 
        save_click_count();  //May want to call this elsewhere too, like on a timer.
    }
    return true;
}

void save_click_count()
{
    uint32_t cur_timer = timer_read32();
    if (cur_time - last_time > 3600000) //if unsaved for an hour
    {
        //save new time here
        last_time = cur_time;
    }
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    // Layer 0

    KEYMAP(
                        KC_F13, KC_F14, KC_F15, KC_F16, KC_F17, KC_F18, KC_F19, KC_F20, KC_F21, KC_F22, KC_F23, KC_F24,
                        KC_F1,  KC_F2,  KC_F3,  KC_F4,  KC_F5,  KC_F6,  KC_F7,  KC_F8,  KC_F9,  KC_F10, KC_F11, KC_F12,

    KC_ESC, KC_PSCR,    KC_GRV, KC_1,   KC_2,   KC_3,   KC_4,   KC_5,   KC_6,   KC_7,   KC_8,   KC_9,   KC_0,   KC_MINS,KC_EQL, KC_NO,  KC_BSPC,    KC_INS, KC_HOME,KC_PGUP,    KC_NLCK,KC_PSLS,KC_PAST,KC_PMNS,
    KC_CALC,KC_WHOM,    KC_TAB, KC_Q,   KC_W,   KC_E,   KC_R,   KC_T,   KC_Y,   KC_U,   KC_I,   KC_O,   KC_P,   KC_LBRC,KC_RBRC,        KC_NO,      KC_DEL, KC_END, KC_PGDN,    KC_P7,  KC_P8,  KC_P9,  KC_PPLS,
    KC_VOLU,KC_MCUT,    KC_LGUI,KC_A,   KC_S,   KC_D,   KC_F,   KC_G,   KC_H,   KC_J,   KC_K,   KC_L,   KC_SCLN,KC_QUOT,        KC_BSLS,KC_ENT,             KC_UP,              KC_P4,  KC_P5,  KC_P6,  KC_PCMM,
    KC_VOLD,KC_MCOPY,   KC_LSFT,KC_LSFT,KC_Z,   KC_X,   KC_C,   KC_V,   KC_B,   KC_N,   KC_M,   KC_COMM,KC_DOT, KC_SLSH,        KC_NO,  KC_RSFT,    KC_LEFT,KC_HOME,KC_RGHT,    KC_P1,  KC_P2,  KC_P3,  KC_PENT,
    KC_MUTE,KC_MPSTE,   KC_LCTL,        KC_LALT,                        KC_SPC,                                         KC_RALT,        KC_RCTL,             KC_DOWN,            KC_NO,  KC_P0,  KC_PDOT,KC_NO
    ),

/* 101-key keymaps
 */
    /* 0: default
     * ,---.   ,---------------. ,---------------. ,---------------. ,-----------.
     * |Esc|   |F1 |F2 |F3 |F4 | |F5 |F6 |F7 |F8 | |F9 |F10|F11|F12| |PrS|ScL|Pau|
     * `---'   `---------------' `---------------' `---------------' `-----------'
     * ,-----------------------------------------------------------. ,-----------. ,---------------.
     * |  `|  1|  2|  3|  4|  5|  6|  7|  8|  9|  0|  -|  =|Backspa| |Ins|Hom|PgU| |NmL|  /|  *|  -|
     * |-----------------------------------------------------------| |-----------| |---------------|
     * |Tab  |  Q|  W|  E|  R|  T|  Y|  U|  I|  O|  P|  [|  ]|    \| |Del|End|PgD| |  7|  8|  9|   |
     * |-----------------------------------------------------------| `-----------' |-----------|  +|
     * |CapsLo|  A|  S|  D|  F|  G|  H|  J|  K|  L|  ;|  '|Return  |               |  4|  5|  6|   |
     * |-----------------------------------------------------------|     ,---.     |---------------|
     * |Shift   |  Z|  X|  C|  V|  B|  N|  M|  ,|  ,|  /|Shift     |     |Up |     |  1|  2|  3|   |
     * |-----------------------------------------------------------| ,-----------. |-----------|Ent|
     * |Ctrl|    |Alt |          Space              |Alt |    |Ctrl| |Lef|Dow|Rig| |      0|  .|   |
     * `----'    `---------------------------------------'    `----' `-----------' `---------------'
     */
/*
    KEYMAP_101(
     KC_ESC,       KC_F1,  KC_F2,  KC_F3,  KC_F4,  KC_F5,  KC_F6,  KC_F7,  KC_F8,  KC_F9, KC_F10, KC_F11, KC_F12,        KC_PSCR,KC_SLCK, KC_BRK,

     KC_GRV,   KC_1,   KC_2,   KC_3,   KC_4,   KC_5,   KC_6,   KC_7,   KC_8,   KC_9,   KC_0,KC_MINS, KC_EQL,KC_BSPC,      KC_INS,KC_HOME,KC_PGUP,     KC_NLCK,KC_PSLS,KC_PAST,KC_PMNS,
     KC_TAB,   KC_Q,   KC_W,   KC_E,   KC_R,   KC_T,   KC_Y,   KC_U,   KC_I,   KC_O,   KC_P,KC_LBRC,KC_RBRC,KC_BSLS,      KC_DEL, KC_END,KC_PGDN,       KC_P7,  KC_P8,  KC_P9,
    KC_CAPS,   KC_A,   KC_S,   KC_D,   KC_F,   KC_G,   KC_H,   KC_J,   KC_K,   KC_L,KC_SCLN,KC_QUOT,         KC_ENT,                                    KC_P4,  KC_P5,  KC_P6,KC_PPLS,
    KC_LSFT,        KC_Z,   KC_X,   KC_C,   KC_V,   KC_B,   KC_N,   KC_M,   KC_COMM, KC_DOT,KC_SLSH,        KC_RSFT,               KC_UP,               KC_P1,  KC_P2,  KC_P3,
    KC_LCTL,     KC_LALT,                         KC_SPC,                                   KC_RALT,        KC_RCTL,     KC_LEFT,KC_DOWN,KC_RGHT,       KC_P0,        KC_PDOT,KC_PENT
    ),
*/
};

const uint16_t PROGMEM fn_actions[] = {
      ACTION_MODS_KEY(MOD_RCTL, KC_C)
};
