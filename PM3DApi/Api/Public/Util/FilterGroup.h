#pragma once

struct FilterGroup
{
    enum Enum
    {
        eFLOOR = 0,
        eOBSTACLE = (1 << 1),
        eSNOWBALL = (1 << 2),
        eTUNNEL = (1 << 3),
        eSKIER = (1 << 4)
    };
};

struct BuriablePenguin
{
    enum Enum
    {
        eCanBury = 0,
        eCannotBury = (1 << 1)
    };
};
