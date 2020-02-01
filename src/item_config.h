#pragma once

#include <halley.hpp>
#include "blacksmith_actions.h"
using namespace Halley;

class ItemConfig {
public:
	void load(const ConfigNode& node);

	String id;
	String imageBroken;
	String imageFixed;
	std::vector<BlacksmithActions> actions;
};

class ItemCollection {
public:
	ItemCollection();
	ItemCollection(const ConfigNode& node);
	
	void load(const ConfigNode& node);
	ItemConfig& getItem(const String& name);

private:
	std::map<String, ItemConfig> items;
};