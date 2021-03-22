#include "keyboard.h"
#include "port.h"
#include "../cpu/isr.h"
#include "video.h"
#include "../kernel/utils/utils.h"
#include "../shell/string.h"

#define SC_MAX 57

static char key_buffer[256];

const char scancode_to_char[] = {
  '?', '?', '1', '2', '3', '4', '5',
  '6', '7', '8', '9', '0', '-', '=',
  '?', '?', 'Q', 'W', 'E', 'R', 'T',
  'Y', 'U', 'I', 'O', 'P', '[', ']',
  '?', '?', 'A', 'S', 'D', 'F', 'G',
  'H', 'J', 'K', 'L', ';', '\'', '`',
  '?', '\\', 'Z', 'X', 'C', 'V', 'B',
  'N', 'M', ',', '.', '/', '?', '?',
  '?', ' '
};


void print_letter(uint8_t scancode) {
    switch (scancode) {
        case 0x0:
            print_string("ERROR", GREEN_TEXT);
            break;
        case 0x1:
            print_string("ESC", GREEN_TEXT);
            break;
        case 0x2:
            print_string("1", GREEN_TEXT);
            break;
        case 0x3:
            print_string("2", GREEN_TEXT);
            break;
        case 0x4:
            print_string("3", GREEN_TEXT);
            break;
        case 0x5:
            print_string("4", GREEN_TEXT);
            break;
        case 0x6:
            print_string("5", GREEN_TEXT);
            break;
        case 0x7:
            print_string("6", GREEN_TEXT);
            break;
        case 0x8:
            print_string("7", GREEN_TEXT);
            break;
        case 0x9:
            print_string("8", GREEN_TEXT);
            break;
        case 0x0A:
            print_string("9", GREEN_TEXT);
            break;
        case 0x0B:
            print_string("0", GREEN_TEXT);
            break;
        case 0x0C:
            print_string("-", GREEN_TEXT);
            break;
        case 0x0D:
            print_string("+", GREEN_TEXT);
            break;
        case 0x0E:
            print_string("Backspace", GREEN_TEXT);
            break;
        case 0x0F:
            print_string("Tab", GREEN_TEXT);
            break;
        case 0x10:
            print_string("Q", GREEN_TEXT);
            break;
        case 0x11:
            print_string("W", GREEN_TEXT);
            break;
        case 0x12:
            print_string("E", GREEN_TEXT);
            break;
        case 0x13:
            print_string("R", GREEN_TEXT);
            break;
        case 0x14:
            print_string("T", GREEN_TEXT);
            break;
        case 0x15:
            print_string("Y", GREEN_TEXT);
            break;
        case 0x16:
            print_string("U", GREEN_TEXT);
            break;
        case 0x17:
            print_string("I", GREEN_TEXT);
            break;
        case 0x18:
            print_string("O", GREEN_TEXT);
            break;
        case 0x19:
            print_string("P", GREEN_TEXT);
            break;
        case 0x1A:
            print_string("[", GREEN_TEXT);
            break;
        case 0x1B:
            print_string("]", GREEN_TEXT);
            break;
        case 0x1C:
            print_string("ENTER", GREEN_TEXT);
            break;
        case 0x1D:
            print_string("LCtrl", GREEN_TEXT);
            break;
        case 0x1E:
            print_string("A", GREEN_TEXT);
            break;
        case 0x1F:
            print_string("S", GREEN_TEXT);
            break;
        case 0x20:
            print_string("D", GREEN_TEXT);
            break;
        case 0x21:
            print_string("F", GREEN_TEXT);
            break;
        case 0x22:
            print_string("G", GREEN_TEXT);
            break;
        case 0x23:
            print_string("H", GREEN_TEXT);
            break;
        case 0x24:
            print_string("J", GREEN_TEXT);
            break;
        case 0x25:
            print_string("K", GREEN_TEXT);
            break;
        case 0x26:
            print_string("L", GREEN_TEXT);
            break;
        case 0x27:
            print_string(";", GREEN_TEXT);
            break;
        case 0x28:
            print_string("'", GREEN_TEXT);
            break;
        case 0x29:
            print_string("`", GREEN_TEXT);
            break;
        case 0x2A:
            print_string("LShift", GREEN_TEXT);
            break;
        case 0x2B:
            print_string("\\", GREEN_TEXT);
            break;
        case 0x2C:
            print_string("Z", GREEN_TEXT);
            break;
        case 0x2D:
            print_string("X", GREEN_TEXT);
            break;
        case 0x2E:
            print_string("C", GREEN_TEXT);
            break;
        case 0x2F:
            print_string("V", GREEN_TEXT);
            break;
        case 0x30:
            print_string("B", GREEN_TEXT);
            break;
        case 0x31:
            print_string("N", GREEN_TEXT);
            break;
        case 0x32:
            print_string("M", GREEN_TEXT);
            break;
        case 0x33:
            print_string(",", GREEN_TEXT);
            break;
        case 0x34:
            print_string(".", GREEN_TEXT);
            break;
        case 0x35:
            print_string("/", GREEN_TEXT);
            break;
        case 0x36:
            print_string("Rshift", GREEN_TEXT);
            break;
        case 0x37:
            print_string("Keypad *", GREEN_TEXT);
            break;
        case 0x38:
            print_string("LAlt", GREEN_TEXT);
            break;
        case 0x39:
            print_string("Space", GREEN_TEXT);
            break;
        default:
            /* 'keuyp' event corresponds to the 'keydown' + 0x80
             * it may still be a scancode we haven't implemented yet, or
             * maybe a control/escape sequence */
            if (scancode <= 0x7f) {
                print_string("Unknown key down", GREEN_TEXT);
            } else if (scancode <= 0x39 + 0x80) {
                print_string("key up ", GREEN_TEXT);
                print_letter(scancode - 0x80);
            } else print_string("Unknown key up", GREEN_TEXT);
            break;
    }
}

static void keyboard_callback(registers_t *regs) {
    uint8_t scancode = port_byte_in(0x60);

    if(scancode > SC_MAX) return;

    char letter = scancode_to_char[(int)scancode];
    append(key_buffer, letter);
    char str[2] = {letter, '\0'};
    print_string(str, GREEN_TEXT);
}

void init_keyboard() {
    /*First arg is index of gate(33) and second is irs_t handler function*/
    register_interrupt_handler(IRQ1, keyboard_callback);
}

/*register_interrupt_handler takes function pointer as its second argument, so
 *when kernel calls init_keyboard(), the function that the second argument function 
 *pointer points to is registered(keyboard_callback will registered in the array
 *interrupt_handlers(type isr_t) at irq1)
 */