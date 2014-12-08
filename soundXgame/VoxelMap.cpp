#include "projectMacros.h"
#include "projectGrafics.h"
#include "Utility.h"
#include "VoxelMap.h"

VoxelMap::VoxelMap(void) 
{
	numberOfVoxelers=0;

	loaded=false;
	
	voxelSize = 10.0f;
	scale=8.0f;
	//position.x=100;
	//position.y=100;
	size.x = 200;
	size.y = 200;
	BumpFactor=0.5;
}

VoxelMap::~VoxelMap()
{
//	 voxels = NULL;


}

void VoxelMap::ReLoade(char* filename)
{	
//	MainSizzes.x=1.0f;
//	MainSizzes.y=1.0f;



	if(filename == "FromDaCode")
	{
		mapWidth = BLUOB_BLUE.width;
		mapHeight = BLUOB_BLUE.height;
		numberOfVoxelers=BLUOB_BLUE.width*BLUOB_BLUE.height;

		MainSizzes.y = (float)size.x/mapWidth;
		size.y=mapHeight*MainSizzes.y;
		for(unsigned short I = 0;I<numberOfVoxelers*4;I+=4)
		{
			int i = I/4;
			int x = i%mapWidth;
			int y= i/mapWidth;
		
			for(int c=0;c<4;c++)
			{
				voxels[i].farb.Bytss[c] =  BLUOB_BLUE.pixel_data[I+c];
			}

			voxels[i].color = voxels[i].farb.s32;	
			voxels[i].size.ix=voxels[i].size.yps=MainSizzes.y;
			voxels[i].position.iX=(x*scale)+ *position.x;
			voxels[i].position.Yps=(y*scale)+ *position.y;
			voxels[i].MainDimensions = &MainSizzes;
			voxels[i].factor=127;
			if(x>0 && x<mapWidth && y>0 && y<mapHeight)
			{
				voxels[i].SetNeighbours(voxels[(i-1)-mapWidth].color ,voxels[i-(mapWidth-1)].color,voxels[i+1+mapWidth].color,voxels[i+(mapWidth-1)].color);
			}
		}
	}
	else
	{
		Loader loader(filename);

		numberOfVoxelers = loader.count();
		mapWidth = loader.width();
		mapHeight = loader.height();
		
		MainSizzes.y = (float)size.x/mapWidth;
		size.y=mapHeight*MainSizzes.y;

		for(int count=0;count<numberOfVoxelers;count++)
		{
			int x = count % mapWidth;
			int y = count / mapWidth;

			voxels[count].farb.Bytss[0]=loader.Pixel(x,y)->byte[0];
			voxels[count].farb.Bytss[1]=loader.Pixel(x,y)->byte[1];
			voxels[count].farb.Bytss[2]=loader.Pixel(x,y)->byte[2];
			voxels[count].farb.Bytss[3]=loader.Pixel(x,y)->byte[3];

			voxels[count].color = voxels[count].farb.s32;  ///????
			voxels[count].factor=127;
			voxels[count].size.ix=voxels[count].size.yps=MainSizzes.y;
			voxels[count].position.iX = (x*scale) + *position.x;
			voxels[count].position.Yps = (y*scale) + *position.y;
			voxels[count].MainDimensions = &MainSizzes;

			if(x>0 && x<(mapWidth-1) && y>0 && y<(mapHeight-1))
				voxels[count].SetNeighbours(voxels[(count-1)-mapWidth].color ,voxels[count-(mapWidth-1)].color,voxels[count+1+mapWidth].color,voxels[count+(mapWidth-1)].color);

		}
		//delete loader.data;
	}
}

