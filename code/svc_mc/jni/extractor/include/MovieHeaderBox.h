#ifndef MOVIE_BOX_HEADER_H
#define MOVIE_BOX_HEADER_H

#include "BaseBox.h"
#include "FullBox.h"

namespace Boxes {
	class MovieHeaderBox : public BaseBox, public FullBox {
	public:
		MovieHeaderBox (uint32_t size, uint64_t type);
		~MovieHeaderBox ();

		void SetCreationTime (uint64_t cTime);
		void SetModificationTime (uint64_t mTime);
		void SetTimeScale (uint32_t sTime);
		void SetDuration (uint64_t duration);
		void SetPreferredRate (uint32_t rate);
		void SetPreferredVolume (uint16_t volume);
		void SetMatrixA (uint32_t a);
		void SetMatrixB (uint32_t b);
		void SetMatrixU (uint32_t u);
		void SetMatrixC (uint32_t c);
		void SetMatrixD (uint32_t d);
		void SetMatrixV (uint32_t v);
		void SetMatrixX (uint32_t x);
		void SetMatrixY (uint32_t y);
		void SetMatrixW (uint32_t w);
		void SetPreviewTime (uint32_t pTime);
		void SetPreviewDuration (uint32_t pDuration);
		void SetPosterTime (uint32_t posterT);
		void SetSelectionTime (uint32_t selectionT);
		void SetSelectionDuration (uint32_t selectionD);
		void SetCurrentTime (uint32_t currentT);
		void SetNextTrackID (uint32_t id);

		uint64_t GetCreationTime ();
		uint64_t GetModificationTime ();
		uint32_t GetTimeScale ();
		uint64_t GetDuration ();
		uint32_t GetPreferredRate ();
		uint16_t GetPreferredVolume ();
		uint8_t* GetReserved ();
		uint32_t GetMatrixA ();
		uint32_t GetMatrixB ();
		uint32_t GetMatrixU ();
		uint32_t GetMatrixC ();
		uint32_t GetMatrixD ();
		uint32_t GetMatrixV ();
		uint32_t GetMatrixX ();
		uint32_t GetMatrixY ();
		uint32_t GetMatrixW ();
		uint32_t GetPreviewTime ();
		uint32_t GetPreviewDuration ();
		uint32_t GetPosterTime ();
		uint32_t GetSelectionTime ();
		uint32_t GetSelectionDuration ();
		uint32_t GetCurrentTime ();
		uint32_t GetNextTrackID ();

		void Print(uint32_t level);

	private:
		uint64_t creationTime;
		uint64_t modificationTime;
		uint32_t timeScale;
		uint64_t duration;
		uint32_t preferredRate;
		uint16_t preferredVolume;
		uint8_t reserved[10];
		uint32_t matrixA;
		uint32_t matrixB;
		uint32_t matrixU;
		uint32_t matrixC;
		uint32_t matrixD;
		uint32_t matrixV;
		uint32_t matrixX;
		uint32_t matrixY;
		uint32_t matrixW;
		uint32_t previewTime;
		uint32_t previewDuration;
		uint32_t posterTime;
		uint32_t selectionTime;
		uint32_t selectionDuration;
		uint32_t currentTime;
		uint32_t nextTrackID;
	};
}

#endif /* MOVIE_BOX_HEADER_H */