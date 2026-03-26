//****************************************************************************************************************
//
// SMC3.ino is a basic Motor PID driver designed for motion simulators with upto 3 motors (written for Arduino UNO R3).
//
// Original version by @RufusDufus, published at https://www.xsimulator.net/community/threads/smc3-arduino-3dof-motor-driver-and-windows-utilities.4957.
//
// This version Copyright (C) 2023 Klaus Schmidinger <Klaus.Schmidinger@tvdr.de>
//
// REVISED VERSION FOR LUIS (130kg Rig / Sequential 10s Soft Start / MODE2)
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation
// files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy,
// modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software
// is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
// OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
// LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR
// IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
//****************************************************************************************************************

// Set to MODE1 for use with a typical H-Bride that requires PWM and 1 or 2 direction inputs
// Set to MODE2 for a 43A "Chinese" IBT-2 H-Bridge from e-bay or equiv

#define MODE2

// Enable the following line to build a version that is compatible with DOFReality hardware.

//#define DOFR_BOX     '1'  // number of the DOFReality control box (1=left, 2=right)

// Adjust these according to your firmware version.

#define DOFR_BOARD   'U'  // Arduino board (U = UNO, M = MEGA)
#define DOFR_MODEL    6   // model ID (5=H6, 6=P6)
#define DOFR_CONF    'X'  // SFU gear configuration (A or B for 6 motors, X for 3 or 2 motors)

// The following assembles the version response expected by the DOFReality Tool.

#ifdef DOFR_BOX
#  if (DOFR_BOX == '1')
#     define V_BOX_C "1"
#     define V_BOX_S ""
#  elif (DOFR_BOX == '2')
#     define V_BOX_C "2"
#     define V_BOX_S "R"
#  else
#     error "invalid setting for DOFR_BOX"
#  endif
#endif

#if (DOFR_BOARD == 'U')
#   define V_BOARD_C "U"
#   define V_BOARD_S "UNO"
#elif (DOFR_BOARD == 'M')
#   define V_BOARD_C "M"
#   define V_BOARD_S "MEGA" // TODO needs verification
#else
#   error "invalid setting for DOFR_BOARD"
#endif

#if (DOFR_MODEL == 5)
#   define V_TYPE_C "H"
#   define V_MODEL_C "5"
#   define V_MOTORS_C "6"
#elif (DOFR_MODEL == 6)
#   define V_TYPE_C "P"
#   define V_MODEL_C "6"
#   define V_MOTORS_C "6"
#else
#   error "invalid setting for DOFR_MODEL"
#endif

#ifdef MODE1
#  define V_MODE_C "1" // TODO needs verification (maybe used in "_M1_" and "MODE1"?)
#endif
#ifdef MODE2
#  define V_MODE_C "2" // TODO needs verification (maybe used in "_M2_" and "MODE2"?)
#endif

#if (DOFR_CONF == 'A')
#   define V_CONF_C "A"
#elif (DOFR_CONF == 'B')
#   define V_CONF_C "B"
#elif (DOFR_CONF == 'X')
#   define V_CONF_C ""
#else
#   error "invalid setting for DOFR_CONF"
#endif

#define DOFR_VERSION "name_hex:" V_BOARD_C V_TYPE_C V_MOTORS_C V_BOX_S "_M1_" V_CONF_C ".00000\n" \
                     "compile: date=Jan 01 2018 time=00:00:00\n" \
                     V_BOARD_S " MODE1 SMC3_P\n" \
                     "ID_MODEL=" V_MODEL_C " v6d=" V_BOX_C

//    Note about directions: Internally "forward" means that the values of "target" and "feedback" increase,
//    the motor moves the platform "up". "Backward" is the opposite. The DOFReality platform uses hall sensors
//    for the feedback, which deliver increasing values when turned clockwise, and decreasing values when
//    turned counterclockwise. Depending on the configuration ("A" or "B") some of the sensors will turn
//    clockwise when the platform goes down, so the feedback is reversed for that motor. To make sure feedback
//    is always increasing when the platform goes up, this is taken care of by the REVERSE macro. For the sensors
//    that run reversed, the motors also need to run reversed, which is also taken care of accordingly.

#ifdef DOFR_BOX
#  if (DOFR_CONF == 'A')
#     if (DOFR_BOX == '1')
#        define REVERSE { 1, 1, 1 }
#     elif (DOFR_BOX == '2')
#        define REVERSE { 0, 0, 1 }
#     endif
#  elif (DOFR_CONF == 'B')
#     if (DOFR_BOX == '1')
#        define REVERSE { 1, 1, 0 }
#     elif (DOFR_BOX == '2')
#        define REVERSE { 0, 0, 0 }
#     endif
#  endif
#endif
#ifndef REVERSE
#  define REVERSE { 0, 0, 0 }
#endif

//    One would expect that increasing incoming target values should make the platform move upwards.
//    However, the DOFReality control boxes have an odd behavior: motors L1 and L3, as well as motor
//    R2 move *downwards* when the incoming target values increase! Apparently SRS (Sim Racing Studio)
//    and even FlyPT Mover expect this, so the INVERT macro takes care of this.

#ifdef DOFR_BOX
#  if (DOFR_BOX == '1')
#     define INVERT { 1, 0, 1 }
#  elif (DOFR_BOX == '2')
#     define INVERT { 0, 1, 0 }
#  endif
#endif
#ifndef INVERT
#  define INVERT { 0, 0, 0 }
#endif

