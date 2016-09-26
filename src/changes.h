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

#ifndef CHANGES
#define CHANGES

#include "params.h"
#include "defines.h"
#include "ocsound.h"
#include <math.h>
#include <iostream>
#include <vector>

using namespace std;

void peakselection(vector<double> &changesvect, int total_time); 
void applyderivative(vector<double> &onebandvector, int context);
void onsetfunction(Tmatrix bands, double maxbandsval, vector<double>& output, int context, float mu); 
void applythreshold(vector<double> &vectchanges, double maxbandsval, int context, float theta);
void computeonsets(int total_time, double *changesvect, Tmatrix &bands, double maxvalband, double timeres, int context, float theta, float mu);

#endif
