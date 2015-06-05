#ifndef FILE_TYPE_BOX_H
#define FILE_TYPE_BOX_H

#include "BaseBox.h"

namespace Boxes {
	class FileTypeBox : public BaseBox {
	public:
		FileTypeBox (uint32_t size, uint64_t type);
		~FileTypeBox ();

		void SetMajorBrand (uint32_t brand);
		void SetMinorVersion (uint32_t version);
		void AddAltBrand (uint32_t brand);

		uint32_t GetMajorBrand ();
		uint32_t GetMinorVersion ();
		uint32_t GetAltCount ();
		uint32_t GetAltBrand (uint32_t index);

		void Print(uint32_t level);

	private:
		uint32_t majorBrand;
		uint32_t minorVersion;
		std::vector<uint32_t> altBrands;
	};
}

#endif /* FILE_TYPE_BOX_H */