#include "InputManager.h"
#include "DataStructs.h"
#include "projectMacros.h"

DWORD _timerTicks;
DWORD _lastTicks;
DWORD _frameTicks;
DWORD _clicktimer;
DWORD _doubleClickLength;
int _fps;
float _Viewport[]={0,0,SCREENWIDTH_STARTUP,SCREENHEIGHT_STARTUP};
Vector3 _ViewPortNormalizedMouseCoordinates;
bool _buttonChange[16];
bool _axisChange[16];

bool _HasFlag(unsigned value,unsigned flag)
{
	return (value / flag > 0);
}

double _TicksToSeconds(DWORD ticks)
{
	return (float)ticks/1000.0;
}

double 
_updateTimer(void)
{
	_frameTicks = GetTickCount() - _lastTicks;
	_clicktimer = ((_clicktimer > 0)&&(_clicktimer<=_doubleClickLength))? (_clicktimer - _frameTicks):0;
	_lastTicks += _frameTicks;
	return _TicksToSeconds(_frameTicks);
}


InputManager::InputManager(void)
{
	_lastTicks = GetTickCount();
	_frameTicks = 0;
	_doubleClickLength = 200;

	this->keyList = List<unsigned char, 30>();
	this->MouseMotionObservers = List<IObserver*,MAXIMUM_NUMBER_ON_OBSERVERS_PER_EVENT>();
	this->MouseWheelObservers = List<IObserver*,MAXIMUM_NUMBER_ON_OBSERVERS_PER_EVENT>();
	this->KeyboardObservers = List<IObserver*,MAXIMUM_NUMBER_ON_OBSERVERS_PER_EVENT>();
	this->SpecialKeyObservers = List<IObserver*,MAXIMUM_NUMBER_ON_OBSERVERS_PER_EVENT>();
	this->mouseClickObservers = List<IObserver*,MAXIMUM_NUMBER_ON_OBSERVERS_PER_EVENT>();

	FrameTime = 0;
	Mouse.X=Mouse.Y=0;
	Mouse.Position.x=Mouse.Position.y=0.0f;
	LEFTnewState=MIDDLEnewState=RIGHTnewState=false;
	Mouse.LEFT.CLICK=Mouse.RIGHT.CLICK=Mouse.MIDDLE.CLICK=false;
	Mouse.LEFT.HOLD=Mouse.RIGHT.HOLD=Mouse.MIDDLE.HOLD=false;
	Mouse.LEFT.DOUBLE=Mouse.RIGHT.DOUBLE=Mouse.MIDDLE.DOUBLE=false;
	Mouse.Movement = *VectorF::Zero;

	_ViewPortNormalizedMouseCoordinates=Vector3(0,0,0);

	Controler1.Enabled=true;
	Controler1.NumberOfButtons=10;
	Controler1.NumberOfAxis=3;
}

InputManager::~InputManager(void)
{
	//if(MouseMotionObservers.Count() > 0)
	//	for(GobID ID=MouseMotionObservers.First();ID<=MouseMotionObservers.Last();ID=MouseMotionObservers.Next(ID))
	//		MouseMotionObservers.Remove(ID);
	//if(KeyboardObservers.Count() > 0)
	//	for(GobID ID=KeyboardObservers.First();ID<=KeyboardObservers.Last();ID=KeyboardObservers.Next(ID))
	//		KeyboardObservers.Remove(ID);
	//if(SpecialKeyObservers.Count() > 0)
	//	for(GobID ID=SpecialKeyObservers.First();ID<=SpecialKeyObservers.Last();ID=SpecialKeyObservers.Next(ID))
	//		SpecialKeyObservers.Remove(ID);
	//if(mouseClickObservers.Count() > 0)
	//	for(GobID ID=mouseClickObservers.First();ID<=mouseClickObservers.Last();ID=mouseClickObservers.Next(ID))
	//		mouseClickObservers.Remove(ID);
	//if(MouseWheelObservers.Count() > 0)
	//	for(GobID ID=MouseWheelObservers.First();ID<=MouseWheelObservers.Last();ID=MouseWheelObservers.Next(ID))
	//		MouseWheelObservers.Remove(ID);
}

void InputManager::attachMouseMove(IObserver* obs) 
{
	if(obs->checkForObservability(OBSERVATE_MOUSE))
		this->MouseMotionObservers.Add(obs);
}

void InputManager::attachKey(IObserver* obs) 
{
	if(obs->checkForObservability(OBSERVATE_KEYBOARD))
		this->KeyboardObservers.Add(obs);
}

