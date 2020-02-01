#include <systems/items_system.h>
#include "src/sprite_layers.h"
#include "components/move_animation_component.h"

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

		if (curBeat != getRhythmService().getCurrentBeat()) {
			curBeat = getRhythmService().getCurrentBeat();
			onBeat();
		}
	}

private:
	void updateQueue()
	{
		if (getRhythmService().getItemStartBeat() == getRhythmService().getCurrentBeat()) {
			startItem();
		}
		
		if (getRhythmService().getItemEndBeat() == getRhythmService().getCurrentBeat()) {
			getItemService().popQueue();
		}
		
		if (getItemService().updateQueue()) {
			createItem(getItemService().getItemAt(2));
			nextStageAll();
		}
	}

	void createItem(const ItemConfig& itemConfig)
	{
		auto itemId = nextItemId++;

		const auto startPos = getItemPos(ItemState::QueuePre);
		const auto endPos = getItemPos(ItemState::QueueBack);

		// Create new item
		getWorld().createEntity()
			.addComponent(PositionComponent(startPos))
			.addComponent(SpriteComponent(Sprite().setImage(getResources(), itemConfig.imageBroken).setPivot(Vector2f(0.5f, 0.5f)), int(SpriteLayers::Items), 1))
			.addComponent(ItemComponent(itemId, itemConfig.id, ItemState::QueueBack))
			.addComponent(MoveAnimationComponent(true, startPos, endPos, getRhythmService().getBeatLength(), 0, MoveType::Conveyour));
	}

	void startItem()
	{
		for (auto& item: itemFamily) {
			if (item.item.state == ItemState::CurrentWait) {
				nextStage(item);
				break;
			}
		}
	}

	void nextStageAll()
	{
		for (auto& item: itemFamily) {
			nextStage(item);
		}
	}

	void nextStage(ItemFamily& item)
	{
		item.item.state = ItemState(int(item.item.state) + 1);
		onStateChange(item);
	}

	void onStateChange(ItemFamily& item)
	{
		const auto& itemConfig = getItemService().getItemConfig(item.item.type);

		// Update pos
		if (item.item.state != ItemState::CurrentActive) {
			MoveType moveType;
			switch (item.item.state) {
			case ItemState::QueueBack:
			case ItemState::QueueFront:
				moveType = MoveType::Conveyour;
				break;
			case ItemState::CurrentWait:
			case ItemState::Done:
			case ItemState::Out:
				moveType = MoveType::Jump;
				break;
			case ItemState::CurrentActive:
			case ItemState::Dead:
			case ItemState::QueuePre:
				moveType = MoveType::Teleport;
				break;
			}
			sendMessage(item.entityId, MoveMessage(getItemPos(item.item.state), getRhythmService().getBeatLength(), moveType));
		}

		// Get ready
		if (item.item.state == ItemState::CurrentWait) {
			getRhythmService().onNewItem(itemConfig);
		}

		// Update image
		if (item.item.state == ItemState::Done) {
			item.sprite.sprite.setImage(getResources(), itemConfig.imageFixed).setPivot(Vector2f(0.5f, 0.5f));
		}

		// Delete old
		if (item.item.state == ItemState::Dead) {
			getWorld().destroyEntity(item.entityId);
		}
	}

	void onBeat()
	{
		for (auto& item: itemFamily) {
			if (item.item.state == ItemState::CurrentActive) {
				item.position.position = BlacksmithActionsUtils::actionToPos(getRhythmService().getActionAtBeat(getRhythmService().getCurrentBeat()));
				break;
			}
		}
	}

	static Vector2f getItemPos(ItemState state)
	{
		switch (state) {
		case ItemState::QueuePre:
			return Vector2f(-35, 110);
		case ItemState::QueueBack:
			return Vector2f(0, 110);
		case ItemState::QueueFront:
			return Vector2f(35, 110);
		case ItemState::CurrentWait:
			return Vector2f(80, 110);
		case ItemState::Done:
			return Vector2f(325, 110);
		case ItemState::Out:
			return Vector2f(425, 110);
		default:
			return Vector2f(-100, -100);
		}
	}

	int curBeat = -1;
	int nextItemId = 0;
};

REGISTER_SYSTEM(ItemsSystem)

