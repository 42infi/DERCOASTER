#include <stdlib.h>
#include "rgb_effects.h"
#include "light_ws2812.h"

// Define the number of LEDs in the strip
#define NUM_LEDS 20

// Effect function prototypes
void fadeInOut();
void rainbow();
void fade();
void rainbowCycle();
void rainbowColorChaseEffect();
void twinkle();
void meteorShower();
void colorWipe();
void theaterChase();
void singleColorChaseEffect();

// New effect prototypes
void strobeEffect();
void policeLightsEffect();
void cylonBounce();
void confettiEffect();
void flash();


// Define the LED strip
struct cRGB leds[NUM_LEDS];

// Define the current effect index
uint8_t effectIndex = 0;

// Define the starting color (der8auer Yellow)
uint8_t effectR = 242;
uint8_t effectG = 230;
uint8_t effectB = 23;

// Effects static variables
uint8_t step = 0;
uint8_t slow_cnt = 0;
uint8_t led = 0;

// Define an array of function pointers to the different effects
// (Make sure NUM_EFFECTS is updated to reflect the new total count)
void (*effects[NUM_EFFECTS])() = { 
    fadeInOut, 
    rainbow, 
    fade, 
    rainbowCycle, 
    rainbowColorChaseEffect, 
    twinkle, 
    meteorShower, 
    colorWipe, 
    theaterChase, 
    singleColorChaseEffect,
    strobeEffect,
    policeLightsEffect,
    cylonBounce,
    confettiEffect,
    flash
};

// Define the loop function
void rgb_effects_loop() {
    
  // Call the current effect function
  effects[effectIndex]();
  
  // Update LEDs
  ws2812_sendarray((uint8_t *)leds, NUM_LEDS * 3);
  
}

void rgb_set_effect(uint8_t effect) {
    effectIndex = (effect) % NUM_EFFECTS;
    
    // Reset LEDs
    for (int i = 0; i < NUM_LEDS; i++) {
        leds[i].r = 0;
        leds[i].g = 0;
        leds[i].b = 0;
    }
    
    step = 0;
    led = 0;
    slow_cnt = 0;
    
}

void rgb_get_effect(uint8_t* effect) { 
    *effect = effectIndex;
}

void rgb_set_color(uint8_t r, uint8_t g, uint8_t b) {
    effectR = r;
    effectG = g;
    effectB = b;
}

void rgb_get_color(uint8_t* r, uint8_t* g, uint8_t* b) {
    *r = effectR;
    *g = effectG;
    *b = effectB;
}

void rainbow_get_values(uint8_t hue, uint8_t* r, uint8_t* g, uint8_t* b) {
    uint8_t red, green, blue;
    uint8_t segment = hue / 43;
    uint8_t offset = hue % 43;

    switch (segment) {
      case 0:
        red = 255;
        green = offset * 6;
        blue = 0;
        break;
      case 1:
        red = 255 - offset * 6;
        green = 255;
        blue = 0;
        break;
      case 2:
        red = 0;
        green = 255;
        blue = offset * 6;
        break;
      case 3:
        red = 0;
        green = 255 - offset * 6;
        blue = 255;
        break;
      case 4:
        red = offset * 6;
        green = 0;
        blue = 255;
        break;
      default:
        red = 255;
        green = 0;
        blue = 255 - offset * 6;
        break;
    }
    
    *r = red;
    *g = green;
    *b = blue;
    
}

// GPT3.5 effects 2023-03-17

void rainbow() {
  for (int i = 0; i < NUM_LEDS; i++) {
    rainbow_get_values(step, &leds[i].r, &leds[i].g, &leds[i].b);
  }
  step += 1;
}

void colorWipe() {
    slow_cnt++;
    if(slow_cnt < 10) {
        return;
    }
    slow_cnt = 0;
    
    step &= 1;
    
    leds[led].r = step == 0 ? effectR : 0;
    leds[led].g = step == 0 ? effectG : 0;
    leds[led].b = step == 0 ? effectB : 0;
    
    led++;
    if (led >= NUM_LEDS) {
        led = 0;
        step = !step;
    }
}

void theaterChase() {
    int i;
    
    slow_cnt++;
    if(slow_cnt < 10) {
        return;
    }
    slow_cnt = 0;

    if (step == 0) {
        for (i = 0; i < NUM_LEDS; i += 3) {
            leds[i].r = effectR;
            leds[i].g = effectG;
            leds[i].b = effectB;
        }
        step = 1;
    } else if (step == 1) {
        for (i = 0; i < NUM_LEDS; i += 3) {
            leds[i].r = 0;
            leds[i].g = 0;
            leds[i].b = 0;
        }
        for (i = 1; i < NUM_LEDS; i += 3) {
            leds[i].r = effectR;
            leds[i].g = effectG;
            leds[i].b = effectB;
        }
        step = 2;
    } else {
        for (i = 1; i < NUM_LEDS; i += 3) {
            leds[i].r = 0;
            leds[i].g = 0;
            leds[i].b = 0;
        }
        for (i = 2; i < NUM_LEDS; i += 3) {
            leds[i].r = effectR;
            leds[i].g = effectG;
            leds[i].b = effectB;
        }
        step = 0;
    }
}

