#include <systems/sprite_render_system.h>

using namespace Halley;

class SpriteRenderSystem final : public SpriteRenderSystemBase<SpriteRenderSystem> {
public:	
	void render(RenderContext& rc)
	{
		for (auto& e: spriteFamily) {
			auto& sprite = e.sprite.sprite;
			if (sprite.hasMaterial()) {
				const Vector2f basePos = e.position.position;
				const Vector2f pos = e.spritePositionOverride.hasValue() ? e.spritePositionOverride.get().position.value_or(basePos) : basePos;
				sprite.setPos(pos);
				getPainterService().spritePainter.add(sprite, e.sprite.mask, e.sprite.layer, pos.y);
			}
		}
		for (auto& e: textFamily) {
			auto& text = e.textLabel.text;
			text.setPosition(e.position.position);
			getPainterService().spritePainter.add(text, e.textLabel.mask, e.textLabel.layer, text.getPosition().y);
		}
	}
};

REGISTER_SYSTEM(SpriteRenderSystem)

