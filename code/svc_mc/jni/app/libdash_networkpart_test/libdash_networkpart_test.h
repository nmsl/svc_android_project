#ifndef NETWORK_TEST_H
#define NETWORK_TEST_H

#include "TestChunk.h"
#include "PersistentHTTPConnection.h"
#include "DASHManager.h"

#include <iostream>
#include <fstream>

using namespace dash;
using namespace dash::network;
using namespace libdashtest;
using namespace std;

namespace libdashtest
{
	class NetworkTest
	{
		public:
			NetworkTest();
			~NetworkTest();

		private:
			void download(IConnection *connection, IChunk *chunk, ofstream *file);

    		HTTPConnection  			*httpconnection;
    		PersistentHTTPConnection 	*peristenthttpconnection;
	};
}

#endif /* NETWORK_TEST_H */