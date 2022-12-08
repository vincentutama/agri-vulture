/*
    Program Drone TA Vincent - Safety Subsystem
    (c) Vincent Utama, 2022
    All rights reserved.

    Functionality: 
    
    Version log:
    
    2022-12-08:
        v1.0.0  - Initial release
        
*/
// ==== DEFINES ===================================================================================

// ==== PINOUT ==================
#define posLed 3
#define navLed 6
#define acoLed 9
// ==== Debug and Test options ==================
//#define _DEBUG_
//#define _TEST_

//===== Debugging macros ========================
#ifdef _DEBUG_
#define SerialD Serial
#define _PM(a) SerialD.print(millis()); SerialD.print(": "); SerialD.println(a)
#define _PP(a) SerialD.print(a)
#define _PL(a) SerialD.println(a)
#define _PX(a) SerialD.println(a, HEX)
#else
#define _PM(a)
#define _PP(a)
#define _PL(a)
#define _PX(a)
#endif




// ==== INCLUDES ==================================================================================

// ==== Uncomment desired compile options =================================
// ----------------------------------------
// The following "defines" control library functionality at compile time,
// and should be used in the main sketch depending on the functionality required
// Should be defined BEFORE #include <TaskScheduler.h>  !!!
//
// #define _TASK_TIMECRITICAL       // Enable monitoring scheduling overruns
// #define _TASK_SLEEP_ON_IDLE_RUN  // Enable 1 ms SLEEP_IDLE powerdowns between runs if no callback methods were invoked during the pass
// #define _TASK_STATUS_REQUEST     // Compile with support for StatusRequest functionality - triggering tasks on status change events in addition to time only
// #define _TASK_WDT_IDS            // Compile with support for wdt control points and task ids
// #define _TASK_LTS_POINTER        // Compile with support for local task storage pointer
// #define _TASK_PRIORITY           // Support for layered scheduling priority
// #define _TASK_MICRO_RES          // Support for microsecond resolution
// #define _TASK_STD_FUNCTION       // Support for std::function (ESP8266 ONLY)
// #define _TASK_DEBUG              // Make all methods and variables public for debug purposes
// #define _TASK_INLINE             // Make all methods "inline" - needed to support some multi-tab, multi-file implementations
// #define _TASK_TIMEOUT            // Support for overall task timeout
// #define _TASK_OO_CALLBACKS       // Support for callbacks via inheritance
// #define _TASK_EXPOSE_CHAIN       // Methods to access tasks in the task chain
// #define _TASK_SCHEDULING_OPTIONS // Support for multiple scheduling options
// #define _TASK_DEFINE_MILLIS      // Force forward declaration of millis() and micros() "C" style
// #define _TASK_EXTERNAL_TIME      // Custom millis() and micros() methods
// #define _TASK_THREAD_SAFE        // Enable additional checking for thread safety
// #define _TASK_SELF_DESTRUCT      // Enable tasks to "self-destruct" after disable

#include <TaskScheduler.h>



// ==== GLOBALS ===================================================================================
int navBright = 0;
bool navPlus = true;

int posCount = 0;
bool posState = LOW;
unsigned int posCnum = 15;
// ==== Scheduler ==============================
Scheduler ts;

void navigationLights();
void positionLights();

// ==== Scheduling defines (cheat sheet) =====================
/*
  TASK_MILLISECOND  - one millisecond in millisecond/microseconds
  TASK_SECOND       - one second in millisecond/microseconds
  TASK_MINUTE       - one minute in millisecond/microseconds
  TASK_HOUR         - one hour in millisecond/microseconds
  TASK_IMMEDIATE    - schedule task to runn as soon as possible
  TASK_FOREVER      - run task indefinitely
  TASK_ONCE         - run task once
  TASK_NOTIMEOUT    - set timeout interval to No Timeout
  
  TASK_SCHEDULE     - schedule is a priority, with "catch up" (default)
  TASK_SCHEDULE_NC  - schedule is a priority, without "catch up"
  TASK_INTERVAL     - interval is a priority, without "catch up"
  
  TASK_SR_OK        - status request triggered with an OK code (all good)
  TASK_SR_ERROR     - status request triggered with an ERROR code
  TASK_SR_CANCEL    - status request was cancelled
  TASK_SR_ABORT     - status request was aborted
  TASK_SR_TIMEOUT   - status request timed out
*/

// ==== Task definitions ========================
Task navlight (15 * TASK_MILLISECOND, TASK_FOREVER, &navigationLights, &ts, true);
Task poslight (100 * TASK_MILLISECOND, TASK_FOREVER, &positionLights, &ts, true);

// ==== CODE ======================================================================================

/**************************************************************************/
/*!
    @brief    Standard Arduino SETUP method - initialize sketch
    @param    none
    @returns  none
*/
/**************************************************************************/
void setup() {
  // put your setup code here, to run once:
#if defined(_DEBUG_) || defined(_TEST_)
  Serial.begin(115200);
  delay(100);
  _PL("Scheduler Template: setup()");
#endif
  pinMode(posLed, OUTPUT);
  pinMode(navLed, OUTPUT);
  pinMode(acoLed, OUTPUT);
  
  //Anti-collision lights are always ON
  digitalWrite(acoLed, HIGH);
}


/**************************************************************************/
/*!
    @brief    Standard Arduino LOOP method - using with TaskScheduler there 
              should be nothing here but ts.execute()
    @param    none
    @returns  none
*/
/**************************************************************************/
void loop() {
  ts.execute();
}


/**************************************************************************/
/*!
    @brief    Callback method of task1 - explain
    @param    none
    @returns  none
*/
/**************************************************************************/
void navigationLights() {
//  _PM("Navigation lights active");
//  task code
  if(navBright <= 0 and navPlus == false){
    navPlus = true;
  }
  if(navBright >=248 and navPlus == true){
    navPlus = false;
  }
  
  if(navPlus == true){
    if(navBright>248){navBright = 254;}
    navBright = navBright + 8;
  }
  if(navPlus == false){
    if(navBright<1){navBright = 0;}
    navBright = navBright - 8;
  }
  analogWrite(navLed, navBright);
//  _PM(navBright);
}

/**************************************************************************/
/*!
    @brief    Callback method of task1 - explain
    @param    none
    @returns  none
*/
/**************************************************************************/
void positionLights() {
  //_PM("Position lights active");
//  task code
  if(posCount < posCnum and posState == LOW){
    posCount++;
  }
  if(posCount >= posCnum){
    posCount++;
    posState = HIGH; 
  }
  if(posCount > posCnum + 1){
    posCount = 0;
    posState = LOW;
  }
  digitalWrite(posLed, posState);
}
