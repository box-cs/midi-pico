#include "pico/stdlib.h"
#include <map>
#include <string>
#include <utility>
#include <vector>

using std::vector;

namespace Kemper {
enum class MidiMessage : uint8_t {
  NOTE_OFF = 0x80,
  NOTE_ON = 0x90,
  POLYPHONIC_AFTERTOUCH = 0xA0,
  CONTROL_CHANGE = 0xB0,
  PROGRAM_CHANGE = 0xC0,
  CHANNEL_AFTERTOUCH = 0xD0,
  PITCH_BEND = 0xE0
};

enum class MidiControlChange : uint8_t {
  // Source: KEMPER PROFILER MIDI Parameter Documentation
  // Function /////////////////////////// Value
  TOGGLE_MODULES = 16,                 // ANY
  TOGGLE_MODULE_A = 17,                // ANY
  TOGGLE_MODULE_B = 18,                // ANY
  TOGGLE_MODULE_C = 19,                // ANY
  TOGGLE_MODULE_D = 20,                // ANY
  TOGGLE_MODULE_X = 22,                // ANY
  TOGGLE_MODULE_MOD = 24,              // ANY
  TOGGLE_MODULE_DELAY = 26,            // ANY
  TOGGLE_MODULE_DELAY_WITH_TAIL = 27,  // ANY
  TOGGLE_MODULE_REVERB = 28,           // ANY
  TOGGLE_MODULE_REVERB_WITH_TAIL = 29, // ANY
  TAP_TEMPO = 30,                      // 1 | 0
  TUNER = 31,                          // 1 | 0
  ROTARY_SPEAKER = 33,                 // 1 | 0
  DELAY_INFINITY = 34,                 // 1 | 0
  DELAY_AND_REVERB_FREEZE = 35,        // 1 | 0
  PERFORMANCE_PAGEUP = 48,             // 1 | 0
  PERFORMANCE_PAGEDOWN = 49,           // 1 | 0
  PERFORMANCE_SELECT_SLOT_1 = 50,      // 1
  PERFORMANCE_SELECT_SLOT_2 = 51,      // 1
  PERFORMANCE_SELECT_SLOT_3 = 52,      // 1
  PERFORMANCE_SELECT_SLOT_4 = 53,      // 1
  PERFORMANCE_SELECT_SLOT_5 = 54,      // 1
  MORPH = 80,                          // 1 | 0
  // NRPN
  DATA_ENTRY_MSB = 6,  // 0-127
  DATA_ENTRY_LSB = 38, // 0-127
  ADDRESS_NUMBER = 98, // 0-127
  ADDRESS_PAGE = 99,   // 0-127
};

// https://learn.sparkfun.com/tutorials/midi-tutorial/all
void send_midi_message(auto uartId, const MidiMessage message,
                       MidiControlChange cc, uint8_t value) {
  const uint8_t *midi_message = new uint8_t[3]{std::to_underlying(message),
                                               std::to_underlying(cc), value};
  uart_write_blocking(uartId, midi_message, 3);
  delete[] midi_message;
};

// KEMPER PROFILER MIDI Parameter Documentation -- NRPN Definition
// Kemper catalogues modules in decimal
// ie: Setting “Reverb Mix” value to 8192 in module REV (NRPN #6169)
// MSB(“address page”) is 61($3D) and LSB(“address number”) is 69($45).
// auto message = {
//   {0xB0, 0x63, 0x3D},
//   {0xB0, 0x62, 0x45},
//   {0xB0, 0x06, (8192 >> 7) & 7F },
//   {0xB0, 0x26, (8192 & 7F )}
// }
void send_nrpn_message(auto uartId, uint16_t nrpn, uint16_t value) {
  // value max is 8192 = 0x2000
  send_midi_message(uartId, MidiMessage::CONTROL_CHANGE,
                    MidiControlChange::ADDRESS_PAGE, nrpn / 100);
  send_midi_message(uartId, MidiMessage::CONTROL_CHANGE,
                    MidiControlChange::ADDRESS_NUMBER, nrpn % 100);
  send_midi_message(uartId, MidiMessage::CONTROL_CHANGE,
                    MidiControlChange::DATA_ENTRY_MSB, (value >> 7) & 0x7F);
  send_midi_message(uartId, MidiMessage::CONTROL_CHANGE,
                    MidiControlChange::DATA_ENTRY_LSB, value & 0x7F);
};
} // namespace Kemper