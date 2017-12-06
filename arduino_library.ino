
#include <Adafruit_NeoPixel.h>

byte neopix_gamma[] = {
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
    1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,
    2,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4,  5,  5,  5,
    5,  6,  6,  6,  6,  7,  7,  7,  7,  8,  8,  8,  9,  9,  9, 10,
   10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
   17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
   25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
   37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
   51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
   69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
   90, 92, 93, 95, 96, 98, 99,101,102,104,105,107,109,110,112,114,
  115,117,119,120,122,124,126,127,129,131,133,135,137,138,140,142,
  144,146,148,150,152,154,156,158,160,162,164,167,169,171,173,175,
  177,180,182,184,186,189,191,193,196,198,200,203,205,208,210,213,
  215,218,220,223,225,228,231,233,236,239,241,244,247,249,252,255 };
// integer math stuff

#define INTEGER_SCALE 8191

unsigned long long_div(unsigned long numerator, unsigned long denominator)
{
  return (numerator * INTEGER_SCALE) / denominator;
}

unsigned long long_mult(unsigned long op1, unsigned long op2)
{
  return op1 * op2 / INTEGER_SCALE;
}

byte byte_mult(byte op1, byte op2)
{
  return (byte)((uint16_t)op1 * (uint16_t)op2 / (uint16_t)INTEGER_SCALE);
}

int neoPixelPin = 3;

// How many NeoPixels we will be using, charge accordingly
int pixelCount = 16;

// Instatiate the NeoPixel from the ibrary
Adafruit_NeoPixel strip = Adafruit_NeoPixel(pixelCount, neoPixelPin, NEO_GRB + NEO_KHZ800);

void setup() {
  //Serial.begin(9600);           // set up Serial library at 9600 bps
  strip.begin();
  //Serial.println("started!");  // prints hello with ending line break 
}

void loop() {
  calculate_strip();
  delay(10);
  //Serial.println('b');
  //strip.setPixelColor(1, strip.Color(233,0,0));
  //strip.show();
}

void calculate_strip() {
  unsigned long mils = millis();
  for (int i; i < pixelCount; i++) {
    unsigned long this_phase = long_div(i, pixelCount - 1);
    unsigned long lamp_phase[3] = {this_phase, this_phase, this_phase};

    unsigned long phasor1[3];
    unsigned long phasor1_cycle[3] = {7000, 5000, 5000};
    phasor(mils, phasor1_cycle, phasor1);

    unsigned long addmodulo1[3];
    addmodulo(lamp_phase, phasor1, addmodulo1);
    
    unsigned long times1[3];
    unsigned long op2[3] = {150, 150, 150};
    times(addmodulo1, op2, times1);

    strip.setPixelColor(i, neopix_gamma[times1[0]], neopix_gamma[times1[1]], neopix_gamma[times1[2]]);
    //strip.setPixelColor(i, strip.Color(233,0,0));
  }
  strip.show();
}

void phasor(unsigned long mils, unsigned long cycle[3], unsigned long out[3]) {
  out[0] = long_div((mils % cycle[0]), cycle[0]);
  out[1] = long_div((mils % cycle[1]), cycle[1]);
  out[2] = long_div((mils % cycle[2]), cycle[2]);
}

void times(unsigned long op1[3], unsigned long op2[3], unsigned long out[3]) {
  out[0] = long_mult(op1[0], op2[0]);
  out[1] = long_mult(op1[1], op2[1]);
  out[2] = long_mult(op1[2], op2[2]);
}

void add(unsigned long op1[3], unsigned long op2[3], unsigned long out[3]) {
  out[0] = op1[0] + op2[0];
  out[1] = op1[1] + op2[1];
  out[2] = op1[2] + op2[2];
}

void addmodulo(unsigned long op1[3], unsigned long op2[3], unsigned long out[3]) {
  out[0] = (op1[0] + op2[0]) % INTEGER_SCALE;
  out[1] = (op1[1] + op2[1]) % INTEGER_SCALE;
  out[2] = (op1[2] + op2[2]) % INTEGER_SCALE;
}

void addconstrain(unsigned long op1[3], unsigned long op2[3], unsigned long out[3]) {
  out[0] = constrain(op1[0] + op2[0], 0, INTEGER_SCALE);
  out[1] = constrain(op1[1] + op2[1], 0, INTEGER_SCALE);
  out[2] = constrain(op1[2] + op2[2], 0, INTEGER_SCALE);
}