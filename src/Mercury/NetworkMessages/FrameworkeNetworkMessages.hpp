struct PlainTextMessage : public NetworkMessage
{
    const string text;

    PlainTextMessage(string text) : NetworkMessage("PlainTextMessage"), text(text) {}
};

struct NewConnectionMessage : public NetworkMessage
{
    const string ipAddress;
    const int port;

    NewConnectionMessage(string ipAddress, int port) : NetworkMessage("NewConnectionMessage"), ipAddress(ipAddress), port(port) {}
};

struct ConnectionTimeoutMessage : public NetworkMessage
{
    ConnectionTimeoutMessage() : NetworkMessage("ConnectionTimeoutMessage") {}
};

struct PingMessage : public NetworkMessage
{
    PingMessage() : NetworkMessage("PingMessage") {}
};
