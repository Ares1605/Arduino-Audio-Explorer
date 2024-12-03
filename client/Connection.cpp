// #include <WiFiS3.h>
// #include "ClientManager.h"

// Connection::Connection(ClientManager* clientManager_, WiFiClient* client_) {
//   clientManager = clientManager_;
//   client = client_;
//   callback = nullptr;
// }
// Connection::~Connection() {
//     if (client) {
//         client->stop();
//     }
// }
// void Connection::onResponse(void (*callback_)(String message)) {
//   callback = callback_;
//   clientManager->addConnection(this);
// }
