#include "../include/libdash.h"
#include "manager/DASHManager.h"

using namespace dash;

IDASHManager* CreateDashManager()
{
	return new DASHManager();
}