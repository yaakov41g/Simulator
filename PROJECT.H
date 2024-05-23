#include <owl\owlpch.h>
#include <owl\applicat.h>
#include <owl\edit.h>
#include <owl\inputdia.h>
#include <owl\inputdia.rc>
#include <osl\geometry.h>
#include <owl\listbox.h>
#include <owl\chooseco.h>
#include <owl\combobox.h>
class TMyWindow;
//============================================================================
enum MoveResult {
	MR_MOVED,						// Bacteria moved
	MR_DIED,                   // Bacteria died
	MR_REPRODUCED,              // Bacteria reproduced
	MR_CANNOTMOVE              // Bacteria cannot move.
};
//============================================================================
enum BacteriaCells {
	CS_BACTERIA,					// Base class, bacteria
	CS_HUNTER,                 // Hunter class bacteria
	CS_RUNNER,                 // Runner class bacteria
};
//===========================================================================
class TPartic : public TPoint{    //a molecula of oil.

 public:
  TPartic(){}
  TPartic(int x,int y) : TPoint(x,y){}
  static TColor ParColor;         //color of oil(food).
};
//============================================================================
class Bacteria : public TRect{

	public:
		TColor Color;        			// Color of bacteria.
		int Energy;                 	// Energy level
      int PreEnergy;                // Pre energy level.
		int Age;                    	// Current age
		int MoveTable[8][2];				// Constant table of movements
		int PosX, PosY;               //a basic position of the bacteria.
		int Preleft,Pretop;           //preposition of bacteria
		TMyWindow *Parent;            //parent window of the bacteries.
		bool toEat;                   //if bacteria will eat food now .
      /*** Base class instance methods: */
		Bacteria(TMyWindow *parent,int e, int x, int y);
		Bacteria();

		void SetMoveTable();          // set the table directions around bacteria.
		virtual BacteriaCells Cells(void) = 0;	// Return the type of bacteria
		virtual ~Bacteria(void){}
		operator==(const Bacteria& other) const
		{return ToBool(other.PosX==PosX && other.PosY==PosY);}//(left,top...?)
		virtual MoveResult Move(void) = 0;// performs one move.
		TColor getColor() 					//return color of the bacteria.
			{ return Color; }
};
//============================================================================
class Hunter : public Bacteria {
	public:

		int molx,moly,preMolx,preMoly;  //position of finded molecula.
		bool Found;                     //if bacteria found a molecula.

		Hunter(TMyWindow *parent,int e,int x,int y);//Initial creation constructor
		Hunter(const Hunter & H){}        // Copy constructor
		Hunter();

		static int Sense;  			// distance from wich a molecula is sensed.
		static int ReproAge;     	// Age at which a bacteria can reproduce

		virtual ~Hunter(void){}           // Destructor
		virtual BacteriaCells Cells(void) // Return the type of bacteria
			{ return CS_HUNTER; }
		void Find();            			 //seeking a molecula.
		MoveResult Move(void);				 // Ask bacteria to make one move
};
//============================================================================
class Runner : public Bacteria {

	 public:
	 /*** Instance variables: */
							  
	 int MoveCount;                  // Number of moves since last turn
	 int Direction;                  // Currently selected direction
    int TurnGene;                   // Number of moves till turn.

	 static const int Runner::MaxTurnGene;// Maximum movement gene value
	 static const int Runner::MinTurnGene;// Minimum movement gene value

	 Runner(TMyWindow *parent,int e,int x,int y);// Initial creation constructor
	 Runner(const Runner & R);        // Copy constructor
	 virtual ~Runner(void){}			 // Destructor
	 Runner();
	 virtual BacteriaCells Cells(void)// Return type of bacteria
		{ return CS_RUNNER; }
	 void Turn();
	 MoveResult Move(void);  // Ask Runner to move
};
//=========================================================================
struct BacteriaDataStruct{          //transfer struct for BacteriaDataDialog.
  BacteriaDataStruct();
  char HunterDiMovCostEdit [15];
  char HunterOrMovCostEdit [15];
  char HunterFoodValue		[15];
  char HunterMinRepEnerEdit[15];
  char HunterMaxRepEnerEdit[15];
  char HunterRepAgeEdit		[15];
  char HunterSenseRange		[15];
  char RunnerDiMovCostEdit	[15];
  char RunnerOrMovCostEdit	[15];
  char RunnerFoodValue		[15];
  char RunnerMinRepEnerEdit[15];
  char RunnerMaxRepEnerEdit[15];
  char HunterQuantityEdit	[15];
  char RunnerQuantityEdit	[15];
  char FoodQuantityEdit		[15];
};
//=============================================================================
struct Info{                    //transfer struct for BacteriaInfoDialog.
  Info();
  char HunOrigQuantity[17];     //Hunter Original Quantity.
  char RunOrigQuantity[17];
  char FooOrigQuantity[17];
  char HunPresQuantity[17];     //Hunter Present Quantity.
  char RunPresQuantity[17];
  char FooPresQuantity[17];
  char HunNumDE		 [17];     //number of died hunters.
  char RunNumDE		 [17];
  char FooNumDE		 [17];     //number of eaten food particles.
  char HunPerDE		 [17];     //% of died hunters.
  char RunPerDE		 [17];
  char FooPerDE		 [17];
  char HunNumReprod	 [17];     //number of reproducted hunters.
  char RunNumReprod	 [17];
  char HunPerReprod	 [17];
  char RunPerReprod	 [17];
  char HunNumFooEat	 [17];     //number of food particles eatem by  hunters.
  char RunNumFooEat	 [17];
  char HunPerFooEat	 [17];
  char RunPerFooEat	 [17];
};
//=============================================================================
struct ElementSize{             //transfer struct for SizeDialog.
  ElementSize();
  TComboBoxData   ComboStainW;  //transferData container for oil stain's weight.
  TComboBoxData   ComboStainH;  //transferData container for oil stain's height.
  TComboBoxData   ComboBact;//transferData container for bacteria height&weight.
};
struct Rate{
  Rate(){strcpy(rate,"250");}
  char rate[20];
};
//=============================================================================
class BacteriaDataDialog : public TDialog{ //for changing bacteria traits.
  public:
	 BacteriaDataDialog(TWindow* parent, int resId, BacteriaDataStruct& bds);
};
//=============================================================================
class BacteriaInfoDialog : public TDialog{ //information about the
															// simulation state.
  public:
	 BacteriaInfoDialog(TWindow* parent, int resId, Info& bis);
};
//=============================================================================
class SizeDialog : public TDialog{ //for changing size of bacteria and/or stain.
  public:
	 SizeDialog(TWindow* parent, int resId,ElementSize& es);
};
//=============================================================================
class TMyInputDialog : public TDialog{
  public:
	 TMyInputDialog(TWindow* parent, int resId,Rate& rat);
};
//############################################################################
class TMyWindow : public TWindow {

