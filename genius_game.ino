// C++ code
//

// LEDs ports
#define LED_RED			2
#define LED_YELLOW		3
#define LED_GREEN		4
#define LED_BLUE		5

// Buttons ports
#define BUTTON_RED		8
#define BUTTON_YELLOW	9
#define BUTTON_GREEN	10
#define BUTTON_BLUE		11

// Other constants
#define NUM_LEDS		4		// Number of LEDs in the game
#define NUM_ROUNDS		4		// Number of rounds in the game
#define TIMESPAN		1000	// Duration of an LED blink
#define UNDEFINED		-1		// Undefined variable for the player's answer

// States in the game
enum States {
  READY_TO_NEXT_ROUND,
  PLAYER_ANSWERING,
  GAME_COMPLETED_SUCCESS,
  GAME_COMPLETED_FAIL
};

int leds[NUM_LEDS] = {LED_RED, LED_YELLOW, LED_GREEN, LED_BLUE};
int buttons[NUM_LEDS] = {BUTTON_RED, BUTTON_YELLOW, BUTTON_GREEN, BUTTON_BLUE};
int ledsSequence[NUM_ROUNDS];
int roundNum = 0;
int ledsAnswered = 0;

// Starts a random game and sets the sequence of LEDs
void startGame() {
  int gameNum = analogRead(0);
  randomSeed(gameNum);
  for (int i = 0; i < NUM_ROUNDS; i++) {
    ledsSequence[i] = drawColor();
  }
}

// Returns a random LED color
int drawColor() {
  return random(LED_RED, LED_BLUE + 1);
}

// Initialize all the LEDs and buttons
void initializeLedsAndButtons() {
  for (int i = 0; i < NUM_LEDS; i++) {
    pinMode(leds[i], OUTPUT);
    pinMode(buttons[i], INPUT_PULLUP);
  }
}

// Blink the selected LED and return its port number
int blinkLed(int led) {
  digitalWrite(led, HIGH);
  delay(TIMESPAN);
  digitalWrite(led, LOW);
  delay(TIMESPAN);
  return led;
}

// Blink all LEDs in the game one by one
void blinkAllLedsOneByOne() {
  for (int i = 0; i < NUM_LEDS; i++) {
    blinkLed(leds[i]);
  }
}

// Light up all LEDs in the game at the same time
void blinkAllLedsAtSameTime() {
  for (int i = 0; i < NUM_LEDS; i++) {
    digitalWrite(leds[i], HIGH);
  }
}

// Check the player answer read on the clicked button
int checkPlayerAnswer() {
  if (digitalRead(BUTTON_RED) == LOW) {
    return blinkLed(LED_RED);
  } else if (digitalRead(BUTTON_YELLOW) == LOW) {
    return blinkLed(LED_YELLOW);
  } else if (digitalRead(BUTTON_GREEN) == LOW) {
    return blinkLed(LED_GREEN);
  } else if (digitalRead(BUTTON_BLUE) == LOW) {
    return blinkLed(LED_BLUE);
  } else {
  	return UNDEFINED;
  }
}

// Blink all LEDs in the round
void blinkRoundLeds() {
  for (int i = 0; i < roundNum; i++) {
    blinkLed(ledsSequence[i]);
  }
}

// Returns the current state of the game
int currentState() {
  if (roundNum <= NUM_ROUNDS) {
    if (ledsAnswered == roundNum) {
      return READY_TO_NEXT_ROUND;
    } else {
      return PLAYER_ANSWERING;
    }
  } else if (roundNum == NUM_ROUNDS + 1) {
    return GAME_COMPLETED_SUCCESS;
  } else {
    return GAME_COMPLETED_FAIL;
  }
}

// Prepare a new round of the game
void prepareNewRound() {
  roundNum++;
  ledsAnswered = 0;
  if (roundNum <= NUM_ROUNDS) {
    blinkRoundLeds();    
  }
}

void processPlayerAnswer() {
  int answer = checkPlayerAnswer();
  
  if (answer == UNDEFINED) {
    return;
  }
  
  if (answer == ledsSequence[ledsAnswered]) {
    ledsAnswered++;
  } else {
    roundNum = NUM_ROUNDS + 2;
  }
}

void gameCompletedSuccess() {
  blinkAllLedsOneByOne();
  return;
}

void gameCompletedFail() {
  blinkAllLedsAtSameTime();
  return;
}

void setup() {
  Serial.begin(9600);
  initializeLedsAndButtons();
  startGame();
}

void loop() {
  switch (currentState()){
    case READY_TO_NEXT_ROUND:
      prepareNewRound();
      break;
  	case PLAYER_ANSWERING:
      processPlayerAnswer();
      break;
  	case GAME_COMPLETED_SUCCESS:
      gameCompletedSuccess();
      break;
  	case GAME_COMPLETED_FAIL:
      gameCompletedFail();
      break;
  }
}