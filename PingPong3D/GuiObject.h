#pragma once

#include "Shape.h"
#include "Logic.h"

/*________________________________*/
// GuiObject class declaration.
class GuiObject
{
public:
	GuiObject(std::size_t idExt);
	virtual ~GuiObject(void);
	virtual void draw(TTF_Font *font, Logic &logic) = 0;
	virtual void handleMouseButtonUp(Logic &logic, float x, float y) = 0;
	virtual void handleMouseButtonDown(Logic &logic, float x, float y) = 0;
	virtual void handleKeyDown(Logic &logic) = 0;

	std::size_t getId();

protected:
	std::size_t id;
};

/*________________________________*/
//Button class declaration.
class Button : public GuiObject
{
public:
	Button(float x, float y, float w, float h, std::size_t idExt, std::string n, int tid);
	~Button();
	
	// Overridden virtual methods.
	void draw(TTF_Font *font, Logic &logic);
	void handleMouseButtonUp(Logic &logic, float x, float y);
	void handleMouseButtonDown(Logic &logic, float x, float y);
	void handleKeyDown(Logic &logic);

private:
	void addButtons();
	int drawText(TTF_Font *font, Logic &logic);			// Change arguments later!
	bool ptInRect(float ptx, float pty);
private:
	float x;
	float y;
	float w;
	float h;
	int textureId;
	std::string name;
	std::string caption;
	//bool bPressed;
	enum{START_BUTTON, OPTIONS_BUTTON};
};