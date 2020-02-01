#pragma once

#include <halley.hpp>
using namespace Halley;

#include "../item_config.h"

class ItemService : public Service
{
public:
	void setItems(ItemCollection items);

	bool updateQueue();
	const ItemConfig& getItemAt(int index) const;
	void popQueue();

private:
	ItemCollection items;
	std::vector<String> itemQueue;

	void addNextItem();
};