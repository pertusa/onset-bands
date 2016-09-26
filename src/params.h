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

#ifndef PARAMS
#define PARAMS
#include <math.h>

#define DIVFACTOR	2147483000.0	// 2^21 Divide time-domain signal samples by this value to normalize them

/* Spectrum analysis parameters */

#define kDR 	 	1       // Decimation ratio applied (important: no low-pass filtering done!)
#define kN		1	// Input window size (0->43ms, 1->96ms, 2->192ms)
#define kRES	 	4       // winsize=winsize*res (Zero padding) for 44kHz. If sr=22kHz, this value is multiplied by two
#define kPERCENTAGE	50	// Overlapping percentage

/* Parameters for onset detection */

#define kSILENCE22	70.0    // Default silence threshold (3.0 without RMS, 70 with RMS)
#define kSILENCE44	140.0 //200.0   // Default silence threshold for 44 kHz
#define NEWPAR		0.18	// Threshold at the end of onset detection stage. 0.18 without RMS
//#define NEWPARCONTEXT	0.2 

#define APPLYRMS	true	// If true, RMS is applied to build the bands

/* Band parameters */

#define kMINBANDFREQ	48.9994087219		// MidiNote= G0 (1st note of piano-roll)
const double stepinterbands=pow(2.0,1.0/24.0);	// Bands separation (Hz)

#endif
