#include "ergodox.h"
#include "debug.h"
#include "action_layer.h"

#define BASE 0 // default layer
#define RUSS 1 // russian (Зубачёва, модифицированная)
#define SYMB 2 // symbols
#define SYMU 3 // symbols unicode
#define MDIA 4 // media keys

#define A_MDIA 0

enum keyboard_functions
{
    kf_ver,

    kf_F1, kf_F2, kf_F3, kf_F4, kf_F5, kf_F6, kf_F7, kf_F8, kf_F9, kf_F10, kf_F11, kf_F12,

    kf_flw
};

uint16_t kf_timers[10];

void handle_kf(keyrecord_t *record, uint8_t id, uint8_t opt)
{
    if (id == kf_F11 || id == kf_F12)
    {
        uint8_t const kc = id - kf_F11 + KC_F11;
        if (record->event.pressed)
        {
            register_code(kc);
        }
        else
        {
            unregister_code(kc);
        }
        return;
    }

    uint8_t const timer_index = id - kf_F1;

    if (record->event.pressed)
    {
        kf_timers[timer_index] = timer_read();
    }
    else
    {
        uint16_t e = timer_elapsed(kf_timers[timer_index]);
        bool const is_8 = e > 3000 && id == kf_F8;

        if (is_8)
        {
            switch (rand() % 20)
            {
            case  0: SEND_STRING("It is certain");      break;
            case  1: SEND_STRING("It is decidedly so"); break;
            case  2: SEND_STRING("Without a doubt");    break;
            case  3: SEND_STRING("Yes - definitely");   break;
            case  4: SEND_STRING("You may rely on it"); break;

            case  5: SEND_STRING("As I see it, yes");   break;
            case  6: SEND_STRING("Most likely");        break;
            case  7: SEND_STRING("Outlook good");       break;
            case  8: SEND_STRING("Signs point to yes"); break;
            case  9: SEND_STRING("Yes");                break;

            case 10: SEND_STRING("Reply hazy, try again");     break;
            case 11: SEND_STRING("Ask again later");           break;
            case 12: SEND_STRING("Better not tell you now");   break;
            case 13: SEND_STRING("Cannot predict now");        break;
            case 14: SEND_STRING("Concentrate and ask again"); break;

            case 15: SEND_STRING("Don't count on it");   break;
            case 16: SEND_STRING("My reply is no");      break;
            case 17: SEND_STRING("My sources say no");   break;
            case 18: SEND_STRING("Outlook not so good"); break;
            case 19: SEND_STRING("Very doubtful");       break;
            }
        }
        else
        {
            bool const is_long_press = e > 200;
            uint8_t const kc_base = is_long_press ? KC_F1 : KC_1;
            uint8_t const kc = kc_base + timer_index;

            register_code(kc);
            unregister_code(kc);
        }
    }
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/* Keymap 0: Basic layer
 *
 * ,--------------------------------------------------.           ,--------------------------------------------------.
 * | ESC    | 1/F1 | 2/F2 | 3/F3 | 4/F4 | 5/F5 | F11  |           |  F12 | 6/F6 | 7/F7 | 8/F8 | 9/F9 | 0/F10|  SYMB  |
 * |--------+------+------+------+------+-------------|           |------+------+------+------+------+------+--------|
 * | Tab    |   Q  |   D  |   R  |   W  |   B  |PrScrn|           |Pause |   J  |   F  |   U  |   P  |   ;  |  Rus   |
 * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |CapsLock|   A  |   S  |   H  |   T  |   G  |------|           |------|   Y  |   N  |   E  |   O  |   I  |        |
 * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |LSh/Home|   Z  |   X  |   M  |   C  |   V  |      |           |      |   K  |   L  |  ,   |  .   |   :  |RSh/End |
 * `--------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------'
 *  |Backspc|HYPR+T|HYPR+M|  Alt | CTRL |                                       |  Flw |MediaL|      |Insert|Delete|
 *   `----------------------------------'                                       `----------------------------------'
 *                                        ,-------------.       ,-------------.
 *                                        |      |      |       |      |      |
 *                                 ,------|------|------|       |------+------+------.
 *                                 |      |      |      |       | PgUp |      |      |
 *                                 |Space | CMD  |------|       |------| App  |Enter |
 *                                 |      |      |      |       | PgDn |      |      |
 *                                 `--------------------'       `--------------------'
 */
// If it accepts an argument (i.e, is a function), it doesn't need KC_.
// Otherwise, it needs KC_*
[BASE] = KEYMAP(  // layer 0 : default
        // left hand
        KC_ESC,         M(kf_F1),     M(kf_F2),    M(kf_F3),   M(kf_F4),   M(kf_F5),   M(kf_F11),
        KC_TAB,         KC_Q,         KC_D,        KC_R,       KC_W,       KC_B,       KC_PSCR,
        KC_CAPS,        KC_A,         KC_S,        KC_H,       KC_T,       KC_G,
        SFT_T(KC_HOME), KC_Z,         KC_X,        KC_M,       KC_C,       KC_V,       KC_NO,
        KC_BSPC,        HYPR(KC_P),   HYPR(KC_M),  KC_LALT,    KC_LCTL,
                                                                           KC_NO,   KC_NO,
                                                                                    KC_NO,
                                                                   KC_SPC, KC_LGUI, KC_NO,
        // right hand
        M(kf_F12),      M(kf_F6),     M(kf_F7),    M(kf_F8),   M(kf_F9),   M(kf_F10),  TG(SYMB),
        KC_PAUS,        KC_J,         KC_F,        KC_U,       KC_P,       KC_SCLN,    TG(RUSS),
                        KC_Y,         KC_N,        KC_E,       KC_O,       KC_I,       KC_NO,
        KC_NO,          KC_K,         KC_L,        KC_COMM,    KC_DOT,     KC_COLN,    SFT_T(KC_END),
                                      M(kf_flw),   F(A_MDIA),  KC_NO,      KC_INS,     KC_DELT,
        KC_NO,   KC_NO,
        KC_PGUP,
        KC_PGDN, KC_APP,KC_ENT
    ),
/* Keymap 1: Russian
 *
 * ,--------------------------------------------------.           ,--------------------------------------------------.
 * |        |      |      |      |      |      |      |           |      |      |      |      |      |      |        |
 * |--------+------+------+------+------+-------------|           |------+------+------+------+------+------+--------|
 * |        |   Ф  |   Ы  |   А  |   Я  |   Э  |      |           |      |   Ц  |   М  |   Р  |   П  |   Х  |        |
 * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |        |   Г  |   И  |   Е  |   О  |   У  |------|           |------|   Л  |   Т  |   С  |   Н  |   З  |        |
 * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |        |   Ш  |   Ь  |   Ю  |   Ж  |      |      |           |      |   Б  |   Д  |   В  |   К  |   Ч  |        |
 * `--------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------'
 *  |       |      |      |      |      |                                       |      |      |      |      |      |
 *   `----------------------------------'                                       `----------------------------------'
 *                                        ,-------------.       ,-------------.
 *                                        |      |      |       |      |      |
 *                                 ,------|------|------|       |------+------+------.
 *                                 |      |      |      |       |      |      |      |
 *                                 |      |      |------|       |------|      |      |
 *                                 |      |      |      |       |      |      |      |
 *                                 `--------------------'       `--------------------'
 */
// If it accepts an argument (i.e, is a function), it doesn't need KC_.
// Otherwise, it needs KC_*
[RUSS] = KEYMAP(  // layer 1 : russian
        // left hand
        KC_TRNS,        KC_TRNS,      KC_TRNS,     KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,
        KC_TRNS,        KC_A,         KC_S,        KC_F,       KC_Z,       KC_QUOT,    KC_TRNS,
        KC_TRNS,        KC_U,         KC_B,        KC_T,       KC_J,       KC_E,
        KC_TRNS,        KC_I,         KC_M,        KC_DOT,     KC_SCLN,    KC_SPC,     KC_TRNS,
        KC_TRNS,        KC_TRNS,      KC_TRNS,     KC_TRNS,    KC_TRNS,
                                                                           KC_NO,   KC_NO,
                                                                                    KC_NO,
                                                                  KC_TRNS, KC_TRNS, KC_NO,
        // right hand
        KC_TRNS,        KC_TRNS,      KC_TRNS,     KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,
        KC_TRNS,        KC_W,         KC_V,        KC_H,       KC_G,       KC_LBRC,    KC_TRNS,
                        KC_K,         KC_N,        KC_C,       KC_Y,       KC_P,       KC_TRNS,
        KC_TRNS,        KC_COMM,      KC_L,        KC_D,       KC_R,       KC_X,       KC_TRNS,
                                      KC_TRNS,     KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,
        KC_TRNS, KC_TRNS,
        KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS
    ),
/* Keymap 2: Symbol Layer
 * 
 * ,--------------------------------------------------.           ,--------------------------------------------------.
 * |        |  F1  |  F2  |  F3  |  F4  |  F5  | F11  |           |  F12 |  F6  |  F7  |  F8  |  F9  | F10  |        |
 * |--------+------+------+------+------+-------------|           |------+------+------+------+------+------+--------|
 * |        |   !  |   &  |   #  |   |  |   @  |      |           |      |   $  |   ^  |   %  |   *  |   ?  |        |
 * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |        |   +  |   (  |   [  |   {  |   ~  |------|           |------|   `  |   }  |   ]  |   )  |   -  |        |
 * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |        |   .  |   /  |   <  |   =  |   "  |      |           |      |   '  |   _  |   >  |   \  |   ,  |        |
 * `--------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------'
 *   |      |      |      |      |      |                                       |      |      |      |      |      |
 *   `----------------------------------'                                       `----------------------------------'
 *                                        ,-------------.       ,-------------.
 *                                        |      |      |       |      |      |
 *                                 ,------|------|------|       |------+------+------.
 *                                 |      |      |      |       |      |      |      |
 *                                 |      |      |------|       |------|      |      |
 *                                 |      |      |      |       |      |      |      |
 *                                 `--------------------'       `--------------------'
 */
// SYMBOLS
[SYMB] = KEYMAP(
       // left hand
       KC_TRNS,   KC_F1,    KC_F2,    KC_F3,          KC_F4,        KC_F5,          KC_F11,
       KC_TRNS,   KC_EXLM,  KC_AMPR,  KC_HASH,        KC_PIPE,      KC_AT,          KC_TRNS,
       KC_TRNS,   KC_PLUS,  KC_LPRN,  KC_LBRC,        KC_LCBR,      KC_TILD,
       KC_TRNS,   KC_DOT,   KC_SLSH,  KC_LT,          KC_EQL,       KC_DQT,         KC_TRNS,
       KC_TRNS,   KC_TRNS,  KC_TRNS,  KC_TRNS,        KC_TRNS,
                                       KC_TRNS,KC_TRNS,
                                               KC_TRNS,
                               KC_TRNS,KC_TRNS,KC_TRNS,
       // right hand
       KC_F12,  KC_F6,          KC_F7,              KC_F8,    KC_F9,    KC_F10,   KC_TRNS,
       KC_TRNS, KC_DLR,         KC_CIRC,            KC_PERC,  KC_ASTR,  KC_QUES,  KC_TRNS,
                KC_GRV,         KC_RCBR,            KC_RBRC,  KC_RPRN,  KC_MINS,  KC_TRNS,
       KC_TRNS, KC_QUOT,        KC_UNDERSCORE,      KC_GT,    KC_BSLS,  KC_COMM,  KC_TRNS,
                                KC_TRNS,            KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,
       KC_TRNS, KC_TRNS,
       KC_TRNS,
       KC_TRNS, KC_TRNS, KC_TRNS
),
/* Keymap 3: Unicode Symbol Layer
 * 
 * ,--------------------------------------------------.           ,--------------------------------------------------.
 * |        |      |      |      |      |      |      |           |      |      |      |      |      |      |        |
 * |--------+------+------+------+------+-------------|           |------+------+------+------+------+------+--------|
 * |        |   !  |   &  |   #  |   |  |   @  |      |           |      |   $  |   ^  |   %  |   *  |   ?  |        |
 * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |        |   +  |   (  |   [  |   {  |   ~  |------|           |------|   `  |   }  |   ]  |   )  |   -  |        |
 * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |        |   .  |   /  |   <  |   =  |   "  |      |           |      |   '  |   _  |   >  |   \  |   ,  |        |
 * `--------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------'
 *   |      |      |      |      |      |                                       |      |      |      |      |      |
 *   `----------------------------------'                                       `----------------------------------'
 *                                        ,-------------.       ,-------------.
 *                                        |      |      |       |      |      |
 *                                 ,------|------|------|       |------+------+------.
 *                                 |      |      |      |       |      |      |      |
 *                                 |      |      |------|       |------|      |      |
 *                                 |      |      |      |       |      |      |      |
 *                                 `--------------------'       `--------------------'
 */
// SYMBOLS
[SYMU] = KEYMAP(
       // left hand
       KC_TRNS,   UC(0x2620), UC(0x2622), UC(0x2623), UC(0x262D), UC(0x262E), UC(0x00A9),
       KC_TRNS,   UC(33),     UC(38),     UC(35),     UC(124),    UC(64),     KC_TRNS,
       KC_TRNS,   UC(43),     UC(40),     UC(91),     UC(123),    UC(126),
       KC_TRNS,   UC(46),     UC(47),     UC(60),     UC(61),     UC(34),     KC_TRNS,
       KC_TRNS,   KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,
                                       KC_TRNS,KC_TRNS,
                                               KC_TRNS,
                               KC_TRNS,KC_TRNS,KC_TRNS,
       // right hand
       UC(0x00AE), UC(0x262F), UC(0x275D), UC(0x275E), UC(0x2757), UC(0x2753), KC_TRNS,
       KC_TRNS,    UC(36),     UC(94),     UC(37),     UC(42),     UC(63),     KC_TRNS,
                   UC(96),     UC(125),    UC(93),     UC(41),     UC(45),     KC_TRNS,
       KC_TRNS,    UC(39),     UC(95),     UC(62),     UC(92),     UC(44),     KC_TRNS,
                               KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,
       KC_TRNS, KC_TRNS,
       KC_TRNS,
       KC_TRNS, KC_TRNS, KC_TRNS
),
/* Keymap 4: Media and mouse keys
 *
 * ,--------------------------------------------------.           ,--------------------------------------------------.
 * |        | DEBUG|      |      |      |      |Reset |           |Power |Sleep | Wake |      |      |      |  Mute  |
 * |--------+------+------+------+------+-------------|           |------+------+------+------+------+------+--------|
 * |        |      | Lclk | MsUp | Rclk |WheelU|      |           |      |      |      |  Up  |      |      |  VolUp |
 * |--------+------+------+------+------+------|      |           | MSel |------+------+------+------+------+--------|
 * |        |      |MsLeft|MsDown|MsRght|WheelD|------|           |------|      | Left | Down |Right |      |  VolDn |
 * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |        |      |      |      |      |      |      |           | Stop |  <<  |  |<  |  ||  |  >|  |  >>  |        |
 * `--------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------'
 *   |      |      |      |      |      |                                       |      |      |      |      |      |
 *   `----------------------------------'                                       `----------------------------------'
 *                                        ,-------------.       ,-------------.
 *                                        | Ver  | Mail |       |      |      |
 *                                 ,------|------|------|       |------+------+------.
 *                                 |  WWW | WWW  |MyComp|       |      |      |      |
 *                                 |   <  |  >   |------|       |------|      |      |
 *                                 |      |      | Calc |       |      |      |      |
 *                                 `--------------------'       `--------------------'
 */
// MEDIA AND MOUSE
[MDIA] = KEYMAP(
       KC_TRNS,   DEBUG,   KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, RESET,
       KC_TRNS,   KC_TRNS, KC_BTN1, KC_MS_U, KC_BTN2, KC_WH_U, KC_TRNS,
       KC_TRNS,   KC_TRNS, KC_MS_L, KC_MS_D, KC_MS_R, KC_WH_D,
       KC_TRNS,   KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
       KC_TRNS,   KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
                                           M(kf_ver), KC_MAIL,
                                                    KC_MY_COMPUTER,
                                  KC_WBAK, KC_WFWD, KC_CALCULATOR,
    // right hand
       KC_PWR,   KC_SLEP, KC_WAKE, KC_TRNS, KC_TRNS, KC_TRNS, KC_MUTE,
       KC_MSEL,  KC_TRNS, KC_TRNS, KC_UP,   KC_TRNS, KC_TRNS, KC_VOLU,
                 KC_TRNS, KC_LEFT, KC_DOWN, KC_RGHT, KC_TRNS, KC_VOLD,
       KC_MSTP,  KC_MRWD, KC_MPRV, KC_MPLY, KC_MNXT, KC_MFFD, KC_TRNS,
                          KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
       KC_TRNS, KC_TRNS,
       KC_TRNS,
       KC_TRNS, KC_TRNS, KC_TRNS
),
};

