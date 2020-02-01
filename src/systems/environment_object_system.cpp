#include <systems/environment_object_system.h>
#include "src/sprite_layers.h"
#include "components/sprite_animation_component.h"
#include "components/position_component.h"

using namespace Halley;

class EnvironmentObjectSystem final : public EnvironmentObjectSystemBase<EnvironmentObjectSystem> {
public:
	void init()
	{
		loadScene(getResources().get<ConfigFile>("gameplay/env_objects")->getRoot());
	}
	
	void update(Time t)
    {
		for (auto& e: objectFamily) {
			if (!e.environmentObject.animTarget.isEmpty()) {
				e.environmentObject.animTime -= float(t);
				if (e.environmentObject.animTime <= 0) {
					e.spriteAnimation.player.setSequence(e.environmentObject.animTarget);
					e.environmentObject.animTarget = "";
				}
			}
		}
	}

private:
	void loadScene(const ConfigNode& node)
	{
		for (auto& n: node["objects"].asSequence()) {
			const auto id = n["id"].asString();
			const auto anim = n["animation"].asString();
			const auto layerName = n["layer"].asString();
			const auto pos = n["position"].asVector2f();

			auto layer = SpriteLayers::BackgroundObject;
			if (layerName == "smith") {
				layer = SpriteLayers::Smith;
			}

			getWorld().createEntity()
				.addComponent(SpriteComponent(Sprite(), int(layer), 1))
				.addComponent(SpriteAnimationComponent(AnimationPlayer(getResources().get<Animation>(anim))))
				.addComponent(PositionComponent(pos))
				.addComponent(EnvironmentObjectComponent(id, 0, ""));
		}
	}
};

REGISTER_SYSTEM(EnvironmentObjectSystem)

