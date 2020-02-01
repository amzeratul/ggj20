#include "item_config.h"

static BlacksmithActions chrToAction(char chr)
{
	if (chr == 'A') {
		return BlacksmithActions::Anvil;
	} else if (chr == 'B') {
		return BlacksmithActions::Love;
	} else if (chr == 'Y') {
		return BlacksmithActions::Furnace;
	} else if (chr == 'X') {
		return BlacksmithActions::Bucket;
	} else if (chr == '.') {
		return BlacksmithActions::Idle;
	}
	throw Exception("Invalid item action, \"" + String(chr) + "\"", 0);
}

void ItemConfig::load(const ConfigNode& node)
{
	id = node["id"].asString();
	imageFixed = node["imageFixed"].asString();
	imageBroken = node["imageBroken"].asString();

	auto as = node["actions"].asString();
	for (size_t i = 0; i < as.length(); ++i) {
		actions.push_back(chrToAction(as[i]));
	}
}

ItemCollection::ItemCollection()
{
}

ItemCollection::ItemCollection(const ConfigNode& node)
{
	load(node);
}

void ItemCollection::load(const ConfigNode& node)
{
	for (auto& n: node["items"].asSequence()) {
		ItemConfig item;
		item.load(n);
		items[item.id] = item;
		ids.push_back(item.id);
	}
}

const ItemConfig& ItemCollection::getItem(const String& name) const
{
	return items.at(name);
}

const std::vector<String>& ItemCollection::getIds() const
{
	return ids;
}
