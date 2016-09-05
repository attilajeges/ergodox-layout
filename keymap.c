/*
 * algernon's ErgoDox EZ layout, please see the readme.md file!
 */

#include <stdarg.h>
#include "ergodox.h"
#include "led.h"
#include "debug.h"
#include "action_layer.h"
#include "action_util.h"
#include "mousekey.h"
#include "timer.h"
#include "eeconfig.h"
#include "wait.h"
#include "version.h"

/* Layers */

enum {
  BASE = 0,
  ADORE,
  ARRW,
  APPSEL,
  HUN,
  NMDIA,
};

/* Macros */

enum {
  NONE = 0,
  // Buttons that do extra stuff
  A_GUI,
  A_MPN,

  // TMUX keys
  A_TMUXP,

  // Application select keys
  APP_SLK, // Slack
  APP_EMCS, // Emacs
  APP_TERM, // Terminal
  APP_CHRM, // Chrome
  APP_MSIC, // Music

  // Diagonal mouse movement
  A_MUL,
  A_MUR,
  A_MDL,
  A_MDR,

  // Mouse acceleration
  A_ACL0,
  A_ACL1,
  A_ACL2,

  // Hungarian layer keys
  HU_AA, // Á
  HU_OO, // Ó
  HU_EE, // É
  HU_UU, // Ú
  HU_II, // Í
  HU_OE, // Ö
  HU_UE, // Ü
  HU_OEE, // Ő
  HU_UEE, // Ű
};

/* Fn keys */

enum {
  F_BSE = 0,
  F_HUN,
  F_GUI,
  F_SFT,
  F_ALT,
  F_CTRL
};

/* Custom keycodes */

enum {
  CT_CLN = 0,
  CT_TA,
  CT_LBP,
  CT_RBP,
  CT_TMUX,

  // Function / number keys
  KF_1, // 1, F1
  KF_2, // 2, F2
  KF_3, // ...
  KF_4,
  KF_5,
  KF_6,
  KF_7,
  KF_8,
  KF_9,
  KF_10,
};

/* States & timers */

uint16_t gui_timer = 0;

uint16_t kf_timers[12];

#if KEYLOGGER_ENABLE
# ifdef AUTOLOG_ENABLE
bool log_enable = true;
# else
bool log_enable = false;
# endif
#endif

bool time_travel = false;

/* The Keymap */

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

/* Keymap 0: Base Layer
 *
 * ,-----------------------------------------------------.           ,-----------------------------------------------------.
 * | Next/Prev | 1 F1 | 2 F2 | 3 F3 | 4 F4 | 5 F5 | Apps |           |  F12 | 6 F6 | 7 F7 | 8 F8 | 9 F9 | 0 F10|       F11 |
 * |-----------+------+------+------+------+-------------|           |------+------+------+------+------+------+-----------|
 * |         ~ |   '  |   ,  |   .  |   P  |   Y  |   (  |           |  )   |   F  |   G  |   C  |   R  |  L   | \         |
 * |-----------+------+------+------+------+------|   [  |           |  ]   |------+------+------+------+------+-----------|
 * | Tab/ARROW |   A  |   O  |   E  |   U  |   I  |------|           |------|   D  |   H  |   T  |   N  |  S   | = / Arrow |
 * |-----------+------+------+------+------+------| tmux |           | tmux |------+------+------+------+------+-----------|
 * | Play/Pause|   /  |   Q  |   J  |   K  |   X  |      |           | Pane |   B  |   M  |   W  |   V  |  Z   |      Stop |
 * `-----------+------+------+------+------+-------------'           `-------------+------+------+------+------+-----------'
 *     |       |      |      |      |   :  |                                       |   -  |      |      |      |       |
 *     `-----------------------------------'                                       `-----------------------------------'
 *                                         ,-------------.           ,-------------.
 *                                         | LAlt | GUI  |           | MDIA | Del  |
 *                                  ,------|------|------|           |------+------+------.
 *                                  |      |      | Ctrl |           | LEAD |      |      |
 *                                  |Backsp|LShift|------|           |------| Enter| Space|
 *                                  |      |      | ESC  |           | HUN  |      |      |
 *                                  `--------------------'           `--------------------'
 */
[BASE] = KEYMAP(
// left hand
 M(A_MPN)           ,TD(KF_1)    ,TD(KF_2)    ,TD(KF_3),TD(KF_4),TD(KF_5),KC_APP
,KC_GRV             ,KC_QUOT     ,KC_COMM     ,KC_DOT  ,KC_P    ,KC_Y    ,TD(CT_LBP)
,TD(CT_TA)          ,KC_A        ,KC_O        ,KC_E    ,KC_U    ,KC_I
,KC_MPLY            ,KC_SLSH     ,KC_Q        ,KC_J    ,KC_K    ,KC_X    ,TD(CT_TMUX)
,KC_NO              ,KC_NO       ,KC_NO       ,KC_NO   ,TD(CT_CLN)

                                                            ,F(F_ALT),F(F_GUI)
                                                                     ,F(F_CTRL)
                                                    ,KC_BSPC,F(F_SFT),KC_ESC

                                                                // right hand
                                                               ,KC_F12    ,TD(KF_6),TD(KF_7)  ,TD(KF_8),TD(KF_9),TD(KF_10),KC_F11
                                                               ,TD(CT_RBP),KC_F    ,KC_G      ,KC_C    ,KC_R    ,KC_L     ,KC_BSLS
                                                                          ,KC_D    ,KC_H      ,KC_T    ,KC_N    ,KC_S     ,KC_EQL
                                                               ,M(A_TMUXP),KC_B    ,KC_M      ,KC_W    ,KC_V    ,KC_Z     ,KC_MSTP
                                                                                   ,KC_MINS   ,KC_NO   ,KC_NO   ,KC_NO    ,KC_NO

                                                               ,OSL(NMDIA),KC_DEL
                                                               ,KC_LEAD
                                                               ,F(F_HUN)  ,KC_ENT ,KC_SPC
    ),

