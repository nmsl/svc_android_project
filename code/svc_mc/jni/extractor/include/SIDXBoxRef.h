#ifndef SIDXBOXREF_H
#define SIDXBOXREF_H

#include "config.h"

namespace Boxes {
	class SIDXBoxRef {
	public:
		SIDXBoxRef();
		~SIDXBoxRef();

		void SetType (bool t);
		void SetSize (uint32_t s);
		void SetDuration (uint32_t d);
		void SetSAP (bool isSAP);
		void SetSAPType (uint32_t SAPtype);
		void SetSAPDeltaTime (uint32_t SAPtime);

		bool GetType ();
		uint32_t GetSize ();
		uint32_t GetDuration ();
		bool IsSAP ();
		uint32_t GetSAPType ();
		uint32_t GetSAPDeltaTime ();

		void Print(uint32_t level);
	private:
		bool reference_type;
		uint32_t reference_size;
		uint32_t subsegment_duration;
		bool start_with_SAP;
		uint32_t SAP_type;
		uint32_t SAP_delta_time;
	};
}

#endif /* SIDXBOXREF_H */