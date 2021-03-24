#include "keyboard.h"
#include "port.h"
#include "../cpu/isr.h"
#include "video.h"
#include "../kernel/utils/utils.h"
#include "../shell/string.h"
#include "../shell/function.h"
#include "../shell/commands.h"

#define SC_MAX 57
#define BACKSPACE 0x0E
#define ENTER 0x1C

static char key_buffer[256];
/*ctr is the index used for key_buffer. Then increments after every character append so next character will be added after key press.
 *after each append, incremented ctr is used to set next value in keybuffer to NULL (\0) */
short ctr = 0;


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



static void keyboard_callback(registers_t *regs) {
    uint8_t scancode = port_byte_in(0x60);
    int len = string_length(key_buffer);

    if(scancode > SC_MAX) return;

    if(scancode == BACKSPACE){
        if(key_buffer[0] != '\0'){
            print_backspace();
            ctr -=1;
            key_buffer[ctr] = '\0';
        }
    }else if(scancode == ENTER){
        print_nl();
        execute_command(key_buffer);
        key_buffer[0] = '\0';
        ctr = 0;
    }else{
        char letter = scancode_to_char[(int)scancode];
        append(key_buffer, letter, &ctr);
        key_buffer[ctr] = '\0';
        char str[2] = {letter, '\0'};
        print_string(str);
    }
    UNUSED(regs);
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