/* Keymap 1: Adore layer
 *
 * ,-----------------------------------------------------.           ,-----------------------------------------------------.
 * | Play/Pause| 1 F1 | 2 F2 | 3 F3 | 4 F4 | 5 F5 | Apps |           |  F12 | 6 F6 | 7 F7 | 8 F8 | 9 F9 | 0 F10|       F11 |
 * |-----------+------+------+------+------+-------------|           |------+------+------+------+------+------+-----------|
 * |        `~ |   Y  |   W  |   G  |   L  |   M  |   (  |           |  )   |   F  |   H  |   C  |   P  |  X   | \         |
 * |-----------+------+------+------+------+------|   [  |           |  ]   |------+------+------+------+------+-----------|
 * | Tab/Arrow |   A  |   O  |   E  |   I  |   U  |------|           |------|   D  |   R  |   T  |   N  |  S   | =         |
 * |-----------+------+------+------+------+------| tmux |           | tmux |------+------+------+------+------+-----------|
 * |           |   Z  |   Q  |   '  |   ,  |   .  |      |           | pane |   B  |   K  |   V  |   J  |  /   |           |
 * `-----------+------+------+------+------+-------------'           `-------------+------+------+------+------+-----------'
 *     |       |      |      |      |   :  |                                       |   -  |      |      |      |       |
 *     `-----------------------------------'                                       `-----------------------------------'
 *                                         ,-------------.           ,-------------.
 *                                         | LAlt | GUI  |           | MDIA | Del  |
 *                                  ,------|------|------|           |------+------+------.
 *                                  |      |      | Ctrl |           | HUN  |      |      |
 *                                  |Backsp|LShift|------|           |------| Enter| Space|
 *                                  |      |      | ESC  |           | LEAD |      |      |
 *                                  `--------------------'           `--------------------'
 */
[ADORE] = KEYMAP(
// left hand
 KC_MPLY            ,TD(KF_1)    ,TD(KF_2)    ,TD(KF_3),TD(KF_4),TD(KF_5),KC_APP
,KC_GRV             ,KC_Y        ,KC_W        ,KC_G    ,KC_L    ,KC_M   ,TD(CT_LBP)
,TD(CT_TA)          ,KC_A        ,KC_O        ,KC_E    ,KC_I    ,KC_U
,KC_NO              ,KC_Z        ,KC_Q        ,KC_QUOT ,KC_COMM ,KC_DOT ,TD(CT_TMUX)
,KC_NO              ,KC_NO       ,KC_NO       ,KC_NO   ,TD(CT_CLN)

                                                            ,F(F_ALT),F(F_GUI)
                                                                     ,F(F_CTRL)
                                                    ,KC_BSPC,F(F_SFT),KC_ESC

                                                                // right hand
                                                               ,KC_F12    ,TD(KF_6),TD(KF_7),TD(KF_8),TD(KF_9),TD(KF_10),KC_F11
                                                               ,TD(CT_RBP),KC_F    ,KC_H    ,KC_C    ,KC_P    ,KC_X     ,KC_BSLS
                                                                          ,KC_D    ,KC_R    ,KC_T    ,KC_N    ,KC_S     ,KC_EQL
                                                               ,M(A_TMUXP),KC_B    ,KC_K    ,KC_V    ,KC_J    ,KC_SLSH  ,KC_NO
                                                                                   ,KC_MINS ,KC_NO   ,KC_NO   ,KC_NO    ,KC_NO

                                                               ,OSL(NMDIA),KC_DEL
                                                               ,F(F_HUN)
                                                               ,KC_LEAD   ,KC_ENT ,KC_SPC
    ),

/* Keymap 2: Arrow layer
 *
 * ,-----------------------------------------------------.           ,-----------------------------------------------------.
 * |           |      |      |      |      |      |      |           |      |      |      |      |      |      |           |
 * |-----------+------+------+------+------+-------------|           |------+------+------+------+------+------+-----------|
 * |           |      |      |      |      |      |      |           |      |      | Home |  Up  |  End |      |           |
 * |-----------+------+------+------+------+------|      |           |      |------+------+------+------+------+-----------|
 * |           |      |      |      |      |      |------|           |------|      | Left | Down | Rght |      |           |
 * |-----------+------+------+------+------+------|      |           |      |------+------+------+------+------+-----------|
 * |           |      |      |      |      |      |      |           |      |      |      |      |      |      |           |
 * `-----------+------+------+------+------+-------------'           `-------------+------+------+------+------+-----------'
 *      |      |      |      |      |      |                                       |      |      |      |      |      |
 *      `----------------------------------'                                       `----------------------------------'
 *                                         ,-------------.           ,-------------.
 *                                         |      |      |           |      |      |
 *                                  ,------|------|------|           |------+------+------.
 *                                  |      |      |      |           |      |      |      |
 *                                  | Enter|      |------|           |------| PgUp | PgDn |
 *                                  |      |      |      |           |      |      |      |
 *                                  `--------------------'           `--------------------'
 */

