#include "ItemBase.h"

ItemBase::ItemBase()
{
}

ItemBase::~ItemBase()
{
}

int ItemBase::GetType() const
{
    return _itemType;
}

void ItemBase::KillItem()
{
    _invalidFlag = true;
}

bool ItemBase::IsDead() const
{
    return _deadFlag;
}

bool ItemBase::IsInvalid() const
{
    return _invalidFlag;
}
