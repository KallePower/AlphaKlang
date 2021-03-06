#include "FirstPerson.h"
//#include "projectMacros.h"

int FirstPerson::ID = -1;


FirstPerson::FirstPerson(void)
{
	ModeName = "FirstPerson";
	isPrimarMode=true;
	MouseCapture=true;
}

//FirstPerson::~FirstPerson(void)
//{
//
//}



bool
FirstPerson::Initialize(void)
{
	
	angle		= 0;
	lx			= 0;
	lz			= -1;
	x			= -40.0f;
	y			= 0;
	z			= 40.0f;
	eyeY		= 1;
	moveSpeed	= 0.5f;
	mouseSpeed	= 1.0f;
	mouseX = SCREENWIDTH/2;
	mouseY = SCREENHEIGHT/2;
	INPUT->attachKey(this);
	INPUT->attachMouseMove(this);
	INPUT->attachMouseWheel(this);
	INPUT->attachSpecial(this);
	ID = this->CamModeID();
	accelerate = false;
	return true;
}

void
FirstPerson::OnActivate(void)
{
	//Enable recieving Mouse-Input and Cursor-warping.
	EnableMouseCapture();
	//update view...
	camera->NeedViewUpdate=true;
	glutReshapeWindow(SCREENWIDTH-1,SCREENHEIGHT-1);
}

int 
FirstPerson::OnDeactivate(void)
{
	return FirstPerson::ID;
}

void 
FirstPerson::UpdateMode(void)
{ 
	if(GUI->IsAnyGUIActive())
		DisableMouseCapture();
	else
		EnableMouseCapture();

	camera->move(x, eyeY, z);
	camera->rotate(x+lx, y + 3.0f, z+lz); 
	this->IsDirty=false;

	gluLookAt(camera->transform.position.x, camera->transform.position.y, camera->transform.position.z,
	camera->transform.rotation.x,camera->transform.rotation.y,camera->transform.rotation.z,	0, 1, 0);
}

void
FirstPerson::DisableMouseCapture(void)
{
	if(MouseCapture)
	{	glutSetCursor(GLUT_CURSOR_INHERIT);
		MouseCapture=false;  }
}

void 
FirstPerson::EnableMouseCapture(void)
{
	if(!MouseCapture)
	{  glutSetCursor(GLUT_CURSOR_NONE);
	   MouseCapture = true; }
}

void 
FirstPerson::keyPress(char key)
{

	if(IsActive)
	{
		switch(key)
		{
			case 119: // W
				x += lx * moveSpeed;
				z += lz * moveSpeed;
				IsDirty=true;
				break;
			case 115: // S
				x -= lx * moveSpeed;
				z -= lz * moveSpeed;
				IsDirty=true;
				break;
			case 97: // A
				x += lz * moveSpeed;
				z += lx * (moveSpeed*-1);
				IsDirty=true;
				break;
			case 100: // D
				x -= lz * moveSpeed;
				z -= lx * (moveSpeed*-1);
				IsDirty=true;
				break;
		}
				

		UpdateMode();
	}
}

void
FirstPerson::specialKeyPressed(int key)
{
	if(IsActive)
	{
	switch (key) {
		case GLUT_KEY_UP:
			x += lx * moveSpeed;
			z += lz * moveSpeed;
			IsDirty=true;
			break;
		case GLUT_KEY_DOWN:
			x -= lx * moveSpeed;
			z -= lz * moveSpeed;
			IsDirty=true;
			break;
		case GLUT_KEY_LEFT:
			x += lz * moveSpeed;
			z += lx * (moveSpeed*-1);
			IsDirty=true;
			break;
		case GLUT_KEY_RIGHT:
			x -= lz * moveSpeed;
			z -= lx * (moveSpeed*-1);
			IsDirty=true;
			break;
		}
	UpdateMode();
	}
}

void
FirstPerson::mouseMotion(int newX, int newY)
{
	if(!IsActive)
		return;

	if(MouseCapture)
	{
		// if mouse haven't change -> return
		if(newX == mouseX && newY == mouseY)
			return;

		// check difference between last-frame mouse pos
		int diffX = newX - mouseX;
		int diffY = newY - mouseY;

		// calculate
		angle += 0.005f * diffX * mouseSpeed;
		lx = sin(angle);
		lz = -cos(angle);
		eyeY += (float)diffY / 300;

		// set fixed restriction to top and bottom
		if(eyeY < 1.5f + y)
			eyeY = 1.5f+ y;
		else if(eyeY > 4.5f+ y)
			eyeY = 4.5f+ y;

		// set mouse pos center to screen
		mouseX = SCREENWIDTH / 2;
		mouseY = SCREENHEIGHT / 2;

		// fix to static mouse pos
		glutWarpPointer(mouseX, mouseY);

		// flag check data uptodate
		IsDirty=true;
		UpdateMode();
	}

}

void
FirstPerson::UpdateHeight(float y)
{
	float diffEyeY = y - this->y;
	this->y = y;
	eyeY += diffEyeY;
}

VectorF
FirstPerson::GetPlayerPosition()
{
	VectorF pos = VectorF(x, z);
	return pos;
}
