#include "item_service.h"

void ItemService::setItems(ItemCollection i)
{
	items = std::move(i);
}

bool ItemService::updateQueue()
{
	bool changed = false;
	while (itemQueue.size() < 3) {
		addNextItem();
		changed = true;
	}
	return changed;
}

const ItemConfig& ItemService::getItemAt(int index) const
{
	return items.getItem(itemQueue.at(index));
}

void ItemService::popQueue()
{
	if (!itemQueue.empty()) {
		itemQueue.erase(itemQueue.begin());
	}
}

void ItemService::addNextItem()
{
	// TODO
	itemQueue.push_back("sword");
}
