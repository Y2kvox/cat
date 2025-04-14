#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdint.h>

#define SAMPLE_RATE 44100
#define DURATION 0.5
#define PI 3.141592653589793

void generate_melody(const char *name, const char *filename) {
    double freqs[] = {523.25, 587.33, 659.25, 698.46, 783.99, 880.00};
    int name_len = strlen(name);
    int samples_per_note = SAMPLE_RATE * DURATION;
    int meow_samples = SAMPLE_RATE * 0.3;
    int total_samples = samples_per_note * name_len + meow_samples;

    int16_t *buffer = calloc(total_samples, sizeof(int16_t));
    for (int i = 0; i < name_len; i++) {
        int idx = name[i] - 'a';
        double freq = freqs[idx % 6];
        for (int j = 0; j < samples_per_note; j++) {
            double t = (double)j / SAMPLE_RATE;
            buffer[i * samples_per_note + j] = (int16_t)(32760.0 * 0.1 * sin(2 * PI * freq * t));
        }
    }
    for (int i = 0; i < meow_samples; i++) {
        double t = (double)i / SAMPLE_RATE;
        double freq = 440.0 + 100.0 * sin(2 * PI * 5 * t);
        buffer[name_len * samples_per_note + i] = (int16_t)(32760.0 * 0.1 * sin(2 * PI * freq * t) * (1 - t / 0.3));
    }

    FILE *f = fopen(filename, "wb");
    uint8_t header[44] = {
        0x52, 0x49, 0x46, 0x46, 0, 0, 0, 0, 0x57, 0x41, 0x56, 0x45,
        0x66, 0x6d, 0x74, 0x20, 16, 0, 0, 0, 1, 0, 1, 0,
        0x44, 0xac, 0, 0, 0x88, 0x58, 0x01, 0, 2, 0, 16, 0,
        0x64, 0x61, 0x74, 0x61, 0, 0, 0, 0
    };
    int data_size = total_samples * 2;
    *(uint32_t *)(header + 4) = 36 + data_size;
    *(uint32_t *)(header + 40) = data_size;
    fwrite(header, 1, 44, f);
    fwrite(buffer, sizeof(int16_t), total_samples, f);
    fclose(f);
    free(buffer);
}

int main() {
    generate_melody("yusra feng", "her_opera.wav"); // Replace "emma" with her name
    return 0;
}