//    By default the motors move until the values of feedback and target are the same.
//    The DOFReality P6 platform with SFU gears scales the requested target positions down by the
//    following percentages (the values have been determined by measuring the curves displayed by
//    the "SMC3 Setup and Monitor Utility"). '0' means no scaling.
//    It would appear that the values are the same for all types of platforms, but let's be
//    prepared for different setups. If you increase these values (maximum is 100, for which you
//    should use 0 instead, for less processing), your platform will move further. So be careful,
//    because this may mean that parts might collide!

#ifdef DOFR_BOX
#  define SCALE { 46, 46, 46 }
#endif
#ifndef SCALE
#  define SCALE { 0, 0, 0 }
#endif

//    COMMAND SET:
//
//    []                         Drive all motors to defined stop positions and hold there (not implemented)
//    [Axx],[Bxx],[Cxx]          Send position updates for Motor 1,2,3 where xx is the binary position limited to range 0-1024
//    [Dxx],[Exx],[Fxx]          Send the Kp parameter for motor 1,2,3 where xx is the Kp value multiplied by 100
//    [Gxx],[Hxx],[Ixx]          Send the Ki parameter for motor 1,2,3 where xx is the Ki value multiplied by 100
//    [Jxx],[Kxx],[Lxx]          Send the Kd parameter for motor 1,2,3 where xx is the Kd value multiplied by 100
//    [Mxx],[Nxx],[Oxx]          Send the Ks parameter for motor 1,2,3 where xx is the Ks value
//    [Pxy],[Qxy],[Rxy]          Send the PWMmin and PWMmax values x is the PWMmin and y is PWMmax each being in range 0-255
//    [Sxy],[Txy],[Uxy]          Send the Motor Min/Max Limits (x) and Input Min/Max Limits (y) (Note same value used for Min and Max)
//    [Vxy],[Wxy],[Xxy]          Send the Feedback dead zone (x) and the PWM reverse duty (y) for each motor
//    [Zx-]                      Send the PIDProcessDivider (second byte not yet used)
//    [~xy]                      Send timer 1 (x) and timer 2 (y) frequencies (kHz)
//    [mox]                      Monitor motor x (x=1,2,3 - 0 turns off monitoring)
//    [rdx]                      Read a value from the controller where x is the code for the parameter to read
//    [enx]                      Enable motor x (x=1,2,3)
//    [ena]                      Enable all motors
//    [sav]                      Save parameters to non-volatile memory
//    [ver]                      Send the SMC3 software version
//    [v00]                      Send DOFReality compatible software version (only with DOFR_BOX macro defined)
//

//    Arduino UNO pins used:
//
//                           +----+              +------+
//                         +-|PWR |--------------| USB  |-----+
//                         | |    |              |      |     |
//                         | +----+              +------+    o|
//                         |                                 o|
//                         |                                 o|AREF
//                         |                                 o|GND
//                       NC|o                                o|13
//                    IOREF|o   +----+                       o|12
//                    RESET|o   |    |                       o|11~    Motor 3 PWM
//                     3.3V|o   |    |                       o|10~    Motor 2 PWM
//                       5V|o   |    |    Arduino            o|9~     Motor 1 PWM
//                      GND|o   |    |      UNO              o|8
//                      GND|o   |    |                       |
//                      VIN|o   |    |                       o|7      Motor 3 ENB
//                         |    |    |                       o|6~     Motor 3 ENA
//    Motor 1 Feedback   A0|o   |    |                       o|5~     Motor 2 ENB
//    Motor 2 Feedback   A1|o   |    |                       o|4      Motor 2 ENA
//    Motor 3 Feedback   A2|o   |    |                       o|3~     Motor 1 ENB
//                       A3|o   |    |                       o|2      Motor 1 ENA
//                       A4|o   |    |                       o|1
//                       A5|o   +-/\-+                       o|0
//                         +-\                        /---------+
//                            \--------------------/

#include <EEPROM.h>

#define SOFTWARE_VERSION 210  // divide by 100 to get version - i.e. 101 = version 1.01

#define EEPROM_MARKER 115  // increment whenever the layout of data stored in EEPROM changes

#define BAUD_RATE 500000   // serial data rate

#define LED_BLINK_TIME 1000   // milliseconds

// defines for setting and clearing register bits
#ifndef cbi
#  define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#  define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

// defines for setting and clearing bits
#define SetBit(var, bit) (var |= 1 << bit)
#define ClrBit(var, bit) (var &= ~(1 << bit))

#define START_BYTE '['                  // Start Byte for serial commands
#define END_BYTE ']'                    // End Byte for serial commands
#define PROCESS_PERIOD_uS 250           // 244 -> 4096 per second approx

#define RESOLUTION  1024                // resolution of feedback sensors (0-1023)
#define MAX_POS     (RESOLUTION - 1)
#define NEUTRAL_POS (RESOLUTION / 2)    // the neutral position of the motor
#define INV(n)      (MAX_POS - (n))     // inverts feedback values

//    Soft start/stop

#define ADAPTATION_TIME    4000  // time (ms) to adapt to target postions
#define ADAPTATION_RES      100  // resolution of adaptation
#define COMM_LOST_TIME     2000  // max. time (ms) since last serial communication, after which motors go to idle
#define COMM_BACK_TIME      100  // min. time (ms) since last serial communication, after which motors go to neutral