const uint16_t PROGMEM fn_actions[] = 
{
    [A_MDIA] = ACTION_LAYER_ONESHOT(MDIA),
};

const macro_t *action_get_macro(keyrecord_t *record, uint8_t id, uint8_t opt)
{
  // MACRODOWN only works in this function
    switch (id)
    {
        case kf_ver:
        {
            if (record->event.pressed)
            {
                SEND_STRING("SMS " __DATE__ " " __TIME__);
            }
        }
        break;

        case kf_F1 ... kf_F12:
        {
            handle_kf(record, id, opt);
        }
        break;

        default:
            return MACRO_NONE;
    }
    return MACRO_NONE;
};

// Runs just one time when the keyboard initializes.
void matrix_init_user(void)
{
    set_unicode_input_mode(UC_WIN);
};

#define FLW_ONE_KEY(arg1) if (arg1 == second)
#define FLW_TWO_KEYS(arg1, arg2) if (arg1 == first && arg2 == second)
#define FLW_ONE_MATCH(arg) do { SEND_STRING("\b" arg); return true; } while (false)
#define FLW_TWO_MATCH(arg) do { SEND_STRING("\b\b" arg); return true; } while (false)
#define FLW_ONE(key, str) FLW_ONE_KEY(key) { FLW_ONE_MATCH(str); }
#define FLW_TWO(key1, key2, str) FLW_TWO_KEYS(key1, key2) { FLW_TWO_MATCH(str); }

