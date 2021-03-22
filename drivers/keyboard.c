#include "keyboard.h"
#include "port.h"
#include "../cpu/isr.h"
#include "video.h"
#include "../kernel/utils/utils.h"
#include "../shell/string.h"

#define SC_MAX 57
#define BACKSPACE 0x0e
#define ENTER 0x1c

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


void clear_buffer(char str[])
{
    for(int i = 0; i < string_length(str); i++){
        str[i] = '\0';
    }
}


static void keyboard_callback(registers_t *regs) {
    uint8_t scancode = port_byte_in(0x60);

    if(scancode > SC_MAX) return;

    if(scancode == BACKSPACE){
        if(backspace(key_buffer)){
            print_backspace();
        }
    }else if(scancode == ENTER){
        print_nl();
        execute_command(key_buffer);
        key_buffer[0] = '\0';
    }else{
        char letter = scancode_to_char[(int)scancode];
        char str[2] = {letter, '\0'};
        append(key_buffer, letter);
        print_string(str);
    }
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