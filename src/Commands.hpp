class DisplayPlainTextCommand : public Command
{
private:
    string text;

public:

    DisplayPlainTextCommand(PlainTextMessage m) : text(m.text) {}

    void execute()
    {
        cout << text << endl;
    }
};

class CheckLoginConnectionCommand : public Command
{
private:
    NewConnectionMessage m;

public:

    CheckLoginConnectionCommand(NewConnectionMessage m) : m(m) {}

    void execute()
    {
        cout << "New connection " <<m.connectionId << ' ' << m.ipAddress << ':' << toString(m.port) << endl;
        CheckLoginThread::startThread();
    }
};

class MoveBlobCommand : public Command
{
private:
    GameWorld& gameWorld;
    int connectionId;
    double direction;

public:

    MoveBlobCommand(GameWorld& gameWorld, MoveInDirectionMessage m)
    :   gameWorld(gameWorld), connectionId(m.connectionId), direction(m.direction) {}

    void execute()
    {
        Player* player = gameWorld.findPlayerByConnectionId(connectionId);
        if (player != NULL)
        {
            BlobController& blobController = player->blobController;
            blobController.move(direction);
        }
    }
};

class MoveTowardCoordinatesCommand : public Command
{
private:
    GameWorld& gameWorld;
    int connectionId;
    double direction;
    double x, y;

public:

    MoveTowardCoordinatesCommand(GameWorld& gameWorld, MoveTowardCoordinatesMessage m)
    :   gameWorld(gameWorld), connectionId(m.connectionId), x(m.x), y(m.y) {}

    void execute()
    {
        Player* player = gameWorld.findPlayerByConnectionId(connectionId);
        if (player != NULL)
        {
            ViewWindow& window = player->window;
            BlobController& blobController = player->blobController;
            degreeAngleBetweenPoints(window.serverToClientView(blobController.getBlob(0).getCenterPoint()), Point(x, window.length - y), direction);
            MoveInDirectionMessage mm(direction);
            mm.connectionId = connectionId;
            MoveBlobCommand mc(gameWorld, mm);
            mc.execute();
        }
    }
};

class SendChatMessageCommand : public Command
{
private:
    int connectionId;
    NetworkMessageMailer& mailer;
    GameWorld& gameWorld;
    string username;
    string text;

public:
    SendChatMessageCommand (NetworkMessageMailer& mailer, GameWorld& gameWorld, ChatMessage message)
    :   connectionId(message.connectionId), mailer(mailer), gameWorld(gameWorld), username(message.username), text(message.text) {}

    void execute()
    {
        if (username == "")
        {
            Player* player = gameWorld.findPlayerByConnectionId(connectionId);
            if (player != NULL)
            {
                username = player->username;
            }
            else
            {
                return;
            }
            for (int i = 0; i < gameWorld.players.size(); i++)
            {
                mailer.push(gameWorld.players[i]->connectionId, new ChatMessage(username, text));
            }
        }
        else
        {
            //  Send message to specific user here later.
        }
    }
};

class LoginCommand : public Command
{
private:
    GameWorld& gameWorld;
    NetworkMessageMailer& mailer;
    string username;
    string password;
    int connectionId;

public:

    LoginCommand(GameWorld& gameWorld, LoginMessage m, NetworkMessageMailer& mailer)
    :   gameWorld(gameWorld), mailer(mailer),
        username(m.username), password(m.password), connectionId(m.connectionId) {}

    void execute()
    {
        // Do authentication here later.
        if (gameWorld.findPlayerByConnectionId(connectionId) == NULL)
        {
            cout << "New player " << username << " on connection " << connectionId << endl;
            gameWorld.addPlayer(connectionId, connectionId, username);
            ChatMessage cm("", "Joined the game");
            cm.connectionId = connectionId;
            SendChatMessageCommand c(mailer, gameWorld, cm);
            c.execute();
        }
        else
        {
            mailer.push(connectionId, new LoginFailedMessage());
        }
    }
};

class RemovePlayerCommand : public Command
{
private:
    GameWorld& gameWorld;
    NetworkMessageMailer& mailer;
    int connectionId;
    ConnectionPool& connectionPool;

public:

    RemovePlayerCommand(GameWorld& gameWorld, NetworkMessageMailer& mailer, ConnectionTimeoutMessage m, ConnectionPool& connectionPool)
    :   gameWorld(gameWorld), mailer(mailer), connectionId(m.connectionId), connectionPool(connectionPool) {}

    RemovePlayerCommand(GameWorld& gameWorld, NetworkMessageMailer& mailer, LogoutMessage m, ConnectionPool& connectionPool)
    :   gameWorld(gameWorld), mailer(mailer), connectionId(m.connectionId), connectionPool(connectionPool) {}

    void execute()
    {
        cout << "Disconnect connection " << connectionId << endl;
        connectionPool.getConnection(connectionId).disconnect();
        Player* player = gameWorld.findPlayerByConnectionId(connectionId);
        if (player != NULL)
        {
            cout << "Remove player " << player->playerId << " " << player->username << endl;
            ChatMessage cm("", "Left the game");
            cm.connectionId = connectionId;
            SendChatMessageCommand c(mailer, gameWorld, cm);
            c.execute();
            gameWorld.removePlayerByConnectionId(connectionId);
        }
    }
};

class BoostCommand : public Command
{
private:
    GameWorld& gameWorld;
    int connectionId;

public:

    BoostCommand(GameWorld& gameWorld, BoostMessage message) : gameWorld(gameWorld), connectionId(message.connectionId) {}

    void execute()
    {
        Player* player = gameWorld.findPlayerByConnectionId(connectionId);
        if (player != NULL)
        {
            BlobController& blobController = player->blobController;
            blobController.boost();
        }
    }
};

class SetBlobPropertiesCommand : public Command
{
private:
    GameWorld& gameWorld;
    SetBlobPropertiesMessage message;

public:

    SetBlobPropertiesCommand(GameWorld& gameWorld, SetBlobPropertiesMessage message) : gameWorld(gameWorld), message(message) {}

    void execute()
    {
        Player* player = gameWorld.findPlayerByConnectionId(message.connectionId);
        if (player != NULL)
        {
            BlobController& blobController = player->blobController;
            //  Right now the only property we're setting is the color.
            blobController.setColor(message.properties[0].value);
        }
    }
};

class SplitCommand : public Command
{
private:
    GameWorld& gameWorld;
    SplitMessage message;

public:

    SplitCommand(GameWorld& gameWorld, SplitMessage message) : gameWorld(gameWorld), message(message){}

    void execute()
    {
        Player* player = gameWorld.findPlayerByConnectionId(message.connectionId);
        if (player != NULL)
        {
            BlobController& blobController = player->blobController;
            blobController.split();
        }
    }
};

class PingReplyCommand : public Command
{
private:
    int connectionId;
    NetworkMessageMailer& mailer;

public:

    PingReplyCommand(PingMessage ping, NetworkMessageMailer& mailer) : connectionId(ping.connectionId), mailer(mailer) {}

    void execute()
    {
        mailer.push(connectionId, new PingMessage());
    }
};
