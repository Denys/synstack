#include "synth.h"

const float ct_synth_notes[] = {
    // c1
    32.70319566f, 34.64782887f, 36.70809599f, 38.89087297f, 41.20344461f,
    43.65352893f, 46.24930284f, 48.99942950f, 51.91308720f, 55.00000000f,
    58.27047019f, 61.73541266f,
    // c2
    65.40639133f, 69.29565774f, 73.41619198f, 77.78174593f, 82.40688923f,
    87.30705786f, 92.49860568f, 97.99885900f, 103.82617439f, 110.00000000f,
    116.54094038f, 123.47082531f,
    // c3
    130.81278265f, 138.59131549f, 146.83238396f, 155.56349186f, 164.81377846f,
    174.61411572f, 184.99721136f, 195.99771799f, 207.65234879f, 220.00000000f,
    233.08188076f, 246.94165063f,
    // c4
    261.62556530f, 277.18263098f, 293.66476792f, 311.12698372f, 329.62755691f,
    349.22823143f, 369.99442271f, 391.99543598f, 415.30469758f, 440.00000000f,
    466.16376152f, 493.88330126f,
    // c5
    523.25113060f, 554.36526195f, 587.32953583f, 622.25396744f, 659.25511383f,
    698.45646287f, 739.98884542f, 783.99087196f, 830.60939516f, 880.00000000f,
    932.32752304f, 987.76660251f,
    // c6
    1046.50226120f, 1108.73052391f, 1174.65907167f, 1244.50793489f,
    1318.51022765f, 1396.91292573f, 1479.97769085f, 1567.98174393f,
    1661.21879032f, 1760.00000000f, 1864.65504607f, 1975.53320502f,
    // c7
    2093.00452240f, 2217.46104781f, 2349.31814334f, 2489.01586978,
    2637.02045530f, 2793.82585146f, 2959.95538169f, 3135.96348785,
    3322.43758064f, 3520.00000000f, 3729.31009214f, 3951.06641005,
    // c8
    4186.00904481f, 4434.92209563f, 4698.63628668f, 4978.03173955f,
    5274.04091061f, 5587.65170293f, 5919.91076339f, 6271.92697571f,
    6644.87516128f, 7040.00000000f, 7458.62018429f, 7902.13282010f};
