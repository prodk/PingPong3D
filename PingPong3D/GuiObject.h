// GuiObject.h - declaration of the GuiObject class and all its child classes.
// (c) Nikolay Prodanov, Juelich, summer 2013.
#ifndef GUIOBJECT_H
#define GUIOBJECT_H

#include "Shape.h"
#include "Logic.h"

/*________________________________*/
// GuiObject 'is a' Observer.
class GuiObject //: public Observer
{
public:
	//GuiObject(std::size_t idExt);
	GuiObject(float xExt, float yExt, float wExt, float hExt, std::size_t idExt, std::string n);
	virtual ~GuiObject(void);

	// Pure virtual methods.
	virtual void drawPressed(TTF_Font *font) = 0;
	virtual void drawUnpressed(TTF_Font *font) = 0;
	//virtual void handleMouseButtonUp(Logic &logic, float x, float y) = 0;
	//virtual void handleMouseButtonDown(Logic &logic, float x, float y) = 0;
	//virtual void handleKeyDown(Logic &logic) = 0;

	//virtual void notify(Subject* s);	// Overridden virtual Observer function.

	// Fixed implementation methods.
	//FMOD_RESULT setSound(FMOD::System *sys, FMOD::Sound *snd);
	std::size_t getId() const;
	bool isPressed(float ptx, float pty);
	bool isPressed();
	void setPressed(bool pressed);

protected:
	int drawText(const std::string & txt, GLfloat x, GLfloat y, GLfloat w, GLfloat h,
		TTF_Font *font);
	bool ptInRect(float ptx, float pty);

protected:
	float x;
	float y;
	float w;
	float h;
	std::size_t id;
	std::string name;

	bool bIsPressed;
	//bool bPlaySound;
	//FMOD::Sound* sound;
	//FMOD::System *system;
	//FMOD::Channel *channel;	// Maybe delete later.
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

//private:
	//bool ptInRect(float ptx, float pty);

protected:
	//enum{START_BTN, OPTIONS_BTN, HOWTO_BTN, TRAIN_BTN, BACK_BTN};
	/*float x;
	float y;
	float w;
	float h;*/
	int textureId;
	//std::string name;
	//std::string caption;	// Button text.
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
	//void draw(TTF_Font *font, Logic &logic);
	void drawPressed(TTF_Font *font);
	void drawUnpressed(TTF_Font *font);
	void setCaption(const std::string &c);

//private:
	//bool ptInRect(float ptx, float pty);

private:
	//enum{BACKGRSND_BTN, ACTIONSND_BTN, ROUND_BTN, BACK_BTN};
	/*float x;
	float y;
	float w;
	float h;*/
	float xSmall;			// Where to draw the small part of the button.
	float wSmall;			// Width of the small button.
	//int textureId;
	//std::string name;
	std::string caption;	// The second rectangle text.
};

#endif // GUIOBJECT_H