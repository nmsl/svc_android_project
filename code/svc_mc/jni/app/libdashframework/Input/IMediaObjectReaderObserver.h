#ifndef LIBDASH_FRAMEWORK_INPUT_IMEDIAOBJECTREADEROBSERVER_H_
#define LIBDASH_FRAMEWORK_INPUT_IMEDIAOBJECTREADEROBSERVER_H_

namespace libdash
{
	namespace framework
	{
		namespace input
		{
			class IMediaObjectReaderObserver
			{
				public:
					virtual ~IMediaObjectReaderObserver    () {}

					virtual int ReadSegment () = 0;
			};
		}
	}
}
#endif /* LIBDASH_FRAMEWORK_INPUT_IMEDIAOBJECTREADEROBSERVER_H_ */
