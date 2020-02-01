#include <systems/ui_system.h>
#include "components/sprite_animation_component.h"
#include "src/sprite_layers.h"
#include "components/sprite_component.h"
#include "components/position_component.h"

using namespace Halley;

class UISystem final : public UISystemBase<UISystem> {
public:
	void init()
	{
		getWorld().createEntity()
			.addComponent(SpriteComponent(Sprite(), int(SpriteLayers::UI), 1))
			.addComponent(SpriteAnimationComponent(AnimationPlayer(getResources().get<Animation>("health_bar"))))
			.addComponent(UIComponent("health"))
			.addComponent(PositionComponent(Vector2f(5, 5)));
	}
	
	void update(Time t)
    {
		UiFamily& healthBar = uiFamily.match([](const UiFamily& e) { return e.uI.id == "health"; });
		healthBar.spriteAnimation.player.setSequence(toString(getItemService().getHealth() * 10) + "%");
	}
};

REGISTER_SYSTEM(UISystem)

