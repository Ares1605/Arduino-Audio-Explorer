// #ifndef Connection_h
// #define Connection_h

// #include <WiFiS3.h>

// // Forward declaration instead of including ClientManager.h
// class ClientManager;  // Add this line

// class Connection {
// friend class ClientManager;

// private:
//     ClientManager* clientManager;
// public:
//     WiFiClient* client;
//     void (*callback)(String message);
//     Connection(ClientManager* clientManager_, WiFiClient* client_);
//     ~Connection();
//     void onResponse(void (*callback)(String message));
// };

// #endif