void InputManager::attachSpecial(IObserver* obs) 
{
	if(obs->checkForObservability(OBSERVATE_KEYBOARD))
		this->SpecialKeyObservers.Add(obs);
}

void InputManager::attachMouseClick(IObserver* obs)
{
	if(obs->checkForObservability(OBSERVATE_MOUSE)||obs->checkForObservability(OBSERVATE_CLICKS))
		this->mouseClickObservers.Add(obs);
}

void InputManager::attachMouseWheel(IObserver* obs)
{
	if(obs->checkForObservability(OBSERVATE_WHEELS))
		this->MouseWheelObservers.Add(obs);
}


void
InputManager::DetachMouseMove(IObserver* obs)
{
	this->MouseMotionObservers.Remove(obs);
}
void 
InputManager::DetachMouseClick(IObserver* obs)
{
	this->mouseClickObservers.Remove(obs);
}
void
InputManager::DetachKeyboard(IObserver* obs)
{
	this->KeyboardObservers.Remove(obs);
}

void
InputManager::DetachMouseWheel(IObserver* obs)
{
	this->MouseWheelObservers.Remove(obs);
}

void
InputManager::DetachSpecialkeys(IObserver* obs)
{
	this->SpecialKeyObservers.Remove(obs);
}

void 
InputManager::notifyJoystick(int id,int button,bool state,int AxisX,int AxisY,int AxisZ)
{
	//Todo:  implement Invokationlist for JoysticObserverCalbacks...
}

void
InputManager::registerKey(unsigned char key)
{
//	std::cout<< "INPUT: key: \"" << key << "\" pressed !" << std::endl;

	this->keyList.Add(key);
}

void
InputManager::registerKeyUp(unsigned char key)
{
//	std::cout<< "INPUT: key: \"" << key << "\" released !" << std::endl;

	this->keyList.Remove(key);

}

void
InputManager::notifyKey() 
{
	if( (this->keyList.Count() != 0) && (KeyboardObservers.Count() != 0) )
	{
		for(unsigned ID = this->keyList.First(); ID <= this->keyList.Last(); ID = this->keyList.Next(ID))
		{
			char key = this->keyList[ID];
			for(unsigned ID2 = this->KeyboardObservers.First(); ID2 <= this->KeyboardObservers.Last(); ID2 = this->KeyboardObservers.Next(ID2))
			{
				((IInteractive*)KeyboardObservers[ID2])->keyPress(key);
			}
		}
	}
}

void
InputManager::FireEvents(void)
{
	this->notifyKey();
}

void
InputManager::notifySpecialKey(int key) 
{
	if(SpecialKeyObservers.Count()>0)
	{
		for(unsigned ID = SpecialKeyObservers.First(); ID <= SpecialKeyObservers.Last(); ID = SpecialKeyObservers.Next(ID))
			((IInteractive*)SpecialKeyObservers[ID])->specialKeyPressed(key);
	}
}

void
InputManager::notifyMouse(int x, int y) 
{
	setMousePosition(x,y);

	if(MouseMotionObservers.Count()==0)
		return;

	for(unsigned ID = MouseMotionObservers.First(); ID <= MouseMotionObservers.Last(); ID = MouseMotionObservers.Next(ID))
		((IInteractive*)MouseMotionObservers[ID])->mouseMotion(x,y);
}

void
InputManager::notifyWheel(int wheel)
{
	if(MouseWheelObservers.Count()==0)
		return;

	for(unsigned ID = MouseWheelObservers.First(); ID <= MouseWheelObservers.Last(); ID = MouseWheelObservers.Next(ID) )
	{
		if(MouseWheelObservers[ID]->checkForObservability(OBSERVATE_KEYBOARD))
			((IInteractive*)MouseWheelObservers[ID])->mouseWheel(wheel,wheel==0?Mouse.WheelV:Mouse.WheelH);
		else
		{
			if(wheel==0)
				((IWheelee*)MouseWheelObservers[ID])->WheelVRoll(Mouse.WheelV);
			else
				((IWheelee*)MouseWheelObservers[ID])->WheelHRoll(Mouse.WheelH);
		}
	}
}

