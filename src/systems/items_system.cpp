#include <systems/items_system.h>

using namespace Halley;

class ItemsSystem final : public ItemsSystemBase<ItemsSystem> {
public:
	void init()
	{
		updateQueue();
	}
	
	void update(Time t)
    {
		updateQueue();
	}

private:
	void updateQueue()
	{
		if (getRhythmService().getItemEndBeat() == getRhythmService().getCurrentBeat()) {
			getItemService().popQueue();
		}
		
		if (getItemService().updateQueue()) {
			const auto& newItem = getItemService().getItemAt(0);
			getRhythmService().onNewItem(newItem);
		}
	}
};

REGISTER_SYSTEM(ItemsSystem)

