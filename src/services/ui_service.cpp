#include "ui_service.h"

void UIService::update(Time t)
{
	timeLeft -= t;
	if (timeLeft < 0 && !curMessage.isEmpty()) {
		curMessage = "";
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
