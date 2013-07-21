// GuiObject.h - declaration of the GuiObject class and all its child classes.
// (c) Nikolay Prodanov, Juelich, summer 2013.
#ifndef GUIOBJECT_H
#define GUIOBJECT_H

#include "Shape.h"
#include "Logic.h"

/*________________________________*/
// GuiObject 'is a' Observer.
class GuiObject : public Observer
{
public:
	GuiObject(std::size_t idExt);
	virtual ~GuiObject(void);

	// Pure virtual methods.
	virtual void draw(TTF_Font *font, Logic &logic) = 0;
	virtual void handleMouseButtonUp(Logic &logic, float x, float y) = 0;
	virtual void handleMouseButtonDown(Logic &logic, float x, float y) = 0;
	virtual void handleKeyDown(Logic &logic) = 0;

	virtual void notify(Subject* s);	// Overridden virtual Observer function.

	// Fixed implementation methods.
	FMOD_RESULT setSound(FMOD::System *sys, FMOD::Sound *snd);
	std::size_t getId();
	int drawText(const std::string & txt, GLfloat x, GLfloat y, GLfloat w, GLfloat h,
	TTF_Font *font, Logic &logic);

protected:
	std::size_t id;
	bool bPlaySound;
	FMOD::Sound* sound;
	FMOD::System *system;
	FMOD::Channel *channel;	// Maybe delete later.
};

/*________________________________*/
// Button is a GuiObject.
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
	//void addButtons();
	//int drawText(TTF_Font *font, Logic &logic);
	bool ptInRect(float ptx, float pty);

private:
	enum{START_BTN, OPTIONS_BTN, HOWTO_BTN, TRAIN_BTN, BACK_BTN};
	float x;
	float y;
	float w;
	float h;
	int textureId;
	std::string name;
	std::string caption;	// Button text.
};

/*________________________________*/
// Button is a GuiObject.
class OptionsButton : public GuiObject
{
public:
	OptionsButton(float x, float y, float w, float h, std::size_t idExt, 
		std::string n, int tid, Logic &logic);
	~OptionsButton();
	
	// Overridden virtual methods.
	void draw(TTF_Font *font, Logic &logic);
	void handleMouseButtonUp(Logic &logic, float x, float y);
	void handleMouseButtonDown(Logic &logic, float x, float y);
	void handleKeyDown(Logic &logic);

private:
	//void addButtons();
	//int drawText(TTF_Font *font, Logic &logic);
	bool ptInRect(float ptx, float pty);

private:
	enum{BACKGRSND_BTN, ACTIONSND_BTN, ROUND_BTN, BACK_BTN};
	float x;
	float y;
	float w;
	float h;
	float xSmall;			// Where to draw the small part of the button.
	float wSmall;			// Width of the small button.
	int textureId;
	std::string name;
	std::string caption;	// Button text.
};

#endif // GUIOBJECT_H