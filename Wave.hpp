#include "DataChunk.hpp"

enum ton {
    C2=0, CIS2, D2, DIS2, E2, F2, FIS2, G2, GIS2, A2, BB2, B2,
    C3, CIS3, D3, DIS3, E3, F3, FIS3, G3, GIS3, A3, BB3, B3,
    C4, CIS4, D4, DIS4, E4, F4, FIS4, G4, GIS4, A4, BB4, B4,
    C5, CIS5, D5, DIS5, E5, F5, FIS5, G5, GIS5, A5, BB5, B5
};

class Wave {

  public:

	Wave(int mSeconds, int samplesPerSecond);

	void write_wav(char *path);
	void create_sin_data(DataChunk *dataChunk, double freq);

	int numSamples;
	int mSeconds;
	int samplesPerSecond;
	DataChunk *dataChunk;


  private:

	typedef struct {
		char signature[4];
		unsigned int length;
		char riffType[4];
	} wav_header;
	/*__attribute__((__packed__));*/

	typedef struct {
		char signature[4];
		int length;
		unsigned short wFormatTag;
		unsigned short wChannels;
		unsigned int dwSamplesPerSec;
		unsigned int dwAvgBytesPerSec;
		unsigned short wBlockAlign;
		unsigned short wBitsPerSample;
	} format;

	typedef struct {
		char signature[4];
		unsigned int length;
	} data_header;

	void init_wav_header(wav_header *h);
	void init_pcm_format(format *f);
	void init_data_header(data_header *dh);
	float dreieck(float t);

};
