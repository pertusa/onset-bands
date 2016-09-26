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

#ifndef BANDS
#define BANDS

#include <stdlib.h>
#include <iostream>
#include <vector>

#define vectorbands vector<band>

using namespace std;

class band {
  public:
	band();

	band(const band& bandcopy);

	// Creates a triangular window, from low to high bin. Window values rate from 0 to 1

	band(int low, int center, int high, double center_freq);
	band(double low, double center, double high, double center_freq);

	// Returns value of that band for a given FFT values
	double applyband(double *fft, int fftlen);
	double applybanddB(double *fft, int fftlen);
	double applybandRMS(double *fft, int fftlen);
	double applybandNorm(double *fft, int fftlen);

  	int lowsample;
  	int highsample;
  	int centersample;
  	int size;
	int effectivesize;
	double centerfreq;
	double sumvalues;
  	vector<double> values;

};

ostream &operator<<(ostream &output, const band b);

void generatebands(double min_freq, double max_freq, vectorbands &spectralbands, double freq_resolution);
void generatehalfbands(double min_freq, double max_freq, vectorbands &spectralbands, double freq_resolution);
void generateexacthalfbands(double min_freq, double max_freq, vectorbands &spectralbands, double freq_resolution);

#endif
