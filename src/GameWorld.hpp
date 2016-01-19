class GameWorld
{
public:
    int width, length, depth;
    double resistance;

    DefaultPhysicsEngine physicsEngine;
    deque<Player*> players;
    ScoreBoard scoreBoard;

    Pellet* pellets;
    BigPellet* bigPellets;
    ScoreMultiplier* scoreMultipliers;
    AIController* ais;
    Splitter* splitters;
    Bomb* bombs;
    Poison* poison;

    GameWorld()
    :   width(5000), length(width), depth(0), resistance(0.2),
        physicsEngine(DefaultPhysicsEngine(resistance)), scoreBoard(ScoreBoard(players))
    {
        gameWorldProperties.width = width;
        gameWorldProperties.length = length;
        gameWorldProperties.depth = depth;
        gameWorldProperties.resistance = resistance;

        pellets = new Pellet[100];
        bigPellets = new BigPellet[3];
        scoreMultipliers = new ScoreMultiplier[2];
        ais = new AIController[5];
        splitters = new Splitter[10];
        bombs = new Bomb[5];
        poison = new Poison[5];
    }

    void addPlayer(int connectionId, long playerId, string username)
    {
        players.push_back(new Player(connectionId, playerId, username));
    }

    void removePlayer(Player& player)
    {
        for (int i = 0; i < players.size(); i++)
        {
            if (players[i] == &player)
            {
                delete &player;
                players.erase(players.begin() + i);
            }
        }
    }

    void removePlayerByPlayerId(long playerId)
    {
        for (int i = 0; i < players.size(); i++)
        {
            if (players[i]->playerId == playerId)
            {
                removePlayer(*players[i]);
            }
        }
    }

    void removePlayerByConnectionId(int connectionId)
    {
        for (int i = 0; i < players.size(); i++)
        {
            if (players[i]->connectionId == connectionId)
            {
                removePlayer(*players[i]);
            }
        }
    }

    Player* findPlayerByPlayerId(long playerId)
    {
        for (int i = 0; i < players.size(); i++)
        {
            if (players[i]->playerId == playerId)
            {
                return players[i];
            }
        }
        return NULL;
    }

    Player* findPlayerByConnectionId(int connectionId)
    {
        for (int i = 0; i < players.size(); i++)
        {
            if (players[i]->connectionId == connectionId)
            {
                return players[i];
            }
        }
        return NULL;
    }

    deque<PhysicalObject*> getVisibleObjects(ViewWindow window)
    {
        deque<PhysicalObject*> visibleObjects;
        deque<PhysicalObject*> objects = PhysicalObject::getObjects();

        for (int i = 0; i < objects.size(); i++)
        {
            if (window.inView(objects[i]))
            {
                visibleObjects.push_back(objects[i]);
            }
        }
        return visibleObjects;
    }

    inline void update()
    {
        physicsEngine.update(PhysicalObject::getObjects());
    }
};