void
InputManager::notifyQlicks(void)
{
	if(mouseClickObservers.Count()==0)
		return;

	bool click = false;

	if(Mouse.LEFT.CLICK)
	{click=true;
		for(unsigned it = mouseClickObservers.First(); it <= mouseClickObservers.Last(); it=mouseClickObservers.Next(it)) 
		{
			if(mouseClickObservers[it]->checkForObservability(OBSERVATE_CLICKS))
				((IClickable*)mouseClickObservers[it])->LeftClick(Mouse.Position);
			else 
				((IInteractive*)mouseClickObservers[it])->mouseClicks(0,click,Mouse.Position);
		}
	}
	else if(Mouse.LEFT.RELEASE)
	{
		for(unsigned it = mouseClickObservers.First(); it <= mouseClickObservers.Last(); it=mouseClickObservers.Next(it)) 
		{
			if(mouseClickObservers[it]->checkForObservability(OBSERVATE_CLICKS))
				((IClickable*)mouseClickObservers[it])->LeftRelease(Mouse.Position);
			else 
				((IInteractive*)mouseClickObservers[it])->mouseClicks(0,click,Mouse.Position);
		}
	}

	if(click=Mouse.RIGHT.CLICK)
	{click=true;
		for(unsigned it = mouseClickObservers.First(); it <= mouseClickObservers.Last(); it=mouseClickObservers.Next(it)) 
		{
			if(mouseClickObservers[it]->checkForObservability(OBSERVATE_CLICKS))
				((IClickable*)mouseClickObservers[it])->RightClick(Mouse.Position);
			else
				((IInteractive*)mouseClickObservers[it])->mouseClicks(1,click,Mouse.Position);
		}
	}
	else if(Mouse.RIGHT.RELEASE)
	{
		for(unsigned it = mouseClickObservers.First(); it <= mouseClickObservers.Last(); it=mouseClickObservers.Next(it))
		{
			if(mouseClickObservers[it]->checkForObservability(OBSERVATE_CLICKS))
				((IClickable*)mouseClickObservers[it])->LeftRelease(Mouse.Position);
			else 
				((IInteractive*)mouseClickObservers[it])->mouseClicks(1,false,Mouse.Position);
		}
	}

	if(click=Mouse.MIDDLE.CLICK)
	{click=true;
		for(unsigned it = mouseClickObservers.First(); it <= mouseClickObservers.Last(); it=mouseClickObservers.Next(it)) 
		{
			if(mouseClickObservers[it]->checkForObservability(OBSERVATE_CLICKS))
				((IClickable*)mouseClickObservers[it])->MiddleClick(Mouse.Position);
			else
				((IInteractive*)mouseClickObservers[it])->mouseClicks(2,click,Mouse.Position);
		}
	}
	else if(Mouse.MIDDLE.RELEASE)
	{
		for(unsigned it = mouseClickObservers.First(); it <= mouseClickObservers.Last(); it=mouseClickObservers.Next(it)) 
		{
			if(mouseClickObservers[it]->checkForObservability(OBSERVATE_CLICKS))
				((IClickable*)mouseClickObservers[it])->MiddleRelease(Mouse.Position);
			else 
				((IInteractive*)mouseClickObservers[it])->mouseClicks(2,false,Mouse.Position);
		}
	}
}

void
InputManager::UpdateMouseButtons(int button,int state,int x,int y)
{
		switch (button)
		{
		case GLUT_LEFT_BUTTON:
			LEFTnewState = !state;
			break;
		case GLUT_MIDDLE_BUTTON:
			MIDDLEnewState =  !state;
			break;
		case GLUT_RIGHT_BUTTON:
			RIGHTnewState = !state;
			break;
		}

		setMousePosition(x,y);

		if(mouseClickObservers.Count()>0)
		{
			for(unsigned ID = mouseClickObservers.First(); ID <= mouseClickObservers.Last(); ID = mouseClickObservers.Next(ID))
			{
				if(mouseClickObservers[ID]->checkForObservability(OBSERVATE_KEYBOARD))
					((IInteractive*)mouseClickObservers[ID])->mouseClicks(button,state==GLUT_DOWN,Mouse.Position);
			}
		}
}

void
InputManager::UpdateMouseWheel(int wheel,int state,int x,int y)
{
	// Resets The Mouse-Wheel variables to WHEEL::NONE... 
	// otherwhise the last detected UP or DOWN state will stay as state
	Mouse.WheelV = Mouse.WheelH = WHEEL::NONE;

	if(wheel == 0)			// the first mouswheel -> veretical scroll
		Mouse.WheelV = (WHEEL)state;
	else if(wheel == 1)		// the second mousewheel (if your Mouse has one...)
		Mouse.WheelH = (WHEEL)state;

//Output Status to Console if MOUSE_TEST_OUTPUT is defined...	
#ifdef MOUSE_TEST_OUTPUT 
	if(Mouse.WheelV == WHEEL::UP)
		std::cout<<"\nWheelUP";
	if(Mouse.WheelV == WHEEL::DOWN)
		std::cout<<"\nWheelDOWN";
#endif

// call the invoktionList
	notifyWheel(wheel);
}

