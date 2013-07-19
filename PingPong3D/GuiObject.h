// GuiObject.h - declaration of the GuiObject class and all its child classes.
// (c) Nikolay Prodanov, Juelich, summer 2013.
#ifndef GUIOBJECT_H
#define GUIOBJECT_H

#include "Shape.h"
#include "Logic.h"

/*________________________________*/
// GuiObject class declaration.
class GuiObject : public Observer
{
public:
	GuiObject(std::size_t idExt);
	virtual ~GuiObject(void);
	virtual void draw(TTF_Font *font, Logic &logic) = 0;
	virtual void handleMouseButtonUp(Logic &logic, float x, float y) = 0;
	virtual void handleMouseButtonDown(Logic &logic, float x, float y) = 0;
	virtual void handleKeyDown(Logic &logic) = 0;

	virtual void notify(Subject* s);

	FMOD_RESULT setSound(FMOD::System *sys, FMOD::Sound *snd);

	std::size_t getId();

protected:
	std::size_t id;
	bool bPlaySound;
	FMOD::Sound* sound;
	FMOD::System *system;
	FMOD::Channel *channel;	// Maybe delete later.
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
	enum{START_BUTTON, OPTIONS_BUTTON, BACKGR_SOUND_BUTTON, ACT_SOUND_BUTTON};
};

#endif // GUIOBJECT_H