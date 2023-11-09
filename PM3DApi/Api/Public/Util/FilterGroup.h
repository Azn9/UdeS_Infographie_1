#pragma once

struct FilterGroup
{
	enum Enum
	{
		eFLOOR = 0,
		eOBSTACLE = (1 << 1),
		eSNOWBALL = (1 << 2)
	};
};