void
InputManager::UpdateJoysticks(int id,unsigned buttons,int AxisX,int AxisY,int AxisZ)
{
	switch (id)
	{
	case 0:

		if(Controler1.Buttons!=buttons)
		{
			for(int i = 0;i<Controler1.NumberOfButtons;i++)
				if(_buttonChange[i] = buttons>>31-i)
					notifyJoystick(0,i,_buttonChange[i],Controler1.aX=AxisX,Controler1.aY=AxisY,Controler1.aZ=AxisZ);
		}
		else
		{
			_axisChange[0]=Controler1.aX!=AxisX;
			Controler1.aX = AxisX;
			_axisChange[1]=Controler1.aX!=AxisY;
			Controler1.aY = AxisY;
			_axisChange[2]=Controler1.aX!=AxisZ;
			Controler1.aZ = AxisZ;
			if(_axisChange[0]||_axisChange[1]||_axisChange[2])
				notifyJoystick(id,-1,NULL,Controler1.aX,Controler1.aY,Controler1.aZ);
		}
		break;

	case 1:
		//Todo: Implement second Joystick...
		break;
	}
}

float* 
InputManager::GetViewportRectangle(void)
{
	return &_Viewport[0];
}

void 
InputManager::SaveViewportRectangle(int x,int y,int w,int h)
{
	_Viewport[0]=x;
	_Viewport[1]=y;
	_Viewport[2]=w;
	_Viewport[3]=h;
}

void 
InputManager::SetDoubleclickTime(int milliseconds)
{
	_doubleClickLength = (DWORD)milliseconds;
}

void
InputManager::setMousePosition(int x,int y)
{
	Mouse.Movement.x = x-Mouse.Position.x;
	Mouse.Movement.y = y-Mouse.Position.y;
	Mouse.Position.x = Mouse.X = x;
	Mouse.Position.y = Mouse.Y = y;

	_ViewPortNormalizedMouseCoordinates.x = 2.f*(Mouse.Position.x/_Viewport[2])-1.f;
	_ViewPortNormalizedMouseCoordinates.y = 2.f*(1.f-Mouse.Position.y/_Viewport[3])-1.f;

	glm::vec4 ray_clip(_ViewPortNormalizedMouseCoordinates.x,_ViewPortNormalizedMouseCoordinates.y,-1.f,1.f);

/*
	
	glm::vec4 ray_eye = inverse (projection_matrix) * ray_clip;
	ray_eye = vec4 (ray_eye.xy, -1.0, 0.0);
	vec3 ray_wor = (inverse (view_matrix) * ray_eye).xyz;
// don't forget to normalise the vector at some point
	ray_wor = normalise (ray_wor);
	*/
}



void
InputManager::setMouseButtons(void)
{
	//evaluate Left-button-state:
	Mouse.LEFT.RELEASE = Mouse.LEFT.HOLD? !(Mouse.LEFT.HOLD = LEFTnewState):false;
	Mouse.LEFT.CLICK = (Mouse.LEFT.CLICK||Mouse.LEFT.DOUBLE)? !(Mouse.LEFT.HOLD = LEFTnewState): !Mouse.LEFT.HOLD? LEFTnewState:false;
	if(Mouse.LEFT.CLICK)
	{
		if(_clicktimer>0)
		{	Mouse.LEFT.CLICK = !(Mouse.LEFT.DOUBLE = true); }
		_clicktimer = _doubleClickLength;
	}
	else
		Mouse.LEFT.DOUBLE=false;
	

	//evaluate Right-button-state:
	Mouse.RIGHT.RELEASE = Mouse.RIGHT.HOLD? !(Mouse.RIGHT.HOLD = RIGHTnewState):false;
	Mouse.RIGHT.CLICK = (Mouse.RIGHT.CLICK||Mouse.RIGHT.DOUBLE)? !(Mouse.RIGHT.HOLD = RIGHTnewState): !Mouse.RIGHT.HOLD? RIGHTnewState:false;
	if(Mouse.RIGHT.CLICK)
	{
		if(_clicktimer>0)
			Mouse.RIGHT.CLICK = !(Mouse.RIGHT.DOUBLE = true);
		_clicktimer = _doubleClickLength;
	}
	else
		Mouse.RIGHT.DOUBLE=false;

	//evaluate Middle-button-state:
	Mouse.MIDDLE.RELEASE = Mouse.MIDDLE.HOLD? !(Mouse.MIDDLE.HOLD = MIDDLEnewState):false;
	Mouse.MIDDLE.CLICK = (Mouse.MIDDLE.CLICK||Mouse.MIDDLE.DOUBLE)? !(Mouse.MIDDLE.HOLD = MIDDLEnewState): !Mouse.MIDDLE.HOLD? MIDDLEnewState:false;
	if(Mouse.MIDDLE.CLICK)
	{
		if(_clicktimer>0)
			Mouse.MIDDLE.CLICK = !(Mouse.MIDDLE.DOUBLE = true);
		_clicktimer = _doubleClickLength;
	}
	else
		Mouse.MIDDLE.DOUBLE=false;


	//reset Mouse-Wheels:
	Mouse.WheelV = Mouse.WheelH = WHEEL::NONE;


	//fire click-events if there where clicks during this frame...
	notifyQlicks();
}

