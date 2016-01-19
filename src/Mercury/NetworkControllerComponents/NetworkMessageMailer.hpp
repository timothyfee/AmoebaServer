class NetworkMessageMailer
{
private:
    deque<NetworkMessage*> messages;
    volatile bool& pushing;
    volatile bool& popping;

public:

    NetworkMessageMailer() : pushing(*new bool), popping(*new bool)
    {
        pushing = false;
        popping = false;
    }

    ~NetworkMessageMailer()
    {
        delete &pushing;
        delete &popping;
    }

    int messageCount()
    {
        while (pushing || popping) {}
        return messages.size();
    }

    void push(int connectionId, NetworkMessage* message)
    {
        while (pushing || popping) {}
        pushing = true;
        message->connectionId = connectionId;
        messages.insert(this->messages.end(), message);
        pushing = false;
    }

    void push(int connectionId, NetworkMessage& message)
    {
        push(connectionId, &message);
    }

    void push(int connectionId, deque<NetworkMessage*> messages)
    {
        while (pushing || popping) {}
        pushing = true;
        for (int i = 0; i < messages.size(); i++)
        {
            messages[i]->connectionId = connectionId;
        }
        this->messages.insert(this->messages.end(), messages.begin(), messages.end());
        pushing = false;
    }

    void push(deque<NetworkMessage*> messages)
    {
        while (pushing || popping) {}
        pushing = true;
        this->messages.insert(this->messages.end(), messages.begin(), messages.end());
        pushing = false;
    }

    NetworkMessage* pop()
    {
        while (pushing || popping) {}
        popping = true;
        NetworkMessage *ret = messages.back();
        messages.pop_back();
        popping = false;
        return ret;
    }

    NetworkMessage* pop(int connectionId)
    {
        while (pushing || popping) {}
        popping = true;
        NetworkMessage *ret;
        for (int i = messages.size() - 1; i >= 0; i--)
        {
            if (messages[i]->connectionId == connectionId)
            {
                ret = messages[i];
                messages.erase(messages.begin() + i);
                break;
            }
        }
        popping = false;
        return ret;
    }

    NetworkMessage* peek()
    {
        while (pushing || popping) {}
        popping = true;
        NetworkMessage *ret = messages.back();
        popping = false;
        return ret;
    }

    NetworkMessage* peek(int connectionId)
    {
        while (pushing || popping) {}
        popping = true;
        NetworkMessage *ret;
        for (int i = messages.size() - 1; i > 0; i--)
        {
            if (messages[i]->connectionId == connectionId)
            {
                ret = messages[i];
                break;
            }
        }
        popping = false;
        return ret;
    }

    deque<NetworkMessage*> dump()
    {
        while (pushing || popping) {}
        popping = true;
        deque<NetworkMessage*> ret = messages;
        messages.clear();
        popping = false;
        return ret;
    }

    deque<NetworkMessage*> dump(int connectionId)
    {
        while (pushing || popping) {}
        popping = true;
        deque<NetworkMessage*> ret;
        for (int i = 0; i < messages.size(); i++)
        {
            if (messages[i]->connectionId == connectionId)
            {
                ret.push_back(messages[i]);
                messages.erase(messages.begin() + i);
            }
        }
        popping = false;
        return ret;
    }

    deque<NetworkMessage*> copyStack()
    {
        while (pushing || popping) {}
        popping = true;
        deque<NetworkMessage*> ret = messages;
        popping = false;
        return ret;
    }

    deque<NetworkMessage*> copyStack(int connectionId)
    {
        while (pushing || popping) {}
        popping = true;
        deque<NetworkMessage*> ret;
        for (int i = 0; i < messages.size(); i++)
        {
            if (messages[i]->connectionId == connectionId)
            {
                ret.push_back(messages[i]);
            }
        }
        popping = false;
        return ret;
    }
};