enum States {
  Idle,        // position at power up
  ToIdle,      // moving towards the idle position
  ToNeutral,   // moving towards the neutral position
  Adapting,    // adapting to dynamic target position
  Synced       // synchronized with requested target position
};

// --- LUIS: CUSTOM SOFT START VARIABLES ---
unsigned long softStartStartTime = 0;
bool softStartActive = false;
#define SOFT_START_DURATION 10000  // 10 Seconds total sequential startup

unsigned long AdaptationStart;
unsigned long LastComm = 0;
int AdaptationFactor = 0;
States State = Idle;

//    Motor data

typedef struct {
  // dynamic values:
  int feedback;            // current position (0-1023)
  int target;              // target position (0-1023)
  int idle;                // idle position (0-1023)
  int pwmOut;              // current PWM value; >0 = up, <0 = down
  bool disabled;           // motor is not authorized to move
  // pid calculation values:
  int cumError;            // cumulative PID error
  long dTerm_x100;         // derivative D term
  int lastPos;             // last position (for calculating dTerm_x100)
  int kdCounter;           // counter for calculating dTerm_x100
  // setup in EEPROM:
  int deadzone;            // feedback deadzone to ignore around center position
  int cutoffMin;           // min position beyond which the motor is disabled
  int cutoffMax;           // max position beyond which the motor is disabled
  int clipMin;             // min acceptable target position
  int clipMax;             // max acceptable target position
  int centerOffset;        // adjust center offset of feedback position
  int kp_x100;             // the proportional term of the PID loop; larger value drives harder (0-1000)
  int ki_x100;             // the integral term to steady state; large values may overshoot and oscillate
  int kd_x100;             // the derivative term reduces overshoot; slows movement under large steps
  int ks;                  // the smoothing parameter; limits kd to only update every N runs
  int pwmMin;              // min PWM value
  int pwmMax;              // max PWM value
  int pwmRev;              // PWM value for backing up after overshooting
  // programmatic values:
  int number;              // motor number (for motor specific handling)
  bool reverse;            // motor and feedback is reverse
  bool invert;             // target values are inverted
  int scale;               // scale factor for target values (percent)
  int feedbackPin;         // analog input pin for positioning feedback
  int enaPinA;             // ENA output pin A for Motor H-Bridge (ie PortD bit position)
  int enaPinB;             // ENA output pin B for Motor H-Bridge (ie PortD bit position)
  int pwmPin;              // PWM output pin
} MotorData;

MotorData Motor1;
MotorData Motor2;
MotorData Motor3;

// Array of pointers to the motors, for indexed access:

MotorData *Motor[] = { &Motor1, &Motor2, &Motor3 };

const bool Reverse[] = REVERSE;
const bool Invert[] = INVERT;
const int Scale[] = SCALE;
const int FeedbackPin[] = { A0, A1, A2 };
const int EnaPinA[] = { 2, 4, 6 };
const int EnaPinB[] = { 3, 5, 7 };
const int PwmPin[] = { 9, 10, 11 };

const int DEBUG_PIN = 8;   // Used for testing to monitor PID timing on Oscilloscope

// --- PROTOTYPES ---
int GetFeedback(MotorData &m);
void MyPWMWrite(uint8_t pin, uint8_t val);
void SendResponse(unsigned int Cmd);

void InitMotor(MotorData &m, int Index)
{
  m.feedback     =    0;
  m.target       = NEUTRAL_POS;
  m.idle         =    0;
  m.pwmOut       =    0;
  m.disabled     = true;
  m.cumError     =    0;
  m.dTerm_x100   =    0;
  m.lastPos      =    0;
  m.kdCounter    =    0;
  m.deadzone     =    0;
  m.cutoffMin    =   50;
  m.cutoffMax    = INV(m.cutoffMin);
  m.clipMin      =  190;
  m.clipMax      = INV(Motor1.clipMin);
  m.centerOffset =    0;
  m.kp_x100      =  120;
  m.ki_x100      =    1;
  m.kd_x100      =   10;
  m.ks           =    5;
  m.pwmMin       =    0;
  m.pwmMax       =  180;
  m.pwmRev       =  200;
  m.number       = Index + 1;
  m.reverse      = Reverse[Index];
  m.invert       = Invert[Index];
  m.scale        = Scale[Index];
  m.feedbackPin  = FeedbackPin[Index];
  m.enaPinA      = EnaPinA[Index];
  m.enaPinB      = EnaPinB[Index];
  m.pwmPin       = PwmPin[Index];

  pinMode(m.feedbackPin, INPUT);
  pinMode(m.enaPinA, OUTPUT);
  pinMode(m.enaPinB, OUTPUT);
  pinMode(m.pwmPin, OUTPUT);

  m.idle = m.lastPos = GetFeedback(m);
}

unsigned int RxBuffer[5] = { 0 };   // 5 byte Rx Command Buffer
unsigned int LoopCount = 0;         // typically at most around 4000, so 'unsigned int' is enough
byte ErrorCount = 0;                // serial receive error detected by invalid packet start/end bytes
byte PowerScale = 7;                // used to divide(shift) PID result changes when in low power

int OutputPort;

int PIDProcessDivider = 1;  // divider for the PID process timer
int SerialFeedbackEnabled = 0;

