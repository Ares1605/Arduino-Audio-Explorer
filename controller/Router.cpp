#include <Arduino.h>
#include "Router.h"

Router::Router() {

}

void Router::bind(String endpoint_, void (*callback_)(String)) {
    endpoint = endpoint_;
    callback = callback_;
}

void Router::recieve() {
    void (*callback)(int);
};
