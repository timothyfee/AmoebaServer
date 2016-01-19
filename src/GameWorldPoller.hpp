class GameWorldPoller
{
private:
    GameWorld &gameWorld;

public:
    GameWorldPoller(GameWorld& gameWorld) : gameWorld(gameWorld) {}

    deque<NetworkMessage*> poll()
    {
        deque<NetworkMessage*> messages;
        deque<Player*> players = gameWorld.players;

        for (int i = 0; i < players.size(); i++)
        {
            ViewWindow window = players[i]->window;
            deque<PhysicalObject*> objects = gameWorld.getVisibleObjects(window);
            deque<NetworkMessage*> playerMessages = deconstructObjects(window, objects);
            playerMessages.push_back(new PingMessage());
            setConnectionIds(players[i]->connectionId, playerMessages);
            messages.insert(messages.end(), playerMessages.begin(), playerMessages.end());
        }
        return messages;
    }

private:

    void setConnectionIds(int connectionId, deque<NetworkMessage*>& playerMessages)
    {
        for (int i = 0; i < playerMessages.size(); i++)
        {
            playerMessages[i]->connectionId = connectionId;
        }
    }

    deque<NetworkMessage*> deconstructObjects(ViewWindow window, deque<PhysicalObject*> objects)
    {
        deque<NetworkMessage*> messages;
        for (int i = 0; i < objects.size(); i++)
        {
            PhysicalObject& o = *objects[i];
            string objectType = o.type;
            switchString(objectType)
            {
                primaryCaseString("Pellet")
                Pellet& pellet = (Pellet&)o;
                Point p = window.serverToClientView(pellet.getCenterPoint());
                messages.push_back(new PelletPositionMessage(p.x, p.y, pellet.objectId));
                endCase

                caseString("Blob")
                Blob& blob = (Blob&)o;
                deque<KeyValuePair> properties;
                Point p = window.serverToClientView(blob.getCenterPoint());
                properties.push_back(KeyValuePair("x", toString(p.x)));
                properties.push_back(KeyValuePair("y", toString(p.y)));
                properties.push_back(KeyValuePair("size", toString(blob.width)));
                properties.push_back(KeyValuePair("color", blob.color));
                if (&blob == blob.blobs[0])
                {
                    properties.push_back(KeyValuePair("username", blob.player.username + " " + toString((int)blob.currentSize - Blob::minSize)));
                }
                else
                {
                    properties.push_back(KeyValuePair("username", toString((int)blob.currentSize - Blob::minSize)));
                }
                properties.push_back(KeyValuePair("id", toString(blob.objectId)));
                messages.push_back(new BlobStateMessage(properties));
                endCase

                caseString("Splitter")
                deque<KeyValuePair> properties;
                Point p = window.serverToClientView(o.getCenterPoint());
                properties.push_back(KeyValuePair("x", toString(p.x)));
                properties.push_back(KeyValuePair("y", toString(p.y)));
                properties.push_back(KeyValuePair("size", toString(o.width)));
                properties.push_back(KeyValuePair("color", "green"));
                properties.push_back(KeyValuePair("username", "Splitter"));
                properties.push_back(KeyValuePair("id",toString(o.objectId)));
                messages.push_back(new BlobStateMessage(properties));
                endCase

                caseString("Bomb")
                Bomb& bomb = (Bomb&)o;
                deque<KeyValuePair> properties;
                Point p = window.serverToClientView(bomb.getCenterPoint());
                properties.push_back(KeyValuePair("x", toString(p.x)));
                properties.push_back(KeyValuePair("y", toString(p.y)));
                properties.push_back(KeyValuePair("size", toString(bomb.width)));
                properties.push_back(KeyValuePair("color", bomb.color));
                properties.push_back(KeyValuePair("username", "Bomb"));
                properties.push_back(KeyValuePair("id",toString(o.objectId)));
                messages.push_back(new BlobStateMessage(properties));
                endCase

                caseString("Poison")
                deque<KeyValuePair> properties;
                Point p = window.serverToClientView(o.getCenterPoint());
                properties.push_back(KeyValuePair("x", toString(p.x)));
                properties.push_back(KeyValuePair("y", toString(p.y)));
                properties.push_back(KeyValuePair("size", toString(o.width)));
                properties.push_back(KeyValuePair("color", "purple"));
                properties.push_back(KeyValuePair("username", "Poison"));
                properties.push_back(KeyValuePair("id",toString(o.objectId)));
                messages.push_back(new BlobStateMessage(properties));
                endCase

                caseString("ScoreMultiplier")
                ScoreMultiplier& sm = (ScoreMultiplier&)o;
                deque<KeyValuePair> properties;
                Point p = window.serverToClientView(sm.getCenterPoint());
                properties.push_back(KeyValuePair("x", toString(p.x)));
                properties.push_back(KeyValuePair("y", toString(p.y)));
                properties.push_back(KeyValuePair("size", toString(sm.width)));
                properties.push_back(KeyValuePair("color", sm.color));
                properties.push_back(KeyValuePair("username", toString(sm.factor) + "X"));
                properties.push_back(KeyValuePair("id",toString(o.objectId)));
                messages.push_back(new BlobStateMessage(properties));
                endCase

                caseString("BigPellet")
                deque<KeyValuePair> properties;
                Point p = window.serverToClientView(o.getCenterPoint());
                properties.push_back(KeyValuePair("x", toString(p.x)));
                properties.push_back(KeyValuePair("y", toString(p.y)));
                properties.push_back(KeyValuePair("size", toString(o.width)));
                properties.push_back(KeyValuePair("color", "pink"));
                properties.push_back(KeyValuePair("username", ""));
                properties.push_back(KeyValuePair("id",toString(o.objectId)));
                messages.push_back(new BlobStateMessage(properties));
                endCase
            };
            endStringSwitch
        }
        return messages;
    }
};
