#pragma once
#include "build/include/symbols.h" // Please change it to your own path

#define yield() asm volatile("yield"); // Use yield to change the running task

static void UserTaskInit(uint8_t* stack); // Task init code: Make sure it's short and fast!
static void UserTaskCode(uint8_t* stack); // Task code: In-looped already!
static void UserTask(uint8_t* stack){
    UserTaskInit(stack + 8);
    yield();
    while(1){
        UserTaskCode(stack + 8);
        yield();
    }
}

int main(){
    return 0;
}