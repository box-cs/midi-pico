#include "hardware/gpio.h"
#include "pico/stdlib.h"
#include <stdio.h>

volatile int32_t alarm_id{};
volatile bool pressed = false;

// Source: https://gist.github.com/kates/9d8b33587b01694898e3f874142db5f3
// (debouncing with alarm)
int64_t alarm_callback(alarm_id_t id, void *user_data) {
  pressed = false;
  return 0;
}

void callback(uint gpio, uint32_t events) {
  if (pressed) {
    cancel_alarm(alarm_id);
  } else {
    pressed = true;
    printf("Pressed: %d \n", pressed);
  }
  alarm_id = add_alarm_in_ms(25, &alarm_callback, NULL, false);
}

void setup() {
  stdio_init_all();
  // BUTTON
  gpio_init(16);
  gpio_set_dir(16, GPIO_IN);
}

int main() {
  setup();
  do {
    gpio_set_irq_enabled_with_callback(16, GPIO_IRQ_EDGE_RISE, true, &callback);
    printf("Sleeping...");
    for (;;)
      ;
  } while (1);
}