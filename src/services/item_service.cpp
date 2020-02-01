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

const ItemConfig& ItemService::getItemConfig(const String id) const
{
	return items.getItem(id);
}

int ItemService::getItemLevel(const String& id) const
{
	auto iter = itemLevels.find(id);
	if (iter != itemLevels.end()) {
		return iter->second;
	}
	return 0;
}

void ItemService::onItemDone(const String& id, bool itemOK)
{
	const int curLevel = getItemLevel(id);
	const int newLevel = clamp((itemOK ? curLevel + 1 : 0), 0, 3);

	if (itemOK) {
		score += (getItemMult(curLevel) * mult + 5) / 10;
		mult = clamp(mult + 1, 10, 40);
		health = clamp(health + 1, 0, 10);
	} else {
		mult = 10;
	}
	
	itemLevels[id] = newLevel;
}

void ItemService::onMiss()
{
	mult = 10;
	health = clamp(health - 1, 0, 10);
}

int ItemService::getItemMult(int level) const
{
	switch (level) {
	case 0:
		return 10;
	case 1:
		return 15;
	case 2:
		return 25;
	case 3:
		return 50;
	default:
		return 10;
	}
}

int ItemService::getMult() const
{
	return mult;
}

int ItemService::getScore() const
{
	return score;
}

int ItemService::getHealth() const
{
	return health;
}

bool ItemService::isAlive() const
{
	return health > 0;
}

void ItemService::flagRestart()
{
	restart = true;
}

bool ItemService::needsRestart() const
{
	return restart;
}

void ItemService::addNextItem()
{
	// TODO
	itemQueue.push_back("sword");
}
