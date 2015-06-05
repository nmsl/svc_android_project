#ifndef MY_HANDLER_H
#define MY_HANDLER_H

#include "../libdashframework/MPD/AdaptationSetHelper.h"
#include "../Managers/MultimediaManager.h"

// Jargo - Extractor class
#include "Manager.h"

namespace dashhandler
{
	struct settings_t
	{
		int period;
		int videoAdaptationSet;
        int videoRepresentation;
	};

	class DASHHandler
	{
		public:
			DASHHandler ();
			~DASHHandler ();

			dash::mpd::IMPD* GetMPD		();

			void onDownloadMPDPressed 	(const std::string &url);
			void onStartButtonPressed 	();
			void onStopButtonPressed 	();
			void onSettingsChanged 		(int period, int videoAdaptationSet, int videoRepresentation);

			// Jargo - Return Extractor
			Bitstream::Manager* GetExtractor();

		private:
			settings_t 					currentSettings;
			CRITICAL_SECTION 			monitorMutex;
			managers::MultimediaManager *multimediaManager;

			bool SettingsChanged (int period, int videoAdaptationSet, int videoRepresentation);
			void SetSettings (int period, int videoAdaptationSet, int videoRepresentation);

	};
}

#endif /* MY_HANDLER_H */