[ARRW] = KEYMAP(
// left hand
 KC_TRNS ,KC_TRNS    ,KC_TRNS   ,KC_TRNS    ,KC_TRNS    ,KC_TRNS    ,KC_TRNS
,KC_TRNS ,KC_TRNS    ,KC_TRNS   ,KC_TRNS    ,KC_TRNS    ,KC_TRNS    ,KC_TRNS
,KC_TRNS ,KC_TRNS    ,KC_TRNS   ,KC_TRNS    ,KC_TRNS    ,KC_TRNS
,KC_TRNS ,KC_TRNS    ,KC_TRNS   ,KC_TRNS    ,KC_TRNS    ,KC_TRNS    ,KC_TRNS
,KC_TRNS ,KC_TRNS    ,KC_TRNS   ,KC_TRNS    ,KC_TRNS

                                             ,KC_TRNS ,KC_TRNS
                                                      ,KC_TRNS
                                    ,KC_ENT  ,KC_TRNS ,KC_TRNS

                                                                // right hand
                                                               ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS
                                                               ,KC_TRNS ,KC_TRNS ,KC_HOME ,KC_UP   ,KC_END  ,KC_TRNS ,KC_TRNS
                                                                        ,KC_TRNS ,KC_LEFT ,KC_DOWN ,KC_RGHT ,KC_TRNS ,KC_TRNS
                                                               ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS
                                                                                 ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS

                                                               ,KC_TRNS ,KC_TRNS
                                                               ,KC_TRNS
                                                               ,KC_TRNS ,KC_PGUP ,KC_PGDN
    ),

/* Keymap 3: Application select layer
 *
 * ,-----------------------------------------------------.           ,-----------------------------------------------------.
 * |           |Music |Slack |Emacs |Term  |Chrome|      |           |      |      |      |      |      |      |           |
 * |-----------+------+------+------+------+-------------|           |------+------+------+------+------+------+-----------|
 * |           |      |      |      |      |      |      |           |      |      |      |      |      |      |           |
 * |-----------+------+------+------+------+------|      |           |      |------+------+------+------+------+-----------|
 * |           |      |      |      |      |      |------|           |------|      |      |      |      |      |           |
 * |-----------+------+------+------+------+------|      |           |      |------+------+------+------+------+-----------|
 * |           |      |      |      |      |      |      |           |      |      |      |      |      |      |           |
 * `-----------+------+------+------+------+-------------'           `-------------+------+------+------+------+-----------'
 *      |      |      |      |      |      |                                       |      |      |      |      |      |
 *      `----------------------------------'                                       `----------------------------------'
 *                                         ,-------------.           ,-------------.
 *                                         |      |      |           |      |      |
 *                                  ,------|------|------|           |------+------+------.
 *                                  |      |      |      |           |      |      |      |
 *                                  |      |      |------|           |------|      |      |
 *                                  |      |      |      |           |      |      |      |
 *                                  `--------------------'           `--------------------'
 */

[APPSEL] = KEYMAP(
// left hand
 KC_TRNS ,M(APP_MSIC),M(APP_SLK),M(APP_EMCS),M(APP_TERM),M(APP_CHRM),KC_TRNS
,KC_TRNS ,KC_TRNS    ,KC_TRNS   ,KC_TRNS    ,KC_TRNS    ,KC_TRNS    ,KC_TRNS
,KC_TRNS ,KC_TRNS    ,KC_TRNS   ,KC_TRNS    ,KC_TRNS    ,KC_TRNS
,KC_TRNS ,KC_TRNS    ,KC_TRNS   ,KC_TRNS    ,KC_TRNS    ,KC_TRNS    ,KC_TRNS
,KC_TRNS ,KC_TRNS    ,KC_TRNS   ,KC_TRNS    ,KC_TRNS

                                             ,KC_TRNS ,KC_TRNS
                                                      ,KC_TRNS
                                    ,KC_TRNS ,KC_TRNS ,KC_TRNS

                                                                // right hand
                                                               ,KC_TRNS ,KC_NO   ,KC_NO   ,KC_NO   ,KC_NO   ,KC_NO   ,KC_TRNS
                                                               ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS
                                                                        ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS
                                                               ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS
                                                                                 ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS

                                                               ,KC_TRNS ,KC_TRNS
                                                               ,KC_TRNS
                                                               ,KC_TRNS ,KC_TRNS  ,KC_TRNS
    ),


/* Keymap 4: Hungarian Layer
 *
 * ,-----------------------------------------------------.           ,-----------------------------------------------------.
 * |           |      |      |      |      |      |      |           |      |      |      |      |      |      |           |
 * |-----------+------+------+------+------+-------------|           |------+------+------+------+------+------+-----------|
 * |           |      |  Ő   |      |  Ű   |      |      |           |      |      |      |      |      |      |           |
 * |-----------+------+------+------+------+------|      |           |      |------+------+------+------+------+-----------|
 * |           |   Á  |  Ó   |  É   |  Ú   |  Í   |------|           |------|      |      |      |      |      |           |
 * |-----------+------+------+------+------+------|      |           |      |------+------+------+------+------+-----------|
 * |           |      |  Ö   |      |  Ü   |      |      |           |      |      |      |      |      |      |           |
 * `-----------+------+------+------+------+-------------'           `-------------+------+------+------+------+-----------'
 *      |      |      |      |      |      |                                       |      |      |      |      |      |
 *      `----------------------------------'                                       `----------------------------------'
 *                                         ,-------------.           ,-------------.
 *                                         |      |      |           |      |      |
 *                                  ,------|------|------|           |------+------+------.
 *                                  |      |      |      |           |      |      |      |
 *                                  |      |      |------|           |------|      |      |
 *                                  |      |      |      |           | BASE |      |      |
 *                                  `--------------------'           `--------------------'
 */

