#pragma once

#include <halley.hpp>
using namespace Halley;

#include "../item_config.h"

class ItemService : public Service
{
public:
	void setItems(ItemCollection items);

	void pumpQueue();
	const ItemConfig& getItemAt(int index) const;

private:
	ItemCollection items;
	std::vector<String> itemQueue;
};