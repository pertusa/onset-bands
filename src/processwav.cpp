/****************
Copyright (c) 2005-2009 Antonio Pertusa Ibáñez <pertusa@dlsi.ua.es>
Copyright (c) 2005-2009 José Manuel Iñesta Quereda <inesta@dlsi.ua.es>
Copyright (c) 2009 Universidad de Alicante


This onset detection system is free software: you can redistribute it and/or
 modify it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or (at your
 option) any later version.

This onset detection system is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this code.  If not, see <http://www.gnu.org/licenses/>.

 Comments are welcomed
 
**************/

#include "processwav.h"

/* Global variables, for efficiency */ 

SNDFILE *wavfile;
SF_INFO wavinfo;
int first_window;
int *buf;
int *bufaux;
Float *data;
double maxval=-200.0;
Float *window;	   	// Window applied for FFT 
pinfo spectruminfo;

/*--------------------------------------------
  read wavefile
----------------------------------------------*/

void open_wavfile(char* name) 
{
	if (!(wavfile= sf_open(name, SFM_READ, &wavinfo))) 
	{
	 	printf("Error : File %s can not be processed.\n", name);
	 	sf_perror (NULL);
	 	exit(1);
	}
}

/*-------------------------------------------------
 Get input vector from wav file. 
--------------------------------------------------*/

int get_samples(int len, int dr) 
{
	int i,j,k,n_samples,sum;

	if (dr==1) return sf_read_int(wavfile, buf, len);
	else {
		n_samples=sf_read_int(wavfile, bufaux,len*dr);

		for (i=0, j=0; i<len && j<n_samples; i++) {
			sum=0;
			for (k=0;k<dr;j++,k++) 
				sum+=bufaux[j];
			buf[i]=sum/dr;
		}
	        return n_samples/dr;
	}
}

/*----------------------------------------------
 Read wave data (only 16 bits?)
-----------------------------------------------*/

int read_data(Float *input) {

	long nread;
	int i,j;
	int overlapped=0;
	int len;
	
	if (spectruminfo.percentage!=0) 
		overlapped=(int)round(100.0/(100-spectruminfo.percentage));

	// First window data
	
	if (first_window || spectruminfo.percentage==0) 
	{
		nread = get_samples(spectruminfo.N,spectruminfo.dr);
	
		for (i=0; i<nread; i++) 
 		   data[i]=(buf[i]/DIVFACTOR);  // 2^21
		len=nread;
	}
	
	// Other windows data (for overlapping)
		
	else {
		for (j=spectruminfo.N/overlapped,i=0; j<spectruminfo.N; j++,i++)
			data[i]=data[j];
		
		nread = get_samples((spectruminfo.N)-i,spectruminfo.dr);
		len=i+nread;
		
	  	for (j=0; i<len; i++,j++)	
			data[i]=(buf[j]/DIVFACTOR);
	}

	// Copy output data

	for (i=0; i<len; i++) 
		input[i]=data[i];
	
	if (len<spectruminfo.N) return 0;
	return len;
}

/*-------------------------------------------
 
 Initialize FFT parameters

------------------------------------------------*/