[HUN] = KEYMAP(
// left hand
 KC_NO   ,KC_NO   ,KC_NO    ,KC_NO   ,KC_NO    ,KC_NO   ,KC_NO
,KC_NO   ,KC_NO   ,M(HU_OEE),KC_NO   ,M(HU_UEE),KC_NO   ,KC_NO
,KC_NO   ,M(HU_AA),M(HU_OO) ,M(HU_EE),M(HU_UU) ,M(HU_II)
,KC_NO   ,KC_NO   ,M(HU_OE) ,KC_NO   ,M(HU_UE) ,KC_NO   ,KC_NO
,KC_NO   ,KC_NO   ,KC_NO    ,KC_NO   ,KC_NO

                                             ,KC_NO   ,KC_NO
                                                      ,KC_NO
                                    ,KC_NO   ,KC_TRNS ,KC_TRNS

                                                                // right hand
                                                               ,KC_TRNS ,KC_NO   ,KC_NO   ,KC_NO   ,KC_NO   ,KC_NO   ,KC_NO
                                                               ,KC_NO   ,KC_NO   ,KC_NO   ,KC_NO   ,KC_NO   ,KC_NO   ,KC_NO
                                                                        ,KC_NO   ,KC_NO   ,KC_NO   ,KC_NO   ,KC_NO   ,KC_NO
                                                               ,KC_NO   ,KC_NO   ,KC_NO   ,KC_NO   ,KC_NO   ,KC_NO   ,KC_NO
                                                                                 ,KC_NO   ,KC_NO   ,KC_NO   ,KC_NO   ,KC_NO

                                                               ,KC_NO   ,KC_NO
                                                               ,KC_NO
                                                               ,F(F_BSE),KC_TRNS  ,KC_TRNS
    ),

/* Keymap 5: Navigation & Media layer
 *
 * ,-----------------------------------------------------.           ,-----------------------------------------------------.
 * | MS Slow   |      |      |      |      |      |ScrLCK|           |ScrLCK|      |      |      |      |      |           |
 * |-----------+------+------+------+------+-------------|           |------+------+------+------+------+------+-----------|
 * | MS Normal |      | Home |  Up  | PgUp |      |      |           |Scroll|      |MsUpL | MsUp |MsUpR |      |PrintScreen|
 * |-----------+------+------+------+------+------|      |           |  Up  |------+------+------+------+------+-----------|
 * | MS Fast   |      | Left | Down | Right|      |------|           |------|      |MsLeft| MsDn |MsRght|      |           |
 * |-----------+------+------+------+------+------|      |           |Scroll|------+------+------+------+------+-----------|
 * | Play/Pause|      | End  | Down | PgDn |      |      |           | Down |      |MsDnL | MsDn |MsDnR |      |      Stop |
 * `-----------+------+------+------+------+-------------'           `-------------+------+------+------+------+-----------'
 *      |      |      |      |      |      |                                       |      |      |      |      |      |
 *      `----------------------------------'                                       `----------------------------------'
 *                                         ,-------------.           ,-------------.
 *                                         | Mute | VlUp |           | BASE | MClk |
 *                                  ,------|------|------|           |------+------+------.
 *                                  |      |      | VlDn |           | Prev |Left  |Right |
 *                                  |  SPC | Enter|------|           |------| Click| Click|
 *                                  |      |      | ESC  |           | Next |      |      |
 *                                  `--------------------'           `--------------------'
 */
[NMDIA] = KEYMAP(
// left hand
 M(A_ACL0)  ,KC_NO       ,KC_NO      ,KC_NO   ,KC_NO   ,KC_NO   ,LGUI(KC_L)
,M(A_ACL1)  ,KC_NO       ,KC_HOME    ,KC_UP   ,KC_PGUP ,KC_NO   ,KC_NO
,M(A_ACL2)  ,KC_NO       ,KC_LEFT    ,KC_DOWN ,KC_RIGHT,KC_NO
,KC_MPLY    ,KC_NO       ,KC_END     ,KC_DOWN ,KC_PGDN ,KC_NO   ,KC_NO
,KC_NO      ,KC_NO       ,KC_NO      ,KC_NO   ,KC_NO
                                                        ,KC_MUTE ,KC_VOLU
                                                                 ,KC_VOLD
                                                 ,KC_SPC,KC_ENTER,KC_ESC

                                                                     // right hand
                                                                     ,LGUI(KC_L),KC_NO   ,KC_NO   ,KC_NO   ,KC_NO   ,KC_NO    ,KC_NO
                                                                     ,KC_WH_U   ,KC_NO   ,M(A_MUL),KC_MS_U ,M(A_MUR),KC_NO    ,KC_PSCR
                                                                                ,KC_NO   ,KC_MS_L ,KC_MS_D ,KC_MS_R ,KC_NO    ,KC_NO
                                                                     ,KC_WH_D   ,KC_NO   ,M(A_MDL),KC_MS_D ,M(A_MDR),KC_NO    ,KC_MSTP
                                                                                         ,KC_NO   ,KC_NO   ,KC_NO   ,KC_NO    ,KC_NO

                                                                     ,KC_TRNS   ,KC_MS_BTN3
                                                                     ,KC_MPRV
                                                                     ,KC_MNXT   ,KC_BTN1 ,KC_BTN2
    ),
};

const uint16_t PROGMEM fn_actions[] = {
   [F_BSE]  = ACTION_LAYER_CLEAR(ON_PRESS)
  ,[F_HUN]  = ACTION_LAYER_INVERT(HUN, ON_PRESS)
  ,[F_GUI]  = ACTION_MACRO_TAP(A_GUI)
  ,[F_SFT]  = ACTION_MODS_ONESHOT (MOD_LSFT)
  ,[F_ALT]  = ACTION_MODS_ONESHOT (MOD_LALT)
  ,[F_CTRL] = ACTION_MODS_ONESHOT (MOD_LCTL)
};

static macro_t *ang_do_hun (keyrecord_t *record, uint16_t accent, uint16_t hun_char)
{
  uint8_t need_shift = 0;
  uint8_t hold_shift = 0;

  if (!record->event.pressed)
    return MACRO_NONE;

  layer_off (HUN);

  if (keyboard_report->mods & MOD_BIT (KC_LSFT)) {
    hold_shift = 1;
    need_shift = 1;
    unregister_code (KC_LSFT);
  }
  if ((get_oneshot_mods () & MOD_BIT(KC_LSFT)) && !has_oneshot_mods_timed_out ()) {
    need_shift = 1;
    hold_shift = 0;
    unregister_code (KC_LSFT);
  }

  clear_oneshot_mods ();

  register_code (KC_RALT);
  unregister_code (KC_RALT);
  if (accent == (KC_DQT)) {
    register_code (KC_RSFT);
  }
  register_code (accent);
  unregister_code (accent);
  if (need_shift && accent != (KC_DQT)) {
    register_code (KC_RSFT);
  } else if (accent == (KC_DQT) && !need_shift) {
    unregister_code (KC_RSFT);
  }
  register_code (hun_char);
  unregister_code (hun_char);
  if (need_shift || accent == (KC_DQT))
    unregister_code (KC_RSFT);
  if (hold_shift)
    register_code (KC_LSFT);

  return MACRO_NONE;
}