int Timer1FreqkHz = 25;   // PWM freq used for Motor 1 and 2
int Timer2FreqkHz = 31;   // PWM freq used for Motor 3

//****************************************************************************************************************
//    Setup the PWM pin frequencies
//
//****************************************************************************************************************

void InitialisePWMTimer1(int Freq)     // Used for pins 9 and 10
{
    uint8_t wgm = 8;    //setting the waveform generation mode to 8 - PWM Phase and Freq Correct
    TCCR1A = (TCCR1A & B11111100) | (wgm & B00000011);
    TCCR1B = (TCCR1B & B11100111) | ((wgm & B00001100) << 1);
    TCCR1B = (TCCR1B & B11111000) | 0x01;    // Set the prescaler to minimum (ie divide by 1)

    unsigned int CountTOP;

    CountTOP = (F_CPU / 2) / ((unsigned int)Freq * 1000);    // F_CPU is the oscillator frequency - Freq is the wanted PWM freq/1000

    ICR1 = CountTOP;          // Set the TOP of the count for the PWM
}

void InitialisePWMTimer2(int Freq)
{
    int Timer2DivideMode = (((unsigned int)Freq * 1000) >= 15000) ? 0x01 : 0x02;
    TCCR2B = TCCR2B & 0b11111000 | Timer2DivideMode;
}

void MyPWMWrite(uint8_t pin, uint8_t val)
{
    #define OCR1A_MEM      0x88
    #define OCR1B_MEM      0x8A

    // casting "val" to be larger so that the final value (which is the partially
    // the result of multiplying two potentially high value int16s) will not truncate
    uint32_t tmp = val;

    if (val == 0)
    {
        digitalWrite(pin, LOW);
    }
    else if (val == 255)
    {
        digitalWrite(pin, HIGH);
    }
    else
    {
        uint16_t regLoc16 = 0;
        uint16_t top;

        switch(pin)
        {
            case 9:
                sbi(TCCR1A, COM1A1);
                regLoc16 = OCR1A_MEM;
                top = ICR1;                  // Timer1_GetTop();
                break;
            case 10:
                sbi(TCCR1A, COM1B1);
                regLoc16 = OCR1B_MEM;
                top = ICR1;                  // Timer1_GetTop();
                break;
        }
        tmp = (tmp * top) / 255;
        _SFR_MEM16(regLoc16) = tmp;          // (tmp * top) / 255;
    }
}

// --- Direction and Soft Start Logic ---
#define BACKWARD false
#define FORWARD  true

void SetDir(MotorData &m, bool Fwd)
{
    if (Fwd ^ m.reverse)
    {
        SetBit(OutputPort, m.enaPinA);
#ifndef MODE2
        ClrBit(OutputPort, m.enaPinB);
#endif
    }
    else
    {
        ClrBit(OutputPort, m.enaPinA);
#ifndef MODE2
        SetBit(OutputPort, m.enaPinB);
#endif
    }
}

void SetPwm(MotorData &m, int Pwm, bool isInverted)
{
    int safePwm = Pwm;

    // Check if we are currently in the 10-second startup window
    if (softStartActive)
    {
        unsigned long elapsed = millis() - softStartStartTime;
        
        if (elapsed < SOFT_START_DURATION)
        {
            unsigned long thirdTime = SOFT_START_DURATION / 3; // ~3.3 seconds point
            int limit = 255; 

            if (m.number == 1) // MOTOR 1 (Right Motor)
            {
                if (elapsed < thirdTime)
                {
                    limit = map(elapsed, 0, thirdTime, 30, 255);
                }
            } 
            else if (m.number == 2) // MOTOR 2 (Left Motor)
            {
                if (elapsed < thirdTime)
                {
                    limit = 0; 
                }
                else if (elapsed < 2 * thirdTime)
                {
                    limit = map(elapsed, thirdTime, 2 * thirdTime, 30, 255);
                }
            }
            else if (m.number == 3) // MOTOR 3
            {
                if (elapsed < 2 * thirdTime)
                {
                    limit = 0; 
                }
                else
                {
                    limit = map(elapsed, 2 * thirdTime, SOFT_START_DURATION, 30, 255);
                }
            }

            // MODE2 IBT-2 Safety: Handle Inverted PWM for your 130kg rig
            if (isInverted)
            {
                int invLimit = 255 - limit;
                if (safePwm < invLimit) safePwm = invLimit;
            }
            else
            {
                if (safePwm > limit) safePwm = limit;
            }
        }
        else
        {
            // 10 seconds have passed, exit soft start mode permanently
            softStartActive = false;
        }
    }

    if (m.number == 3)
    {
        analogWrite(m.pwmPin, safePwm);
    }
    else
    {
        MyPWMWrite(m.pwmPin, safePwm);
    }
}

//****************************************************************************************************************
//    EEPROM handling
//
//****************************************************************************************************************

#define EEPROMread  false
#define EEPROMwrite true
static bool EEPROMrw = EEPROMread;

void EEPROMbyte(int &Address, int &Value)
{
    if (EEPROMrw == EEPROMread)
    {
        Value = EEPROM.read(Address++);
    }
    else if (EEPROMrw == EEPROMwrite)
    {
        EEPROM.update(Address++, Value);
    }
}

