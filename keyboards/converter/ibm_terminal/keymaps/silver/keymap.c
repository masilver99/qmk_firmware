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
#include <stdio.h>
#include "keycode.h"
#include "debug.h"
#include "util.h"
#include "ibm_terminal.h"
#include <hd44780.h>
#include "timer.h"
#include "string.h"
#include <avr/eeprom.h>

#define KC_MCOPY LCTL(KC_C)        // Copy
#define KC_MPSTE LCTL(KC_V)        // Copy
#define KC_MCUT  LCTL(KC_X)        // Copy

uint32_t last_save_time = 0;
uint32_t last_click_count;
const uint32_t START_CLICK_COUNT = 13900;
uint32_t click_count;
char lcd_cc[17];
//char lcd_cps[7];

uint32_t current_time;
//uint8_t cps_total_clicks;
//uint16_t cps;
//uint16_t max_cps;
//uint32_t cps_total_time;

/*  Need to implement
typedef struct {
    uint32_t start_time;
    uint8_t click_count;
} cps_struct;

cps_struct timeframe[3] = {{0, 0},{0,0},{0,0}};

uint8_t current_frame = 0;
uint8_t last_frame = 2;
*/

void display_key_count(void) {
    lcd_goto(0x40);
    sprintf(lcd_cc, "%16lu", (long)click_count);
    lcd_puts(lcd_cc);
}

/*  Need to finish the cps code at some point
void calc_cps(void) {
    
    current_time = timer_read32();
    //Get current tf
    //Perform housekeeping on timeframes i.e. is it new, is it expired, etc

    //no frames yet, so start the frames
    if (timeframe[current_frame].start_time == 0)
        timeframe[current_frame].start_time = current_time;

    if (current_time - timeframe[current_frame].start_time > 3000) {
        //frame has expired, move on to the next one.
        current_frame++;
        if (current_frame > 2){
           current_frame = 0;
        }
        last_frame++;
        if (last_frame > 2) {
            last_frame = 0;
        }
        timeframe[current_frame].start_time = current_time;
        timeframe[current_frame].click_count = 0;
    }
    
    timeframe[current_frame].click_count++;    

    cps_total_clicks = timeframe[current_frame].click_count + timeframe[last_frame].click_count;
    cps_total_time = (current_time - timeframe[last_frame].start_time);
    cps = round((double)cps_total_clicks / ((double)cps_total_time / 16.0));

    if (cps > max_cps){
        max_cps = cps;
    }

    //sprintf(lcd_cps, "%-6lu", cps_total_time); 
    sprintf(lcd_cps, "%lu/%lu  ", cps, max_cps); 
    lcd_print(16, lcd_cps);




    //calc cps based total clicks / current time / starttime of last and 
    //round and display on screen
    
}
*/
// Called with every keystroke
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    // Display key click count
    if (record->event.pressed) {
        click_count++;
        display_key_count();
        //calc_cps();  //need to finish
    }
    return true;
}


// Called when keyboard is initialized
void matrix_init_user(void) {
    click_count = eeprom_read_dword(EECONFIG_CLICK_COUNT);
    click_count = START_CLICK_COUNT > click_count ? START_CLICK_COUNT : click_count;

    last_click_count = click_count;
    last_save_time = timer_read32();
    memset(lcd_cc,0,sizeof(lcd_cc));
    //memset(lcd_cps,0,sizeof(lcd_cps)); //Need to finish
    lcd_init();
    lcd_clrscr();
    lcd_print(16, "Keyboard is up!");
}

void save_click_count(bool ignore_time)
{
    // If no more key clicks, don't bother saving
    if (last_click_count != click_count) {
        //Note: The ATMEGA32u4 has a limited number of writes before the memory becomes
        //unstable.  This routine attempts to limit the number of saves for that reason

        // if already saved in the last time period, don't bother saving
        uint32_t cur_time = timer_read32();
        if (cur_time - last_save_time > 3600000 || ignore_time) { //if unsaved for an hour (3600000) ->3 min (180000)
            //update will only write if the value differs in the eeprom
            eeprom_update_dword(EECONFIG_CLICK_COUNT, click_count);
            lcd_print(16, "Saved");
            last_click_count = click_count;
            last_save_time = cur_time;
        }
    }
}

//Called at a rate about 10 per sec
//Be very careful about adding long running routines here
void matrix_scan_user(void){
    save_click_count(false);
}

void suspend_power_down_user(void)
{
    save_click_count(true);
    lcd_print(16, "--> Suspended <-");
}

void suspend_wakeup_init_user(void)
{
    display_key_count();
    lcd_print(16, "Resumed");
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    // Layer 0

    KEYMAP(
                        KC_F13, KC_F14, KC_F15, KC_F16, KC_F17, KC_F18, KC_F19, KC_F20, KC_F21, KC_F22, KC_F23, KC_F24,
                        KC_F1,  KC_F2,  KC_F3,  KC_F4,  KC_F5,  KC_F6,  KC_F7,  KC_F8,  KC_F9,  KC_F10, KC_F11, KC_F12,

    KC_ESC, KC_PSCR,    KC_GRV, KC_1,   KC_2,   KC_3,   KC_4,   KC_5,   KC_6,   KC_7,   KC_8,   KC_9,   KC_0,   KC_MINS,KC_EQL, KC_NO,  KC_BSPC,    KC_INS, KC_HOME,KC_PGUP,    KC_NLCK,KC_PSLS,KC_PAST,KC_PMNS,
    KC_CALC,KC_WHOM,    KC_TAB, KC_Q,   KC_W,   KC_E,   KC_R,   KC_T,   KC_Y,   KC_U,   KC_I,   KC_O,   KC_P,   KC_LBRC,KC_RBRC,        KC_BSLS,      KC_DEL, KC_END, KC_PGDN,    KC_P7,  KC_P8,  KC_P9,  KC_PPLS,
    KC_VOLU,KC_MCUT,    KC_LGUI,KC_A,   KC_S,   KC_D,   KC_F,   KC_G,   KC_H,   KC_J,   KC_K,   KC_L,   KC_SCLN,KC_QUOT,        KC_NO  ,KC_ENT,             KC_UP,              KC_P4,  KC_P5,  KC_P6,  KC_PCMM,
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
