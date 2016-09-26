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

#ifndef MYFFT
#define MYFFT
#include <fftw3.h>
#include <math.h>
#include <string.h>

#define Float double
#define TWO_PI (M_PI * 2.0)

/*--------------------------------- Global variables --------------------------------------*/

/*-------------------------------- myfft.c functions  -------------------------------------*/

Float mus_linear_to_db(Float x);
Float mus_radians_to_hz(Float rads);
Float mus_hz_to_radians(Float hz);
Float mus_degrees_to_radians(Float degree);
Float mus_radians_to_degrees(Float rads);
Float mus_db_to_linear(Float x);

void Hanning(Float *win, int n);
void mus_fftw(Float *rl, int n, int dir);
void fourier_spectrum(Float *sf, Float *fft_data, int fft_size, int data_len, Float *window, int win_len);
bool isNaN(double val);

#endif