void fade() {
  for (int i = 0; i < NUM_LEDS; i++) {
    uint8_t value = (i + 1) * 255 / (NUM_LEDS + 1);
    rainbow_get_values(step + (i * 256 / NUM_LEDS), &leds[i].r, &leds[i].g, &leds[i].b);
    leds[i].r = (leds[i].r * value) / 255;
    leds[i].g = (leds[i].g * value) / 255;
    leds[i].b = (leds[i].b * value) / 255;
  }
  step += 2;
}

void twinkle() {
    slow_cnt++;
    if(slow_cnt < 2) {
        return;
    }
    slow_cnt = 0;
    
    int i;
    
    if (step == 0) {
        leds[led].r = effectR;
        leds[led].g = effectG;
        leds[led].b = effectB;
        step = 1;
    } else {
        leds[led].r = 0;
        leds[led].g = 0;
        leds[led].b = 0;
        led++;
        if (led >= NUM_LEDS) {
            led = 0;
        }
        step = 0;
    }

    for (i = 0; i < NUM_LEDS; i++) {
        if (i == led) {
            leds[i].r = effectR;
            leds[i].g = effectG;
            leds[i].b = effectB;
        } else {
            if (leds[i].r > 0 || leds[i].g > 0 || leds[i].b > 0) {
                leds[i].r = (leds[i].r * 9) / 10;
                leds[i].g = (leds[i].g * 9) / 10;
                leds[i].b = (leds[i].b * 9) / 10;
            }
        }
    }
}

// Fade in and out
void fadeInOut() {
    static uint8_t bright = 0;
    if (step == 0) {
        bright++;
        if (bright >= 255) {
            step = 1;
        }
    } else {
        bright--;
        if (bright <= 0) {
            step = 0;
        }
    }
    
    for (int i = 0; i < NUM_LEDS; i++) {
        leds[i].r = bright * effectR / 255;
        leds[i].g = bright * effectG / 255;
        leds[i].b = bright * effectB / 255;
    }
}

// Rainbow cycle
void rainbowCycle() {
  uint16_t i;
  for (i = 0; i < NUM_LEDS; i++) {
    uint8_t index = ((i * 256 / NUM_LEDS) + step) & 255;
    uint8_t r, g, b;
    
    if (index < 85) {
      r = index * 3;
      g = 255 - index * 3;
      b = 0;
    } else if (index < 170) {
      index -= 85;
      r = 255 - index * 3;
      g = 0;
      b = index * 3;
    } else {
      index -= 170;
      r = 0;
      g = index * 3;
      b = 255 - index * 3;
    }
    
    leds[i].r = r;
    leds[i].g = g;
    leds[i].b = b;
  }
  step++;
}

void meteorShower() {
    const uint8_t meteorSize = 10;
    const uint8_t meteorTrailDecay = 3;
    const uint8_t meteorRandomDecay = 1;
    static uint8_t meteors[NUM_LEDS];
    static int lastMeteor = 0;

    for (int i = 0; i < NUM_LEDS; i++) {
        if (meteors[i] == 0 && rand() % 10 == 0) {
            meteors[i] = meteorSize + rand() % (NUM_LEDS / 2);
            lastMeteor = i;
        }

        if (meteors[i] > 0) {
            leds[i].r = effectR;
            leds[i].g = effectG;
            leds[i].b = effectB;

            if (meteorRandomDecay) {
                meteors[i] -= rand() % meteorTrailDecay;
            } else {
                meteors[i] -= meteorTrailDecay;
            }
            
            if (meteors[i] < 0) {
                meteors[i] = 0;
            }
        } else {
            leds[i].r = 0;
            leds[i].g = 0;
            leds[i].b = 0;
        }
    }

    if (lastMeteor < NUM_LEDS - 1) {
        lastMeteor++;
    } else {
        lastMeteor = 0;
    }
}

// GPT-4 effects 2023-03-20

void rainbowColorChaseEffect() {
    slow_cnt++;
    if(slow_cnt < 3) {
        return;
    }
    slow_cnt = 0;
    
    uint8_t i;
    for (i = 0; i < NUM_LEDS; i++) {
        if (i == led) {
            rainbow_get_values(step, &leds[i].r, &leds[i].g, &leds[i].b);
        } else if(i < led) {
            leds[i].r = leds[i].r * 8 / 10;
            leds[i].g = leds[i].g * 8 / 10;
            leds[i].b = leds[i].b * 8 / 10;
        } else {
            leds[i].r = 0;
            leds[i].g = 0;
            leds[i].b = 0;
        }
    }
    led = (led + 1) % NUM_LEDS;
    step = (step + 1) % 256;
}

