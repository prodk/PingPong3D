// GuiObject.h - declaration of the GuiObject class and all its child classes.
// (c) Nikolay Prodanov, summer 2013, Juelich, Germany.
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

	// Virtual functions.
	virtual void draw(TTF_Font *font);

	// Fixed implementation methods.
	bool isPressed(float ptx, float pty);	// Confusion, it is not pressed, but under the cursor.
	bool isPressed();						// True only if the object is really pressed.
	void setPressed(bool pressed);
	void setFocus(bool focus);
	bool hasFocus();
	int getId() const;

protected:
	// Pure virtual functions.
	virtual void drawPressed(TTF_Font *font) = 0;
	virtual void drawUnpressed(TTF_Font *font) = 0;

	int drawText(const std::string & txt, GLfloat x, GLfloat y, GLfloat w, GLfloat h,
		TTF_Font *font);
	bool ptInRect(float ptx, float pty);
	void createTextTexture();

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
	float txtW;			// Width of text.
	float txtX;

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

protected:
	// Overridden virtual functions.
	virtual void drawPressed(TTF_Font *font);
	virtual void drawUnpressed(TTF_Font *font);
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
	void setCaption(const std::string &c);

private:
	// Overridden virtual functions.
	void drawPressed(TTF_Font *font);
	void drawUnpressed(TTF_Font *font);
	
	float xSmall;			// Where to draw the small part of the button.
	float wSmall;			// Width of the small button.
	float xSmallPressed;
	std::string caption;	// The text of the second rectangle.
};

#endif // GUIOBJECT_H