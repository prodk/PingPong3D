// GuiObject.h - declaration of the GuiObject class and all its child classes.
// (c) Nikolay Prodanov, Juelich, summer 2013.
#ifndef GUIOBJECT_H
#define GUIOBJECT_H

#include "Shape.h"
#include "Logic.h"

/*________________________________*/
// GuiObject.
class GuiObject
{
public:
	GuiObject(float xExt, float yExt, float wExt, float hExt, std::size_t idExt, std::string n);
	virtual ~GuiObject(void);

	// Pure virtual methods.
	virtual void drawPressed(TTF_Font *font) = 0;
	virtual void drawUnpressed(TTF_Font *font) = 0;

	// Fixed implementation methods.
	bool isPressed(float ptx, float pty);
	bool isPressed();
	void setPressed(bool pressed);
	void setFocus(bool focus);
	bool hasFocus();
	int GuiObject::getId() const;

protected:
	int drawText(const std::string & txt, GLfloat x, GLfloat y, GLfloat w, GLfloat h,
		TTF_Font *font);
	bool ptInRect(float ptx, float pty);

protected:
	float x;
	float y;
	float w;
	float h;
	float xPressed;
	float yPressed;
	float wPressed;
	float hPressed;
	std::size_t id;
	std::string name;

	bool bIsPressed;
	bool bHasFocus;
};

/*________________________________*/
// Button is a GuiObject.
class Button : public GuiObject
{
public:
	Button(float x, float y, float w, float h, std::size_t idExt, std::string n, int tid);
	virtual ~Button();
	
	// Overridden virtual methods.
	virtual void drawPressed(TTF_Font *font);
	virtual void drawUnpressed(TTF_Font *font);

protected:
	int textureId;
};

/*________________________________*/
// OptionsButton is a variant of the Button.
class OptionsButton : public Button
{
public:
	OptionsButton(float x, float y, float w, float h, std::size_t idExt, 
		std::string n, int tid, std::string cap);
	~OptionsButton();
	
	// Overridden virtual methods.
	void drawPressed(TTF_Font *font);
	void drawUnpressed(TTF_Font *font);
	void setCaption(const std::string &c);

private:
	float xSmall;			// Where to draw the small part of the button.
	float wSmall;			// Width of the small button.
	float xSmallPressed;
	std::string caption;	// The second rectangle text.
};

#endif // GUIOBJECT_H