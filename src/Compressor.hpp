class Compressor : public CompressionAlgorithm
{
public:

    string compress(string& data)
    {
        data = replace(":PelletPositionMessage;", ":P;", data);
        data = replace(":BlobStateMessage;", ":B;", data);
        data = replace(":HighScoreMessage;", ":H;", data);
        data = replace(":CurrentScoreMessage;", ":C;", data);
        data = replace("begin:", "@:", data);
        data = replace("@:stream;", "begin:stream;", data);
        data = replace("end:", "!:", data);
        data = replace("!:stream;", "end:stream;", data);
        return data;
    }

    string decompress(string& data)
    {
        data = replace(":M;", ":MoveTowardCoordinatesMessage;", data);
        data = replace("@:", "begin:", data);
        data = replace("!:", "end:", data);
        return data;
    }
};
