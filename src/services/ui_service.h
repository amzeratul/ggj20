#include <halley.hpp>
using namespace Halley;

class UIService : public Service {
public:
    void update(Time t);

    void showMessage(const String& msg, float time);
	void showImportantMessage(const String& msg, float time);

	const String& getMessage() const;
	Colour4f getColour() const;
	Colour4f getOutColour() const;

private:
    String curMessage;
    Time timeLeft = 0;
	Colour4f col;
	Colour4f outCol;
};
