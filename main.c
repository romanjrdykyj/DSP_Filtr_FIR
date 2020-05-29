#include <dsplib.h>

const long d = ((long)100*22368)>>14; 			//krok

#define num_filter 55							//dlugosc filtru

//wspolczynniki filtru Blackmana
const short cff[num_filter] = {0,0,-1,-2,-7,-14,-24,-38,-54,-71,-85,-91,-83,-54,3,95,226,400,614,864,1141,1432,1722,1991,
					   2223,2403,2516,2554,2516,2403,2223,1991,1722,1432,1141,864,614,400,226,95,3,-54,-83,-91,
					   -85,-71,-54,-38,-24,-14,-7,-2,-1,0,0};


#define num_samples 5000			//dlugosc sygnalu tj. rozmiar tablicy probek
short samples[num_samples];			//sygnal po filtracji
short samples2[num_samples];		//sygnal poddawany filtracji
short buff[57];

//wlasna funckja realizuj¹ca filtracje filtrem typu FIR
void blockfir(short* input, const short* filter, short* output, int numSamples, int numFilter)
{
	long y;
	int i;
	int j;
	for(i=numSamples; i>=0; i--)			//petla iterujaca od kocna tablicy probek
	{
		y = 0;
		for(j=0; j<numFilter; j++)			//petla iteruj¹ca po wsp. filtru
		{
			y = _smaci(y,input[i-j],filter[j]);	//obliczenia na potrzeby FIR
		}
		output[i] = (int)(_sround(y) >> 15);	//zapisanie wyniku w odpowiedniej tablicy wraz ze zmiana formatu z Q30 na Q15
	}

}


void saw(short* wsk_tablicy, unsigned int l_el_tablicy, int krok){  //sygnal piloksztaltny
	static int amplituda = 0;		//akumulator
	int i;
	int y;
	for(i = 0; i<l_el_tablicy; i++){
		y = amplituda;
		wsk_tablicy[i]=y;
		amplituda = amplituda + krok;		//nastepuje przepelnienie zakresu
	}
}


void step_by_step()					//funkcja realizuj¹ca filtracje metod¹ "próbka po próbce" uzywajac funkcji z DSPLIB
{
	int z;
	for(z=0; z<num_samples; z++)	//petla iterujaca po wielkosci sygna³u
	{
		//uzycie funkcji z DSPLIB; przedostatni argument funkcji równy 1 gdyz iterujemy jedna probke
		fir((DATA*)&samples2[z], (DATA*)cff, (DATA*)&samples[z], (DATA*)buff , 1, num_filter);
	}
}


void main(void) {

	//rand16init();										//funckja generujaca liczby losowe
	//rand16((DATA*)samples2, num_samples); 			//funkcja generujaca szum z bilbioteki DSPlib

	saw(samples2, num_samples, d);				//funckja generujaca sygnal piloksztaltny

	//blockfir(samples2, cff, samples2, num_samples, num_filter);	//uruchomienie wlasnej funkcji filtruj¹cej filtrem FIR


	//fir((DATA*)samples2, (DATA*)cff, (DATA*)samples, (DATA*)buff , num_samples, num_filter);	//wywolanie funckji z DSPLIB

	step_by_step();		//implementacja funkcji z zadania 4


	while (1); // do not exit
}
