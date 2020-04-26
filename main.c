#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
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

    int daemonize = 0, immediate = 0, option = 0, redis_port = 6379;
    while((option = getopt(argc, argv,"dihr:")) != -1){
        switch(option){
            case 'd':
                daemonize = 1;
                break;
            case 'i':
                immediate = 1;
                break;
            case 'r':
                redis_port = atoi(optarg);
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
            keylogger(keyboard, immediate, redis_port);

            close(keyboard);
            free(KEYBOARD_DEVICE);
        }
    } else {
        keylogger(keyboard, immediate, redis_port);

        close(keyboard);
        free(KEYBOARD_DEVICE);
    }

    return 0;
}

void print_usage_and_quit(char *application_name){
    printf("Usage: %s [-d] [-i] [-r]\n\nA little tool for text expansion. Shortcuts can be 5 chars long max."
    "\n\nhelp:\n -h display this help message\n -d daemonize\n -i immediately execute shortcut\n -r custom redis port\n", application_name);
    exit(1);
}
