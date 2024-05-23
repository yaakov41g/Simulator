#include "project.h"
#include "project.rh"
#include <math.h>
#include <owl\dc.h>
#include <owl\gdiobjec.h>
#include <owl\mdi.h>
//#############################################################################
Bacteria::Bacteria(TMyWindow *parent,int e,int x,int y):TRect(x,y,x+1,y+1)
{
	Parent=parent;
	PreEnergy=Energy=e;
	toEat=false;
}
//-----------------------------------------------------------------------------
Bacteria::Bacteria()
{
}
//-----------------------------------------------------------------------------
void Bacteria::SetMoveTable()
{
  MoveTable[0][0]=left-1;
  MoveTable[0][1]=top-1;
  MoveTable[1][0]=left;
  MoveTable[1][1]=top-1;
  MoveTable[2][0]=left+1;
  MoveTable[2][1]=top-1;
  MoveTable[3][0]=left+1;
  MoveTable[3][1]=top;
  MoveTable[4][0]=left+1;
  MoveTable[4][1]=top+1;
  MoveTable[5][0]=left;
  MoveTable[5][1]=top+1;
  MoveTable[6][0]=left-1;
  MoveTable[6][1]=top+1;
  MoveTable[7][0]=left-1/*Parent->BacteriaSize*/;
  MoveTable[7][1]=top;
}
//=============================================================================
int Hunter::Sense=10;  			// distance from wich a molecula is sensed.
int Hunter::ReproAge=12;     	// Age at which a bacteria can reproduce
//=============================================================================
Hunter::Hunter(TMyWindow *parent, int E,int X, int Y):Bacteria(parent,E,X,Y)
{
  PosX=X;
  PosY=Y;
  Found=false;
  left=PosX;
  top=PosY;
  SetMoveTable();
  Preleft=X;
  Pretop=Y;
  molx=moly=0;
  preMolx=preMoly=0;  //!
  Age=0;
  Color=TColor::LtRed;
}
//-----------------------------------------------------------------------------
Hunter::Hunter()//:bacteria    //!
{
                        
}
//---------------------------------------------------------------------------
void Hunter::Find()
{
	bool Break=false;
	int x,y,i;
	for (i=1;i<Sense;i+=1){
	  for ( x=PosX-i, y=PosY-i;x<=PosX+i;x+=1)
		if ((x>=0)&&(y>=0))
		 if ((Parent->net[x][y]>Parent->IsOccupied)||(Parent->net[x][y]<
								Parent->IsOccupied)&&(Parent->net[x][y]>0)){
			Break=true;
			break;
			}
	  if (!Break)
	  for (x=PosX+i,y=PosY-i;y<=PosY+i;y+=1)
		if ((x>=0)&&(y>=0))
		 if ((Parent->net[x][y]>Parent->IsOccupied)||(Parent->net[x][y]<
								Parent->IsOccupied)&&(Parent->net[x][y]>0)){
			Break=true;
			break;
			}
	  if (!Break)
	  for (x=PosX-i,y=PosY+i;x<=PosX+i;x+=1)
		if ((x>=0)&&(y>=0))
		 if ((Parent->net[x][y]>Parent->IsOccupied)||(Parent->net[x][y]<
								Parent->IsOccupied)&&(Parent->net[x][y]>0)){
			Break=true;
			break;
			}
	  if (!Break)
	  for (x=PosX-i,y=PosY-i+1;y<=PosY+i;y+=1)
      if ((x>=0)&&(y>=0))
		 if ((Parent->net[x][y]>Parent->IsOccupied)||(Parent->net[x][y]<
		 						Parent->IsOccupied)&&(Parent->net[x][y]>0)){
			Break=true;
			break;
			}
	  if (Break){
		 if ((x>=0)&&(y>=0)){//care about deviation of the array 'net'.
			Found=true;
			molx=x;
			moly=y;
//If found another food (because of the previous was eaten by another bacteria).
			if ((x!=preMolx)&&(y!=preMoly)){
			  PosX=left;
			  PosY=top;
			  preMolx=x;
			  preMoly=y;
			  }//if x!=
			}  //if x>=
		 else
			Found=false;
		 break; //from for'1'
		 } //if break
	  }//for'1'
}
//---------------------------------------------------------------------------
MoveResult Hunter::Move(void)
{
	double Distx=abs(molx-(PosX))+0.00000001;//...0001 is to avoid situation of
	double Disty=abs(moly-(PosY));                            //division by zero.
	double TangPosXY=Disty/Distx;    //tanges of (PosX,PosY) to the molecula.
//--  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --
 //Selecting direction.
	int j=0;
	if (!Found){ //if the bacteria did not find a particle within the distance.
	 int rnd=random(8);
	  Preleft=left;
	  Pretop=top;
	  PosX=left=MoveTable[rnd][0];
	  PosY=top=MoveTable[rnd][1];
	  }
	else{
	 for (j=0;j<8;j++)           //if the molecula is neighbour to left&top.
	  if (TRect(MoveTable[j][0],MoveTable[j][1],MoveTable[j][0]+
				Parent->BacteriaSize,MoveTable[j][1]+Parent->BacteriaSize)==TRect(
				  molx,moly,molx+Parent->BacteriaSize,moly+Parent->BacteriaSize))//?
		 break;
	 if (j<=7){
		Preleft=left;
		Pretop=top;
		left=PosX=MoveTable[j][0];
		top=PosY=MoveTable[j][1];
		Found=false;
      toEat=true;
		}
	 else{    	//if the molecula is not near left&top.
	  Preleft=left;
	  Pretop=top;
	  if ((molx<PosX)&&(moly<=PosY))// __|//The molecula is above and left to the bacteria.
		 if (Distx<Disty){
			if (fabs(abs(top-1-PosY)/(abs(left-PosX)+0.00000001)-TangPosXY)<fabs(abs(top-1-PosY)/(abs(left-1-PosX)+0.00000001)-TangPosXY))
			  top-=1;
			else{
			  left-=1;
			  top-=1;
			  }
			}
		 else  //if distx is not < than disty.
			if (fabs(abs(top-PosY)/(abs(left-1-PosX)+0.00000001)-TangPosXY)<fabs(abs(top-1-PosY)/(abs(left-1-PosX)+0.00000001)-TangPosXY))
			  left-=1;
			else{
			  top-=1;
			  left-=1;
			  }
	  else   //if not (molx<PosX)&&(moly<PosY).
		 if ((molx>=PosX)&&(moly<=PosY))// |__    //notice not to create overlapping by '<='.
			if (Distx<Disty)
			  if (fabs(abs(top-1-PosY)/(abs(left-PosX)+0.00000001)-TangPosXY)<fabs(abs(top-1-PosY)/(abs(left+1-PosX)+0.00000001)-TangPosXY))
				 top-=1;
			  else{
				 left+=1;
				 top-=1;
				 }
			else  //if distx is not < than disty.
			  if (fabs(abs(top-PosY)/(abs(left+1-PosX)+0.00000001)-TangPosXY)<fabs(abs(top-1-PosY)/(abs(left+1-PosX)+0.00000001)-TangPosXY))
				 left+=1;
			  else{
				 top-=1;
				 left+=1;
				 }  //   __
		 else    //if |
			if ((molx>PosX)&&(moly>PosY))
									 // __
			  if (Distx<Disty)// |.
				 if (fabs(abs(top+1-PosY)/(abs(left-PosX)+0.00000001)-TangPosXY)<fabs(abs(top+1-PosY)/(abs(left+1-PosX)+0.00000001)-TangPosXY))
					top+=1;
				 else{
					left+=1;
					top+=1;
					}                                    //___
			  else  //if distx is not < than disty.   //|  .
				 if (fabs(abs(top-PosY)/(abs(left+1-PosX)+0.00000001)-TangPosXY)<fabs(abs(top+1-PosY)/(abs(left+1-PosX)+0.00000001)-TangPosXY))
					left+=1;
				 else{
					top+=1;
					left+=1;
					}  //     __
			else    // if     |
			  if ((molx<=PosX)&&(moly>PosY))
										 //__
				 if (Distx<Disty)//   .|
					if (fabs(abs(top+1-PosY)/(abs(left-PosX)+0.00000001)-TangPosXY)<fabs(abs(top+1-PosY)/(abs(left-1-PosX)+0.00000001)-TangPosXY))
					  top+=1;
					else{
					  left-=1;
					  top+=1;
					  }                                    //__
				 else  //if distx is not < than disty.   //.  |
					if (fabs(abs(top-PosY)/(abs(left-1-PosX)+0.00000001)-TangPosXY)<fabs(abs(top+1-PosY)/(abs(left-1-PosX)+0.00000001)-TangPosXY))
					  left-=1;
					else{
					  top+=1;
					  left-=1;
					  }
		 }
	  }//else(if !found)
	right=left+1;
	bottom=top+1;
 //updating the energy level and the age.
	PreEnergy=Energy;
	if ((left!=Preleft)&&(top!=Pretop))  //if the step was diagonal:
	  Energy-=Parent->HDiagMoveCost;
	else
	  Energy-=Parent->HOrthMoveCost;
	Age++;
	if (Energy<=0)
	  return MR_DIED;
	else
	  if ((Energy>random(Parent->HMaxRepEnergy-Parent->HMinRepEnergy)+
	  										Parent->HMinRepEnergy)&&(Age>=ReproAge))
		 return MR_REPRODUCED;
	  else
		 return MR_MOVED;
}
//============================================================================
const int Runner::MaxTurnGene=7;// Maximum movement gene value
const int Runner::MinTurnGene=1;// Minimum movement gene value
//----------------------------------------------------------------------------
Runner::Runner(TMyWindow *parent, int e,int x,int y):Bacteria(parent,e,x,y)
{
  left=PosX=x;
  top=PosY=y;
  MoveCount=50;
  Preleft=x;
  Pretop=y;
  SetMoveTable();
  TurnGene=random(MaxTurnGene-MinTurnGene)+MinTurnGene;  //?
  Color=TColor::LtBlue;
}
//---------------------------------------------------------------------------
Runner::Runner()
{
}
//---------------------------------------------------------------------------
void Runner::Turn()
{
  MoveCount++;
  if (MoveCount>=TurnGene){
	 TurnGene=random(MaxTurnGene-MinTurnGene)+MinTurnGene;
	 MoveCount=0;
	 Direction=random(8);
	 }
}
//---------------------------------------------------------------------------
MoveResult Runner::Move(void)
{
	 Preleft=left;
	 Pretop=top;
	 left=PosX=MoveTable[Direction][0]; //Direction has updated in lifecycle.
	 top=PosY=MoveTable[Direction][1];
	 right=left+1;
	 bottom=top+1;
//updating the energy level.
  PreEnergy=Energy;
  if ((left!=Preleft)&&(top!=Pretop)) //if diagonal step.
	  Energy-=Parent->RDiagMoveCost;
	else                    // if orthagonal step.
	  Energy-=Parent->ROrthMoveCost;

  if (Energy<=0)
	  return MR_DIED;
  else
	 if ((Energy>random(Parent->RMaxRepEnergy-Parent->RMinRepEnergy)+
	 										Parent->RMinRepEnergy))
		return MR_REPRODUCED;
	 else
		return MR_MOVED;
}
//----------------------------------------------------------------------------
BacteriaDataStruct::BacteriaDataStruct()
{
	strcpy(HunterDiMovCostEdit,"7");
	strcpy(HunterOrMovCostEdit,"3");
	strcpy(HunterFoodValue,"8");
	strcpy(HunterMinRepEnerEdit,"400");
	strcpy(HunterMaxRepEnerEdit,"450");
	strcpy(HunterRepAgeEdit,"12");
   strcpy(HunterSenseRange,"10");
	strcpy(RunnerDiMovCostEdit,"3");
	strcpy(RunnerOrMovCostEdit,"1");
	strcpy(RunnerFoodValue,"10");
	strcpy(RunnerMinRepEnerEdit,"500");
	strcpy(RunnerMaxRepEnerEdit,"550");
	strcpy(HunterQuantityEdit,"100");
	strcpy(RunnerQuantityEdit,"100");
	strcpy(FoodQuantityEdit,"2000");
}
//=============================================================================
Info::Info()
{
  HunOrigQuantity[0]=0;
  RunOrigQuantity[0]=0;
  FooOrigQuantity[0]=0;
  HunPresQuantity[0]=0;
  RunPresQuantity[0]=0;
  FooPresQuantity[0]=0;
  HunNumDE		  [0]=0;
  RunNumDE		  [0]=0;
  FooNumDE		  [0]=0;
  HunPerDE		  [0]=0;
  RunPerDE		  [0]=0;
  FooPerDE		  [0]=0;
  HunNumReprod	  [0]=0;
  RunNumReprod   [0]=0;
  HunPerReprod	  [0]=0;
  RunPerReprod	  [0]=0;
  HunNumFooEat   [0]=0;
  RunNumFooEat   [0]=0;
  HunPerFooEat   [0]=0;
  RunPerFooEat   [0]=0;
}
//=============================================================================
ElementSize::ElementSize()
{
  ComboStainW.SelectString("450");
  ComboStainW.AddString("250");
  ComboStainW.AddString("400");
  ComboStainW.AddString("600");
  ComboStainH.SelectString("240");
  ComboStainH.AddString("150");
  ComboStainH.AddString("250");
  ComboStainH.AddString("400");
  ComboBact.SelectString("3");
  ComboBact.AddString("3");
  ComboBact.AddString("5");
  ComboBact.AddString("8");
}
//=============================================================================
BacteriaDataDialog::BacteriaDataDialog(
	 TWindow* parent, int resId, BacteriaDataStruct& bds):TDialog(parent, resId)
{
  new TEdit(this, IDC_EDIT5, sizeof(bds.HunterDiMovCostEdit));
  new TEdit(this, IDC_EDIT6, sizeof(bds.HunterOrMovCostEdit));
  new TEdit(this, IDC_EDIT33,sizeof(bds.HunterFoodValue));
  new TEdit(this, IDC_EDIT7, sizeof(bds.HunterMinRepEnerEdit));
  new TEdit(this, IDC_EDIT8, sizeof(bds.HunterMaxRepEnerEdit));
  new TEdit(this, IDC_EDIT10, sizeof(bds.HunterRepAgeEdit));
  new TEdit(this, IDC_EDIT27, sizeof(bds.HunterSenseRange));
  new TEdit(this, IDC_EDIT1, sizeof(bds.RunnerDiMovCostEdit));
  new TEdit(this, IDC_EDIT2, sizeof(bds.RunnerOrMovCostEdit));
  new TEdit(this, IDC_EDIT30,sizeof(bds.RunnerFoodValue));
  new TEdit(this, IDC_EDIT3, sizeof(bds.RunnerMinRepEnerEdit));
  new TEdit(this, IDC_EDIT4, sizeof(bds.RunnerMaxRepEnerEdit));
  new TEdit(this, IDC_EDIT11, sizeof(bds.HunterQuantityEdit));
  new TEdit(this, IDC_EDIT12, sizeof(bds.RunnerQuantityEdit));
  new TEdit(this, IDC_EDIT13, sizeof(bds.FoodQuantityEdit));
  SetTransferBuffer(&bds);
}
//=============================================================================
BacteriaInfoDialog::BacteriaInfoDialog(TWindow* parent, int resId, Info& bis)
							:TDialog(parent, resId)
{
  new TEdit(this,IDC_EDIT9,sizeof(bis.HunOrigQuantity));
  new TEdit(this,IDC_EDIT14,sizeof(bis.RunOrigQuantity));
  new TEdit(this,IDC_EDIT15,sizeof(bis.FooOrigQuantity));
  new TEdit(this,IDC_EDIT16,sizeof(bis.HunPresQuantity));
  new TEdit(this,IDC_EDIT17,sizeof(bis.RunPresQuantity));
  new TEdit(this,IDC_EDIT18,sizeof(bis.FooPresQuantity));
  new TEdit(this,IDC_EDIT19,sizeof(bis.HunNumDE));
  new TEdit(this,IDC_EDIT20,sizeof(bis.RunNumDE));
  new TEdit(this,IDC_EDIT21,sizeof(bis.FooNumDE));
  new TEdit(this,IDC_EDIT22,sizeof(bis.HunPerDE));
  new TEdit(this,IDC_EDIT23,sizeof(bis.RunPerDE));
  new TEdit(this,IDC_EDIT24,sizeof(bis.FooPerDE));
  new TEdit(this,IDC_EDIT25,sizeof(bis.HunNumReprod));
  new TEdit(this,IDC_EDIT26,sizeof(bis.RunNumReprod));
  new TEdit(this,IDC_EDIT28,sizeof(bis.HunPerReprod));
  new TEdit(this,IDC_EDIT29,sizeof(bis.RunPerReprod));
  new TEdit(this,IDC_EDIT31,sizeof(bis.HunNumFooEat));
  new TEdit(this,IDC_EDIT32,sizeof(bis.RunNumFooEat));
  new TEdit(this,IDC_EDIT34,sizeof(bis.HunPerFooEat));
  new TEdit(this,IDC_EDIT35,sizeof(bis.RunPerFooEat));
  SetTransferBuffer(&bis);
}
//=============================================================================
SizeDialog::SizeDialog(TWindow* parent, int resId,ElementSize& es)
												 :TDialog(parent, resId)
{
  new TComboBox(this,IDC_COMBOBOX2,15);
  new TComboBox(this,IDC_COMBOBOX3,15);
  new TComboBox(this,IDC_COMBOBOX5,15);
  SetTransferBuffer(&es);
}
//=============================================================================
TMyInputDialog::TMyInputDialog(TWindow* parent, int resId,Rate& rat)
													:TDialog(parent, resId)
{
  new TEdit(this,IDC_EDIT36,20);
  SetTransferBuffer(&rat);
}
//=============================================================================