void EEPROMword(int &Address, int &Value)
{
    if (EEPROMrw == EEPROMread)
    {
        Value = EEPROM.read(Address++) * 256 + EEPROM.read(Address++);
    }
    else if (EEPROMrw == EEPROMwrite)
    {
        int h = Value / 256;
        int l = Value - (256 * h);
        EEPROM.update(Address++, h);
        EEPROM.update(Address++, l);
    }
}

void EEPROMhandler(bool ReadWrite)
{
    EEPROMrw = ReadWrite;
    int Address = 1; // address 0 contains the EEPROM_MARKER
    for (int i = 0; i < 3; i++)
    {
        MotorData *m = Motor[i];
        EEPROMbyte(Address, m->deadzone);
        EEPROMbyte(Address, m->cutoffMin);
        EEPROMbyte(Address, m->clipMin);
        EEPROMword(Address, m->kp_x100);
        EEPROMword(Address, m->ki_x100);
        EEPROMword(Address, m->kd_x100);
        EEPROMword(Address, m->ks);
        EEPROMbyte(Address, m->pwmMin);
        EEPROMbyte(Address, m->pwmMax);
        EEPROMbyte(Address, m->pwmRev);
    }
    EEPROMbyte(Address, PIDProcessDivider);
    EEPROMbyte(Address, Timer1FreqkHz);
    EEPROMbyte(Address, Timer2FreqkHz);
}

//****************************************************************************************************************
//    Write all configurable parameters to EEPROM
//
//****************************************************************************************************************

void WriteEEPROM()
{
    EEPROM.update(0, EEPROM_MARKER);
    EEPROMhandler(EEPROMwrite);
}

//****************************************************************************************************************
//    Read all configurable parameters from EEPROM
//
//****************************************************************************************************************

void ReadEEPROM()
{
    int Marker = EEPROM.read(0);
    if (Marker != EEPROM_MARKER) // EEPROM was not set before, set default values
    {
        WriteEEPROM();
        return;
    }
    EEPROMhandler(EEPROMread);
    Motor1.cutoffMax = INV(Motor1.cutoffMin);
    Motor1.clipMax   = INV(Motor1.clipMin);
    Motor2.cutoffMax = INV(Motor2.cutoffMin);
    Motor2.clipMax   = INV(Motor2.clipMin);
    Motor3.cutoffMax = INV(Motor3.cutoffMin);
    Motor3.clipMax   = INV(Motor3.clipMin);
}

//****************************************************************************************************************
//    Send two bytes via serial in response to a request for information
//
//****************************************************************************************************************

void SendTwoValues(int id, int v1, int v2)
{
    Serial.write(START_BYTE);
    Serial.write(id);
    Serial.write(v1);
    Serial.write(v2);
    Serial.write(END_BYTE);
}

//****************************************************************************************************************
//    Send a single word value via serial in response to a request for information
//
//****************************************************************************************************************

void SendValue(int id, int value)
{
    int high = value / 256;
    int low = value - (high * 256);
    SendTwoValues(id, high, low);
}

//****************************************************************************************************************
//    Get two byte values from RxBuffer
//
//****************************************************************************************************************

void GetTwoValues(int &v1, int &v2)
{
    v1 = RxBuffer[1];
    v2 = RxBuffer[2];
}

//****************************************************************************************************************
//    Get a word value from RxBuffer
//
//****************************************************************************************************************

int GetValue()
{
    return RxBuffer[1] * 256 + RxBuffer[2];
}

//****************************************************************************************************************
//    Calculate how many PID calculations have been performed since last requested
//
//****************************************************************************************************************

unsigned int DeltaLoopCount()
{
    unsigned int Count = LoopCount;
    LoopCount = 0;
    return Count;
}

//****************************************************************************************************************
//    Process the incoming serial commands
//
//****************************************************************************************************************

