#ifndef TRUNENTRY_H
#define TRUNENTRY_H

#include "config.h"

namespace Boxes {
	enum {
		TRUN_DATA_OFFSET	= 0x01,
		TRUN_FIRST_FLAG 	= 0x04,
		TRUN_DURATION 		= 0x100,
		TRUN_SIZE 			= 0x200,
		TRUN_FLAGS 			= 0x400,
		TRUN_CTS_OFFSET 	= 0x800
	};

	class TrunEntry {
	public:
		TrunEntry ();
		~TrunEntry ();

		void SetDuration (uint32_t d);
		void SetSize (uint32_t s);
		void SetFlags (uint32_t f);
		void SetCTSOffset (uint32_t offset);

		uint32_t GetDuration ();
		uint32_t GetSize ();
		uint32_t GetFlags ();
		uint32_t GetCTSOffset ();

		void Print (uint32_t level);

	private:
		uint32_t duration;
		uint32_t size;
		uint32_t flags;
		uint32_t CTS_offset;
	};
}

#endif /* TRUNENTRY_H */