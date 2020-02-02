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

private:
	ItemCollection items;
	std::vector<String> itemQueue;
	std::map<String, int> itemLevels;

	int score = 0;
	int mult = 10;
	int health = 10;

	int nItemsComplete = 0;

	bool restart = false;
	Difficulty difficulty = Difficulty::Easy;

	MissCallback callback;

	void addNextItem();
};