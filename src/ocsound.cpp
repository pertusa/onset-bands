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

#include "ocsound.h"

void generateorc(char *filename, int samplerate)
{
   ofstream fd(ORCFILE, ios::out | ios::trunc);

   if (!fd.is_open()) {
            cerr << "cannot open " << ORCFILE <<  endl;
            exit (-1);
   }

   if (samplerate==44100) fd << kORC44;
   else if (samplerate==22050) fd << kORC22;
   else {
     cerr << "Internal error (samplerate) in generateorc" << endl;
     exit(-1);
   }
   fd << filename << kORC2;

   fd.close();

/***
sr = 44100
kr = 4410
nchnls = 1

instr 1
  idur = p3
  ain  soundin  "piano4.wav"
           out      ain
endin

instr 2
  idur = p3
;  asal rand     20000
  asal oscil  15000, 1000, 102 ; amp, frec, numtabla
       out      asal
endin
*****/

}

void generatesco(double *onsetsvect, int ntime, double time_res) 
{
   ofstream fd(SCOFILE, ios::out | ios::trunc);

   if (!fd.is_open()) {
            cerr << "cannot open " << SCOFILE <<  endl;
            exit (-1);
   }

   fd << kSCO1 << ntime*time_res << kSCO2;

   for (int i=0; i<ntime; i++)
   {
	if (onsetsvect[i]!=0)
		fd << "i 2 " << i*time_res << " 0.01\n";
   }

   fd.close();




/**
t 0 60 ; tempo = 60 BPM constante para tener los tiempos en segundos

f 102 0 1024 10 1 ; generacion de la tabla con la onda sinusoidal

i 1 0 150 ; sonido original desde inicio (0) hasta duracion (150)

i 2 0.0697 0.01 ; beep en tiempo de duracion 0.01 s
i 2 1.1842 0.01 
i 2 2.4845 0.01 
i 2 4.3421 0.01 
i 2 4.8994 0.01 
i 2 5.3174 0.01 
i 2 5.7818 0.01 
i 2 6.2462 0.01 
i 2 6.4319 0.01 
i 2 6.9428 0.01 
i 2 8.2431 0.01 

**/

}