bool flw_matcher(uint16_t first, uint16_t second)
{
    if (IS_LAYER_ON(RUSS))
    {
        FLW_ONE(KC_T, "`")
        FLW_ONE(KC_B, "q")
        FLW_ONE(KC_I, "o")
        FLW_ONE(KC_M, "]")
    }
    else
    {
        FLW_TWO(KC_A, KC_A, "alignas")
        FLW_TWO(KC_A, KC_O, "alignof")

        FLW_TWO(KC_B, KC_R, "break")

        FLW_TWO(KC_C, KC_S, "case")
        FLW_TWO(KC_C, KC_H, "catch")
        FLW_TWO(KC_C, M(kf_F2), "char16_t")
        FLW_TWO(KC_C, M(kf_F4), "char32_t")
        FLW_TWO(KC_C, KC_L, "class")
        FLW_TWO(KC_C, KC_O, "const")
        FLW_TWO(KC_C, KC_E, "constexpr")
        FLW_TWO(KC_C, KC_A, "const_cast<")
        FLW_TWO(KC_C, KC_T, "continue")

        FLW_TWO(KC_D, KC_T, "decltype(")
        FLW_TWO(KC_D, KC_F, "default")
        FLW_TWO(KC_D, KC_L, "delete")
        FLW_TWO(KC_D, KC_C, "dynamic_cast<")

        FLW_TWO(KC_E, KC_L, "else")
        FLW_TWO(KC_E, KC_N, "enum")
        FLW_TWO(KC_E, KC_P, "explicit")
        FLW_TWO(KC_E, KC_T, "extern")

        FLW_TWO(KC_F, KC_A, "false")
        FLW_TWO(KC_F, KC_R, "friend")

        FLW_TWO(KC_I, KC_L, "inline")
        FLW_TWO(KC_I, KC_C,  "#include <")

        FLW_TWO(KC_N, KC_S, "namespace")
        FLW_TWO(KC_N, KC_E, "noexcept")
        FLW_TWO(KC_N, KC_P, "nullptr")

        FLW_TWO(KC_O, KC_V, "override")

        FLW_TWO(KC_P, KC_V, "private:")
        FLW_TWO(KC_P, KC_T, "protected:")
        FLW_TWO(KC_P, KC_B, "public:")

        FLW_TWO(KC_R, KC_E, "reflexpr(")
        FLW_TWO(KC_R, KC_G, "register")
        FLW_TWO(KC_R, KC_C, "reinterpret_cast<")
        FLW_TWO(KC_R, KC_T, "return")

        FLW_TWO(KC_S, KC_I, "signed")
        FLW_TWO(KC_S, KC_Z, "sizeof")
        FLW_TWO(KC_S, KC_T, "static")
        FLW_TWO(KC_S, KC_A, "static_assert(")
        FLW_TWO(KC_S, KC_C, "static_cast<")
        FLW_TWO(KC_S, KC_R, "struct")
        FLW_TWO(KC_S, KC_W, "switch")

        FLW_TWO(KC_T, KC_E, "template<")
        FLW_TWO(KC_T, KC_L, "thread_local")
        FLW_TWO(KC_T, KC_W, "throw")
        FLW_TWO(KC_T, KC_R, "true")
        FLW_TWO(KC_T, KC_D, "typedef")
        FLW_TWO(KC_T, KC_I, "typeid")
        FLW_TWO(KC_T, KC_N, "typename")

        FLW_TWO(KC_U, KC_N, "union")
        FLW_TWO(KC_U, KC_S, "using")

        FLW_TWO(KC_V, KC_I, "virtual")
        FLW_TWO(KC_V, KC_O, "volatile")

        FLW_TWO(KC_W, KC_H, "while")

        FLW_ONE(KC_A, "auto")
        FLW_ONE(KC_B, "bool")
        FLW_ONE(KC_C, "char")
        FLW_ONE(KC_D, "double")
        FLW_ONE(KC_F, "float")
        FLW_ONE(KC_G, "goto")
        FLW_ONE(KC_I, "int")
        FLW_ONE(KC_L, "long")
        FLW_ONE(KC_M, "mutable")
        FLW_ONE(KC_O, "operator")
        FLW_ONE(KC_S, "short")
        FLW_ONE(KC_T, "this")
        FLW_ONE(KC_U, "unsigned")
        FLW_ONE(KC_V, "void")
        FLW_ONE(KC_W, "wchar_t")
    }

    return false;
}

