#include "GuiObject.h"


GuiObject::GuiObject(int x, int y, int w, int h)
{
	rcBox.x = x;
	rcBox.y = y;
	rcBox.w = w;
	rcBox.h = h;
}


GuiObject::~GuiObject(void)
{
}
