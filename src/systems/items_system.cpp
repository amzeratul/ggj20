#include <systems/items_system.h>

using namespace Halley;

class ItemsSystem final : public ItemsSystemBase<ItemsSystem> {
public:
	void update(Time t)
    {
	}
};

REGISTER_SYSTEM(ItemsSystem)