void VoxelMap::Loade(const char* filename,void* buffer)
{	
	MainSizzes.x=1.0f;
	MainSizzes.y=1.0f;

	voxels = (Voxel*)buffer;

	if(filename == "FromDaCode")
	{
	mapWidth = BLUOB_BLUE.width;
	mapHeight = BLUOB_BLUE.height;
	numberOfVoxelers=BLUOB_BLUE.width*BLUOB_BLUE.height;

	MainSizzes.y = (float)size.x/mapWidth;
	size.y=mapHeight*MainSizzes.y;
	for(unsigned short I = 0;I<numberOfVoxelers*4;I+=4)
	{
		int i = I/4;
		int x = i%mapWidth;
		int y= i/mapWidth;
		
		for(int c=0;c<4;c++)
			voxels[i].farb.Bytss[c] =  BLUOB_BLUE.pixel_data[I+c];

		voxels[i].color = voxels[i].farb.s32;	
		voxels[i].size.ix=voxels[i].size.yps=MainSizzes.y;
		voxels[i].position.iX=(x*scale)+ *position.x;
		voxels[i].position.Yps=(y*scale)+ *position.y;
		voxels[i].MainDimensions = &MainSizzes;
		voxels[i].factor=127;
		if(x>0 && x<mapWidth && y>0 && y<mapHeight)
			voxels[i].SetNeighbours(voxels[(i-1)-mapWidth].color ,voxels[i-(mapWidth-1)].color,voxels[i+1+mapWidth].color,voxels[i+(mapWidth-1)].color);
		}
		loaded=true;
	}
	else
	{
		Loader* loader = new Loader(filename);

		numberOfVoxelers = loader->count();
		mapWidth = loader->width();
		mapHeight = loader->height();
		
		MainSizzes.y = (float)size.x/mapWidth;
		size.y=mapHeight*MainSizzes.y;

		for(int count=0;count<numberOfVoxelers;count++)
		{
			int x = count%mapWidth;
			int y = count/mapWidth;

			voxels[count].farb.Bytss[0]=loader->Pixel(x,y)->byte[0];
			voxels[count].farb.Bytss[1]=loader->Pixel(x,y)->byte[1];
			voxels[count].farb.Bytss[2]=loader->Pixel(x,y)->byte[2];
			voxels[count].farb.Bytss[3]=loader->Pixel(x,y)->byte[3];

			voxels[count].color = voxels[count].farb.s32;  ///????
			voxels[count].factor=127;
			voxels[count].size.ix=voxels[count].size.yps=MainSizzes.y;
			voxels[count].position.iX=(x*scale)+ *position.x;
			voxels[count].position.Yps=(y*scale)+ *position.y;
			voxels[count].MainDimensions = &MainSizzes;
			voxels[count].SetParentVoxmap(this);

			if(x>0 && x<(mapWidth-1) && y>0 && y<(mapHeight-1))
				voxels[count].SetNeighbours(voxels[(count-1)-mapWidth].color ,voxels[count-(mapWidth-1)].color,voxels[count+1+mapWidth].color,voxels[count+(mapWidth-1)].color);
			voxels[count].factorPointer = &BumpFactor;
		}
	
	}
}

void VoxelMap::LoadMap(char* filename,int channel)
{
	Loader* loader = new Loader(filename);
	for(int i=0;i<loader->count();i++)
		this->voxels[i].factor=loader->Pixel(i)->byte[channel];
	printf("Loaded bumpmap %s: %i voxels \n",filename,loader->count());

}

void VoxelMap::Draw(VectorPF position )
{
//	printf("VOXELPLANE: Bumpfactor: %f\n",this->BumpFactor);
	glBegin(GL_QUADS);

	for(int f=0;f<256;f++)
	{
		for(int i = 0;i<numberOfVoxelers;i++)
			if(voxels[i].factor==f)
				voxels[i].vDraw(position);
	}

	glEnd();

}

void VoxelMap::DrawBunt(VectorPF position)
{
	glBegin(GL_QUADS);
	for(int f=0;f<256;f++)
	{
		for(int i = 0;i<numberOfVoxelers;i++)
			if(voxels[i].factor==f)
				voxels[i].vDrawBunt(position);
	}	
	glEnd();
}

void VoxelMap::DrawByte(VectorPF position)
{
	glBegin(GL_QUADS);
	for(int f=0;f<256;f++)
	{
		for(int i = 0;i<numberOfVoxelers;i++)
			if(voxels[i].factor==f)
				voxels[i].vDrawByte(position);
	}	
	glEnd();
}

void* VoxelMap::SetVoxelerBuffer(Voxel* voxelss)
{
	numberOfVoxelers = DINGERBUFFERLENGTH;
	voxels = &voxelss[0];
	return &voxelss[0];
}

Voxel * VoxelMap::GetVoxel(int x,int y)
{
	return &voxels[y*mapWidth+x];
}

Voxel * VoxelMap::GetVoxel(int index)
{
	return &voxels[index];
}

