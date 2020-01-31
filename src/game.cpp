#include "game.h"
#include "game_stage.h"

void initOpenGLPlugin(IPluginRegistry &registry);
void initSDLSystemPlugin(IPluginRegistry &registry, Maybe<String> cryptKey);
void initSDLAudioPlugin(IPluginRegistry &registry);
void initSDLInputPlugin(IPluginRegistry &registry);
void initDX11Plugin(IPluginRegistry &registry);
void initAsioPlugin(IPluginRegistry &registry);

void GGJ20Game::init(const Environment& env, const Vector<String>& args)
{
}

int GGJ20Game::initPlugins(IPluginRegistry& registry)
{
	initOpenGLPlugin(registry);
	initSDLSystemPlugin(registry, {});
	initSDLAudioPlugin(registry);
	initSDLInputPlugin(registry);

#ifdef WITH_DX11
	initDX11Plugin(registry);
#endif
#ifdef WITH_ASIO
	initAsioPlugin(registry);
#endif

	return HalleyAPIFlags::Video | HalleyAPIFlags::Audio | HalleyAPIFlags::Input | HalleyAPIFlags::Network | HalleyAPIFlags::Platform;
}

void GGJ20Game::initResourceLocator(const Path& gamePath, const Path& assetsPath, const Path& unpackedAssetsPath, ResourceLocator& locator) {
	constexpr bool localAssets = true;
	if (localAssets) {
		locator.addFileSystem(unpackedAssetsPath);
	} else {
		const String packs[] = { "gameplay.dat", "images.dat", "shaders.dat", "ui.dat", "config.dat" };
		for (auto& pack: packs) {
			locator.addPack(Path(assetsPath) / pack);
		}
	}
}

String GGJ20Game::getName() const
{
	return "GGJ20";
}

String GGJ20Game::getDataPath() const
{
	return "Halley/ggj20";
}

bool GGJ20Game::isDevMode() const
{
	return true;
}

std::unique_ptr<Stage> GGJ20Game::startGame(const HalleyAPI* api)
{
	this->api = api;

	bool vsync = true;

	api->video->setWindow(WindowDefinition(WindowType::Window, Vector2i(1280, 720), "GGJ20"));
	api->video->setVsync(vsync);
	api->audio->startPlayback();
	return std::make_unique<GameStage>();
}

HalleyGame(GGJ20Game);
