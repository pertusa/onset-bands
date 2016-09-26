# Music note onset detection using one-semitone filterbanks

This is the method described in the following works:

```
"Note onset detection using one semitone filter-bank for MIREX 2009",
A. Pertusa and J. M. Iñesta,
in MIREX 2009 audio onset detection contest,
http://www.music-ir.org/mirex/2009/index.php/Audio_Onset_Detection_Results
```

```
"Recognition of note onsets in digital music using semitone bands",
A. Pertusa, A. Klapuri and J. M. Iñesta, 
Lecture notes in Computer Science, vol. 3773, pp 869-879, 2005.
```

## Compilation

To compile the algorithm in Linux or MacOS, type from the command line:

```
make
```

This software requires the libsndfile and fftw3 libraries. Both are standard in
Linux distributions and they can be obtained from the corresponding package
installer. In MacOS, they can be installed using darwinports. Alternatively,
these libraries can be downloaded from:

http://www.mega-nerd.com/libsndfile/
http://www.fftw.org/

## Usage

The basic syntax once compiled is:

```
./onset input.wav > onsets.txt
```

Some parameters can also be controlled from command line:

```
./onset input.wav [-c context | -t theta | -m mu | -csound | -o odffile ] > onsets.txt
```

where context is the number of aditional frames, theta is the onset
detection threshold, and mu is the silence threshold. The default values are
context=0, theta=0.18 and mu=70.

The input file must be WAV, MONO, and with fs= 22050 Hz or 44100 Hz. The
default output is a list of onset times in seconds printed in the standard
output.

The -csound option generates two files (onset.orc and onset.sco). They
contain the instructions to generate (using CSound) synthesized wavefiles
with added "ticks" in the times when onsets were detected. CSound comes as a
standard package in Linux and MacOS. It can also be downloaded from
www.csounds.com

The -o <filename> option prints the onset detection function into the
specified file.
