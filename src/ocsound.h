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

#ifndef OCSOUND
#define OCSOUND

#include <stdlib.h>
#include <iostream>
#include <string>
#include <fstream>

using namespace std;

#define ORCFILE	"onset.orc"
#define SCOFILE "onset.sco"

const string kORC44="\nsr = 44100\nkr = 4410\nnchnls = 1\n\ninstr 1\n  idur = p3\n  ain  soundin  \"";
const string kORC22="\nsr = 22050\nkr = 2205\nnchnls = 1\n\ninstr 1\n  idur = p3\n  ain  soundin  \"";
const string kORC2="\"\n        out      ain\nendin\n\ninstr 2\n  idur = p3\n;  asal rand     20000\n  asal oscil  18000, 1000, 102 ; amp, frec, numtabla\n       out      asal\nendin\n ";
const string kSCO1="t 0 60 ; tempo = 60 BPM constant for having time measured in secs.\n\nf 102 0 1024 10 1 ; generation of the table with the sinusoidal waveshape\n\ni 1 0 ";
const string kSCO2="; Original sound from the beginning to the end\n\n";

void generateorc(char* filename, int sr);
void generatesco(double *onsets, int ntime, double time_res);

#endif
