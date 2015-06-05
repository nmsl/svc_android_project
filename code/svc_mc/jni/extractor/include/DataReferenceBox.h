#ifndef DATA_REFERENCE_BOX_H
#define DATA_REFERENCE_BOX_H

#include "BaseBox.h"
#include "FullBox.h"
#include "DataEntryURLBox.h"

namespace Boxes {
	class DataReferenceBox : public BaseBox, public FullBox {
	public:
		DataReferenceBox (uint32_t size, uint64_t type);
		~DataReferenceBox ();

		void AddDataEntryURLBox (DataEntryURLBox *url);

		DataEntryURLBox* GetDataEntryURLBox (uint32_t index);

		void Print (uint32_t level);

	private:
		std::vector<DataEntryURLBox*> entryList;
	};
}

#endif /* DATA_REFERENCE_BOX_H */