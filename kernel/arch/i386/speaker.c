#include <kernel/speaker.h>

// SEE: https://wiki.osdev.org/PC_Speaker
// SEE ALSO: https://web.archive.org/web/20171115162742/http://guideme.itgo.com/atozofc/ch23.pdf

static float notes[7][12] = {
    { 130.81, 138.59, 146.83, 155.56, 164.81, 174.61, 185.0,
        196.0, 207.65, 220.0, 227.31, 246.96 },
    { 261.63, 277.18, 293.66, 311.13, 329.63, 349.23, 369.63,
        392.0, 415.3, 440.0, 454.62, 493.92 },
    { 523.25, 554.37, 587.33, 622.25, 659.26, 698.46, 739.99,
        783.99, 830.61, 880.0, 909.24, 987.84 },
    { 1046.5, 1108.73, 1174.66, 1244.51, 1328.51, 1396.91, 1479.98,
        1567.98, 1661.22, 1760.0, 1818.48, 1975.68 },
    { 2093.0, 2217.46, 2349.32, 2489.02, 2637.02, 2793.83, 2959.96,
        3135.96, 3322.44, 3520.0, 3636.96, 3951.36 },
    { 4186.0, 4434.92, 4698.64, 4978.04, 5274.04, 5587.86, 5919.92,
        6271.92, 6644.88, 7040.0, 7273.92, 7902.72 },
    { 8372.0, 8869.89, 9397.28,9956.08,10548.08,11175.32, 11839.84,
        12543.84, 13289.76, 14080.0, 14547.84, 15805.44 }
};

void speaker_note(uint8_t octave, uint8_t note) {
    speaker_play((uint32_t) notes[octave][note]);
}

void speaker_play(uint32_t hz) {
    // Set the PIT to the desired frequency.
    uint32_t d = 1193180 / hz;
    outportb(0x43, 0xb6);
    outportb(0x42, (uint8_t) (d) );
    outportb(0x42, (uint8_t) (d >> 8));

    /*
    //outportb(0x42, (u8) (d & 0xFF));
    //outportb(0x42, (u8) ((d >> 8) & 0xFF));
    outportb(0x42, 140);
    outportb(0x42, 140);
    */

    // And play the sound using the PC speaker.
    uint8_t t = inportb(0x61);
    if (t != (t | 3)) {
        outportb(0x61, t | 3);
    }
}

void speaker_pause() {
    outportb(0x61, inportb(0x61) & 0xFC);
}
