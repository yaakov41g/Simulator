//============================================================================
//           
// OWL Workshop - (C) Copyright 1994, by Sivan Computers Ltd.
//
// Program example: prog9b.cpp ;"the Final Project".
//
//============================================================================
#include <owl\mdi.h>
#include <owl\mdichild.h>
#include <owl\static.h>
#include <owl\messageb.h>
#include <owl\controlb.h>
#include <owl\buttonga.h>
#include <owl\decmdifr.h>
#include <stdio.h>
#include <math.h>
#include "project.rh"
#include "project.h"
#define c_APP_HEADER "Project Of Simulation "

BOOL  ExpressClose 	 = FALSE;
int   NumMDIChild 	 = 0;
int   HighMDIindex 	 = 0;

TMessageBar* Mb;
TControlBar* Cb;
//----------------------------------------------------------------------------
DEFINE_RESPONSE_TABLE1(TMyWindow, TWindow)
  EV_WM_LBUTTONDOWN,
  EV_WM_TIMER,
  EV_WM_MDIACTIVATE,
  EV_COMMAND(CM_FILEEXIT,CmExit),
  EV_COMMAND(CM__EXIT,CmExit),
  EV_COMMAND(CM_ECOSYSTEMSTART,CmStart),
  EV_COMMAND(CM_START,CmStart),
  EV_COMMAND(CM_ECOSTSTEMPAUSE,CmPause),
  EV_COMMAND(CM_PAUSE,CmPause),
  EV_COMMAND(CM_ECOSTSTEMRESETSYSTEM,CmRestart),
  EV_COMMAND(CM_ECOSTSTEMSTATE_INFO,CmStateinfo),
  //EV_COMMAND(CM_BAR_BOTH,  CmBarBoth ),
  //EV_COMMAND(CM_BAR_HORIZ, CmBarHoriz ),
  //EV_COMMAND(CM_BAR_VERT,  CmBarVert ),
  //EV_COMMAND(CM_BAR_NONE,  CmBarNone ),
  EV_COMMAND(CM_SIZE,CmSize),
  EV_COMMAND(CM__SIZE,CmSize),
  EV_COMMAND(CM_RATE,		 CmRate),
  EV_COMMAND(CM_RAT,		 CmRate),
  EV_COMMAND(CM_HELP,       CmHelp),
  EV_COMMAND(CM__HELP,       CmHelp),
  EV_COMMAND_AND_ID(CM__COLOR,CmColor),
  EV_COMMAND_AND_ID(CM_COLHUNTER,CmColor),
  EV_COMMAND_AND_ID(CM_COLRUNNER,CmColor),
  EV_COMMAND_AND_ID(CM_COLFOOD,CmColor),
END_RESPONSE_TABLE;

TColor TPartic::ParColor=TColor::Black; //color of the particles.

