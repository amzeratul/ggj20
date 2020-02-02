#include "game.h"
#include "game_stage.h"
#include "title_stage.h"

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
	constexpr bool localAssets = Debug::isDebug();
	if (localAssets) {
		locator.addFileSystem(unpackedAssetsPath);
	} else {
		const String packs[] = { "music.dat", "sfx.dat", "gameplay.dat", "images.dat", "shaders.dat", "ui.dat", "config.dat" };
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

	auto screenSize = api->system->getScreenSize(0);
	auto windowSize = Vector2i(1920, 1080);
	auto windowType = WindowType::BorderlessWindow;
	if (screenSize != windowSize || Debug::isDebug()) {
		windowSize = Vector2i(1920, 1080) / 5 * 4;
		windowType = WindowType::Window;
	}
	
	zoom = std::max(float(windowSize.x) / 384.0f, float(windowSize.y) / 216.0f);
	
	api->video->setWindow(WindowDefinition(windowType, windowSize, "Law of the Instrument"));
	api->video->setVsync(vsync);
	api->audio->startPlayback();

	api->audio->setGroupVolume("music", 1.0f);
	api->audio->setListener(AudioListenerData(Vector3f(192, 108, -20), 200));

	inputService = std::make_shared<InputService>(*api->input);

	return std::make_unique<TitleStage>();
}

std::shared_ptr<InputService> GGJ20Game::getInputService() const
{
	return inputService;
}

float GGJ20Game::getZoom() const
{
	return zoom;
}

bool GGJ20Game::shouldCreateSeparateConsole() const
{
	return Debug::isDebug();
}

HalleyGame(GGJ20Game);
