#ifndef _IMUSICLISTENER_H_
#define _IMUSICLISTENER_H_

#include <math.h>
#include "projectMacros.h"
#include "Connectable.h"

#define NUMBER_OF_LINEVALUES (2)

struct ListenerData
{
	bool enabled;
	float threshold;
	float Effect;
	int lowerBound;
	int upperBound;
	float value[NUMBER_OF_LINEVALUES];
	int bandWidth;
	float fallOff; /* Zeit in sekunden, auf dem der threshold bleiben soll */
	bool clampf; /* clampen min / max begrenzen f�r end-ausgabe wert */
	float MINClampf;
	float MAXClampf;
};

typedef void(*ListenerFunc)(int,ListenerData*,IGObject*);

class IMusicListener
{
public:
	static const int NUMBER_OF_LISTENINGLINES = 5;
			IMusicListener(void);
	virtual ~IMusicListener(void);

			float listenTo(int,float *fft);
			float* listen(float *fft);
			ListenerData* GetLineData(int);
			void SetThreshold(int,float);
			void SetClambt(int,bool);
			void SetClambt(int,float,float);
			bool Enabled(int,BOOL=3);
			void SetLineBounds(int line,int lower,int upper,int width);
			void AddListenerCallback(int,ListenerFunc,IGObject*);
			
			
protected:
	virtual void applyEffect(int line,float lowValue,float highValue);
	ListenerData Line[NUMBER_OF_LISTENINGLINES];
	ListenerFunc callbacks[NUMBER_OF_LISTENINGLINES]; 
	IGObject*    clients[NUMBER_OF_LISTENINGLINES];
};


class CIMusicListener : public IMusicListener, public IConnectable
{
public:
	CIMusicListener(void){}
	virtual ~CIMusicListener(void){}

protected:

	//virtual void applyEffect(int line,float lowValue,float highValue)
	//{
	//	if(lowValue > (Line[line].threshold - highValue))
	//		Line[line].Effect += highValue * 50 * lowValue;
	//	else
	//		Line[line].Effect -= Line[line].fallOff;
	//}
};


class MusicListener : public CIMusicListener
{

};

#endif