void singleColorChaseEffect() {
    slow_cnt++;
    if(slow_cnt < 3) {
        return;
    }
    slow_cnt = 0;
    
    uint8_t i;
    for (i = 0; i < NUM_LEDS; i++) {
        if (i == led) {
            leds[i].r = effectR;
            leds[i].g = effectG;
            leds[i].b = effectB;
        } else if(i < led) {
            leds[i].r = leds[i].r * 8 / 10;
            leds[i].g = leds[i].g * 8 / 10;
            leds[i].b = leds[i].b * 8 / 10;
        } else {
            leds[i].r = 0;
            leds[i].g = 0;
            leds[i].b = 0;
        }
    }
    led = (led + 1) % NUM_LEDS;
    step = (step + 1) % 256;
}

// -------------------
// New LED Effects
// -------------------

// Strobe effect: rapidly toggles all LEDs off and on.
void strobeEffect() {
    slow_cnt++;
    if (slow_cnt < 3) {
        for (int i = 0; i < NUM_LEDS; i++) {
            leds[i].r = 0;
            leds[i].g = 0;
            leds[i].b = 0;
        }
    } else if (slow_cnt < 6) {
        for (int i = 0; i < NUM_LEDS; i++) {
            leds[i].r = effectR;
            leds[i].g = effectG;
            leds[i].b = effectB;
        }
    } else {
        slow_cnt = 0;
    }
}

// Police lights effect: alternating red and blue chasing pattern.
void policeLightsEffect() {
    slow_cnt++;
    if (slow_cnt < 5) return;
    slow_cnt = 0;
    
    // Clear all LEDs first
    for (int i = 0; i < NUM_LEDS; i++) {
        leds[i].r = 0;
        leds[i].g = 0;
        leds[i].b = 0;
    }
    
    // Create a repeating pattern with a phase shift
    for (int i = 0; i < NUM_LEDS; i++) {
        int phase = (i + led) % 4;
        if (phase < 2) {
            leds[i].r = 255;  // Red segment
            leds[i].g = 0;
            leds[i].b = 0;
        } else {
            leds[i].r = 0;
            leds[i].g = 0;
            leds[i].b = 255;  // Blue segment
        }
    }
    
    led = (led + 1) % 4;
}

// Cylon Bounce effect: a bright dot bounces back and forth with trailing fade.
void cylonBounce() {
    slow_cnt++;
    if (slow_cnt < 5) return;
    slow_cnt = 0;
    
    // Fade all LEDs slightly for a trailing effect
    for (int i = 0; i < NUM_LEDS; i++) {
        leds[i].r = (leds[i].r * 9) / 10;
        leds[i].g = (leds[i].g * 9) / 10;
        leds[i].b = (leds[i].b * 9) / 10;
    }
    
    // Light up the current LED position
    leds[led].r = effectR;
    leds[led].g = effectG;
    leds[led].b = effectB;
    
    // Bounce the LED position between the ends
    static int direction = 1;
    led += direction;
    if (led == 0 || led == NUM_LEDS - 1) {
        direction = -direction;
    }
}

// Confetti Effect: Randomly lights up LEDs with random colors, then fades them for a confetti-like sparkle.
void confettiEffect() {
    // Gradually fade all LEDs
    for (int i = 0; i < NUM_LEDS; i++) {
        leds[i].r = (leds[i].r * 9) / 10;
        leds[i].g = (leds[i].g * 9) / 10;
        leds[i].b = (leds[i].b * 9) / 10;
    }
    // Randomly choose an LED and assign it a random color from the rainbow spectrum
    int pos = rand() % NUM_LEDS;
    uint8_t hue = rand() % 256;
    uint8_t r, g, b;
    rainbow_get_values(hue, &r, &g, &b);
    leds[pos].r = r;
    leds[pos].g = g;
    leds[pos].b = b;
}

void flash() {
    int i;
    
    slow_cnt++;
    
    if(slow_cnt < 10) {
         for (i = 0; i < NUM_LEDS; i++) {
                leds[i].r = 0;
                leds[i].g = 0;
                leds[i].b = 0;
         }
    } else if(slow_cnt >= 10) {
         for (i = 0; i < NUM_LEDS; i++) {
                leds[i].r = effectR;
                leds[i].g = effectG;
                leds[i].b = effectB;
         }
    }
    
    if(slow_cnt > 20){
        slow_cnt = 0;
    }
}