//----------------------------------------------------------------------------
TMyWindow::TMyWindow(TWindow* Parent) : TWindow(Parent)
{
	Bact=NULL;
	Iter=NULL;
	Particle=new TArrayAsVector<TPartic> (1000,0,500);
	itr=new TArrayAsVectorIterator<TPartic> (*Particle);
	Bact=new TArrayAsVector<Bacteria*> (100,0,100);
	Iter=new TArrayAsVectorIterator<Bacteria*> (*Bact);
	for (int i=0;i<270;i++)    //initializing the net array.
	  for (int j=0;j<170;j++){
		 net[i][j]=0;
		 }
	BacteriaSize=3;
	IsOccupied=99;             //a large number to sign net[x][y].
	Sleft=10;                  //left border of stain.
	Sright=450;                //right border of stain.
	Stop=18;                   //top border of stain.
	Sbottom=210;               //bottom border of stain.
	HunterCount=100;
	RunnerCount=100;
	PartCount=Pcount=2000;
	HunColor=TColor::LtRed;    //color of that window's hunter.
	RunColor=TColor::LtBlue;
	ReproductedHunt=0;         //number of hunters that reproduced.
	ReproductedRunn=0;
	Pw=new TPen(TColor::White,0);
	Ph=new TPen(HunColor,0);
	Pr=new TPen(RunColor,0);
	Bw=new TBrush(TColor::White);
	Bh=new TBrush(HunColor);
	Br=new TBrush(RunColor);
	Time=500;       //miliseconds.
	randomize();
}
//----------------------------------------------------------------------------
TMyWindow::~TMyWindow()
{
  if (Bact)
	 delete Bact;
  if (Iter)
	 delete Iter;
  delete Particle;
  KillTimer(1);
  delete Pw,Ph,Pr,Bw,Bh,Br;
}
//----------------------------------------------------------------------------
BOOL TMyWindow::CanClose ()
{
	if (ExpressClose == TRUE) {
		NumMDIChild --;
		return TRUE;
	}
	if (MessageBox ("close this MDI window?", c_APP_HEADER,
						 MB_YESNO | MB_ICONQUESTION) == IDYES) {
		NumMDIChild --;
		return TRUE;
	}
	return FALSE;
}
//----------------------------------------------------------------------------
void TMyWindow::EvLButtonDown(UINT,TPoint& /*point*/)
{
}
//----------------------------------------------------------------------------
void TMyWindow::EvMDIActivate(HWND hWndActivated, HWND /*hWndDeactivated*/)
{
  char txt[150];
  if(Parent->HWindow==hWndActivated)
	 IsActive=true;
  else
	 IsActive=false;
  if (IsActive){
	 wsprintf(txt,"    HUNTERS : %d.    RUNNERS : %d.    PARTICLES : %d.",HCount,RCount,Pcount);
	 Mb->SetText(txt);
    }
}
//----------------------------------------------------------------------------
void TMyWindow::LifeCycle()
{
  dc=new TClientDC(*this);
  Iter->Restart();			//In order to iterate from start.
  int i=0;
  while (i<Bact->GetItemsInContainer()){
		Runner* Run=(Runner*)Iter->Current();   //casting.
		Hunter* Hun=(Hunter*)Iter->Current();   //casting.
		if (Iter->Current()->Cells()==CS_RUNNER)
							 Run->Turn();    //determinning  the direction for runner.
		if (Iter->Current()->Cells()==CS_HUNTER)
  //if particle was not found or it was eaten by another bacteria.
		  if ((!Hun->Found)||
					(net[Hun->molx][Hun->moly]<=IsOccupied))
							 Hun->Find();  //finding the nearest particle of the oil.
					  move=Iter->Current()->Move();
				 Touching(Iter->Current()); //checking if the bacteria
														//moving did not tread on another.
 switch             ( move ){
	case MR_DIED      :Erase(Iter->Current());break;
	case MR_REPRODUCED:Bact->Add(Reproduce(Iter->Current()));
	case MR_MOVED		:{
							 if (Iter->Current()->toEat)
							 Eat(Iter->Current());
							 Draw(Iter->Current());
							 }//case
	}//switch
	 i++;
	 //if(i<Bact->GetItemsInContainer())
		(*Iter)++;
  }//while
  if (Bact->GetItemsInContainer()>10000){
	 KillTimer(1);
	 Parent->SetCaption("Over than 10000 bacteries !");
	 }
  else
	 if (Pcount<=0){
		KillTimer(1);
		Parent->SetCaption("The whole stain was cleaned !");
		}
	 else
		if ((HCount<=0)&&(RCount<=0)){
		  KillTimer(1);
		  Parent->SetCaption("All the bacteries were disappeared!");
		  }
  delete dc;
}
//----------------------------------------------------------------------------
void TMyWindow::Touching(Bacteria* Bac)
{
  if (net[Bac->left][Bac->top]>=IsOccupied){
	 int direc;
	 for (direc=0;direc<8;direc++)
		if (net[Bac->MoveTable[direc][0]][Bac->MoveTable[direc][1]]<IsOccupied)
		  break;
	 if (direc>=8){  //if the bacteria is sorrownded from all sides by others.
		Bac->left=Bac->PosX=Bac->Preleft;
		Bac->top=Bac->PosY=Bac->Pretop;
		Bac->PreEnergy=Bac->Energy;
		move=MR_CANNOTMOVE;
 		}
	 else{
		Bac->left=Bac->PosX=Bac->MoveTable[direc][0];
		Bac->top=Bac->PosY=Bac->MoveTable[direc][1];
		}
	 if (Bac->Cells()==CS_HUNTER){
		Hunter* Hun=(Hunter*)Bac; //casting.
		Hun->Found=false;        //in order to make new looking for a particle.
		}
      Bac->toEat=true;
	 }  //if IsOccupied
  Bac->right=Bac->left+1;
  Bac->bottom=Bac->top+1;
//if the bacteria is out of the stain frame.
  if (!TRect(Sleft-BacteriaSize,Stop-BacteriaSize,Sright+BacteriaSize,
		Sbottom+BacteriaSize).Contains(TRect(Bac->left*BacteriaSize,Bac->top*
		BacteriaSize,Bac->right*BacteriaSize,Bac->bottom*BacteriaSize))){
	 Bac->left=Bac->PosX=Bac->Preleft;
	 Bac->top=Bac->PosY=Bac->Pretop;
	 Bac->right=Bac->left+1;
	 Bac->bottom=Bac->top+1;
	 Bac->PreEnergy=Bac->Energy;
	 move=MR_CANNOTMOVE;
	 }
  Bac->SetMoveTable();
  if (net[Bac->Preleft][Bac->Pretop]>=IsOccupied) //release the preposition.
	 net[Bac->Preleft][Bac->Pretop]-=IsOccupied;
  if (net[Bac->left][Bac->top]<IsOccupied)        //occupied the now position.
	 net[Bac->left][Bac->top]+=IsOccupied;
//if bacteria position contains particles.
  if ((net[Bac->left][Bac->top]>IsOccupied)||((net[Bac->left/
			BacteriaSize][Bac->top]<IsOccupied)&&(net[Bac->left][Bac->top]>0)))
    Bac->toEat=true;
}
//----------------------------------------------------------------------------
void TMyWindow::Erase(Bacteria* bac)
{
  if (bac->Cells()==CS_HUNTER){
	 HCount--;
	 }
  else
	 if (bac->Cells()==CS_RUNNER){
		RCount--;
		}
  dc->FillRect(bac->Preleft*BacteriaSize,bac->Pretop*BacteriaSize,bac->Preleft*
  		BacteriaSize+BacteriaSize,bac->Pretop*BacteriaSize+BacteriaSize,*Bw);
  if (net[bac->left][bac->top]>=IsOccupied) //!
	 net[bac->left][bac->top]-=IsOccupied;//!
  Bact->Detach(bac);
}
//----------------------------------------------------------------------------
Bacteria* TMyWindow::Reproduce(Bacteria* bac)
{
  Bacteria* newBac;
  bac->Energy/=2;
  if (bac->Cells()==CS_HUNTER){
	 newBac=new Hunter(this,bac->Energy,bac->left,bac->top);
	 ReproductedHunt++;
	 HCount++;
	 }
  else
	if (bac->Cells()==CS_RUNNER){
	 newBac=new Runner(this,bac->Energy,bac->left,bac->top);
	 ReproductedRunn++;
	 RCount++;
	 }
  newBac->Color=bac->Color;
  Eat(newBac);           //!
  return newBac;
}
//----------------------------------------------------------------------------
void TMyWindow::Eat(Bacteria* bac)
{
  int pix;
  pix=net[bac->left][bac->top];
  if (pix>=IsOccupied)
	 pix-=IsOccupied;
  if (pix>0) {
	 net[bac->left][bac->top]=IsOccupied;//allways!
	 Pcount-=pix;
	 if (bac->Cells()==CS_RUNNER){
		bac->Energy+=RFoodValue*pix;
		REatenFood+=pix;  // number of particles that have eaten.
		}
	 if (bac->Cells()==CS_HUNTER){
		bac->Energy+=HFoodValue*pix;
		HEatenFood+=pix;  // number of particles that have eaten.
		}
	 }
  bac->toEat=false;
}
//----------------------------------------------------------------------------
void TMyWindow::Draw(Bacteria* bac)
{
  int l=bac->Preleft*BacteriaSize;
  int t=bac->Pretop*BacteriaSize;
  dc->FillRect(l,t,l+BacteriaSize,t+BacteriaSize,*Bw);
  l=bac->left*BacteriaSize;
  t=bac->top*BacteriaSize;
  dc->FillRect(l,t,l+BacteriaSize,t+BacteriaSize,bac->Cells()==CS_RUNNER ? *Br : *Bh);
}
//----------------------------------------------------------------------------
void TMyWindow::EvTimer (UINT /*TimerId*/)
{
  char txt[150];
  LifeCycle();
  if (IsActive){
	 wsprintf(txt,"    HUNTERS : %d.    RUNNERS : %d.    PARTICLES : %d.",HCount,RCount,Pcount);
	 Mb->SetText(txt);
	 }
}
//----------------------------------------------------------------------------
void TMyWindow::CmColor(UINT Id)
{
  TChooseColorDialog::TData choose;
  static TColor    custColors[16] = {
	 0x010101L, 0x101010L, 0x202020L, 0x303030L,
	 0x404040L, 0x505050L, 0x606060L, 0x707070L,
	 0x808080L, 0x909090L, 0xA0A0A0L, 0xB0B0B0L,
	 0xC0C0C0L, 0xD0D0D0L, 0xE0E0E0L, 0xF0F0F0L
  };
  choose.Flags = CC_RGBINIT;
  choose.CustColors = custColors;
  int BactList=Bact->GetItemsInContainer();
  int i;
  switch (Id){
	 case CM_COLHUNTER,CM__COLOR:{choose.Color=HunColor;
		if (TChooseColorDialog(this, choose,0,"Hunter Color").Execute() == IDOK)
		  HunColor = choose.Color;
		delete Ph,Bh;
		Ph=new TPen(HunColor,0);
		Bh=new TBrush(HunColor);
		Iter->Restart();
		for (i=0;i<BactList;i++,(*Iter)++)
		  if (Iter->Current()->Cells()==CS_HUNTER)
			 Iter->Current()->Color = choose.Color;
		}break;
	 case CM_COLRUNNER:{choose.Color=RunColor;
		if (TChooseColorDialog(this, choose,0,"Runner Color").Execute() == IDOK)
		  RunColor = choose.Color;
		delete Pr,Br;
		Pr=new TPen(RunColor,0);
      Br=new TBrush(RunColor);
		Iter->Restart();
		for (i=0;i<BactList;i++,(*Iter)++)
		  if (Iter->Current()->Cells()==CS_RUNNER)
			 Iter->Current()->Color = choose.Color;
		}break;
	 case CM_COLFOOD  :{choose.Color=TPartic::ParColor;
      if (TChooseColorDialog(this, choose,0,"Food Color").Execute() == IDOK)
		  TPartic::ParColor = choose.Color;
		}
	 }
  Invalidate(false);
}
//----------------------------------------------------------------------------
void TMyWindow::Paint(TDC& Dc, bool /*erase*/, TRect&)
{
  char txt[150];
  dc=new TClientDC(*this);
  Dc.MoveTo(Sleft-1-BacteriaSize,Stop-1-BacteriaSize);
  Dc.LineTo(Sright+1+BacteriaSize,Stop-1-BacteriaSize);
  Dc.MoveTo(Sright+1+BacteriaSize,Stop-1-BacteriaSize);
  Dc.LineTo(Sright+1+BacteriaSize,Sbottom+1+BacteriaSize);
  Dc.MoveTo(Sright+1+BacteriaSize,Sbottom+1+BacteriaSize);
  Dc.LineTo(Sleft-1-BacteriaSize,Sbottom+1+BacteriaSize);
  Dc.MoveTo(Sleft-1-BacteriaSize,Sbottom+1+BacteriaSize);
  Dc.LineTo(Sleft-1-BacteriaSize,Stop-1-BacteriaSize);

  itr->Restart();
  for (int i=0;i<Particle->GetItemsInContainer();i++,(*itr)++)
	 if (net[itr->Current().x/BacteriaSize][itr->Current().y/BacteriaSize]>0)
		Dc.SetPixel(itr->Current(),TPartic::ParColor);
  int BactList=Bact->GetItemsInContainer();
  Iter->Restart();
  for (i=0;i<BactList;i++,(*Iter)++)
	 Draw(Iter->Current());
  if (IsActive){
	 wsprintf(txt,"    HUNTERS : %d.    RUNNERS : %d.    PARTICLES : %d.",HCount,RCount,Pcount);
	 Mb->SetText(txt);
    }
  delete dc;
}
//----------------------------------------------------------------------------
void TMyWindow::CmStart()
{
  SetTimer(1,Time);
}
//----------------------------------------------------------------------------
void TMyWindow::CmPause()
{
  KillTimer(1);
}
//---------------------------------------------------------------------------
void TMyWindow::CmRestart()
{
  if (BacteriaDataDialog(this,DIALOG_2,BacteriaDataStruct).Execute()==IDOK){
	 HDiagMoveCost=atoi(BacteriaDataStruct.HunterDiMovCostEdit);
	 HOrthMoveCost=atoi(BacteriaDataStruct.HunterOrMovCostEdit);
	 HFoodValue=   atoi(BacteriaDataStruct.HunterFoodValue);
	 Hunter::ReproAge=    atoi(BacteriaDataStruct.HunterRepAgeEdit);
	 HMinRepEnergy=atoi(BacteriaDataStruct.HunterMinRepEnerEdit);
	 HMaxRepEnergy=atoi(BacteriaDataStruct.HunterMaxRepEnerEdit);
	 Hunter::Sense=       atoi(BacteriaDataStruct.HunterSenseRange);
	 RDiagMoveCost=atoi(BacteriaDataStruct.RunnerDiMovCostEdit);
	 ROrthMoveCost=atoi(BacteriaDataStruct.RunnerOrMovCostEdit);
	 RFoodValue=   atoi(BacteriaDataStruct.RunnerFoodValue);
	 RMinRepEnergy=atoi(BacteriaDataStruct.RunnerMinRepEnerEdit);
	 RMaxRepEnergy=atoi(BacteriaDataStruct.RunnerMaxRepEnerEdit);
	 HunterCount=			 atoi(BacteriaDataStruct.HunterQuantityEdit);
	 RunnerCount=			 atoi(BacteriaDataStruct.RunnerQuantityEdit);
	 PartCount=				 atoi(BacteriaDataStruct.FoodQuantityEdit);
  	 BuildInstance();
	 Invalidate();
	 } //if
}
//----------------------------------------------------------------------------
void TMyWindow::BuildInstance()
{
  randomize();
  HCount=0;         //present number of hunters.
  RCount=0;
  Pcount=0;
  HEatenFood=0;    	// number of particles that have eaten.
  REatenFood=0;    	// number of particles that have eaten.
  ReproductedHunt=0;
  ReproductedRunn=0;
  int x,y;
  for (int i=0;i<270;i++)
	 for (int j=0;j<170;j++){
		net[i][j]=0;
		}
  Particle->Flush();        //!
  for (i=0;i<PartCount;i++){
	 Particle->Add(TPartic(x=random(Sright-Sleft)+Sleft,y=random(Sbottom-Stop)+
	 					Stop));
	 net[x/BacteriaSize][y/BacteriaSize]++;
	 Pcount++;
	 }
  Iter->Restart();
  Bact->Flush();
  for (i=0;i<HunterCount;i++,(*Iter)++){
	 Bact->Add(new Hunter(this,random(HMaxRepEnergy),
			x=(random(Sright-Sleft)+Sleft)/BacteriaSize,
			y=(random(Sbottom-Stop)+Stop)/BacteriaSize));
	 HCount++;
	 }
  for (i=0;i<RunnerCount;i++,(*Iter)++){
	 Bact->Add(new Runner(this,random(RMaxRepEnergy),
			x=(random(Sright-Sleft)+Sleft)/BacteriaSize,
			y=(random(Sbottom-Stop)+Stop)/BacteriaSize));
	 RCount++;
	 }
  Iter->Restart();
  for (i=0;i<Bact->GetItemsInContainer();i++,(*Iter)++){
	 Touching(Iter->Current());
	 Eat(Iter->Current());
	 }
  Parent->SetCaption(Caption);
}
//=============================================================================
void TMyWindow::CmStateinfo()
{
  char text[17];
  strcpy(InfoStruct.HunOrigQuantity,itoa(HunterCount,text,10));
  strcpy(InfoStruct.RunOrigQuantity,itoa(RunnerCount,text,10));
  strcpy(InfoStruct.FooOrigQuantity,itoa(PartCount,text,10));
  strcpy(InfoStruct.HunPresQuantity,itoa(HCount,text,10));
  strcpy(InfoStruct.RunPresQuantity,itoa(RCount,text,10));
  strcpy(InfoStruct.FooPresQuantity,itoa(Pcount,text,10));
  strcpy(InfoStruct.HunNumDE,itoa(HunterCount+ReproductedHunt-HCount,text,10));
  strcpy(InfoStruct.RunNumDE,itoa(RunnerCount+ReproductedRunn-RCount,text,10));
  strcpy(InfoStruct.FooNumDE,itoa(PartCount-Pcount,text,10));
  if(HunterCount>0)
  strcpy(InfoStruct.HunPerDE,itoa((int)((HunterCount+ReproductedHunt-
									HCount)/(float)HunterCount*100),text,10));
  else
  strcpy(InfoStruct.HunPerDE,itoa(0,text,10));
  if(RunnerCount)
  strcpy(InfoStruct.RunPerDE,itoa((int)((RunnerCount+ReproductedRunn-
									RCount)/(float)RunnerCount*100),text,10));
  else
  strcpy(InfoStruct.RunPerDE,itoa(0,text,10));
  if(PartCount)
  strcpy(InfoStruct.FooPerDE,itoa((int)((PartCount-Pcount)/(float)PartCount*100)
																			,text,10));
  else
  strcpy(InfoStruct.FooPerDE,itoa(0,text,10));
  strcpy(InfoStruct.HunNumReprod,itoa(ReproductedHunt,text,10));
  strcpy(InfoStruct.RunNumReprod,itoa(ReproductedRunn,text,10));
  if(HunterCount)
  strcpy(InfoStruct.HunPerReprod,itoa((int)(ReproductedHunt/(float)HunterCount*
																				100),text,10));
  else
  strcpy(InfoStruct.HunPerReprod,itoa(0,text,10));
  if(RunnerCount)
  strcpy(InfoStruct.RunPerReprod,itoa((int)(ReproductedRunn/(float)RunnerCount*
																				100),text,10));
  else
  strcpy(InfoStruct.RunPerReprod,itoa(0,text,10));
  strcpy(InfoStruct.HunNumFooEat,itoa(HEatenFood,text,10));
  strcpy(InfoStruct.RunNumFooEat,itoa(REatenFood,text,10));
  if(PartCount)
  strcpy(InfoStruct.HunPerFooEat,itoa((int)(HEatenFood/(float)PartCount*
																					100),text,10));
  else
  strcpy(InfoStruct.HunPerFooEat,itoa(0,text,10));
  if(PartCount)
  strcpy(InfoStruct.RunPerFooEat,itoa((int)(REatenFood/(float)PartCount*
																					100),text,10));
  else
  strcpy(InfoStruct.RunPerFooEat,itoa(0,text,10));
  BacteriaInfoDialog(this,DIALOG_3,InfoStruct).Execute();
}
//----------------------------------------------------------------------------
/*void TMyWindow::CmBarBoth() { ShowScrollBar(SB_BOTH, TRUE);  }
void TMyWindow::CmBarHoriz(){ ShowScrollBar(SB_BOTH, FALSE);
										ShowScrollBar(SB_HORZ, TRUE);  }
void TMyWindow::CmBarVert() { ShowScrollBar(SB_BOTH, FALSE);
										ShowScrollBar(SB_VERT, TRUE);  }
void TMyWindow::CmBarNone() { ShowScrollBar(SB_BOTH, FALSE); }*/
//----------------------------------------------------------------------------
void TMyWindow::CmSize()
{
  char size[16];
  if (SizeDialog(this,DIALOG_4,SizeStruct).Execute()==IDOK){
	 SizeStruct.ComboStainW.GetSelString(size,16);
	 if (atoi(size)<=Attr.W-11)
		{Sleft=(Attr.W-atoi(size))/2-5;Sright=atoi(size);}
	 else
		{Sleft=2*BacteriaSize;Sright=Attr.W;}
	 SizeStruct.ComboStainH.GetSelString(size,16);
	 if (atoi(size)<=Attr.H-11)
		{Stop=(Attr.H-atoi(size))/2-5;Sbottom=atoi(size);}
	 else
		{Stop=2*BacteriaSize;Sbottom=Attr.H;}
	 SizeStruct.ComboBact.GetSelString(size,16);
	 BacteriaSize=atoi(size);
	 if ((Sright/BacteriaSize>=270)||(Sbottom/BacteriaSize>=170)){
		MessageBox ("You must set weight and height so \nthat"
						" weight/(size of bacteria) will be less\n then 270,"
						"and height/(size of bacteria) will be less\n then 170. ",
						 c_APP_HEADER,MB_OK | MB_ICONEXCLAMATION);
		Sleft=(Attr.W-268*BacteriaSize)/2-10;        //!
		Sright=268*BacteriaSize;                     //!
		Stop=(Attr.H-168*BacteriaSize)/2-10;         //!
		Sbottom=168*BacteriaSize;                    //!
		}
	 if ((Sleft<2*BacteriaSize)||(Stop<2*BacteriaSize))
		  {Sleft=Stop=2*BacteriaSize;}
	 BuildInstance();
	 Invalidate();
  }
}
//----------------------------------------------------------------------------
void TMyWindow::CmRate()
{
  if (TMyInputDialog(this,DIALOG_5,Mseconds).Execute()==IDOK){
	 KillTimer(1);
	 SetTimer(1,atoi(Mseconds.rate));
	 Time=atoi(Mseconds.rate);
	 }
}
//----------------------------------------------------------------------------
void TMyWindow::CmHelp()
{
  ::WinHelp(*this,"h1.hlp",HELP_CONTENTS,HELP_CONTENTS);
}
//----------------------------------------------------------------------------
void TMyWindow::SetupWindow()
{
  TWindow::SetupWindow();
  HCount=0;          // Current population of Hunter.
  HDiagMoveCost=7; 	// Energy cost to move diagonally
  HOrthMoveCost=3; 	// Energy cost to move orthagonally
  HFoodValue=8;    	// Energy obtained from eating food
  HMinRepEnergy=400; // Energy required for reproduction
  HMaxRepEnergy=450; // Energy required for reproduction
  HEatenFood=0;    	// number of particles that have eaten.
  RCount=0;            // Current population of Runner.
  RDiagMoveCost=3; 	  // Energy cost to move diagonally
  ROrthMoveCost=1; 	  // Energy cost to move orthagonally
  RFoodValue=10;    	  // Energy obtained from eating food
  RMinRepEnergy=500;   // minimum reqqired energy for reproduction.
  RMaxRepEnergy=550;   // maximum reqqired energy for reproduction.
  REatenFood=0;    	  // number of particles that have eaten.
  strcpy(Caption,Parent->Title);
  BuildInstance();
}
//############################################################################