static struct {
  uint8_t idx;
} m_accel_state;

const macro_t *action_get_macro(keyrecord_t *record, uint8_t id, uint8_t opt)
{
      switch(id) {
      case A_MPN:
        if (record->event.pressed) {
          if (keyboard_report->mods & MOD_BIT(KC_LSFT) ||
              ((get_oneshot_mods() & MOD_BIT(KC_LSFT)) && !has_oneshot_mods_timed_out())) {
            int oneshot = ((get_oneshot_mods() & MOD_BIT(KC_LSFT)) && !has_oneshot_mods_timed_out());

            if (oneshot)
              clear_oneshot_mods ();
            unregister_code (KC_LSFT);

            register_code (KC_MPRV);
            unregister_code (KC_MPRV);

            if (!oneshot)
              register_code (KC_LSFT);
          } else {
            return MACRO (T(MNXT), END);
          }
        }
        break;

        /* Hungarian layer */
      case HU_AA:
        return ang_do_hun (record, KC_QUOT, KC_A);
      case HU_OO:
        return ang_do_hun (record, KC_QUOT, KC_O);
      case HU_EE:
        return ang_do_hun (record, KC_QUOT, KC_E);
      case HU_UU:
        return ang_do_hun (record, KC_QUOT, KC_U);
      case HU_II:
        return ang_do_hun (record, KC_QUOT, KC_I);
      case HU_OE:
        return ang_do_hun (record, KC_DQT, KC_O);
      case HU_UE:
        return ang_do_hun (record, KC_DQT, KC_U);
      case HU_OEE:
        return ang_do_hun (record, KC_EQL, KC_O);
      case HU_UEE:
        return ang_do_hun (record, KC_EQL, KC_U);

#if MOUSEKEY_ENABLE
        /* Mouse movement */
      case A_MUL:
        if (record->event.pressed) {
          mousekey_on(KC_MS_UP);
          mousekey_on(KC_MS_LEFT);
        } else {
          mousekey_off(KC_MS_UP);
          mousekey_off(KC_MS_LEFT);
        }
        mousekey_send();
        break;

      case A_MUR:
        if (record->event.pressed) {
          mousekey_on(KC_MS_UP);
          mousekey_on(KC_MS_RIGHT);
        } else {
          mousekey_off(KC_MS_UP);
          mousekey_off(KC_MS_RIGHT);
        }
        mousekey_send();
        break;

      case A_MDL:
        if (record->event.pressed) {
          mousekey_on(KC_MS_DOWN);
          mousekey_on(KC_MS_LEFT);
        } else {
          mousekey_off(KC_MS_DOWN);
          mousekey_off(KC_MS_LEFT);
        }
        mousekey_send();
        break;

      case A_MDR:
        if (record->event.pressed) {
          mousekey_on(KC_MS_DOWN);
          mousekey_on(KC_MS_RIGHT);
        } else {
          mousekey_off(KC_MS_DOWN);
          mousekey_off(KC_MS_RIGHT);
        }
        mousekey_send();
        break;

      case A_ACL0 ... A_ACL2:
        if (record->event.pressed) {
          uint8_t idx = id - A_ACL0;
          if (m_accel_state.idx == id) {
            mousekey_off(m_accel_state.idx - A_ACL0 + KC_ACL0);
            m_accel_state.idx = 0;
          } else {
            if (m_accel_state.idx) {
              mousekey_off(m_accel_state.idx - A_ACL0 + KC_ACL0);
              m_accel_state.idx = 0;
            }
            mousekey_on(KC_ACL0 + idx);
            m_accel_state.idx = id;
          }
        }
        break;
#endif

        /* EMACS layer stuff */
      case AE_EMACS:
        if (record->event.pressed) {
          cp_mode = CP_EMACS;
        }
        break;

      case AE_TERM:
        if (record->event.pressed) {
          cp_mode = CP_TERM;
        }
        break;

      case AE_OTHER:
        if (record->event.pressed) {
          cp_mode = CP_OTHER;
        }
        break;

      case AE_VIS:
        if (cp_mode == CP_EMACS && record->event.pressed) {
          return MACRO(T(V), END);
        }
        break;

      case AE_CPYC:
        if (record->event.pressed) {
          m_copypaste_timer = timer_read ();
        } else {
          if (timer_elapsed (m_copypaste_timer) > TAPPING_TERM) {
            // Long press: Cut
            switch (cp_mode) {
            case CP_EMACS:
              return MACRO(T(X), END);
              break;
            case CP_TERM:
              return MACRO(D(RCTRL), D(RSFT), T(X), U(RSFT), U(RCTRL), END);
              break;
            case CP_OTHER:
              return MACRO(D(RCTRL), T(X), U(RCTRL), END);
              break;
            }
          } else {
            // Short press: Copy
            switch (cp_mode) {
            case CP_EMACS:
              return MACRO(T(Y), END);
              break;
            case CP_TERM:
              return MACRO(D(RCTRL), D(RSFT), T(C), U(RSFT), U(RCTRL), END);
              break;
            case CP_OTHER:
              return MACRO(D(RCTRL), T(C), U(RCTRL), END);
              break;
            }
          }
        }
        break;

      case AE_PSTDEL:
        if (record->event.pressed) {
          m_cutdel_timer = timer_read ();
        } else {
          if (timer_elapsed (m_cutdel_timer) > TAPPING_TERM) {
            // Long press: Delete
            switch (cp_mode) {
            case CP_EMACS:
              return MACRO(T(D), END);
              break;
            case CP_TERM:
            case CP_OTHER:
              return MACRO(T(DEL), END);
              break;
            }
          } else {
            // Short press: Paste
            switch (cp_mode) {
            case CP_EMACS:
              if (keyboard_report->mods & MOD_BIT(KC_RSFT)) {
                unregister_code (KC_RSFT);
                return MACRO(U(RSFT), T(RBRC), T(P), D(RSFT), END);
              }
              else
                return MACRO(T(P), END);
              break;
            case CP_TERM:
              return MACRO(D(RCTRL), D(RSFT), T(V), U(RSFT), U(RCTRL), END);
              break;
            case CP_OTHER:
              return MACRO(D(RCTRL), T(V), U(RCTRL), END);
              break;
            }
          }
        }
        break;

      case AE_INS:
        if (record->event.pressed) {
          return MACRO(T(I), END);
        } else {
          layer_clear();
        }
        break;

      case AE_OVR:
        if (record->event.pressed) {
          return MACRO(T(R), END);
        } else {
          layer_clear();
        }
        break;

      case AE_APPND:
        if (record->event.pressed) {
          return MACRO(T(A), END);
        } else {
          layer_clear();
        }
        break;

        /* GUI & AppSel */
      case A_GUI:
        if (record->event.pressed) {
          register_code (KC_LGUI);
          if (record->tap.count && !record->tap.interrupted) {
            if (record->tap.count >= 2) {
              register_code (KC_W);
              layer_on (APPSEL);
              set_oneshot_layer (APPSEL, ONESHOT_START);
            }
          } else {
            record->tap.count = 0;
          }
          gui_timer = 0;
        } else {
          if (record->tap.count >= 2)
            {
              unregister_code (KC_W);
              clear_oneshot_layer_state (ONESHOT_PRESSED);
            }
          gui_timer = timer_read ();
        }
        break;

      case APP_SLK:
        return MACRODOWN(T(S), T(L), T(A), T(C), T(K), T(ENT), END);

      case APP_EMCS:
        return MACRODOWN(T(G), T(N), T(U), T(SPC), T(E), T(M), T(A), T(C), T(S), T(SPC), T(2), T(4), T(ENT), END);

      case APP_TERM:
        return MACRODOWN(T(T), T(E), T(R), T(M), T(I), T(N), T(A), T(L), T(ENT), END);
        break;

      case APP_CHRM:
        return MACRODOWN(T(C), T(H), T(R), T(O), T(M), T(ENT), END);

      case APP_MSIC:
        return MACRODOWN(T(R), T(H), T(Y), T(T), T(H), T(M), T(B), T(O), T(X), T(ENT), END);

        // Tmux keys
      case A_TMUXP:
        return MACRODOWN(D(LALT), T(SPC), U(LALT), T(P), END);
      }

      return MACRO_NONE;
};

