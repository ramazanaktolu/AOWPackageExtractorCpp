#pragma once
class NULLPTR_EGGUCEPTION
{
public:
	NULLPTR_EGGUCEPTION(void*);
	virtual const char* what() noexcept;
};

