#include "title_stage.h"
#include "game.h"
#include "game_stage.h"

void TitleStage::init()
{
	getAudioAPI().stopAllMusic(0.5f);
	titleScreen.setImage(getResources(), "title.png");

	getResources().get<AudioClip>("music/music_mainloop_100_4_4.ogg");
	getResources().get<AudioClip>("music/music_mainloop_120_4_4.ogg");
	getResources().get<AudioClip>("music/music_mainloop_140_4_4.ogg");
}

void TitleStage::onVariableUpdate(Time)
{
	auto input = dynamic_cast<GGJ20Game&>(getGame()).getInputService();
	if (input->getInput().isAnyButtonPressed()) {
		getCoreAPI().setStage(std::make_unique<GameStage>());
	}
}

void TitleStage::onRender(RenderContext& rc) const
{
	auto& game = dynamic_cast<GGJ20Game&>(getGame());
	auto cam = Camera().setZoom(game.getZoom()).setPosition(Vector2f(192, 108));

	rc.with(cam).bind([&] (Painter& painter)
	{
		titleScreen.draw(painter);
	});
}
