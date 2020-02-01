#include <systems/ui_system.h>

using namespace Halley;

class UISystem final : public UISystemBase<UISystem> {
public:
	void init()
	{
		font = TextRenderer()
			.setFont(getResources().get<Font>("Ubuntu Bold"))
			.setSize(6)
			.setColour(Colour4f(1, 1, 1));
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
		auto& items = getItemService();
		String scoreStr = toString(items.getScore());
		String multStr = "x" + toString(items.getMult() / 10) + "." + toString(items.getMult() % 10);
		String hpStr = toString(items.getHealth());
		
		auto text = "Score: " + scoreStr + "\nMult: " + multStr + "\nHP: " + hpStr;
		font.clone().setPosition(Vector2f(4, 4)).setText(text).draw(painter);
    }

	TextRenderer font;
};

REGISTER_SYSTEM(UISystem)

