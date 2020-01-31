#include "systems/sprite_animation_system.h"

class SpriteAnimationSystem final : public SpriteAnimationSystemBase<SpriteAnimationSystem> {
public:
	void update(Halley::Time time, MainFamily& e)
	{
		auto& sprite = e.sprite.sprite;

		auto& player = e.spriteAnimation.player;
		player.update(time);
		player.updateSprite(sprite);
	}

	void onEntitiesAdded(Halley::Span<MainFamily> es)
	{
		for (auto& e: es) {
			e.spriteAnimation.player.update(0);
			e.spriteAnimation.player.updateSprite(e.sprite.sprite);
		}
	}

	void onMessageReceived(const PlayAnimationMessage& message, MainFamily& e)
	{
		
	}
};

REGISTER_SYSTEM(SpriteAnimationSystem)