void handle_pulse(unsigned pulse)
{
    switch (pulse)
    {
        case 2:
        {
            layer_off(RUSS);
        }
        break;
        case 3:
        {
            layer_on(RUSS);
        }
        break;
    }
}

unsigned pulse_count = 0;
uint32_t pulse_timer = 0;

// Runs constantly in the background, in a loop.
void matrix_scan_user(void)
{
    static int iteration = 0;
    
    if (++iteration > 500)
    {
        iteration = 0;
    }
    if (iteration > 450)
    {
        ergodox_right_led_3_on();
    }
    else
    {
        ergodox_right_led_3_off();
    }

    if (IS_LAYER_ON(RUSS))
    {
        ergodox_right_led_1_on();
    }
    else
    {
        ergodox_right_led_1_off();
    }

    if (IS_LAYER_ON(MDIA))
    {
        ergodox_right_led_2_on();
    }
    else
    {
        ergodox_right_led_2_off();
    }

    if (pulse_count && timer_elapsed32(pulse_timer) > 200)
    {
        handle_pulse(pulse_count);
        pulse_count = 0;
    }
};

uint32_t probable_table_en[] =
{
           0x0, // a
      0x104111, // b
      0x104111, // c
      0x104111, // d
      0x7ebeee, // e
      0x104111, // f
      0x104111, // g
           0x0, // h
       0x22808, // i
           0x0, // j
           0x0, // k
     0x1104111, // l
     0x1104111, // m
     0x1104111, // n
     0x24ebcee, // o
      0x104111, // p
           0x0, // q
     0x1104111, // r
     0x1104111, // s
     0x1104111, // t
           0x0, // u
           0x0, // v
        0x4000, // w
           0x0, // x
           0x0, // y
      0x104111  // z
};

