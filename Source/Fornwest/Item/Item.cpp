// Fornwest ©2021 itsdits. All Rights Reserved.

#include "Item.h"

UItem::UItem()
{
	Name = FText::FromString("Item");
	Description = FText::FromString("This is a pen.");
	Action = FText::FromString("Use");
	Weight = 1.0f;
	Value = 1.0f;
}