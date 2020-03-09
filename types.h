/*
    FB01 : Sound editor
    Copyright Meslin Frédéric 2009 - 2010
    fredericmeslin@hotmail.com

    This file is part of FB01 SE

    FB01 SE is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    FB01 SE is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with FB01 SE.  If not, see <http://www.gnu.org/licenses/>.
*/

//Constantes globales
    #define VERSION 2

//Types spéciaux globaux
    #ifndef uchar
    #define uchar  char  unsigned
    #define ushort short unsigned
    #define uint   int   unsigned
    #define ulong  long  unsigned
    #endif

    #ifndef int8
    #define int8   char
    #define uint8  char  unsigned
    #define int16  short
    #define uint16 short unsigned
    #define int32  long
    #define uint32 long  unsigned
    #endif

//Macros globales
    #ifndef RAND
    #define RAND(a, b) (a + (rand() * (b - a + 1)) / RAND_MAX)
    #endif

    #ifndef min
    #define min(a, b) (a < b ? a : b)
    #define max(a, b) (a > b ? a : b)
    #endif