class TMyMDIClient : public TMDIClient {
	public:
		TMyMDIClient () : TMDIClient (){}

protected:

	virtual TMDIChild* InitChild ();
	virtual BOOL 		 CloseChildren () ;
			  void 	  	 CMQuit();
	virtual BOOL 		 CanClose();

	DECLARE_RESPONSE_TABLE (TMyMDIClient);
};
//----------------------------------------------------------------------------
DEFINE_RESPONSE_TABLE1 (TMyMDIClient, TMDIClient)
	EV_COMMAND(CM_NEW, CmCreateChild),
	EV_COMMAND (CM__EXIT,          CMQuit),
	EV_COMMAND (CM_FILEEXIT,          CMQuit),
END_RESPONSE_TABLE;
//----------------------------------------------------------------------------

TMDIChild* TMyMDIClient::InitChild ()
{
	TMDIChild* Frame;
	HighMDIindex++;
	char s[20];
	wsprintf(s,"%d",++NumMDIChild);
	Frame=new TMDIChild ( *this, s, new TMyWindow);
	return Frame;
}
//----------------------------------------------------------------------------
BOOL TMyMDIClient::CloseChildren ()
{
	BOOL result;
	ExpressClose = TRUE;
	result = TMDIClient::CloseChildren ();
	ExpressClose = FALSE;
	NumMDIChild = 0;
	HighMDIindex = 0;
	return result;
}
//----------------------------------------------------------------------------
void TMyMDIClient::CMQuit ()
{
	CloseWindow ();
}
//----------------------------------------------------------------------------
BOOL TMyMDIClient::CanClose ()
{
	return true;
}
//############################################################################

