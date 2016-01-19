#define _usingNetworkFramework_

#include "..\BaseLibraries\StandardFunctionsLib.hpp"
#include "..\BaseLibraries\Sockets.hpp"
#include "..\BaseLibraries\Thread.hpp"

#include "Connection/Connection.hpp"
#include "Connection/ConnectionPool.hpp"
#include "NetworkMessages/NetworkMessage.hpp"
#include "NetworkMessages/FrameworkeNetworkMessages.hpp"

#include "NetworkControllerComponents/Codec.hpp"
#include "NetworkControllerComponents/Interpreter.hpp"
#include "Connection/ConnectionBuffer.hpp"

#include "NetworkControllerComponents/NetworkMessageMailer.hpp"

#include "MiddleWare/StringWare.hpp"
#include "MiddleWareContainers/StringWareContainer.hpp"
#include "MiddleWare/Cipher.hpp"
#include "MiddleWare/CompressionAlgorithm.hpp"

#include "MiddleWare/PairWare.hpp"
#include "MiddleWareContainers/PairWareContainer.hpp"

#include "MiddleWare/MessageWare.hpp"
#include "MiddleWareContainers/MessageWareContainer.hpp"

#include "Threads/ConnectorThread.hpp"
#include "Threads/ReceiverThread.hpp"
#include "Threads/SenderThread.hpp"

#include "NetworkControllers/NetworkController.hpp"
#include "NetworkControllers/NetworkServerController.hpp"
#include "NetworkControllers/NetworkClientController.hpp"

#include "BundledMiddleWare/Loggers/DataStreamLogger.hpp"
#include "BundledMiddleWare/Loggers/KeyValuePairLogger.hpp"
#include "BundledMiddleWare/LagSimulator.hpp"
