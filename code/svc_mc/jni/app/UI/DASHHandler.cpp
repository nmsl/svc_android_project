#include "DASHHandler.h"

#include <android/log.h>

#define  LOG_TAG    "DASHHandler"
#define  LOGV(...)  __android_log_print(ANDROID_LOG_VERBOSE,LOG_TAG, __VA_ARGS__)
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,  LOG_TAG, __VA_ARGS__)
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,   LOG_TAG, __VA_ARGS__)
#define  LOGW(...)  __android_log_print(ANDROID_LOG_WARN,   LOG_TAG, __VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,  LOG_TAG, __VA_ARGS__)

using namespace libdash::framework::mpd;
using namespace dash::mpd;
using namespace std;
using namespace dashhandler;
using namespace dashhandler::managers;

DASHHandler::DASHHandler()
{
	InitializeCriticalSection(&this->monitorMutex);

    this->SetSettings(-1, -1, -1);
	this->multimediaManager = new MultimediaManager();
	this->multimediaManager->SetFrameRate(24);
}
DASHHandler::~DASHHandler()
{
	DeleteCriticalSection(&this->monitorMutex);
}

dash::mpd::IMPD* DASHHandler::GetMPD()
{
    return this->multimediaManager->GetMPD();
}
void DASHHandler::onDownloadMPDPressed (const std::string &url)
{
	if(!this->multimediaManager->Init(url))
	{
		LOGE("[onDownloadMPDPressed] - Error parsing mpd at: %s", url.c_str());
		return;
	}

	LOGI("[onDownloadMPDPressed] - Successfully parsed MPD at: %s", url.c_str());
}
void DASHHandler::onStartButtonPressed ()
{
    // LOGI("[onStartButtonPressed]");
    this->multimediaManager->Start();
}
void DASHHandler::onStopButtonPressed ()
{
    this->multimediaManager->Stop();
}
void DASHHandler::onSettingsChanged (int period, int videoAdaptationSet, int videoRepresentation)
{
    if (this->multimediaManager->GetMPD() == NULL)
    {
        LOGE("[onSettingsChanged] - MPD not found!");
        return;
    }

    if (!this->SettingsChanged(period, videoAdaptationSet, videoRepresentation))
        return;

    IPeriod                         *currentPeriod      = this->multimediaManager->GetMPD()->GetPeriods().at(period);
    std::vector<IAdaptationSet *>   videoAdaptationSets = AdaptationSetHelper::GetVideoAdaptationSets(currentPeriod);
    std::vector<IAdaptationSet *>   audioAdaptationSets = AdaptationSetHelper::GetAudioAdaptationSets(currentPeriod);

    if (videoAdaptationSet >= 0 && videoRepresentation >= 0)
    {
        this->multimediaManager->SetVideoQuality(currentPeriod,
            videoAdaptationSets.at(videoAdaptationSet),
            videoAdaptationSets.at(videoAdaptationSet)->GetRepresentation().at(videoRepresentation));
    }
    else
    {
        // LOGD("[onSettingsChanged] - videoAdaptationSet < 0 || videoRepresentation < 0");
        this->multimediaManager->SetVideoQuality(currentPeriod, NULL, NULL);
    }
    LOGD("[onSettingsChanged] - Settings Changed");
}
// Jargo - return Extractor
Bitstream::Manager* DASHHandler::GetExtractor() {
    return this->multimediaManager->GetExtractor();
}

bool DASHHandler::SettingsChanged (int period, int videoAdaptationSet, int videoRepresentation)
{
    EnterCriticalSection(&this->monitorMutex);

    bool settingsChanged = false;

    if (this->currentSettings.videoRepresentation != videoRepresentation ||
        this->currentSettings.videoAdaptationSet != videoAdaptationSet ||
        this->currentSettings.period != period)
        settingsChanged = true;

    if (settingsChanged)
        this->SetSettings(period, videoAdaptationSet, videoRepresentation);

    LeaveCriticalSection(&this->monitorMutex);

    return settingsChanged;
}
void DASHHandler::SetSettings (int period, int videoAdaptationSet, int videoRepresentation)
{
    this->currentSettings.period                = period;
    this->currentSettings.videoAdaptationSet    = videoAdaptationSet;
    this->currentSettings.videoRepresentation   = videoRepresentation;
}