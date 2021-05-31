#include "NULLPTR_EXCEPTION.h"

NULLPTR_EGGUCEPTION::NULLPTR_EGGUCEPTION(void* v)
{
	
	//printf("%x2", *(int*)v);
	//delete v;
}

const char* NULLPTR_EGGUCEPTION::what() noexcept
{
	return nullptr;
}
