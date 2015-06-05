#ifndef MEDIA_HEADER_BOX_H
#define MEDIA_HEADER_BOX_H

#include "BaseBox.h"
#include "FullBox.h"

namespace Boxes {
	class MediaHeaderBox : public BaseBox, public FullBox {
	public:
		MediaHeaderBox (uint32_t size, uint64_t type);
		~MediaHeaderBox ();

		void SetCreationTime (uint64_t time);
		void SetModificationTime (uint64_t time);
		void SetTimeScale (uint32_t scale);
		void SetDuration (uint64_t duration);
		void SetLanguage (uint8_t c, uint32_t index);
		void SetReserved (uint16_t value);
		void CheckpackedLanguage ();

		uint64_t GetCreationTime ();
		uint64_t GetModificationTime ();
		uint32_t GetTimeScale ();
		uint64_t GetDuration ();
		uint8_t GetLanguage (uint32_t index);
		uint16_t GetReserved ();

		void Print (uint32_t level);

	private:
		uint64_t creationTime;
		uint64_t modificationTime;
		uint32_t timeScale;
		uint64_t duration;
		uint8_t packedLanguage[3];
		uint16_t reserved;
	};
}

#endif /* MEDIA_HEADER_BOX_H */