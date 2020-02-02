#include <systems/ui_render_system.h>
#include "components/sprite_component.h"
#include "src/sprite_layers.h"
#include "components/sprite_animation_component.h"

using namespace Halley;

class UIRenderSystem final : public UIRenderSystemBase<UIRenderSystem> {
public:
	void init()
	{
		font = TextRenderer()
			.setFont(getResources().get<Font>("Equipment"))
			.setSize(16)
			.setColour(Colour4f(1, 1, 1));

		screen = Sprite()
			.setImage(getResources(), "whitebox.png")
			.setColour(Colour4f(0, 0, 0, 0))
			.scaleTo(Vector2f(384.0f, 216.0f));
	}
	
	void render(RenderContext& rc)
	{
        for (auto& c: cameraFamily) {
			auto cam = Camera().setPosition(c.position.position).setZoom(c.camera.zoom);
			rc.with(cam).bind([&] (Painter& painter)
			{
				paint(painter);
			});
			break;
		}
	}

    
private:
	void paint(Painter& painter)
	{
		if (getItemService().isAlive()) {
			paintGameUI(painter);
		} else {
			paintGameOverUI(painter);
		}
    }

	void paintGameUI(Painter& painter)
	{
		// Current message
		if (!getUIService().getMessage().isEmpty()) {		
			font.clone()
				.setPosition(Vector2f(192, 30))
				.setText(getUIService().getMessage())
				.setColour(getUIService().getColour())
				.setOutline(1.0f)
				.setOutlineColour(getUIService().getOutColour())
				.setAlignment(0.5f)
				.draw(painter);
		}
		
		// Progress data
		auto& items = getItemService();
		String scoreStr = toString(items.getScore());
		String multStr = "x" + toString(items.getMult() / 10) + "." + toString(items.getMult() % 10);
		String hpStr = toString(items.getHealth());
		
		// Debug text
		auto text = "Score: " + scoreStr + "\nMult: " + multStr;
		font.clone().setPosition(Vector2f(380, 4)).setText(text).setAlignment(1.0f).draw(painter);
	}

	void paintGameOverUI(Painter& painter)
	{
		float a = 0.8f;
		screen.setColour(Colour4f(0, 0, 0, a)).draw(painter);
		font.clone()
			.setPosition(Vector2f(192, 108))
			.setText("Game Over!\nYour final score was: " + toString(getItemService().getScore()) + "\n\nPress any Button to restart")
			.setOutlineColour(Colour4f(0, 0, 0))
			.setOutline(1.0f)
			.setAlignment(0.5f)
			.setOffset(Vector2f(0.0f, 0.5f))
			.draw(painter);
	}

	Sprite screen;
	TextRenderer font;
};

REGISTER_SYSTEM(UIRenderSystem)

