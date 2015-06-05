#ifndef LIBDASH_FRAMEWORK_INPUT_MEDIAOBJECTREADER_TEST_H_
#define LIBDASH_FRAMEWORK_INPUT_MEDIAOBJECTREADER_TEST_H_

#include "../Portable/MultiThreading.h"
#include "MediaObject.h"
#include "DASHReceiver.h"
#include "IMediaObjectReaderObserver.h"
#include "../Portable/MultiThreading.h"

namespace libdash
{
	namespace framework
	{
		namespace input
		{
			class MediaObjectReaderTest
			{
				public:
					MediaObjectReaderTest          (IMediaObjectReaderObserver *observer);
					virtual ~MediaObjectReaderTest ();

					bool            Start                   ();
					void            Stop                    ();
					virtual int     Read                    (uint8_t *buf, int buf_size);

					// Jargo Test
					void 			StartRead 				();

				private:
					THREAD_HANDLE                       	threadHandle;
					bool                                	run;

					// Jargo Test
					IMediaObjectReaderObserver 				*observer;
					static void 	*ReadTest 				(void *data);
					void 			ReadNotify 				();
			};
		}
	}
}

#endif /* LIBDASH_FRAMEWORK_INPUT_MEDIAOBJECTREADER_TEST_H_ */
