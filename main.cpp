#include "hardware/gpio.h"
#include "kemper.hpp"
#include "pico/stdlib.h"
#include <stdio.h>
#include <utility>

// MIDI Spec: 31250 baud rate, 8 data bits, 1 stop bit, no parity
#define UART_ID uart0
#define UART_TX_PIN 0
#define BAUD_RATE 31250
#define GPIO_BUTTON_1 16

static int32_t alarm_id{};
static bool pressed = false;

// Source: https://gist.github.com/kates/9d8b33587b01694898e3f874142db5f3
int64_t alarm_callback(alarm_id_t id, void *user_data);
void callback(uint gpio, uint32_t events);

void setup() {
  stdio_init_all();
  // UART ///////////////////////////////////////////////////////////
  uart_init(UART_ID, BAUD_RATE);
  gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
  /////////////////////////////////////////////////////////// UART //
  // BUTTON /////////////////////////////////////////////////////////
  gpio_init(GPIO_BUTTON_1);
  gpio_set_dir(GPIO_BUTTON_1, GPIO_IN);
  ///////////////////////////////////////////////////////// BUTTON //
}

int main() {
  setup();
  gpio_set_irq_enabled_with_callback(GPIO_BUTTON_1, GPIO_IRQ_EDGE_RISE, true,
                                     &callback);
  for (;;)
    ;
}

int64_t alarm_callback(alarm_id_t id, void *user_data) {
  pressed = false;
  return 0;
}

void callback(uint gpio, uint32_t events) {
  if (pressed) {
    cancel_alarm(alarm_id);
  } else {
    pressed = true;
    switch (gpio) {
    case GPIO_BUTTON_1:
      printf("MIDI! \n");
      send_midi_message(UART_ID, 0xB0,
                        kemper::MidiControlChange::PERFORMANCE_PAGEUP, 0);
      break;
    }
    printf("Pressed: %d \n", pressed);
  }
  alarm_id = add_alarm_in_ms(200, &alarm_callback, NULL, false);
}
