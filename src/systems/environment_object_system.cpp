#include <systems/environment_object_system.h>
#include "src/sprite_layers.h"
#include "components/sprite_animation_component.h"
#include "halley/entity/components/transform_2d_component.h"

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
		for (auto& e: bouncyFamily) {
			e.sprite.sprite.setScale(1.0f + std::cos(e.bouncy.bounceTime * 2 * 3.14159265f) * 0.05f * e.bouncy.bounceAmplitude);
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
			const auto amplitude = n["bounce"].asFloat();

			auto layer = SpriteLayers::BackgroundObject;
			if (layerName == "smith") {
				layer = SpriteLayers::Smith;
			} else if (layerName == "floor") {
				layer = SpriteLayers::Floor;
			} else if (layerName == "wall") {
				layer = SpriteLayers::Wall;
			}

			getWorld().createEntity()
				.addComponent(SpriteComponent(Sprite(), int(layer), 1))
				.addComponent(SpriteAnimationComponent(AnimationPlayer(getResources().get<Animation>(anim))))
				.addComponent(Transform2DComponent(pos))
				.addComponent(EnvironmentObjectComponent(id, 0, ""))
				.addComponent(BouncyComponent(0, amplitude));
		}
	}
};

REGISTER_SYSTEM(EnvironmentObjectSystem)

