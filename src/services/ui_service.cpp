#include "ui_service.h"

void UIService::update(Time t)
{
	timeLeft -= t;
	if (timeLeft < 0 && !curMessage.isEmpty()) {
		curMessage = "";
	}

	if (fade != FadeMode::Normal) {
		curFadeTime += t;
		if (curFadeTime >= fadeTime) {
			curFadeTime = 0;
			fade = FadeMode::Normal;
		}
	}
}

void UIService::showMessage(const String& msg, float time)
{
	curMessage = msg;
	timeLeft = time;
	col = Colour4f::fromString("#FFFFFF");
	outCol = Colour4f(0, 0, 0);
}

void UIService::showImportantMessage(const String& msg, float time)
{
	curMessage = msg;
	timeLeft = time;
	col = Colour4f::fromString("#BD1806");
	outCol = Colour4f(1, 1, 1);
}

const String& UIService::getMessage() const
{
	return curMessage;
}

Colour4f UIService::getColour() const
{
	return col;
}

Colour4f UIService::getOutColour() const
{
	return outCol;
}

void UIService::fadeIn(Time t)
{
	fade = FadeMode::FadeIn;
	fadeTime = t;
	curFadeTime = 0;
}

void UIService::fadeOut(Time t)
{
	fade = FadeMode::FadeOut;
	fadeTime = t;
	curFadeTime = 0;
}

float UIService::getFadeOpacity() const
{
	if (fade == FadeMode::Normal) {
		return 0;
	} else if (fade == FadeMode::FadeIn) {
		return 1.0f - float(curFadeTime / fadeTime);
	} else if (fade == FadeMode::FadeOut) {
		return float(curFadeTime / fadeTime);
	}
	return 0;
}