  public:
	 TMyWindow(TWindow* parent = 0);
	 ~TMyWindow();
	 BYTE net[270][170];          //places net for particles.
	 int IsOccupied;   				//indictor to net place if it is occupied by
														//a bacteria or not.
	 int BacteriaSize;          	// Size of a Bacteria.
	 int Sleft,Stop,Sright,Sbottom;//size of the stain.
	 int PartCount;               //initial number of particles.
	 int Pcount;                  //present number of particles.
	 int HunterCount;             //number of hunters.
	 int RunnerCount;             //number of runners.
	 int ReproductedHunt;			//number of reproducting hunters.
	 int ReproductedRunn;			//number of reproducting runners.
	 MoveResult move;             //Result of movement.
	 int HCount;         // Current population of Bacteria
	 int Sense;    		// Distance at which food is sensed
	 int HDiagMoveCost; 	// Energy cost to move diagonally
	 int HOrthMoveCost; 	// Energy cost to move orthagonally
	 int HFoodValue;    	// Energy obtained from eating food
	 int ReproAge;     	// Age at which a bacteria can reproduce
	 int HMinRepEnergy;  // Minimum Energy required for reproduction
	 int HMaxRepEnergy;  // Maximum Energy required for reproduction
	 int HEatenFood;    	// number of particles that have been eaten.

	 int RCount;         // Current population of Bacteria
	 int RDiagMoveCost;  // Energy cost to move diagonally
	 int ROrthMoveCost;  // Energy cost to move orthagonally
	 int RFoodValue;     // Energy obtained from eating food
	 int RMinRepEnergy;  // Minimum Energy required for reproduction
	 int RMaxRepEnergy;  // Maximum Energy required for reproduction
	 int REatenFood;    	// number of particles that have been eaten.
	 TClientDC *dc;      //device context for drawing the bacteria.
	 TPen *Pw,*Ph,*Pr;   //pens for drawing the bacteria.
	 TBrush *Bw,*Bh,*Br; //brushes for drawing the bacteria.
	 int Time;           //sets the rate of timer.
	 char Caption[80];   //save the caption before it chanched.
	 bool IsActive;      //flag about a mdichild if is active.

	 void LifeCycle();          //the main fonction of the program.
	 void Touching(Bacteria*);  //don't let a bacteria to tread(לדרוך)on another.
	 void Erase(Bacteria*);     // erasing a bacteria.
	 Bacteria* Reproduce(Bacteria*);      //reproducting a bacteria.
	 void Eat(Bacteria* bac);     	     //eat food which is closed to bacteria.
	 void Draw(Bacteria* bac);			     // Draws itself on the screen
	 void BuildInstance();        	     //build an instance of simulation.
	 TArrayAsVector<TPartic> *Particle;   //points to container of particles.
	 TArrayAsVectorIterator<TPartic> *itr;
	 TArrayAsVector<Bacteria*> *Bact;
	 TArrayAsVectorIterator<Bacteria*> *Iter;//points to container of  bacteries.

	 BacteriaDataStruct BacteriaDataStruct;  //to fill resetsystemdialog.
	 Info InfoStruct;                        //to fill information dialog.
	 ElementSize SizeStruct;                 //to fill size dialog.
	 Rate Mseconds;                          //to fill rate dialog.
  protected:

	 TColor HunColor,RunColor;               //for choose.color in CmColor().
	 void EvLButtonDown(UINT,TPoint&);
	 void EvMDIActivate(HWND hWndActivated, HWND hWndDeactivated);
	 virtual void SetupWindow();
	 virtual BOOL CanClose ();
	 void CmColor(UINT);
	 void CmStart();                         //starting the lifecycle.
	 void CmPause();                         //stoping the lifecycle.
	 void CmRestart();                       //restarting the lifecycle.
	 void CmStateinfo();                     //shows information dialog.
	 void CmBarBoth  () ;                    //not in use.
	 void CmBarHoriz () ;                    //not in use.
	 void CmBarVert  () ;                    //not in use.
	 void CmBarNone  () ;                    //not in use.
	 void CmSize();                          //executes size dialog.
	 void CmRate();                          //executes the rate dialog.
	 void CmHelp();                          //executes application's help.
	 void EvTimer	(UINT TimerId);           
	 virtual void Paint(TDC&, bool /*erase*/, TRect&);
	 DECLARE_RESPONSE_TABLE(TMyWindow);
};
//============================================================================
//class TMyMDIClient : public TMDIClient

