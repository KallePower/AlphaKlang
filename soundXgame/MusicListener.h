#ifndef _MUSIKLISTENER_H_
#define _MUSIKLISTENER_H_

#include "imusicinteractor.h"

class MusicListener :
	public AMusicInteractor,
	public IConnectable
{

public:
	MusicListener(void){}
	virtual ~MusicListener(void){}

protected:
	virtual float* GettFFTData(void)
		{return (float*)AUDIO->GetBackgroundAudioFFT(FFT_SIZE::Small);}
	virtual void MotivatorFunction(float Motivator,int number) 
		{Vector3 temp2,temp;
		//	printf("MusikListener: motivatornumber-%i: %f\n",number,Motivator);
			switch(number)
			{
			case 0:
				//printf("MusikListener: motivator0: %f\n",motivator[0]);
				this->Connection()->scale(motivator[0],(motivator[1]+1.5)*2.5f,(motivator[2]+1.5)*2.5f);
				break;
			case 1:
				temp = this->Connection()->getTransform()->rotation;

				temp.x += Motivator;
				temp.y -= Motivator;
				temp.z += Motivator;
				this->Connection()->rotate(temp);
				
				break;
			case 2:
				temp = this->Connection()->getTransform()->movement;
				temp.x *= Motivator/5;
				temp.y *= -Motivator/3;
				temp.z *= Motivator/7;
				this->Connection()->move(temp);
				
				break;
			}
		}
	virtual void MotivatorFunction(float Motivator[]){}
};

#endif