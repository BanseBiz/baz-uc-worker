#pragma once

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

class Worker {
  public:
    Worker(
      float rate,
      uint16_t stack_size
    );
    void begin();
    void end();
    virtual void init() = 0;
  
  protected:
    static void task(void* params);
    void call();
    virtual void loop() = 0;
    TaskHandle_t _task_handle = NULL;
    const char* _name;
    const uint16_t _delay;
    const uint16_t _stack_size;
    static uint16_t _worker_cnt;
};