class AmoebaNetworkServerController : public NetworkServerController
{
public:
    AmoebaInterpreter interpreter;
    AmoebaCodec codec;

    AmoebaNetworkServerController(int maxConnections, int port)
    :   NetworkServerController(codec, interpreter, maxConnections, port)
    {
//        attach(new LagSimulator(10));
        attach(new Compressor());
//        attach(new DataStreamLogger("", true, true, false));
//        attach(new KeyValuePairLogger("", true, true, false));
    }
};
