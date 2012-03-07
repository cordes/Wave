#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "Wave.hpp"

Wave::Wave(int mSeconds, int samplesPerSecond) {
	this->mSeconds = mSeconds;
	this->samplesPerSecond = samplesPerSecond;
	this->numSamples = (mSeconds / 1000.0) * samplesPerSecond;
	this->dataChunk = new DataChunk(this->numSamples, this->samplesPerSecond);
}

void Wave::init_wav_header(wav_header *h) {
	strcpy(h->signature, "RIFF");
	h->length = 44 - 8 + this->dataChunk->length;
	strcpy(h->riffType, "WAVE");
}

void Wave::init_pcm_format(format *f) {
	strcpy(f->signature, "fmt ");
	f->length = 16;
	f->wFormatTag = 0x0001;
	f->wChannels = 1;
	f->dwSamplesPerSec = this->samplesPerSecond;
	f->wBitsPerSample = 16;
	f->wBlockAlign = f->wChannels * ((f->wBitsPerSample + 7) / 8);
	f->dwAvgBytesPerSec = f->wBlockAlign * f->dwSamplesPerSec;
}

void Wave::init_data_header(data_header *dh) {
	strcpy(dh->signature, "data");
	dh->length = this->dataChunk->length;
}

float Wave::dreieck(float t) {
	t = t - (int) t;
	if (t < 0.25)
		return 4 * t;
	else if (t < 0.75)
		return 4 * (0.5 - t);
	else
		return 4 * (t - 1.0);
}

//ptr[i] = 30000 * dreieck(freq * i/this->samplesPerSecond);

void Wave::create_sin_data(DataChunk *dataChunk, double freq) {
	for (int i = 0; i < dataChunk->length / sizeof(short); i++) {
		dataChunk->data[i] =
            30000 * sin(2 * M_PI * freq * i/dataChunk->samplesPerSecond);
	}
}

void Wave::write_wav(char *path) {
	FILE *fp;
	wav_header h;
	format f;
	data_header dh;

	init_wav_header(&h);
	init_pcm_format(&f);
	init_data_header(&dh);

	fp = fopen(path, "wb");
	fwrite(&h, sizeof(h), 1, fp);
	fwrite(&f, sizeof(f), 1, fp);
	fwrite(&dh, sizeof(dh), 1, fp);
	fwrite(this->dataChunk->data, this->dataChunk->length, 1, fp);
	fclose(fp);
}
