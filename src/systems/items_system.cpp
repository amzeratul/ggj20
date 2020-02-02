#include <systems/items_system.h>
#include "src/sprite_layers.h"
#include "components/move_animation_component.h"
#include "components/bouncy_component.h"
#include "components/particle_component.h"

using namespace Halley;

class ItemsSystem final : public ItemsSystemBase<ItemsSystem> {
public:
	void init()
	{
		updateQueue();
		updateVulcanAnimation(BlacksmithActions::Idle);

		getItemService().setMissCallback([this](int beat) { onMiss(beat); });
	}
	
	void update(Time t)
    {
		itemOK = getRhythmService().isItemOK();

		updateQueue();

		if (curBeat != getRhythmService().getCurrentBeat()) {
			curBeat = getRhythmService().getCurrentBeat();
			onBeat(curBeat);
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
			if (getItemService().isAlive()) {
				if (getItemService().canSpawnItem()) {
					createItem(getItemService().getItemAt(0));
				}
				nextStageAll();
			}
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
			.addComponent(MoveAnimationComponent(true, startPos, endPos, getRhythmService().getBeatLength(), 0, MoveType::Conveyour))
			.addComponent(BouncyComponent(0, 1));
	}

	void startItem()
	{
		if (!getItemService().isAlive()) {
			return;
		}
		
		for (auto& item: itemFamily) {
			if (item.item.state == ItemState::CurrentWait) {
				nextStage(item);
				break;
			}
		}
	}

	bool ended = false;
	void nextStageAll()
	{
		for (auto& item: itemFamily) {
			nextStage(item);
		}
		if (itemFamily.size() == 0 && getRhythmService().getCurrentTime() > 1.0f) {
			if (!ended) {
				ended = true;
				getItemService().endStage();
			}
		}
	}

	void nextStage(ItemFamily& item)
	{
		if (!getItemService().isAlive() && item.item.state != ItemState::CurrentActive) {
			return;
		}
		
		if (item.item.state == ItemState::Out) {
			// Skip "Failed"
			item.item.state = ItemState::Dead;
		} else {
			item.item.state = ItemState(int(item.item.state) + 1);
		}
		onStateChange(item);
	}

	void onStateChange(ItemFamily& item)
	{
		const auto& itemConfig = getItemService().getItemConfig(item.item.type);

		// Get ready
		if (item.item.state == ItemState::CurrentWait) {
			getRhythmService().onNewItem(itemConfig);
		}

		// Update image
		if (item.item.state == ItemState::Done) {
			onItemDone(item, itemConfig);
			updateVulcanAnimation(BlacksmithActions::Idle);
		}

		// Move
		if (item.item.state != ItemState::CurrentActive) {
			MoveType moveType = MoveType::Teleport;
			switch (item.item.state) {
			case ItemState::QueueBack:
			case ItemState::QueueFront:
				moveType = MoveType::Conveyour;
				break;
			case ItemState::CurrentWait:
			case ItemState::Done:
			case ItemState::Out:
			case ItemState::Failed:
				moveType = MoveType::Jump;
				break;
			}
			sendMessage(item.entityId, MoveMessage(getItemPos(item.item.state), getRhythmService().getBeatLength(), moveType));
		}

		// Delete old
		if (item.item.state == ItemState::Dead) {
			getWorld().destroyEntity(item.entityId);
		}
	}

	void onBeat(int beat)
	{
		if (getItemService().isAlive()) {
			for (auto& item: itemFamily) {
				if (item.item.state == ItemState::CurrentActive) {
					const auto curAction = getRhythmService().getActionAtBeat(getRhythmService().getCurrentBeat());
					item.position.position = BlacksmithActionsUtils::actionToPos(curAction);

					if (beat != beatMiss) {
						updateVulcanAnimation(curAction);
						playActionSound(curAction);
					} else {
						updateVulcanAnimation(BlacksmithActions::Fail);
					}
					break;
				}
			}
		}
	}

	String getActionName(BlacksmithActions action)
	{
		switch (action) {
		case BlacksmithActions::Anvil:
			return "anvil";
		case BlacksmithActions::Bucket:
			return "bucket";
		case BlacksmithActions::Furnace:
			return "furnace";
		case BlacksmithActions::Idle:
			return "idle";
		case BlacksmithActions::Love:
			return "love";
		case BlacksmithActions::Fail:
			return "fail";
		}
		return "";
	}

	void updateVulcanAnimation(BlacksmithActions action)
	{
		for (auto& v: vulcanFamily) {
			if (v.environmentObject.id == "vulcan") {
				v.spriteAnimation.player.setSequence(getActionName(action));
				v.environmentObject.animTarget = "idle";
				v.environmentObject.animTime = getRhythmService().getBeatLength() * 0.7f;
			}
		}

		if (action == BlacksmithActions::Anvil || action == BlacksmithActions::Bucket || action == BlacksmithActions::Love || action == BlacksmithActions::Furnace) {
			auto& rng = Random::getGlobal();
			String animName = getActionName(action) + "_part";
			
			for (size_t i = 0; i < 3; ++i) {
				const auto posOffset = Vector2f(rng.getFloat(0, 10), 0).rotate(Angle1f::fromRadians(rng.getFloat(0, 2 * 3.14159265f)));
				const auto pos = BlacksmithActionsUtils::actionToPos(action) + posOffset;
				Vector2f vel = Vector2f(rng.getFloat(50, 100), 0).rotate(Angle1f::fromRadians(rng.getFloat(0, 2 * 3.14159265f)));
				auto e = getWorld().createEntity()
					.addComponent(PositionComponent(pos))
					.addComponent(ParticleComponent(vel))
					.addComponent(SpriteComponent(Sprite(), int(SpriteLayers::Particles), 1))
					.addComponent(SpriteAnimationComponent(AnimationPlayer(getResources().get<Animation>(animName), "default", posOffset.x > 0 ? "left" : "right")));

				e.getComponent<SpriteAnimationComponent>().player.playOnce("move");
			}
		}
	}

	void playActionSound(BlacksmithActions action)
	{
		lastSound = getAPI().audio->postEvent("sfx/" + getActionName(action), AudioPosition::makeFixed());
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
			return Vector2f(80, 120);
		case ItemState::Done:
			return Vector2f(338, 120);
		case ItemState::Failed:
			return Vector2f(192, 300);
		case ItemState::Out:
			return Vector2f(425, 110);
		default:
			return Vector2f(-100, -100);
		}
	}

	void onItemDone(ItemFamily& item, const ItemConfig& itemConfig)
	{	
		if (itemOK) {
			item.sprite.sprite.setImage(getResources(), itemConfig.imageFixed).setPivot(Vector2f(0.5f, 0.5f));
			if (item.item.type == "dog") {
				getUIService().showMessage("Dog is already perfect!", 1.0f);
			} else {
				getUIService().showMessage("Item repaired!", 1.0f);
			}
		} else {
			item.item.state = ItemState::Failed;
			getUIService().showMessage("Repair failed", 1.0f);
		}
		getItemService().onItemDone(itemConfig.id, itemOK);
	}

	void onMiss(int beat)
	{
		beatMiss = beat;
		if (lastSound) {
			lastSound->stop();
			lastSound = {};
		}
		updateVulcanAnimation(BlacksmithActions::Fail);
		getAPI().audio->postEvent("sfx/buzz", AudioPosition::makeFixed());
	}

	int beatMiss = -1;
	int curBeat = -1;
	int nextItemId = 0;
	bool itemOK = false;
	AudioHandle lastSound;
};

REGISTER_SYSTEM(ItemsSystem)

