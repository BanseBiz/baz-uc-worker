#include "Worker.h"

#define WORKER_PRIO 1
#define WORKER_NAME(id) "worker_"+id

uint16_t Worker::_worker_cnt = 0;
std::vector<Worker*> Worker::_workers;

Worker::Worker (
    float rate,
    uint16_t stack_size
) : _name(WORKER_NAME(_worker_cnt++)),
    _delay((uint16_t)(1000.f/rate)),
    _stack_size(stack_size)
{
    _workers.push_back(this);
    _worker_cnt++;
}

void Worker::begin() {
    if (_task_handle != NULL) {
        return;
    }
    xTaskCreate(
        Worker::task,
        _name,
        _stack_size,
        (void*) this,
        WORKER_PRIO,
        &_task_handle
    );
}

void Worker::end() {
    if (_task_handle != NULL) {
        vTaskDelete(_task_handle);
    }
}

void Worker::call() {
    TickType_t last_wake_time;
    const TickType_t frequency = pdMS_TO_TICKS(_delay);
    while(true) {
        last_wake_time = xTaskGetTickCount();
        loop();
        if (_delay > 1) {
            vTaskDelayUntil(&last_wake_time, frequency);
        }
    }
}

void Worker::task(void* params) {
    Worker* instance = (Worker*) params;
    instance->call();
    vTaskDelete(NULL);
}

void Worker::all_begin() {
    for (auto worker : _workers) {
        worker->begin();
    }
}

void Worker::all_init() {
    for (auto worker : _workers) {
        worker->init();
    }
}