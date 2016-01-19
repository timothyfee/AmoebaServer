class Player
{
public:
    long playerId;
    int connectionId;
    string username;
    ViewWindow window;
    Score score;
    BlobController blobController;

    Player(int connectionId, long playerId, string username)
    :   connectionId(connectionId), playerId(playerId), username(username), blobController(BlobController(this, score))
        {
            window.trackBlobController(blobController);
        }
};
