#include "TinyOS.h"

int kern_entry() {

    console_init();
    
    char *msg = "Hello TinyOS!";
    while (1) {
        console_write(msg, strlen(msg));
    }
    
    return 0;
}