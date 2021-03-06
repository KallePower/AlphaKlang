
#include "ControlledValues.h"

ControlledVector3::ControlledVector3(void)
{
	x.SetUp(0,255,128,1.0f/128,x.Clamp);
	x.ControllerActive = true;
	x.SetUp(0,255,128,1.0f/128,y.Clamp);
	y.ControllerActive = true;
	x.SetUp(0,255,128,1.0f/128,z.Clamp);
	z.ControllerActive = true;
}

ControlledVector3::~ControlledVector3(void)
{
	x.~Controlled<float>();
	y.~Controlled<float>();
	z.~Controlled<float>();
}

void 
ControlledVector3::ControllersActive(bool set)
{
	x.ControllerActive = y.ControllerActive = z.ControllerActive = set;
}

Vector3
ControlledVector3::Check(void)
{
	return Vector3(x.Check(),y.Check(),z.Check());
}

void
ControlledVector3::SetMode(Controlled<float>::ControllMode mode)
{
	x.Mode(mode);
	y.Mode(mode);
	z.Mode(mode);
}

void
ControlledVector3::SetMIN(Vector3 min)
{
	 x.MIN = min.x;
	 y.MIN = min.y;
	 z.MIN = min.z;
}

void
ControlledVector3::SetMAX(Vector3 max)
{
	x.MAX = max.x;
	y.MAX = max.y;
	z.MAX = max.z;
}

void
ControlledVector3::SetMOVE(Vector3 move)
{
	x.MOVE = move.x;
	y.MOVE = move.y;
	z.MOVE = move.z;
}

ControlledVector3::operator Vector3(void)
{
	return Vector3(x,y,z);
}

Vector3 
ControlledVector3::operator =(Vector3 setter)
{
	x = setter.x;
	y = setter.y;
	z = setter.z;
	return setter;
}


//
//ControlledFloat::ControlledFloat(void)
//{
//	MOVE = 0;
//	MIN = 0;
//	MAX = 1;
//	_Value = new float(0.5f);
//	Mode(ControlledFloat::ControllMode::Clamp);
//}
//ControlledFloat::~ControlledFloat(void)
//{
//	if(UserMode!=NULL)
//		delete UserMode;
//	delete _Value;
//}
//
//float
//ControlledFloat::checkValue(unsigned short mode)
//{
//	switch (mode)
//	{
//	case 0:
//		return *_Value;
//	case ControlledFloat::ControllMode::Invert:
//		 return -(*_Value);
//	break;
//	case ControlledFloat::ControllMode::Clamp:
//		*_Value = *_Value<MIN?MIN:*_Value>MAX?MAX:*_Value; 
//	break;
//	case ControlledFloat::ControllMode::Cycle:
//		*_Value = *_Value<MIN?MAX-(MIN- *_Value):*_Value>MAX?MIN+(*_Value - MAX): (*_Value+MOVE); 
//	break;
//	case ControlledFloat::ControllMode::PingPong:
//		*_Value = *_Value<MIN?(MIN+(MIN- *_Value))+(MOVE *= -1):*_Value>MAX? (MAX-(*_Value-MAX))+(MOVE *= -1) :(*_Value+MOVE); 
//	break;
//	default:
//		if((UserMode!=NULL)&&(UserMode->ID==mode))
//			return UserMode->checkVALUE(_Value);
//	break;
//	}
//
//	return *_Value;
//}
//
//ControlledFloat*
//ControlledFloat::SetUp(Controlled<float>::ControllMode mode,float initial)
//{
//	*_Value=initial;
//	Mode(mode);
//	return this;
//}
//
//ControlledFloat*
//ControlledFloat::SetUp(float min,float max,float move,float initial,Controlled<float>::ControllMode mode)
//{
//	MIN=min;
//	MAX=max;
//	*_Value = initial;
//	Mode(ControlledFloat::ControllMode::Clamp);
//	return this;
//}
//