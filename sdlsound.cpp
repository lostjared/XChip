#include <SDL2/SDL.h>
#include <cstdio>
#include <cmath>


#define FREQ 300 /* the frequency we want */

unsigned int audio_pos; /* which sample we are up to */
int audio_len; /* how many samples left to play, stops when <= 0 */
float audio_frequency; /* audio frequency in cycles per sample */
float audio_volume; /* audio volume, 0 - ~32000 */


void MyAudioCallback(void* userdata, Uint8* stream, int len) {
	len /= sizeof(Sint16); /* 16 bit */
	int i;
	Sint16* buf = (Sint16*)stream;
	for(i = 0; i < len; i++) { 
		buf[i] = audio_volume * sin(2 * M_PI * audio_pos * audio_frequency);
		audio_pos++;
	}
	audio_len -= len;
	return;


}



int main()
{
	if(SDL_Init(SDL_INIT_AUDIO)) {
    	printf("[SDL] Failed to initialize: %s\n", SDL_GetError());
    	return 1;
  	}

	SDL_AudioSpec want, have;
  	SDL_zero(want);
	want.freq = 42100;
	want.format = AUDIO_S16;
	want.channels = 1;
	want.samples = 4096;
	want.callback = MyAudioCallback;
	SDL_AudioDeviceID dev = SDL_OpenAudioDevice(NULL, 0, &want, &have, SDL_AUDIO_ALLOW_FREQUENCY_CHANGE);

	audio_len = have.freq * 50.0 / 60; /* 5 seconds */
	audio_pos = 0;
	audio_frequency = 2.5 * FREQ / have.freq; /* 1.0 to make it a float */
	audio_volume = 6000; /* ~1/5 max volume */
	SDL_PauseAudioDevice(dev, 0); /* play! */
  
  	while(audio_len > 0) {
    	SDL_Delay(100);
	}

	SDL_CloseAudioDevice(dev);
  	SDL_Quit();

  	return 0;
}