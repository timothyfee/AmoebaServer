struct LoginMessage : public NetworkMessage
{
    const string username;
    const string password;

    LoginMessage(string username, string password) : NetworkMessage("LoginMessage"), username(username), password(password) {}
};

struct LoginFailedMessage : public NetworkMessage
{
    LoginFailedMessage() : NetworkMessage("LoginFailedMessage") {}
};

struct LogoutMessage : public NetworkMessage
{
    LogoutMessage() : NetworkMessage("LogoutMessage") {}
};

struct MoveTowardCoordinatesMessage : public NetworkMessage
{
    const double x;
    const double y;

    MoveTowardCoordinatesMessage(double x, double y) : NetworkMessage("MoveTowardCoordinatesMessage"), x(x), y(y) {}
};

struct MoveInDirectionMessage : public NetworkMessage
{
    const double direction;

    MoveInDirectionMessage(double direction) : NetworkMessage("MoveInDirectionMessage"), direction(direction) {}
};

struct BlobStateMessage : public NetworkMessage
{
    const deque<KeyValuePair> properties;

    BlobStateMessage(deque<KeyValuePair> properties) : NetworkMessage("BlobStateMessage"), properties(properties) {}
};

struct PelletPositionMessage : public NetworkMessage
{
    const double x;
    const double y;
    const long id;

    PelletPositionMessage(double x, double y, long id) : NetworkMessage("PelletPositionMessage"), x(x), y(y), id(id) {}
};

struct SetBlobPropertiesMessage : public NetworkMessage
{
    const deque<KeyValuePair> properties;

    SetBlobPropertiesMessage(deque<KeyValuePair> properties) : NetworkMessage("SetBlobPropertiesMessage"), properties(properties) {}
};

struct ChatMessage : public NetworkMessage
{
    const string username;
    const string text;

    ChatMessage(string username, string text) : NetworkMessage("ChatMessage"), username(username), text(text) {}
};

struct BoostMessage : public NetworkMessage
{
    BoostMessage() : NetworkMessage("BoostMessage") {}
};

struct SplitMessage : public NetworkMessage
{
    SplitMessage() : NetworkMessage("SplitMessage") {}
};

struct HighScoreMessage : public NetworkMessage
{
    const string text;

    HighScoreMessage(string text) : NetworkMessage("HighScoreMessage"), text(text) {}
};

struct CurrentScoreMessage : public NetworkMessage
{
    const string text;

    CurrentScoreMessage(string text) : NetworkMessage("CurrentScoreMessage"), text(text) {}
};
