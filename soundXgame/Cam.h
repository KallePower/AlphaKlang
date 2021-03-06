#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "Utility.h"
#include "InputManager.h"
#include "IGObject.h"
#include "Connectable.h"
#include "AudioObject.h"


enum CAM_MODE : int
{
	set = -1,
	FOLLOWTARGET = 1,
	FIRSTPERSON = 2,
	POINTANDCLICK = 3,
	TARGETGRABBER = 4
};



class CameraMode : public IConnectable
{
protected:
	virtual void UpdateMode(void){}
	bool isPrimarMode;
	int lastActivePrimarMode;

public:
	Cam* camera;
	char* ModeName;
	bool Activate(BOOL setter = 3)
		{
			if(setter<3)
			{
				IsActive=setter;
				if(IsActive)
					OnActivate();
				else 
				{
					if(isPrimarMode)
						lastActivePrimarMode = OnDeactivate();
					else
						OnDeactivate();
				}
			}
			return IsActive;
		}
	virtual void OnActivate(void){}
	virtual int OnDeactivate(void){return 0;}
	bool IsDirty;
	bool IsPrimarMode(void)
	{return isPrimarMode;}
	virtual ~CameraMode(void)
	{
		for(int i = 0 ; i<NumberOfConnectedObjects;i++)
			RemoveConnected<IConnectable>(i+1);
	}
	
	void UpdateAllActiveModes(void)
	{
		if(NumberOfConnectedObjects<=0)
			return;

		CameraMode* m;
		for(int i = 0;i<NumberOfConnectedObjects;i++)
		{
			m = (CameraMode*)getConnectables(ConIDs[i]-1);
			if(m->IsActive || m->IsDirty)
			{	
				m->UpdateMode();
				m->IsDirty=false;
			}
		}
	}
	
	int CamModeID(void)
		{return (int)this->ConnectionID;}

	virtual void SetConnection(IConnectable* camconnector)
		{camera=((CameraMode*)camconnector)->camera;}

	template<typename C> C* AddCameraMode(void)
	{

	Not_hasInitialized();
		
	for(int i=0;i<MAXIMUM_NUMBER_OF_CONNECTIONS;i++)
		if(ConIDs[i] == NULL)
		{
			C::ID = i+1;
			CameraMode* newcon = new C();
			newcon->SetConnection(this);
			newcon->ConnectionID = ConIDs[i] = i+1;
			newcon->ModeName = (char*)typeid(C).name();
			setConnectables(i,(C*)newcon);
			NumberOfConnectedObjects++;
			this->camera->NumberOfModes++;
			return (C*)getConnectables(ConIDs[i]-1);
		}
		return NULL;
	}

	template<typename C> C* Get(ConID id)
	{
		return (C*)getConnectables(id-1);
	}

	template<typename C> C* GetCameraMode(void)
	{
		return (C*)getConnectables(C::ID-1);
	}

};

class Cam : public IWheelee,  public IAudioReciever
{
private:
	static bool				_shareAudioReciever;
	Vector3					*_targetPosition;		//position vector the camera looks at if in FOLLOWTARGET-Mode...
	IObjection<IConnectable>*	_targetObject;
	ConID					*targetConID;			
	double					_fieldOfView;
	GLfloat					_aspect;
	int						_mode;
	float					_distanceToTarget;
	
	void					UpdateDirections(void); // Re-Calculates "forward","right" and "up"
	
protected:
	virtual bool			IsShared(bool=NULL);
	bool					TransformDIRTY;

public:
							Cam(void);
	float					FarClipDistance;
	bool					NeedViewUpdate;
	virtual					~Cam(void);
	void					UpdateView();			// Update Window and or Viewport Changes...
	virtual double			FieldOfView(double = _FNan._Double);
	virtual GLfloat			Aspect(GLfloat = NULL);
	Transform               transform;	
	void		            SetTarget(Vector3* target);
	IGObject*               SetTarget(IObjection<IConnectable>* target);
	IGObject*				GetTarget(void);
	Vector3					GetTargetPosition(void);
	float					GetTargetDistance(void);
	void					followTarget(void);
	void					stopFollowing(void);
	virtual void			WheelVRoll(WHEEL state);
	Vector3					move(float x,float y,float z);
	Vector3		   		    move(Vector3);
	Vector3					rotate(float x,float y,float z);
	Vector3			        rotate(Vector3);
	bool				    ShareAudio(BOOL=3);
	virtual void			Update(void);
	int						NumberOfModes; 
	CAM_MODE                Mode(CAM_MODE = set);
	CAM_MODE                Mode(int);
	CameraMode*			    ModeSocket;					// reference to attached mode-extensions...
	//template<class cM> cM*	mode(CAM_MODE value=set)
	//						{
	//							if(value==set)
	//								return ModeSocket->Get<cM>(Mode(set));
	//							return ModeSocket->Get<cM>(value);
	//						}
};

#endif