uint32_t probable_table_ru[] =
{
           0x0, // а   0
           0x0, // б   1
           0x0, // в   2
           0x0, // г   3
        0x2000, // д   4
      0x809004, // её  5
           0x0, // ж   6
           0x0, // з   7
      0xa09200, // ий  8
           0x0, // к   9
         0x100, // л  10
           0x0, // м  11
    0x10002121, // н  12
       0x10004, // о  13
           0x0, // п  14
           0x0, // р  15
        0x2121, // с  16
        0x2000, // т  17
           0x0, // у  18
           0x0, // ф  19
           0x0, // х  20
           0x0, // ц  21
           0x0, // ч  22
           0x0, // шщ 23
           0x0, // ы  24
           0x0, // ьъ 25
           0x0, // э  26
           0x0, // ю  27
        0x1400, // я  28
};

uint8_t kc_to_ru(uint16_t kc)
{
    switch (kc)
    {
        // фыаяэ
        case KC_A: return 19;
        case KC_S: return 24;
        case KC_F: return 0;
        case KC_Z: return 28;
        case KC_QUOT: return 26;

        // гиеоу
        case KC_U: return 3;
        case KC_B: return 8;
        case KC_T: return 5;
        case KC_J: return 13;
        case KC_E: return 18;

        // шьюж
        case KC_I: return 23;
        case KC_M: return 25;
        case KC_DOT: return 27;
        case KC_SCLN: return 6;

        // цмрпх
        case KC_W: return 21;
        case KC_V: return 11;
        case KC_H: return 15;
        case KC_G: return 14;
        case KC_LBRC: return 20;

        // лтснз
        case KC_K: return 10;
        case KC_N: return 17;
        case KC_C: return 16;
        case KC_Y: return 12;
        case KC_P: return 7;

        // бдвкч
        case KC_COMM: return 1;
        case KC_L: return 4;
        case KC_D: return 2;
        case KC_R: return 9;
        case KC_X: return 22;
    }
    return 255;
}

