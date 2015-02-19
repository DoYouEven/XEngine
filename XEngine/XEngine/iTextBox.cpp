#include "iTextBox.h"
#include "Utilities.h"
#define TB_BORDER 8.5
#define OFFSET 10
#define BTNSIZE 50
#define FONTWIDTH 8.2
#define BLINKRATE 0.7
iTextBox::iTextBox(char* _name, int posX, int posY, int width, int height, int texID) :Inspector(posX, posY, width, height)
{
	
	name = _name;
	baseTexID = texID;
	isPressed = false;
	isTyping = false;
	type = ITextBox;
	cursor = 0;
}

//Custom Keymapping
void iTextBox::handleKey(SDL_Event event,bool keydown)

{
	static float timer = 0;
	
	timer = timer + XUtils->deltaTime();

	if (keydown == true)
	{

		timer = 0;
		if (event.key.keysym.unicode == SDLK_RETURN)
		{

			isTyping = false;
			return;
		}
		if ((event.key.keysym.unicode == (Uint16)' ') || (event.key.keysym.unicode == (Uint16)'.'))
		{

			text += (char)event.key.keysym.unicode;
		}
		if ((event.key.keysym.unicode >= (Uint16)'0') && (event.key.keysym.unicode <= (Uint16)'9'))
		{

			text += (char)event.key.keysym.unicode;
		}
		if ((event.key.keysym.unicode >= (Uint16)'A') && (event.key.keysym.unicode <= (Uint16)'Z'))
		{

			text += (char)event.key.keysym.unicode;
		}
		else if ((event.key.keysym.unicode >= (Uint16)'a') && (event.key.keysym.unicode <= (Uint16)'z'))
		{

			text += (char)event.key.keysym.unicode;
		}
		if ((event.key.keysym.sym == SDLK_BACKSPACE) && (text.length() != 0))
		{
			//Remove a character from the end
			text.erase(text.length() - 1);
		}
	}
	keydown = false;
	
}
bool iTextBox::updateMouse(MouseInput &input)

{
	Inspector::updateMouse(input);

	if (isTyping == true)
	{
		 handleKey(input.event,input.keydown);
	}
	if (isHover == true)
	{
		if (input.LeftMouseButton)
		{
			isPressed = true;
			isTyping = true;
			return true;
		}

	}
	else if (input.LeftMouseButton)
		isTyping = false;

	if (isTyping == true)
		return true;
	else return false;

	
}
void iTextBox::drawInspectorUnit() {

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, baseTexID);


	glColor4f(1.0f, 1.f, 1.f, .8f);
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 0.0f); glVertex2d(position.x + width + BTNSIZE, position.y);
	glTexCoord2f(0.0f, 0.0f); glVertex2d(position.x, position.y);
	glTexCoord2f(0.0f, 1.0f); glVertex2d(position.x, position.y + height);
	glTexCoord2f(1.0f, 1.0f); glVertex2d(position.x + width + BTNSIZE, position.y + height);
	glEnd();

	if (isPressed == true)
	{
		glColor4f(0.2f, 0.2f, 0.7f, 0.8f);
		glLineWidth(2.0f);

	}
	else

	{
		glColor4f(0.1f, 0.1f, 0.1f, 0.8f);
		glLineWidth(8.0f);
	}

	//*************Draw text background grey

	glDisable(GL_TEXTURE_2D);
	glColor4f(1.0f, 1.f, 1.f, .8f);

	glBegin(GL_QUADS);
	glVertex2d(position.x + width - TB_BORDER, position.y + TB_BORDER);
	glVertex2d(position.x + TB_BORDER, position.y + TB_BORDER);
	glVertex2d(position.x + TB_BORDER, position.y + height - TB_BORDER);
	glVertex2d(position.x + width - TB_BORDER, position.y + height - TB_BORDER);
	glEnd();

	
		if (isTyping == true)
		{
			static float timer = BLINKRATE;
			static bool blink = false;
			timer -=  XUtils->deltaTime();
			if (timer < 0)
			{
				blink = !blink;
				timer = BLINKRATE;
			}

			if (blink)
			{

				glLineWidth(2.5);
				glColor4f(0.0f, 0.f, 0.f, 1.0f);
				glBegin(GL_LINES);
				glVertex2d(position.x + TB_BORDER + (text.length() + 1)*FONTWIDTH, position.y + TB_BORDER*1.3);
				glVertex2d(position.x + TB_BORDER + (text.length() + 1)*FONTWIDTH, position.y + height - TB_BORDER*1.3);
				glEnd();
			}

		}
	
	int textX = position.x + (width - 10) / 2;
	int textY = position.y + (height - 10) / 2;

	glColor4f(0.0, 0.0, 0.0, 1.0);
	if (text.length() > 0)
	{
	const char *cstr = text.c_str();
	XUtils->glPrintf(position.x + OFFSET + TB_BORDER, position.y +(height)/1.8, GLUT_BITMAP_8_BY_13, cstr);
	}
	glDisable(GL_BLEND);
	//printf here;

}
char* iTextBox::getName()
{
	return "iButton";
}