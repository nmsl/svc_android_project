#ifndef AVC_CONFIGURATION_BOX_H
#define AVC_CONFIGURATION_BOX_H

#include "BaseBox.h"
#include "AVCConfigSlot.h"

namespace Boxes {
	class AVCConfigurationBox : public BaseBox {
	public:
		AVCConfigurationBox (uint32_t size, uint64_t type);
		~AVCConfigurationBox ();

		void SetConfigurationVersion (uint8_t v);
		void SetAVCProfileIndication (uint8_t i);
		void SetProfileCompatiblitiy (uint8_t p);
		void SetAVCLevelIndication (uint8_t i);
		void SetCompleteRepresentation (uint32_t c);
		void SetNALUintSize (uint32_t size);
		void AddSequenceParameterSet (AVCConfigSlot *slot);
		void AddPictureParameterSet (AVCConfigSlot *slot);

		uint8_t GetConfigurationVersion ();
		uint8_t GetAVCProfileIndication ();
		uint8_t GetProfileCompatiblitiy ();
		uint8_t GetAVCLevelIndication ();
		uint32_t GetCompleteRepresentation ();
		uint32_t GetNALUintSize ();
		AVCConfigSlot* GetSequenceParameterSet (uint32_t index);
		AVCConfigSlot* GetPictureParameterSet (uint32_t index);

		void Print (uint32_t level);

	private:
		uint8_t configurationVersion;
		uint8_t AVCProfileIndication;
		uint8_t profile_compatibility;
		uint8_t AVCLevelIndication;
		uint32_t complete_representation; // For SVC
		uint32_t nal_unit_size;
		std::vector<AVCConfigSlot*> sequenceParameterSets;
		std::vector<AVCConfigSlot*> pictureParameterSets;
	};
}

#endif /* AVC_CONFIGURATION_BOX_H */