#include <systems/ui_system.h>

using namespace Halley;

class UISystem final : public UISystemBase<UISystem> {
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
		auto& items = getItemService();
		String scoreStr = toString(items.getScore());
		String multStr = "x" + toString(items.getMult() / 10) + "." + toString(items.getMult() % 10);
		String hpStr = toString(items.getHealth());
		
		auto text = "Score: " + scoreStr + "\nMult: " + multStr + "\nHP: " + hpStr;
		font.clone().setPosition(Vector2f(4, 4)).setText(text).draw(painter);
	}

	void paintGameOverUI(Painter& painter)
	{
		float a = 0.5f;
		screen.setColour(Colour4f(0, 0, 0, a)).draw(painter);
	}

	Sprite screen;
	TextRenderer font;
};

REGISTER_SYSTEM(UISystem)

