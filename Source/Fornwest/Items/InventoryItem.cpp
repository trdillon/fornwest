// Fornwest ©2021 itsdits. All Rights Reserved.

#include "InventoryItem.h"

FInventoryItem::FInventoryItem()
{
	this->Name = FText::FromString("No Name found");
	this->Description = FText::FromString("No Description found");
	this->Weight = 0.1f;
	this->IsStackable = false;
	this->Thumbnail = nullptr;
}