#include <Arduino.h>
#include "Router.h"

void Router::bind(String endpoint_, void (*callback_)(String)) {
    endpoint = endpoint_;
    callback = callback_;
}

void Router::recieve() {
    void (*callback)(int);
};