static uint8_t is_adore = 0;

// Runs just one time when the keyboard initializes.
void matrix_init_user(void) {
  uint8_t dl;

  set_unicode_input_mode(UC_LNX);

  ergodox_led_all_on();
  for (int i = LED_BRIGHTNESS_HI; i > LED_BRIGHTNESS_LO; i--) {
    ergodox_led_all_set (i);
    wait_ms (5);
  }
  wait_ms(1000);
  for (int i = LED_BRIGHTNESS_LO; i > 0; i--) {
    ergodox_led_all_set (i);
    wait_ms (10);
  }
  ergodox_led_all_off();

  if (!eeconfig_is_enabled())
    eeconfig_init();
  dl = eeconfig_read_default_layer ();
  if (dl == (1UL << ADORE)) {
    is_adore = 1;
  }
};

LEADER_EXTERNS();

static void ang_tap (uint8_t code, ...) {
  uint8_t kc = code;
  va_list ap;

  va_start(ap, code);

  do {
    register_code(kc);
    unregister_code(kc);
    wait_ms(50);
    kc = va_arg(ap, int);
  } while (kc != 0);
  va_end(ap);
}

#define TAP_ONCE(code)  \
  register_code (code); \
  unregister_code (code)

typedef struct {
  bool layer_toggle;
  bool sticky;
} td_ta_state_t;

static void ang_tap_dance_ta_finished (qk_tap_dance_state_t *state, void *user_data) {
  td_ta_state_t *td_ta = (td_ta_state_t *) user_data;

  if (td_ta->sticky) {
    td_ta->sticky = false;
    td_ta->layer_toggle = false;
    layer_off (ARRW);
    return;
  }

  if (state->count == 1 && !state->pressed) {
    register_code (KC_TAB);
    td_ta->sticky = false;
    td_ta->layer_toggle = false;
  } else {
    td_ta->layer_toggle = true;
    layer_on (ARRW);
    td_ta->sticky = (state->count == 2);
  }
}

static void ang_tap_dance_ta_reset (qk_tap_dance_state_t *state, void *user_data) {
  td_ta_state_t *td_ta = (td_ta_state_t *) user_data;

  if (!td_ta->layer_toggle)
    unregister_code (KC_TAB);
  if (!td_ta->sticky)
    layer_off (ARRW);
}

static void ang_tap_dance_fx_on_finished (qk_tap_dance_state_t *state, void *user_data) {
  uint8_t code = state->keycode - KF_1;
  uint8_t kc_base;

  if (state->count == 1)
    kc_base = KC_1;
  else
    kc_base = KC_F1;

  code += kc_base;
  register_code (code);
}


static void ang_tap_dance_fx_on_reset (qk_tap_dance_state_t *state, void *user_data) {
  uint8_t code = state->keycode - KF_1;
  uint8_t kc_base;

  if (state->count == 1)
    kc_base = KC_1;
  else
    kc_base = KC_F1;

  code += kc_base;
  unregister_code (code);
}