bool is_alpha_en(uint16_t kc)
{
    return kc >= KC_A && kc <= KC_Z;
}

bool process_record_user(uint16_t kc, keyrecord_t * record)
{
    uint16_t const current_time = record->event.time;

    static uint16_t taps[3] = {};
    static uint16_t prev_tap_time = 0;
    static uint16_t active_kc = 0;
    static bool     active_uc = false;

    bool const key_down = record->event.pressed;
    bool const key_up   = !key_down;

    if (key_down)
    {
        if (kc == KC_BSPC)
        {
            taps[2] = taps[1]; taps[1] = taps[0]; taps[0] = 0;
        }
        else
        {
            taps[0] = taps[1]; taps[1] = taps[2]; taps[2] = kc;
        }
    }

    bool const same_key = taps[1] == taps[2];
    bool const quick    = timer_elapsed(prev_tap_time) < 330;
    prev_tap_time = current_time;

    uint8_t const layer = biton32(layer_state);

    bool is_en = layer == BASE;
    bool is_ru = layer == RUSS;

    if (key_down && same_key && quick && (is_en || is_ru))
    {
        action_t action = action_for_key(is_en ? SYMB : SYMU, record->event.key);
        bool const has_action = action.code != ACTION_TRANSPARENT && (action.code != ACTION_NO || is_ru);

        bool ignore = false;

        if (is_en)
        {
            bool const all_alphas = is_alpha_en(taps[0]) && is_alpha_en(taps[1]);
            if (all_alphas)
            {
                uint8_t const bit = taps[0] - KC_A;
                uint8_t const row = taps[1] - KC_A;

                ignore = probable_table_en[row] & (1UL << bit);
            }
        }
        else if (is_ru)
        {
            uint8_t const bit = kc_to_ru(taps[0]);
            uint8_t const row = kc_to_ru(taps[1]);

            if (bit != 255 && row != 255)
            {
                ignore = probable_table_ru[row] & (1UL << bit);
            }
        }

        if (ignore)
        {
            taps[0] = taps[1] = taps[2] = 0;
        }

        uint16_t wiked_keycode = keycode_config(keymap_key_to_keycode(SYMU, record->event.key));
        bool const is_unicode = is_ru;

        if (has_action && !active_kc && !ignore)
        {
            active_kc = kc;
            active_uc = is_unicode;
            taps[0] = taps[1] = taps[2] = 0;

            register_code(KC_BSPC);
            unregister_code(KC_BSPC);

            if (is_unicode)
            {
                process_unicode(wiked_keycode, record);
            }
            else
            {
                process_action(record, action);
            }

            return false;
        }
    }

    if (key_up && active_kc && active_kc == kc)
    {
        if (active_uc)
        {
            active_uc = false;
        }
        else
        {
            action_t action = action_for_key(SYMB, record->event.key);
            process_action(record, action);
        }

        active_kc = 0;

        return false;
    }

    if (key_down && kc == M(kf_flw))
    {
        flw_matcher(taps[0], taps[1]);
    }

    return true;
}

void led_set_user(uint8_t code)
{
    static uint8_t prev_code = 0;
    bool const is_pulse = ((code ^ prev_code) & (1 << USB_LED_SCROLL_LOCK)) != 0;
    prev_code = code;
    if (is_pulse)
    {
        ++pulse_count;
        pulse_timer = timer_read32();
    }
}