int
InputManager::calculateFPS(void)
{
	int fps =  int(1.0/FrameTime);
	if(fps!=_fps)
	{
		printf("FPS = %i ,  lastFrameTime = %f \n",fps,FrameTime);
		_fps=fps;
	}
	return fps;
}

void
InputManager::PerFrameReset(void)
{
	FrameTime = _updateTimer();

	setMouseButtons();

#ifdef CONSOLE_OUTPUT_FPS
	calculateFPS();
#endif
	#ifdef MOUSE_TEST_OUTPUT
		if(Mouse.LEFT.CLICK)
			std::cout<<"CLICK\n";
		else if(Mouse.LEFT.RELEASE)
			std::cout<<"\n...RELEASE\n\n";
		else if(Mouse.LEFT.HOLD)
			std::cout<<"HOLD..";
		else if(Mouse.LEFT.DOUBLE)
			std::cout<<"DOUBLE-CLICK!!!\n";

		if(Mouse.RIGHT.CLICK)
			std::cout<<"RIGHT-CLICK\n";
		else if(Mouse.RIGHT.RELEASE)
			std::cout<<"\n...RIGHT-RELEASE\n\n";
		else if(Mouse.RIGHT.HOLD)
			std::cout<<"RIGHT-HOLD..";
		else if(Mouse.RIGHT.DOUBLE)
			std::cout<<"DOUBLE-RIGHT!!!\n";

		if(Mouse.MIDDLE.CLICK)
			std::cout<<"MIDDLE-CLICK\n";
		else if(Mouse.MIDDLE.RELEASE)
			std::cout<<"\n...MIDDLE-RELEASE\n\n";
		else if(Mouse.MIDDLE.HOLD)
			std::cout<<"MIDDLE-HOLD..";
		else if(Mouse.MIDDLE.DOUBLE)
			std::cout<<"DOUBLE-MIDDLE!!!\n";
	#endif
}



bool
IObserver::checkForObservability(int flag)
{
	int obst=observedEvents();
	bool mouse = (obst/OBSERVATE_MOUSE) >= 1;
	obst %= OBSERVATE_MOUSE;
	bool keys = (obst/OBSERVATE_KEYBOARD) >=1;
	obst %= OBSERVATE_KEYBOARD;
	bool wheels = (obst/OBSERVATE_WHEELS) >=1;
	obst %= OBSERVATE_WHEELS;
	bool clicks = (obst/OBSERVATE_CLICKS) >=1;

	switch(flag)
	{
	case OBSERVATE_MOUSE:
		return mouse;
	case OBSERVATE_KEYBOARD:
		return keys;
	case OBSERVATE_WHEELS:
		return wheels;
	case OBSERVATE_CLICKS:
		return clicks;
	}

	return false;
}

IInteractive::IInteractive()
{
	
}
int
IInteractive::observedEvents()
{
	return OBSERVATE_MOUSE|OBSERVATE_KEYBOARD|OBSERVATE_WHEELS;
}

IClickable::IClickable()
{

}
int
IClickable::observedEvents()
{
	return OBSERVATE_CLICKS|OBSERVATE_MOUSE;
}
IWheelee::IWheelee()
{

}
int 
IWheelee::observedEvents()
{
	return OBSERVATE_WHEELS;
}