static void ang_tap_dance_tmux_finished (qk_tap_dance_state_t *state, void *user_data) {
  if (state->count == 1) {
    register_code(KC_LALT);
    register_code(KC_SPC);
    unregister_code(KC_SPC);
    unregister_code(KC_LALT);
  } else {
    register_code(KC_LCTL);
    register_code(KC_A);
    unregister_code(KC_A);
    unregister_code(KC_LCTL);
  }
}

qk_tap_dance_action_t tap_dance_actions[] = {
   [CT_CLN] = ACTION_TAP_DANCE_DOUBLE (KC_COLN, KC_SCLN)
  ,[CT_TA]  = {
     .fn = { NULL, ang_tap_dance_ta_finished, ang_tap_dance_ta_reset },
     .user_data = (void *)&((td_ta_state_t) { false, false })
   }
  ,[CT_LBP] = ACTION_TAP_DANCE_DOUBLE (KC_LBRC, KC_LPRN)
  ,[CT_RBP] = ACTION_TAP_DANCE_DOUBLE (KC_RBRC, KC_RPRN)
  ,[CT_TMUX]= ACTION_TAP_DANCE_FN (ang_tap_dance_tmux_finished)
  ,[KF_1]   = ACTION_TAP_DANCE_FN_ADVANCED (NULL, ang_tap_dance_fx_on_finished, ang_tap_dance_fx_on_reset)
  ,[KF_2]   = ACTION_TAP_DANCE_FN_ADVANCED (NULL, ang_tap_dance_fx_on_finished, ang_tap_dance_fx_on_reset)
  ,[KF_3]   = ACTION_TAP_DANCE_FN_ADVANCED (NULL, ang_tap_dance_fx_on_finished, ang_tap_dance_fx_on_reset)
  ,[KF_4]   = ACTION_TAP_DANCE_FN_ADVANCED (NULL, ang_tap_dance_fx_on_finished, ang_tap_dance_fx_on_reset)
  ,[KF_5]   = ACTION_TAP_DANCE_FN_ADVANCED (NULL, ang_tap_dance_fx_on_finished, ang_tap_dance_fx_on_reset)
  ,[KF_6]   = ACTION_TAP_DANCE_FN_ADVANCED (NULL, ang_tap_dance_fx_on_finished, ang_tap_dance_fx_on_reset)
  ,[KF_7]   = ACTION_TAP_DANCE_FN_ADVANCED (NULL, ang_tap_dance_fx_on_finished, ang_tap_dance_fx_on_reset)
  ,[KF_8]   = ACTION_TAP_DANCE_FN_ADVANCED (NULL, ang_tap_dance_fx_on_finished, ang_tap_dance_fx_on_reset)
  ,[KF_9]   = ACTION_TAP_DANCE_FN_ADVANCED (NULL, ang_tap_dance_fx_on_finished, ang_tap_dance_fx_on_reset)
  ,[KF_10]  = ACTION_TAP_DANCE_FN_ADVANCED (NULL, ang_tap_dance_fx_on_finished, ang_tap_dance_fx_on_reset)
};

