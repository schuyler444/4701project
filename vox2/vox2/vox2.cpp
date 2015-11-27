#include "stdafx.h"
#include "Voxelyze.h"
#include <iostream>
#include <stdio.h>
#include <ctime>

//spit out a randomworm
void randomWorm(CVX_Material* materials[], CVoxelyze* Vox){
	int index;
	for (int x = 0; x < 4; x = x+1){
		for(int y = 0; y < 8; y= y+1){
			
			index = rand() % 4;
			printf("index:%i\n", index);
			CVX_Voxel* Voxel1 = Vox->setVoxel(materials[index], x, y, 0);
			printf("VOXEL COUNT:%i\n", Vox->voxelCount());
		}
	}
}


int main(){
	srand (time (0));
	CVoxelyze Vx(0.005); //5mm voxels
	CVX_Material* pMaterial = Vx.addMaterial(1000000, 1000); //A material with stiffness E=1MPa and density 1000Kg/m^3

	//ASK MANNY ABOUT LINEAR VS BILINEAR MODELS
	CVX_Material* Active_pos = Vx.addMaterial(10000000, 1000000); // active expanding material
	Active_pos->setPoissonsRatio(.35);
	Active_pos->setCte(.01);
	Active_pos->setStaticFriction(.1);
	Active_pos->setKineticFriction(.05);

	CVX_Material* Active_neg = Vx.addMaterial(10000000, 1000000); // active compress material
	Active_neg->setPoissonsRatio(.35);
	Active_neg->setCte(.01);
	Active_neg->setStaticFriction(.1);
	Active_neg->setKineticFriction(.05);

	CVX_Material* Passive_soft = Vx.addMaterial(10000000, 1000000); // low youngs, 0 CTE
	Passive_soft->setPoissonsRatio(.35);
	Passive_soft->setCte(0);
	Passive_soft->setStaticFriction(.1);
	Passive_soft->setKineticFriction(.05);

	CVX_Material* Passive_hard = Vx.addMaterial(500000000, 1000000); // high youngs, 0 CTE
	Passive_hard->setPoissonsRatio(.35);
	Passive_hard->setCte(0);
	Passive_hard->setStaticFriction(.1);
	Passive_hard->setKineticFriction(.05);

	

	CVX_Material* materials[] = {Active_pos, Active_neg, Passive_soft, Passive_hard};
	
	randomWorm(materials,&Vx);
	printf("VOXEL COUNT:%i\n", Vx.voxelCount());
	/*

	CVX_Voxel* Voxel1 = Vx.setVoxel(pMaterial, 0, 0, 0); //Voxel at index x=0, y=0. z=0
	CVX_Voxel* Voxel2 = Vx.setVoxel(pMaterial, 1, 0, 0);
	CVX_Voxel* Voxel3 = Vx.setVoxel(pMaterial, 2, 0, 0); //Beam extends in the +X direction

	Voxel1->external()->setFixedAll(); //Fixes all 6 degrees of freedom with an external condition on Voxel 1
	Voxel3->external()->setForce(0, 0, -1); //pulls Voxel 3 downward with 1 Newton of force.

	Vx.saveJSON("test.vxl.json");
	*/

	Vx.enableFloor(true);
	Vx.setAmbientTemperature(39,true);
	Vx.setGravity(1);

	int temp = 39;
	int flag = 0;

	CVX_Voxel* voxer3 = Vx.voxel(0,3,0);
	printf("%f\n",(voxer3->position()).getY());

	for (int i=0; i<500000; i++) 
	{
		if (temp == 40){
			flag = 0;
		}
		if (temp == 0){
			flag = 1;
		}
		if (flag%2 == 0){
			temp--;
			Vx.setAmbientTemperature(temp,true);
			Vx.doTimeStep(.005); 

		} else{
			temp++;
			Vx.setAmbientTemperature(temp,true);
			Vx.doTimeStep(.005); 

		}

	}

	CVX_Voxel* voxer = Vx.voxel(0,3,0);

	printf("%f",(voxer->position()).getY());



	return 0;
}