void ParseCommand()
{
    MotorData *m;
    unsigned int Cmd = RxBuffer[0];
    switch (Cmd)
    {
        case 'A':
        case 'B':
        case 'C':
            m = Motor[Cmd - 'A'];
            m->target = GetValue();
            if (m->invert)
            {
                m->target = INV(m->target);
            }
            if (m->scale)
            {
                m->target = ((long(m->target - NEUTRAL_POS) * m->scale) / 100) + NEUTRAL_POS;
            }
            m->target = constrain(m->target, m->clipMin, m->clipMax);
            LastComm = millis();
            break;
        case 'D':
        case 'E':
        case 'F':
            Motor[Cmd - 'D']->kp_x100 = GetValue();
            break;
        case 'G':
        case 'H':
        case 'I':
            Motor[Cmd - 'G']->ki_x100 = GetValue();
            break;
        case 'J':
        case 'K':
        case 'L':
            Motor[Cmd - 'J']->kd_x100 = GetValue();
            break;
        case 'M':
        case 'N':
        case 'O':
            Motor[Cmd - 'M']->ks = GetValue();
            break;
        case 'P':
        case 'Q':
        case 'R':
            m = Motor[Cmd - 'P'];
            GetTwoValues(m->pwmMin, m->pwmMax);
            break;
        case 'S':
        case 'T':
        case 'U':
            m = Motor[Cmd - 'S'];
            GetTwoValues(m->cutoffMin, m->clipMin);
            m->cutoffMax = INV(m->cutoffMin);
            m->clipMax = INV(m->clipMin);
            break;
        case 'V':
        case 'W':
        case 'X':
            m = Motor[Cmd - 'V'];
            GetTwoValues(m->deadzone, m->pwmRev);
            break;
        case 'Z':
            PIDProcessDivider = constrain(RxBuffer[1], 1, 10); // second byte not yet used
            break;
        case '~':
            GetTwoValues(Timer1FreqkHz, Timer2FreqkHz);
            InitialisePWMTimer1(Timer1FreqkHz);
            InitialisePWMTimer2(Timer2FreqkHz);
            break;
        case 'm':
            if (RxBuffer[1] == 'o')
            {
                Cmd = RxBuffer[2];
                if (Cmd >= '1' && Cmd <= '3')   // Monitor the given motor (auto sends position and feedback data to host
                {
                    SerialFeedbackEnabled = Cmd - '1' + 1;
                }
                else if (Cmd == '0')   // Switch off auto monitoring data feedback
                {
                    SerialFeedbackEnabled = 0;
                }
            }
            break;
        case 'r':
            if (RxBuffer[1] == 'd')      // rd - Read a value from the ArduinoPID - next byte identifies value to read
            {
                SendResponse(RxBuffer[2]);
            }
            break;
        case 's':
            if (RxBuffer[1] == 'a' && RxBuffer[2] == 'v')   // Save all settings to EEPROM
            {
                WriteEEPROM();
            }
            break;
        case 'v':
            if (RxBuffer[1] == 'e' && RxBuffer[2] == 'r')   // Send back the SMC3 software version
            {
                SendValue('v', SOFTWARE_VERSION);
            }
#ifdef DOFR_BOX
            else if (RxBuffer[1] == '0' && RxBuffer[2] == '0')   // Send back the DOFReality software version
            {
                Serial.println(DOFR_VERSION);
            }
            else if (RxBuffer[1] == 'm' && RxBuffer[2] == 'd')   // Send back the DOFReality model code
            {
                SendTwoValues('v', 'm', char(DOFR_MODEL));
            }
            else if (RxBuffer[1] == '6' && RxBuffer[2] == 'd')   // Send back the DOFReality control box number
            {
                SendValue('v', DOFR_BOX);
            }
#endif
            break;
        case 'e':
            if (RxBuffer[1] == 'n')
            {
                Cmd = RxBuffer[2];
                if (Cmd == 'a')   // Enable all motors
                {
                    Motor1.disabled = false;
                    Motor2.disabled = false;
                    Motor3.disabled = false;
#ifdef MODE2    //  IB-2
                    ClrBit(OutputPort, Motor1.enaPinB);
                    ClrBit(OutputPort, Motor2.enaPinB);
                    ClrBit(OutputPort, Motor3.enaPinB);
                    PORTD = OutputPort;
                    SetBit(OutputPort, Motor1.enaPinB);
                    SetBit(OutputPort, Motor2.enaPinB);
                    SetBit(OutputPort, Motor3.enaPinB);
                    PORTD = OutputPort;
#endif
                }
                else if (Cmd >= '1' && Cmd <= '3')   // Enable the given motor
                {
                    m = Motor[Cmd - '1'];
                    m->disabled = false;
#ifdef MODE2    // IB-2
                    ClrBit(OutputPort, m->enaPinB);
                    PORTD = OutputPort;
                    SetBit(OutputPort, m->enaPinB);
                    PORTD = OutputPort;
#endif
                }
            }
            break;
    }
}

//****************************************************************************************************************
//    Send command response
//
//****************************************************************************************************************

void SendResponse(unsigned int Cmd)
{
    MotorData *m;
    switch (Cmd)
    {
        case 'A':
        case 'B':
        case 'C':
            m = Motor[Cmd - 'A'];
            SendTwoValues(Cmd, (m->invert ? INV(m->feedback) : m->feedback) / 4, (m->invert ? INV(m->target) : m->target) / 4);
            break;
        case 'a':
        case 'b':
        case 'c':
            SendTwoValues(Cmd, PIDProcessDivider * 16 + Motor1.disabled + (Motor2.disabled * 2) + (Motor3.disabled * 4), constrain(Motor[Cmd - 'a']->pwmOut, 0, 255));
            break;
        case 'D':
        case 'E':
        case 'F':
            SendValue(Cmd, Motor[Cmd - 'D']->kp_x100);
            break;
        case 'G':
        case 'H':
        case 'I':
            SendValue(Cmd, Motor[Cmd - 'G']->ki_x100);
            break;
        case 'J':
        case 'K':
        case 'L':
            SendValue(Cmd, Motor[Cmd - 'J']->kd_x100);
            break;
        case 'M':
        case 'N':
        case 'O':
            SendValue(Cmd, Motor[Cmd - 'M']->ks);
            break;
        case 'P':
        case 'Q':
        case 'R':
            m = Motor[Cmd - 'P'];
            SendTwoValues(Cmd, m->pwmMin, m->pwmMax);
            break;
        case 'S':
        case 'T':
        case 'U':
            m = Motor[Cmd - 'S'];
            SendTwoValues(Cmd, m->cutoffMin, m->clipMin);
            break;
        case 'V':
        case 'W':
        case 'X':
            m = Motor[Cmd - 'V'];
            SendTwoValues(Cmd, m->deadzone, m->pwmRev);
            break;
        case 'Y':
            SendTwoValues(Cmd, PIDProcessDivider * 16 + Motor1.disabled + (Motor2.disabled * 2) + (Motor3.disabled * 4), 0);  // Second byte not yet used
            break;
        case 'Z':
            SendValue(Cmd, DeltaLoopCount());
            break;
        case '~':
            SendTwoValues(Cmd, Timer1FreqkHz, Timer2FreqkHz);  // PWM Frequencies to set
            break;
    }
}

