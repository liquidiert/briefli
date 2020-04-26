#include <linux/input.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <hiredis.h>
#include <xdo.h>
#include "keylogger.h"

int loop = 1;

void sigint_handler(int sig){
    loop = 0;
}

int buffer_size = 0;

unsigned char *input_buffer;

void reset_buffer() {
    input_buffer = malloc(sizeof(unsigned char) * 5);
    buffer_size = 0;
}

void extend_buffer(unsigned char to_extend) {
    if (to_extend == 29) return; // filter ctrl
    input_buffer[buffer_size++] = to_extend;
    if (buffer_size == BUFFER_SIZE) {
        reset_buffer();
    }
}

void build_redis_command(char *res) {
    if (input_buffer == NULL) return;
    for (int n = 0; n < buffer_size; n++) {
        snprintf(res + n, 5-n, "%s", keycodes[input_buffer[n]]);
    }
}

/// checks if val in range
/// inclusive start and end
int is_in_range(int start, int end, int val) {
    return start <= val && val <= end; 
}

int is_valid_key(int key) {
    return is_in_range(16, 25, key) || is_in_range(30, 38, key) || is_in_range(44, 50, key)
            || key == 42 || key == 29; // shift and ctrl
}


void keylogger(int keyboard, int immediate){
    int eventSize = sizeof(struct input_event);
    int bytesRead = 0;
    struct input_event events[NUM_EVENTS];
    int i;

    // allocate buffer for shortcuts of maximum size 5
    input_buffer = malloc(sizeof(unsigned char) * 5);
    // redis setup
    redisContext *context = redisConnect("127.0.0.1", 6379);
    redisReply *reply;
    if (context == NULL || context->err) {
        if (context) {
            printf("error: %s\n", context->errstr);
        } else {
            printf("Can't allocate redis context! Is redis installed and running?\n");
        }
        exit(-2);
    }

    // xdo setup
    xdo_t *xdo = xdo_new(NULL);

    signal(SIGINT, sigint_handler);

    while(loop){
        sleep(0.25);
        bytesRead = read(keyboard, events, eventSize * NUM_EVENTS);
        for(i = 0; i < (bytesRead / eventSize); ++i){
            if(events[i].type == EV_KEY){
                if(events[i].value == 1){
                    if(is_valid_key((int) events[i].code)){
                        extend_buffer(events[i].code);
                        if (immediate || events[i].code == 29) {
                            char redis_args[5];
                            build_redis_command(redis_args);
                            reply = redisCommand(context, "get %s", redis_args);
                            if (reply->type == REDIS_REPLY_STRING) {
                                xdo_send_keysequence_window(xdo, CURRENTWINDOW, "Ctrl+BackSpace", 12000);
                                xdo_send_keysequence_window(xdo, CURRENTWINDOW, "Shift", 12000);
                                xdo_enter_text_window(xdo, CURRENTWINDOW, reply->str, 12000);
                                reset_buffer();
                            }
                            freeReplyObject(reply);
                        }
                    }
                }
            }
        }
    }
    free(input_buffer);
    redisFree(context);
    xdo_free(xdo);
}