class TMyApp : public TApplication {
  public:
	 TMyApp() : TApplication() {nCmdShow=SW_SHOWMAXIMIZED;}
	 void InitMainWindow();
	 void InitInstance();
};
//----------------------------------------------------------------------------
void TMyApp::InitMainWindow()
{
  TDecoratedMDIFrame* frame=new TDecoratedMDIFrame(c_APP_HEADER, "MENU_2", *new TMyMDIClient);
  Mb=new TMessageBar(frame);
  TMargins marg;
  marg.Left=marg.Right=4;
  Mb->SetMargins(marg);
  frame->Insert(*Mb,TDecoratedMDIFrame::Bottom);
  Cb=new TControlBar(frame);
  Cb->Insert(*new TButtonGadget(CM_NEW,CM_NEW, TButtonGadget::Command));
  Cb->Insert(*new TSeparatorGadget);
  Cb->Insert(*new TButtonGadget(CM_START,CM_START, TButtonGadget::Command));
  Cb->Insert(*new TButtonGadget(CM_PAUSE,CM_PAUSE, TButtonGadget::Command));
  Cb->Insert(*new TButtonGadget(CM__EXIT,CM__EXIT, TButtonGadget::Command));
  Cb->Insert(*new TSeparatorGadget);
  Cb->Insert(*new TButtonGadget(CM__COLOR,CM__COLOR, TButtonGadget::Command));
  Cb->Insert(*new TButtonGadget(CM_RAT,CM_RAT, TButtonGadget::Command));
  Cb->Insert(*new TButtonGadget(CM__SIZE,CM__SIZE, TButtonGadget::Command));
  Cb->Insert(*new TSeparatorGadget);
  Cb->Insert(*new TButtonGadget(CM__HELP,CM__HELP, TButtonGadget::Command));
  frame->Insert(*Cb,TDecoratedMDIFrame::Top);
  EnableCtl3d();
  SetMainWindow(frame/*new TMDIFrame(c_APP_HEADER, "MENU_2", *new TMyMDIClient)*/);
}
//----------------------------------------------------------------------------
void TMyApp::InitInstance()
{
  TApplication::InitInstance();
}
//############################################################################

int OwlMain(int /*argc*/, char* /*argv*/ [])
{

  return TMyApp().Run();
}