//****************************************************************************************************************
//    Set the Motor output pins to drive the motor in required direction and speed
//
//****************************************************************************************************************

void SetOutputsMotor(MotorData &m)
{
    if (m.feedback > m.clipMax && m.pwmRev != 0)
    {
        m.pwmOut = m.pwmRev;
        SetDir(m, BACKWARD);
        SetPwm(m, m.pwmRev, false);
    }
    else if (m.feedback < m.clipMin && m.pwmRev != 0)
    {
        m.pwmOut = m.pwmRev;
        SetDir(m, FORWARD);
        SetPwm(m, 255 - m.pwmRev, true); // MODE2 IBT-2 Safety
    }
    else if (abs(m.target - m.feedback) > m.deadzone)
    {
        if (m.pwmOut >= 0)
        {
            // Drive Motor Forward
            m.pwmOut = min(m.pwmOut + m.pwmMin, m.pwmMax);
            SetDir(m, FORWARD);
            SetPwm(m, 255 - m.pwmOut, true); // MODE2 IBT-2 Safety
        }
        else
        {
            // Drive Motor Backwards
            m.pwmOut = min(abs(m.pwmOut) + m.pwmMin, m.pwmMax);
            SetDir(m, BACKWARD);
            SetPwm(m, m.pwmOut, false);
        }
    }
    else
    {
        // Brake Motor
        ClrBit(OutputPort, m.enaPinA);
        m.pwmOut = m.pwmMin;
        SetPwm(m, 0, false);
    }
    PORTD = OutputPort;
}

//****************************************************************************************************************
//    Check and update the motor drive
//
//****************************************************************************************************************

int GetFeedback(MotorData &m)
{
    int Feedback = analogRead(m.feedbackPin);
    if (m.reverse)
    {
        Feedback = INV(Feedback);
    }
    return Feedback;
}

void CheckMotor(MotorData &m)
{
    m.feedback = GetFeedback(m);
    if (m.feedback > m.cutoffMax || m.feedback < m.cutoffMin)
    {
        DisableMotor(m);
    }
    m.pwmOut = CalcMotorPID(m);
    if (!m.disabled)
    {
        SetOutputsMotor(m);
    }
    else
    {
        m.pwmOut = 0;
    }
}

//****************************************************************************************************************
//    Soft start/stop
//
//****************************************************************************************************************

void StartAdaptation()
{
    AdaptationStart = millis();
    AdaptationFactor = ADAPTATION_RES;
}

void CalcAdaptationFactor()
{
    long Delta = millis() - AdaptationStart;
    if (Delta < ADAPTATION_TIME)
    {
        AdaptationFactor = ADAPTATION_RES - Delta * ADAPTATION_RES / ADAPTATION_TIME;
    }
    else
    {
        AdaptationFactor = 0;
    }
}

int AdaptedTarget(int From, int To)
{
    return From + long(To - From) * (ADAPTATION_RES - AdaptationFactor) / ADAPTATION_RES;
}

int CalcAdaptedTarget(MotorData &m)
{
    switch (State)
    {
        case Synced:
            return m.target;
        case Idle:
            return m.idle;
        case ToIdle:
            return AdaptedTarget(m.feedback, m.idle);
        case ToNeutral:
            return AdaptedTarget(m.feedback, NEUTRAL_POS);
        case Adapting:
            return AdaptedTarget(NEUTRAL_POS, m.target);
        default:
            return NEUTRAL_POS;
    }
}

void CheckState()
{
    // Remember what the state was before we check for changes
    States OldState = State;

    if (State == Synced)
    {
        if (millis() - LastComm > COMM_LOST_TIME)
        {
            State = ToIdle;
            StartAdaptation();
        }
    }
    else if (State == Idle)
    {
        if (LastComm > 0 && millis() - LastComm < COMM_BACK_TIME)
        {
            State = ToNeutral;
            StartAdaptation();
        }
    }
    else
    {
        CalcAdaptationFactor();
        if (AdaptationFactor == 0)
        {
            switch (State)
            {
                case ToIdle:
                {
                    State = Idle;
                    break;
                }
                case ToNeutral:
                {
                    State = Adapting;
                    StartAdaptation();
                    break;
                }
                case Adapting:
                {
                    State = Synced;
                    StartAdaptation();
                    break;
                }
            }
        }
    }

    // Trigger the 10-second Sequential Soft Start if we just left the Idle state
    if (OldState == Idle && State != Idle) {
        softStartStartTime = millis();
        softStartActive = true;
    }
    
    // Reset the flag if the rig is turned off
    if (State == Idle) {
        softStartActive = false;
    }
}

//****************************************************************************************************************
//    Calculate the motor PID output
//****************************************************************************************************************

