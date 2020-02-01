#include "item_service.h"

void ItemService::setItems(ItemCollection i)
{
	items = std::move(i);
}
