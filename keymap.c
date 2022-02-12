/* Copyright 2020 Jay Greco
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

#include QMK_KEYBOARD_H
#include "action_layer.h"
#include "remote_kb.h"
#include "bitc_led.h"

#define _BASE     0
#define _FUNC     1


enum custom_keycodes {
  PROG = SAFE_RANGE,
};

enum td_keycodes {
    TD_KNOB_LAYER
};

// Tap Dance definitions
qk_tap_dance_action_t tap_dance_actions[] = {
    // Tap once for KC_KP_SLASH, twice for _FUNC layer
    [TD_KNOB_LAYER] = ACTION_TAP_DANCE_LAYER_TOGGLE(KC_KP_SLASH, 1),
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  // Base layer (numpad)
  [_BASE] = LAYOUT(
      TD(TD_KNOB_LAYER),  KC_KP_ASTERISK, KC_KP_MINUS, \
  KC_KP_7, KC_KP_8,  KC_KP_9,        KC_KP_PLUS, \
  KC_KP_4, KC_KP_5,  KC_KP_6,        KC_NO, \
  KC_KP_1, KC_KP_2,  KC_KP_3,        KC_KP_ENTER, \
  KC_KP_0, KC_NO,    KC_KP_DOT,      KC_NO \
  ),
  // Function layer (numpad)
  [_FUNC] = LAYOUT(
       TO(_BASE), RGB_TOG, KC_NLCK,
    KC_NO, KC_NO, RGB_MOD, KC_CALC,
    KC_NO, KC_NO, RGB_HUI, KC_NO,
    KC_NO, KC_NO, RGB_SAI, KC_NO,
    PROG,  KC_NO, RGB_VAI, KC_NO
  ),
};

#ifdef OLED_DRIVER_ENABLE
oled_rotation_t oled_init_user(oled_rotation_t rotation) { return OLED_ROTATION_180; }

// OLED module displays current layer state: Base/Default of Func/RGB
void oled_task_user(void) {
    oled_write_P(PSTR("Layer: "), false);

    switch (get_highest_layer(layer_state)) {
        case _BASE:
            oled_write_P(PSTR("Default\n"), false);
            break;
        case _FUNC:
            oled_write_P(PSTR("RGB\n"), false);
            break;
        default:
            // Or use the write_ln shortcut over adding '\n' to the end of your string
            oled_write_ln_P(PSTR("Undefined"), false);
    }

}
#endif

void matrix_init_user(void) {
  matrix_init_remote_kb();
}

void matrix_scan_user(void) {
  matrix_scan_remote_kb();
}



bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  process_record_remote_kb(keycode, record);
  switch(keycode) {
    // Sets numpad to flashing mode, or can hold reset button under Numpad
    case PROG:
      if (record->event.pressed) {
        set_bitc_LED(LED_DIM);
        rgblight_disable_noeeprom();
        bootloader_jump(); //jump to bootloader
      }
      return true;
      break;
    // After calculator is launched, moves back to Base/Default layer so can immediately start typing in numbers
    case KC_CALC:
      if (record->event.pressed) {
        tap_code(keycode);
        layer_move(_BASE);
      }
      return false;
      break;

    default:
      return true;
    break;
  }
}


bool encoder_update_user(uint8_t index, bool clockwise) {
  switch (get_highest_layer(layer_state)) {
        // In Base/Default layer, encoder changes volume
        case _BASE:
            if (clockwise) {
              tap_code(KC_VOLD);
            } else {
              tap_code(KC_VOLU);
            }
            break;
        // In Func/RGB layer, encoder scrolls up or down by page
        case _FUNC:
            if (clockwise) {
              tap_code(KC_PGDN);
            } else {
              tap_code(KC_PGUP);
            }
            break;
    }
  return true;
}


// Can be ignored: used when trying to troubleshoot Num-Lock
layer_state_t layer_state_set_user(layer_state_t state) {
    switch (get_highest_layer(state)) {
    case _FUNC:
        //unregister_code(KC_NLCK);
        break;
    default: //  for any other layers, or the default layer
        //register_code(KC_NLCK);
        //unregister_code(KC_NLCK);
        break;
    }
  return state;
}

// Turn Bit-C LED off if Num-Lock is off, on if Num-Lock is on
void led_set_kb(uint8_t usb_led) {
  if (usb_led & (1<<USB_LED_NUM_LOCK))
    set_bitc_LED(LED_DIM);
  else
    set_bitc_LED(LED_OFF);
  // After changing Num-Lock, moves back to Base/Default Layer
  layer_move(_BASE);
}
