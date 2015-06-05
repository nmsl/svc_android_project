#ifndef TRACK_HEADER_BOX_H
#define TRACK_HEADER_BOX_H

#include "BaseBox.h"
#include "FullBox.h"

namespace Boxes {
	class TrackHeaderBox : public BaseBox, public FullBox {
	public:
		TrackHeaderBox (uint32_t size, uint64_t type);
		~TrackHeaderBox ();

		void SetCreationTime (uint64_t time);
		void SetModificationTime (uint64_t time);
		void SetTrackID (uint32_t id);
		void SetReserved1 (uint32_t value);
		void SetDuration (uint64_t duration);
		void SetReserved2 (uint32_t value, uint32_t index);
		void SetLayer (uint16_t layer);
		void SetAlternateGroup (uint16_t group);
		void SetVolume (uint16_t volume);
		void SetReserved3 (uint16_t value);
		void SetMatrix (uint32_t value, uint32_t index);
		void SetWidth (uint32_t width);
		void SetHeight (uint32_t height);

		uint64_t GetCreationTime ();
		uint64_t GetModificationTime ();
		uint32_t GetTrackID ();
		uint32_t GetReserved1 ();
		uint64_t GetDuration ();
		uint32_t GetReserved2 (uint32_t index);
		uint16_t GetLayer ();
		uint16_t GetAlternateGroup ();
		uint16_t GetVolume ();
		uint16_t GetReserved3 ();
		uint32_t GetMatrix (uint32_t index);
		uint32_t GetWidth ();
		uint32_t GetHeight ();

		void Print (uint32_t level);

	private:
		uint64_t creationTime;
		uint64_t modificationTime;
		uint32_t trackID;
		uint32_t reserved1;
		uint64_t duration;
		uint32_t reserved2[2];
		uint16_t layer;
		uint16_t alternate_group;
		uint16_t volume;
		uint16_t reserved3;
		uint32_t matrix[9];
		uint32_t width;
		uint32_t height;
	};
}

#endif /* TRACK_HEADER_BOX_H */