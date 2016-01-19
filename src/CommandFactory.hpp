class CommandFactory
{
private:
    GameWorld &gameWorld;
    ConnectionPool& connectionPool;
    NetworkMessageMailer& mailer;

public:

    CommandFactory(GameWorld& gameWorld, ConnectionPool& connectionPool, NetworkMessageMailer& mailer)
    :   gameWorld(gameWorld), connectionPool(connectionPool), mailer(mailer) {}

    deque<Command*> generateCommands(deque<NetworkMessage*> messages)
    {
        deque<Command*> commands = lookupCommands(messages);
        return commands;
    }

private:

    inline deque<Command*> lookupCommands(deque<NetworkMessage*> messages)
    {
        deque<Command*> commands;
        for (int i = 0; i < messages.size(); i++)
        {
            NetworkMessage& message = *messages[i];
            string messageType = message.type;
            switchString(messageType)
            {
                primaryCaseString("MoveTowardCoordinatesMessage")
                MoveTowardCoordinatesMessage& m = (MoveTowardCoordinatesMessage&)message;
                commands.push_back(new MoveTowardCoordinatesCommand(gameWorld, m));
                endCase

                caseString("PingMessage")
                PingMessage& m = (PingMessage&)message;
                commands.push_back(new PingReplyCommand(m, mailer));
                endCase

                caseString("BoostMessage")
                BoostMessage& m = (BoostMessage&)message;
                commands.push_back(new BoostCommand(gameWorld, m));
                endCase

                caseString("SplitMessage")
                SplitMessage& m = (SplitMessage&)message;
                commands.push_back(new SplitCommand(gameWorld, m));
                endCase

                caseString("ChatMessage")
                ChatMessage& m = (ChatMessage&)message;
                commands.push_back(new SendChatMessageCommand(mailer, gameWorld, m));
                endCase

                caseString("NewConnectionMessage")
                NewConnectionMessage& m = (NewConnectionMessage&)message;
                commands.push_back(new CheckLoginConnectionCommand(m));
                endCase

                caseString("LoginMessage")
                LoginMessage& m = (LoginMessage&)message;
                commands.push_back(new LoginCommand(gameWorld, m, mailer));
                endCase

                caseString("SetBlobPropertiesMessage")
                SetBlobPropertiesMessage& m = (SetBlobPropertiesMessage&)message;
                commands.push_back(new SetBlobPropertiesCommand(gameWorld, m));
                endCase

                caseString("LogoutMessage")
                LogoutMessage& m = (LogoutMessage&)message;
                commands.push_back(new RemovePlayerCommand(gameWorld, mailer, m, connectionPool));
                endCase

                caseString("ConnectionTimeoutMessage")
                ConnectionTimeoutMessage& m = (ConnectionTimeoutMessage&)message;
                commands.push_back(new RemovePlayerCommand(gameWorld, mailer, m, connectionPool));
                endCase

                caseString("MoveInDirectionMessage")
                MoveInDirectionMessage& m = (MoveInDirectionMessage&)message;
                commands.push_back(new MoveBlobCommand(gameWorld, m));
                endCase

                caseString("PlainTextMessage")
                PlainTextMessage& m = (PlainTextMessage&)message;
                commands.push_back(new DisplayPlainTextCommand(m));
                endCase
            };
            endStringSwitch
            delete &message;
        }
        return commands;
    }
};
