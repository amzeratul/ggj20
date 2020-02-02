#include <halley.hpp>
using namespace Halley;

enum class FadeMode {
	FadeIn,
	Normal,
	FadeOut
};

class UIService : public Service {
public:
    void update(Time t);

    void showMessage(const String& msg, float time);
	void showImportantMessage(const String& msg, float time);

	const String& getMessage() const;
	Colour4f getColour() const;
	Colour4f getOutColour() const;

	void fadeIn(Time t);
	void fadeOut(Time t);
	float getFadeOpacity() const;

private:
    String curMessage;
    Time timeLeft = 0;
	Colour4f col;
	Colour4f outCol;

	FadeMode fade;
	Time fadeTime;
	Time curFadeTime;
};