int CalcMotorPID(MotorData &m)
{
    int Error = CalcAdaptedTarget(m) - m.feedback;
    if (abs(Error) <= m.deadzone)
    {
        m.cumError = 0;
    }
    else
    {
        m.cumError += Error;
        m.cumError = constrain(m.cumError, -RESOLUTION, RESOLUTION);
    }

    long pTerm_x100 = m.kp_x100 * (long)Error; 
    long iTerm_x100 = (m.ki_x100 * (long)m.cumError);

    m.kdCounter++;
    if (m.kdCounter >= m.ks) 
    {
        int DeltaPosition = m.feedback - m.lastPos;
        m.lastPos = m.feedback;
        m.dTerm_x100 = m.kd_x100 * (long)DeltaPosition;
        m.kdCounter = 0;
    }

    return constrain((pTerm_x100 + iTerm_x100 - m.dTerm_x100) >> PowerScale, -255, 255);
}

//****************************************************************************************************************
//    Disable the Motors - ie put the brakes on
//****************************************************************************************************************

void DisableMotor(MotorData &m)
{
    m.disabled = true;
    ClrBit(OutputPort, m.enaPinA);
    ClrBit(OutputPort, m.enaPinB);
    PORTD = OutputPort;
    SetPwm(m, 0, false);
}

void TogglePin()
{
   static int PinOut = 0;
   PinOut = 1 - PinOut;
   digitalWrite(DEBUG_PIN, PinOut);
}

//****************************************************************************************************************
//    Toggle the builtin LED
//****************************************************************************************************************

void ToggleLed()
{
    static unsigned long LedTimeout = 0;
    static int LedState = 0;
    if (millis() > LedTimeout)
    {
        LedState = 1 - LedState;
        digitalWrite(LED_BUILTIN, LedState);
        LedTimeout += LED_BLINK_TIME;
    }
}

//****************************************************************************************************************
//    Arduino serial event function called if data is available
//****************************************************************************************************************

void serialEvent()
{
    static int BufferEnd = -1;
    while (Serial.available())
    {
        unsigned int RxByte = Serial.read();
        if (BufferEnd == -1)
        {
            if (RxByte != START_BYTE)
            {
                ErrorCount++;
            }
            else
            {
                BufferEnd = 0;
            }
        }
        else
        {
            RxBuffer[BufferEnd] = RxByte;
            BufferEnd++;
            if (BufferEnd > 3)
            {
                if (RxByte == END_BYTE)
                {
                    ParseCommand();
                }
                else
                {
                    ErrorCount++;
                }
                BufferEnd = -1;
            }
        }
    }
}

//****************************************************************************************************************
//    Arduino setup subroutine called at startup/reset
//****************************************************************************************************************

void setup()
{
    InitMotor(Motor1, 0);
    InitMotor(Motor2, 1);
    InitMotor(Motor3, 2);

    ReadEEPROM();

    Serial.begin(BAUD_RATE);
    OutputPort = PORTD;
    DisableMotor(Motor1);
    DisableMotor(Motor2);
    DisableMotor(Motor3);

    InitialisePWMTimer1(Timer1FreqkHz);
    InitialisePWMTimer2(Timer2FreqkHz);

    // set analogue prescale to 16
    sbi(ADCSRA, ADPS2);
    cbi(ADCSRA, ADPS1);
    cbi(ADCSRA, ADPS0);

    // Enable all motors
    Motor1.disabled = false;
    Motor2.disabled = false;
    Motor3.disabled = false;

#ifdef MODE2
    // Reset IBT-2
    ClrBit(OutputPort, Motor1.enaPinB);
    ClrBit(OutputPort, Motor2.enaPinB);
    ClrBit(OutputPort, Motor3.enaPinB);
    PORTD = OutputPort;
    SetBit(OutputPort, Motor1.enaPinB);
    SetBit(OutputPort, Motor2.enaPinB);
    SetBit(OutputPort, Motor3.enaPinB);
    PORTD = OutputPort;
#endif

    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(DEBUG_PIN, OUTPUT);
}

//****************************************************************************************************************
//    Main Arduino Program Loop
//****************************************************************************************************************

void loop()
{
    // Wait until its time and then update PID calcs

    static unsigned long TimesUp = 0;
    while (micros() < TimesUp) { ; }
    TimesUp = micros() + PROCESS_PERIOD_uS;

    static int PIDProcessCounter = 0;
    if (++PIDProcessCounter >= PIDProcessDivider)
    {
        CheckState();
        CheckMotor(Motor1);
        CheckMotor(Motor2);
        CheckMotor(Motor3);
        LoopCount++;
        PIDProcessCounter = 0;
    }

    static int SerialFeedbackCounter = 0;
    if (++SerialFeedbackCounter >= 80)
    {
        if (SerialFeedbackEnabled >= 1 && SerialFeedbackEnabled <= 3) 
        {
            MotorData *m = Motor[SerialFeedbackEnabled - 1];
            SendTwoValues('A' + SerialFeedbackEnabled - 1, (m->invert ? INV(m->feedback) : m->feedback) / 4, (m->invert ? INV(m->target) : m->target) / 4);
            SendTwoValues('a' + SerialFeedbackEnabled - 1, PIDProcessDivider * 16 + Motor1.disabled + (Motor2.disabled * 2) + (Motor3.disabled * 4), constrain(m->pwmOut, 0, 255));
        }
        SerialFeedbackCounter = 0;
    }

    PowerScale = (State == Idle) ? 9 : 7;

    TogglePin();
    ToggleLed();
}