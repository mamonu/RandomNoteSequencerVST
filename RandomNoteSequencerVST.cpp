#include "public.sdk/source/vst2.x/audioeffectx.h"

#include <cstdlib>
#include <ctime>

// RandomNoteSequencerPlugin is a simple VST MIDI sequencer plugin that outputs random notes on the DAW clock.
class RandomNoteSequencerPlugin : public AudioEffectX {
public:
  RandomNoteSequencerPlugin(audioMasterCallback audioMaster)
      : AudioEffectX(audioMaster, 1, 1) {
    // Set up internal state, such as note generation algorithms, note duration, etc.
    std::srand(std::time(nullptr));
  }

  // This function is called by the host DAW at the beginning of each clock tick
  void processMidiEvents(VstMidiEventVec *midiEvents) {
    // Generate a random MIDI note
    int note = std::rand() % 128;

    // Create a MIDI event to represent the generated note
    VstMidiEvent event;
    event.type = kVstMidiType;
    event.byteSize = 24;
    event.deltaFrames = 0;
    event.flags = 0;
    event.noteLength = 1;
    event.noteOffset = 0;
    event.detune = 0;
    event.noteOffVelocity = 0;
    event.reserved1 = 0;
    event.reserved2 = 0;
    event.midiData[0] = 0x90; // MIDI note on message
    event.midiData[1] = note; // MIDI note number
    event.midiData[2] = 100; // MIDI velocity

    // Add the MIDI event to the list of MIDI events to be processed by the host DAW
    midiEvents->push_back(event);
  }

  // This function is called by the host DAW to get the plugin's unique identifier
  VstInt32 getVendorVersion() { return 1000; }
  bool getEffectName(char *name) {
    strcpy(name, "Random Note Sequencer");
    return true;
  }
  bool getVendorString(char *text) {
    strcpy(text, "Assistant");
    return true;
  }
  bool getProductString(char *text) {
    strcpy(text, "Random Note Sequencer Plugin");
    return true;
  }
};

// This macro defines the entry point for the VST plugin, and it must be included in every VST plugin.
// The first argument is the unique identifier for the plugin, and the second argument is the plugin class.
// The unique identifier must be unique across all VST plugins, and it is used by the host DAW to identify
// the plugin.
VstPlugins::VstPlugin *VstPlugins::createPlugin(VstInt32 id,
                                                audioMasterCallback audioMaster) {
  return new RandomNoteSequencerPlugin(audioMaster);
