#include "Wave.hpp"
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>

double freqs[] = {
    65.4064, 69.2957, 73.4162, 77.7817, 82.4069, 87.3071,
    92.4986, 97.9989, 103.826, 110.0, 116.541, 123.471,

    130.813, 138.591, 146.832, 155.563, 164.814, 174.614,
    184.997, 195.998, 207.652, 220.0, 233.082, 246.942,

	261.63, 277.18, 293.67, 311.13, 329.63, 349.23,
	369.99, 392.00, 415.31, 440.00, 466.16, 493.88,

    523.251, 554.365, 587.330, 622.254, 659.255, 698.456,
    739.989, 783.991, 830.609, 880.000, 932.328, 987.767
};

double getFreq(enum ton t, int oktave = 1) { // WTF?!?!
	return freqs[t * oktave];
}

void blend(DataChunk *chunk) {
    int numSamples = chunk->length / sizeof(short);
    int blendSamples = chunk->samplesPerSecond / 100;
    double factor, t = 0, flankenzeit = 0.01;
    double B = flankenzeit / 2.3;
    double delta_t = 1.0 / chunk->samplesPerSecond;

    for (int i = 0; i < blendSamples; i++) {
        factor = exp(-((t-flankenzeit)/B) * ((t-flankenzeit)/B));
        chunk->data[i] *= factor;
        chunk->data[numSamples-i-1] *= factor;
        t += delta_t;
    }
}

DataChunk *create_note(int mSec, enum ton t, Wave *wave) {
    DataChunk *tmpChunk = new DataChunk((mSec / 1000.0) * wave->samplesPerSecond, wave->samplesPerSecond);
    wave->create_sin_data(tmpChunk, getFreq(t));
    blend(tmpChunk);
    return tmpChunk;
}

void write_notes(Wave *wave, enum ton *toene, int *laengen, int num) {
    DataChunk *chunk;
	int offset = 0;
	for (int i = 0; i < num; i++) {
		chunk = create_note(laengen[i], toene[i], wave);
		wave->dataChunk->add(chunk, offset);
        free(chunk); // !!!!!!
	    offset += laengen[i];
	}
}

int main() {
	Wave *wave = new Wave(0, 44100);
	char *path = (char *) malloc(sizeof(char) * 16);
	strncpy(path, "Test.wav", 16);

	enum ton toene[] = {
        B4, E5, G5, FIS5, E5, B5, A5, FIS5,
        E5, G5, FIS5, DIS5, E5, B4
    };
	int laengen[] = {
        500, 750, 250, 500, 1000, 500, 1500, 1500,
        750, 250, 500, 1000, 500, 2000
    };
	int num_notes = sizeof(toene) / sizeof(enum ton);
    write_notes(wave, toene, laengen, num_notes);

    enum ton toene2[] = {
        B2, E2, E2, E2, E2,
        E2, E2, E2, B2
    };
	int laengen2[] = {
        500, 1500, 1500, 1500, 1500,
        1500, 1500, 1500, 1000
    };
	num_notes = sizeof(toene2) / sizeof(enum ton);
	write_notes(wave, toene2, laengen2, num_notes);

	wave->write_wav(path);

	return 0;
}
