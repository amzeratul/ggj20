#pragma once

#include <halley.hpp>
using namespace Halley;

#include "../item_config.h"

class ItemService : public Service
{
public:
	void setItems(ItemCollection items);

private:
	ItemCollection items;
};