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

#ifndef PROCESSWAV
#define PROCESSWAV

#include "myfft.h"
#include "info.h"
#include "params.h"
#include "bands.h"
#include "defines.h"
#include <sndfile.h>
#include <math.h>
#include <iostream>
#include <map>
#include <vector>
#include <stdlib.h>

using namespace std;


void open_wavfile(char* name);
int get_samples(int len, int dr);
int read_data(Float *input);
void initialize_spectrum_parameters();
int input_data_wav(char *name , vectorbands& spectralbands, Tmatrix& outbandvalues, pinfo& tmpspectruminfo); 

#endif
