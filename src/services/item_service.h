#pragma once

#include <halley.hpp>
#include "src/game_state.h"
using namespace Halley;

#include "../item_config.h"

class ItemService : public Service
{
public:
	using MissCallback = std::function<void(int)>;

	explicit ItemService(Resources& resources);
	
	void setItems(ItemCollection items);

	bool updateQueue();
	bool canSpawnItem() const;
	const ItemConfig& getItemAt(int index) const;
	void popQueue();

	const ItemConfig& getItemConfig(const String id) const;

	int getItemLevel(const String& id) const;
	void onItemDone(const String& id, bool itemOK);
	void onMiss(int beat);

	int getItemMult(int level) const;
	int getMult() const;
	int getScore() const;
	int getHealth() const;

	bool isAlive() const;

	void flagRestart();
	bool needsRestart() const;

	void setMissCallback(MissCallback callback);

	Difficulty getDifficulty() const;
	void setDifficulty(Difficulty difficulty);

	void startStage();
	void endStage();
	bool isStageDone() const;
	int getQueueSize() const;

	void setSelf(std::shared_ptr<ItemService> self);
	std::shared_ptr<ItemService> getSelf() const;

private:
	ItemCollection items;
	std::vector<String> itemQueue;
	std::map<String, int> itemLevels;

	int score = 0;
	int mult = 10;
	int health = 10;

	int nItemsComplete = 0;
	int stageItemsGenerated = 0;

	bool stageDone = false;
	bool restart = false;
	Difficulty difficulty = Difficulty::Easy;

	MissCallback callback;
	std::weak_ptr<ItemService> self;

	void addNextItem();
};