void initialize_spectrum_parameters() 
{
	/* Create info data */

	spectruminfo.dr=kDR;		// Decimation ratio (fs=fs/dr)
	spectruminfo.percentage=kPERCENTAGE;	// Overlapping percentage

	if (wavinfo.samplerate==44100) {
		spectruminfo.res=kRES;

		// Input window size
		if (kN==0) 
			spectruminfo.N=2048;
		else if (kN==1) 
			spectruminfo.N=4096;		
		else if (kN==2) 
			spectruminfo.N=8192;
		else cerr << "Internal error! kN unsupported";
	}
	else if (wavinfo.samplerate==22050) {
		spectruminfo.res=kRES;	// Resolution. (winsize=winsize*res). Adds zeroes at the end of the window

		if (kN==0) 
			spectruminfo.N=1024;
		else if (kN==1) 
			spectruminfo.N=2048;		
		else if (kN==2) 
			spectruminfo.N=4096;
		else cerr << "Internal error! kN unsupported";
	}
	else {
		cerr << "Samplerate" << wavinfo.samplerate << "not supported. Use samplerate 22050 or 44100" << endl;
		exit(-1);
	}

	spectruminfo.first_band_freq=kMINBANDFREQ;	// Minimum frequency for band processing

	spectruminfo.samplerate= (wavinfo.samplerate)/(spectruminfo.dr);
	spectruminfo.freq_resolution=(Float)(spectruminfo.samplerate)/((Float)(spectruminfo.N)*spectruminfo.res);
	spectruminfo.winsize= spectruminfo.N*spectruminfo.res;
	spectruminfo.firstsample=(int)round(spectruminfo.W/spectruminfo.freq_resolution); 	// Convert bandwidth (Hz->Samples)
	if (spectruminfo.percentage!=0) spectruminfo.time_resolution= (1.0-((Float)spectruminfo.percentage/100.0))* (spectruminfo.N) / spectruminfo.samplerate;
	else spectruminfo.time_resolution=(Float)(spectruminfo.N)/(Float)(spectruminfo.samplerate);

	spectruminfo.min_sample=(int)floor(spectruminfo.first_band_freq/spectruminfo.freq_resolution);
}


void memoryallocation() {
	buf = (int*)malloc(sizeof(int)*(spectruminfo.winsize));
	bufaux = (int*)malloc(sizeof(int)*(spectruminfo.winsize)*(spectruminfo.dr));
	data = (Float*)malloc(sizeof(Float)*(spectruminfo.winsize));
	window = (Float*)malloc(sizeof(Float)*(spectruminfo.winsize));
}


void computebands(double *fft, vectorbands& spectralbands, vector<double>& bandvalues, int
fftlen, double& maxbandval) 
{
	if (!bandvalues.empty()) bandvalues.clear();

	for (int i=0; i<(int)spectralbands.size(); i++) {
		double val;
		if (spectralbands[i].size!=0) {
		   if (APPLYRMS)
			val=spectralbands[i].applybandRMS(fft,fftlen);
		   else val=spectralbands[i].applyband(fft,fftlen);
		}
		else val=0.0;
		if (val>maxbandval) maxbandval=val;
		bandvalues.push_back(val);
	}
}

int input_data_wav(char *name , vectorbands& spectralbands, Tmatrix& outbandvalues, pinfo& tmpspectruminfo) 
{
	// Open wavfile
	open_wavfile(name);

	// Initialize spectrum parameters (spectruminfo) 
	initialize_spectrum_parameters();

	// memory allocation for input data
	memoryallocation();
	
	Float input[spectruminfo.winsize];
	Float spectrum[spectruminfo.winsize+1];

	// Time axes 
	Float curr_time=(spectruminfo.time_resolution)/2.0;

	// Creation of the Hanning window 
	Hanning(window,spectruminfo.N);

	// Initialize sinusoidal likeness 
	//double Hnorm=initializeSinusoidal();

	// Bands generation
	generatebands(spectruminfo.first_band_freq, spectruminfo.samplerate/2, spectralbands, spectruminfo.freq_resolution);
	spectruminfo.numbands=spectralbands.size();

	first_window=true;
	int len;
	int n_time=0;

	while ((len=read_data(input))) 
	{
		first_window=false;
		
		// Complete input window with zeroes 
		for (int i=len; i<spectruminfo.winsize; i++)
			input[i]=0.0;

		// Update current time 
		curr_time+=spectruminfo.time_resolution;

		// FFT computation
		fourier_spectrum(input, spectrum, spectruminfo.winsize, spectruminfo.winsize, window, spectruminfo.N);
		
		// Bands computation, using the spectrum obtained in the previous step
		outbandvalues.resize(n_time+1);
		outbandvalues[n_time].resize(spectruminfo.numbands);
		vector<double> bandval(spectruminfo.numbands);
		computebands(spectrum, spectralbands, bandval, spectruminfo.winsize / 2, spectruminfo.maxbandsval);
		outbandvalues.push_back(bandval);

		bandval.clear();
	
		n_time++;
	}

	free(buf);
	free(bufaux);
	sf_close(wavfile);
	tmpspectruminfo=spectruminfo;

	return n_time-1;
}
