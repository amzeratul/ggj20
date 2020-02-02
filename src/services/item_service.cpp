#include "item_service.h"

ItemService::ItemService(Resources& resources)
{
	setItems(ItemCollection(resources.get<ConfigFile>("gameplay/items")->getRoot()));
}

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

bool ItemService::canSpawnItem() const
{
	return !itemQueue.empty() && !itemQueue[0].isEmpty();
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

static int getBaseItemScore(Difficulty difficulty)
{
	switch (difficulty) {
	case Difficulty::Easy:
		return 10;
	case Difficulty::Normal:
		return 20;
	case Difficulty::Hard:
		return 30;
	case Difficulty::SuddenDeath:
		return 50;
	}
	return 10;
}

void ItemService::onItemDone(const String& id, bool itemOK)
{
	const int curLevel = getItemLevel(id);
	const int newLevel = clamp((itemOK ? curLevel + 1 : curLevel - 1), 0, 3);

	if (itemOK) {
		score += getBaseItemScore(difficulty) * (getItemMult(curLevel) * mult + 5) / 10;
		mult = clamp(mult + 1, 10, 40);
		health = clamp(health + 1, 0, 10);
	} else {
		mult = 10;
	}

	nItemsComplete++;	
	itemLevels[id] = newLevel;
}

void ItemService::onMiss(int beat)
{
	mult = 10;
	health = clamp(health - (difficulty == Difficulty::SuddenDeath ? 10 : 1), 0, 10);

	if (callback) {
		callback(beat);
	}
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

void ItemService::setMissCallback(MissCallback c)
{
	callback = c;
}

Difficulty ItemService::getDifficulty() const
{
	return difficulty;
}

void ItemService::setDifficulty(Difficulty diff)
{
	difficulty = diff;
}

void ItemService::startStage()
{
	stageItemsGenerated = 0;
}

bool ItemService::isStageDone() const
{
	return itemQueue.at(0).isEmpty();
}

int ItemService::getQueueSize() const
{
	return int(itemQueue.size());
}

static int getMaxItems(int nItemsComplete)
{
	return 1 + (nItemsComplete + 5) / 10;
}

void ItemService::addNextItem()
{
	if (stageItemsGenerated >= 5 && difficulty != Difficulty::SuddenDeath) {
		itemQueue.push_back("");
		return;
	}
	
	const int sz = std::min(getMaxItems(nItemsComplete), int(items.getIds().size()));
	auto& rng = Random::getGlobal();
	auto id = items.getIds().at(rng.getInt(0, sz - 1));

	itemQueue.push_back(id);
	stageItemsGenerated++;
}
