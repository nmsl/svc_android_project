#ifndef DATA_INFORMATION_BOX_H
#define DATA_INFORMATION_BOX_H

#include "BaseBox.h"
#include "DataReferenceBox.h"

namespace Boxes {
	class DataInformationBox : public BaseBox {
	public:
		DataInformationBox (uint32_t size, uint64_t type);
		~DataInformationBox ();

		void SetDataReferenceBox (DataReferenceBox *drefBox);

		DataReferenceBox* GetDataReferenceBox ();

		void Print (uint32_t level);

	private:
		DataReferenceBox *dref;
	};
}


#endif /* DATA_INFORMATION_BOX_H */