// Runs constantly in the background, in a loop.
void matrix_scan_user(void) {
  uint8_t layer = biton32(layer_state);
  bool is_arrow = false;

  if (gui_timer && timer_elapsed (gui_timer) > TAPPING_TERM)
    unregister_code (KC_LGUI);

  if (layer == HUN) {
    ergodox_right_led_2_on();
    ergodox_right_led_3_on();
  } else if (layer == NMDIA) {
    ergodox_right_led_1_on();
    ergodox_right_led_2_on();
  } else if (layer == ADORE) {
    ergodox_right_led_1_on ();
    ergodox_right_led_2_on ();
    ergodox_right_led_3_on ();

    ergodox_right_led_2_set (LED_BRIGHTNESS_HI);
  }

  if (layer_state & (1UL << ARRW)) {
    ergodox_right_led_1_on ();
    ergodox_right_led_3_on ();
    is_arrow = true;
  }

  if (keyboard_report->mods & MOD_BIT(KC_LSFT) ||
      ((get_oneshot_mods() & MOD_BIT(KC_LSFT)) && !has_oneshot_mods_timed_out())) {
    ergodox_right_led_1_set (LED_BRIGHTNESS_HI);
    ergodox_right_led_1_on ();
  } else {
    ergodox_right_led_1_set (LED_BRIGHTNESS_LO);
    if (layer != NMDIA && layer != ADORE && !is_arrow)
      ergodox_right_led_1_off ();
  }

  if (keyboard_report->mods & MOD_BIT(KC_LALT) ||
      ((get_oneshot_mods() & MOD_BIT(KC_LALT)) && !has_oneshot_mods_timed_out())) {
    ergodox_right_led_2_set (LED_BRIGHTNESS_HI);
    ergodox_right_led_2_on ();
  } else {
    ergodox_right_led_2_set (LED_BRIGHTNESS_LO);
    if (layer != HUN && layer != NMDIA && layer != ADORE)
      ergodox_right_led_2_off ();
  }

  if (keyboard_report->mods & MOD_BIT(KC_LCTRL) ||
      ((get_oneshot_mods() & MOD_BIT(KC_LCTRL)) && !has_oneshot_mods_timed_out())) {
    ergodox_right_led_3_set (LED_BRIGHTNESS_HI);
    ergodox_right_led_3_on ();
  } else {
    ergodox_right_led_3_set (LED_BRIGHTNESS_LO);
    if (layer != HUN && layer != ADORE && !is_arrow)
      ergodox_right_led_3_off ();
  }

  LEADER_DICTIONARY() {
    leading = false;
    leader_end ();

#if KEYLOGGER_ENABLE
    SEQ_ONE_KEY (KC_D) {
      ergodox_led_all_on();
      wait_ms(100);
      ergodox_led_all_off();
      log_enable = !log_enable;
    }
#endif

    SEQ_ONE_KEY (KC_Q) {
      register_code16 (LCTL(KC_1));
      unregister_code16 (LCTL(KC_1));
    }

    SEQ_ONE_KEY (KC_T) {
      time_travel = !time_travel;
    }

    SEQ_ONE_KEY (KC_U) {
      qk_ucis_start();
    }

    SEQ_ONE_KEY (KC_V) {
      SEND_STRING (QMK_KEYBOARD "/" QMK_KEYMAP " @ (" QMK_VERSION "/" KEYMAP_VERSION ")");
    }

    SEQ_ONE_KEY (KC_L) {
      /* λ */
      unicode_input_start();
      register_hex(0x03bb);
      unicode_input_finish();
    }

    SEQ_ONE_KEY (KC_Y) {
      ang_tap (KC_BSLS, KC_O, KC_SLSH, 0);
    }

    SEQ_ONE_KEY (KC_S) {
      unicode_input_start(); register_hex(0xaf); unicode_input_finish();
      TAP_ONCE (KC_BSLS);
      register_code (KC_RSFT); TAP_ONCE (KC_MINS); TAP_ONCE (KC_9); unregister_code (KC_RSFT);
      unicode_input_start (); register_hex(0x30c4); unicode_input_finish();
      register_code (KC_RSFT); TAP_ONCE (KC_0); TAP_ONCE (KC_MINS); unregister_code (KC_RSFT);
      TAP_ONCE (KC_SLSH);
      unicode_input_start (); register_hex(0xaf); unicode_input_finish();
    }

    SEQ_TWO_KEYS (KC_W, KC_M) {
      register_code (KC_LALT);
      register_code (KC_F2);
      unregister_code (KC_F2);
      unregister_code (KC_LALT);

      wait_ms (1000);

      ang_tap (KC_M, KC_A, KC_X, KC_MINS, KC_F, KC_O, KC_C, KC_U, KC_S, KC_E, KC_D, KC_ENT, 0);
      register_code (KC_LGUI);
      register_code (KC_UP);
      unregister_code (KC_UP);
      unregister_code (KC_LGUI);
    }

    SEQ_ONE_KEY (KC_A) {
      if (is_adore == 0) {
        default_layer_and (0);
        default_layer_or ((1UL << ADORE));
        eeconfig_update_default_layer ((1UL << ADORE));
        is_adore = 1;

        ergodox_led_all_off ();
        ergodox_right_led_3_on ();
        wait_ms (100);
        ergodox_right_led_2_on ();
        wait_ms (100);
        ergodox_right_led_3_off ();
        ergodox_right_led_1_on ();
        wait_ms (100);
        ergodox_right_led_2_off ();
        wait_ms (100);
        ergodox_right_led_1_off ();
      } else {
        is_adore = 0;
        default_layer_and (0);
        default_layer_or (1UL << BASE);
        eeconfig_update_default_layer ((1UL << BASE));

        ergodox_led_all_off ();
        ergodox_right_led_1_on ();
        wait_ms (100);
        ergodox_right_led_2_on ();
        wait_ms (100);
        ergodox_right_led_1_off ();
        ergodox_right_led_3_on ();
        wait_ms (100);
        ergodox_right_led_2_off ();
        wait_ms (100);
        ergodox_right_led_3_off ();
      }
    }
  }
}

static uint16_t last4[4];

const qk_ucis_symbol_t ucis_symbol_table[] = UCIS_TABLE
(
 UCIS_SYM("poop", 0x1f4a9),
 UCIS_SYM("rofl", 0x1f923),
 UCIS_SYM("kiss", 0x1f619),
 UCIS_SYM("snowman", 0x2603),
 UCIS_SYM("coffee", 0x2615),
 UCIS_SYM("heart", 0x2764),
 UCIS_SYM("bolt", 0x26a1)
);

bool process_record_user (uint16_t keycode, keyrecord_t *record) {
#if KEYLOGGER_ENABLE
  if (log_enable) {
    xprintf ("KL: col=%02d, row=%02d, pressed=%d, layer=%s\n", record->event.key.col,
             record->event.key.row, record->event.pressed, (is_adore) ? "ADORE" : "Dvorak");
  }
#endif

  if (keycode == KC_ESC && record->event.pressed) {
    bool queue = true;

    if ((get_oneshot_mods ()) && !has_oneshot_mods_timed_out ()) {
      clear_oneshot_mods ();
      queue = false;
    }
    if (layer_state & (1UL<<HUN)) {
      layer_off (HUN);
      queue = false;
    }
    return queue;
  }

  if (time_travel && !record->event.pressed) {
    uint8_t p;

    // shift cache one to the left
    for (p = 0; p < 3; p++) {
      last4[p] = last4[p + 1];
    }
    last4[3] = keycode;

    if (last4[0] == KC_D && last4[1] == KC_A && last4[2] == KC_T && last4[3] == KC_E) {
      ang_tap (KC_E, KC_SPC, KC_MINS, KC_D, KC_SPC, KC_QUOT, 0);
      register_code (KC_RSFT);
      register_code (KC_EQL);
      unregister_code (KC_EQL);
      unregister_code (KC_RSFT);

      ang_tap (KC_4, KC_SPC, KC_D, KC_A, KC_Y, KC_S, KC_QUOT, 0);

      return false;
    }
  }

  return true;
}

void qk_ucis_symbol_fallback (void) {
  for (uint8_t i = 0; i < qk_ucis_state.count - 1; i++) {
    uint8_t code;

    if (qk_ucis_state.codes[i] > KF_1)
      code = qk_ucis_state.codes[i] - KF_1 + KC_1;
    else
      code = qk_ucis_state.codes[i];
    register_code(code);
    unregister_code(code);
    wait_ms (10);
  }
}
