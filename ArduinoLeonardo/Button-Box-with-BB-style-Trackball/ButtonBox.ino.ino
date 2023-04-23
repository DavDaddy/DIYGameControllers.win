#include <Keypad.h>
#include <Joystick.h>

#define ENABLE_PULLUPS
#define NUMBUTTONS 16
#define NUMROWS 4
#define NUMCOLS 4

byte buttons[NUMROWS][NUMCOLS] = {
{0, 1, 2, 3},
{4, 5, 6, 7},
{8, 9, 10, 11,},
{12, 13, 14, 15}
};

byte rowPins[NUMROWS] = {3, 2, 1, 0};
byte colPins[NUMCOLS] = {7, 6, 5, 4};

Keypad buttbx = Keypad( makeKeymap(buttons), rowPins, colPins, NUMROWS, NUMCOLS);

Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID,
JOYSTICK_TYPE_JOYSTICK, 20, 0,
false, false, false, false, false, false,
false, false, false, false, false);

const int BUTTON = 0;
bool switchState = false;

#include <Mouse.h>
#define bounce_interval       30
#define base_move_pixels      5
#define exponential_bound     15
#define exponential_base      1.2
// pin definition
#define btn_pin               A4
#define right_pin             A3
#define left_pin              A2
#define down_pin              A1
#define up_pin                A5
#define white_led_pin         12
#define BUTTON                13

class Direction {
public:
  Direction(int pin1, int pin2) {
    this->pins[0] = pin1;
    this->pins[1] = pin2;
    pinMode(this->pins[0], INPUT);
    pinMode(this->pins[1], INPUT);
  };
  int read_action() {
    for(int i = 0; i < 2; ++i) {
      this->current_actions[i] = digitalRead(this->pins[i]);
      this->current_action_times[i] = millis();
      if(this->current_actions[i] != this->last_actions[i]) {
        this->last_actions[i] = this->current_actions[i];
        exponential = (exponential_bound - (this->current_action_times[i] - this->last_action_times[i]));
        exponential = (exponential > 0) ? exponential : 1;
        move_multiply = exponential_base;
        for(int i = 0; i < exponential; ++i) {
          move_multiply *= exponential_base;
        }
        this->last_action_times[i] = this->current_action_times[i];
        if(i == 0) {
          return (-1) * base_move_pixels * move_multiply;
        } else {
          return base_move_pixels * move_multiply;
        }
      }
    }
    return 0;
  };
private:
  int pins[2];
  int current_actions[2];
  int last_actions[2];
  int  exponential;
  double move_multiply;
  unsigned long current_action_times[2];
  unsigned long last_action_times[2];
};

// button and debounce
int btn_state;
int btn_read_state;
unsigned long btn_current_action_time;
unsigned long btn_last_action_time;

// mouse move
int x_move, y_move;
Direction x_direction(left_pin, right_pin);
Direction y_direction(up_pin, down_pin);

int lastButtonState[4] = {0,0,0,0};

void setup() {
Serial.begin(115200);
Joystick.begin();
Joystick.setButton(BUTTON, switchState);
pinMode(8, INPUT_PULLUP);
pinMode(9, INPUT_PULLUP);
pinMode(10, INPUT_PULLUP);
pinMode(11, INPUT_PULLUP);
pinMode(BUTTON, INPUT_PULLUP);
pinMode(white_led_pin, OUTPUT);
pinMode(btn_pin, INPUT);
digitalWrite(white_led_pin, HIGH);
Mouse.begin();
}

void loop() {

bool buttonState = digitalRead(BUTTON);
  if (buttonState == HIGH) {
    switchState = !switchState;
  }
  Joystick.setButton(BUTTON, switchState);
  Joystick.sendState();

btn_read_state = digitalRead(btn_pin);
  if(btn_read_state != btn_state) {
    btn_current_action_time = millis();
    if(btn_current_action_time - btn_last_action_time > bounce_interval) {
      btn_state = btn_read_state;
      btn_last_action_time = btn_current_action_time;
      if(btn_state == HIGH) {
        digitalWrite(white_led_pin, HIGH);
        Mouse.release();
      } else {
        digitalWrite(white_led_pin, LOW);
        Mouse.press();
      }
    }
  }

  x_move = x_direction.read_action();
  y_move = y_direction.read_action();
  if(x_move != 0 || y_move != 0) {
    Mouse.move(x_move, y_move, 0);
  }

if (buttbx.getKeys())
{  
for (int i=0; i<LIST_MAX; i++)
{
if ( buttbx.key[i].stateChanged )
{
switch (buttbx.key[i].kstate) { //Button 1 working perfectly!
case PRESSED:
Joystick.setButton(buttbx.key[i].kchar, 1);
delay(100);
Joystick.setButton(buttbx.key[i].kchar, 0);
break;
case RELEASED:
Joystick.setButton(buttbx.key[i].kchar, 1);
delay(100);
Joystick.setButton(buttbx.key[i].kchar, 0);
break;
}
}
if ( buttbx.key[i].stateChanged ) //Button 2 work like Button 1, i wanna to use this
{ //button like a pushbutton
switch (buttbx.key[i].kstate) {
case PRESSED:
case HOLD:
Joystick.setButton(buttbx.key[i].kchar, 1);
break;
case RELEASED:
case IDLE:
Joystick.setButton(buttbx.key[i].kchar, 0);
break;
}
}
if ( buttbx.key[i].stateChanged ) //Button 3 work like button 1 again
{
switch (buttbx.key[i].kstate) {
case PRESSED:
case HOLD:
Joystick.setButton(buttbx.key[i].kchar, 1);
break;
case RELEASED:
case IDLE:
Joystick.setButton(buttbx.key[i].kchar, 0);
break;
}
}
}
}
}
