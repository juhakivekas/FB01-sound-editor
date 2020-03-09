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

#ifndef PERIPH_H
#define PERIPH_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../types.h"
#include "../win32.h"
#include "../linux.h"

#include "../excep/memory_ex.h"

class Periph
{
public:
//Initialisation du module
    static void Initialiser(void * X11display);
    static void Terminer();
//Détection de touches clavier
    static bool ToucheASCII(const char code);
    static bool ToucheShift();
    static bool ToucheCtrl();
    static bool ToucheAlt();
private:
//Déclarations spécifiques linux
#ifdef LINUX
    static Display * display;
    static bool EtatToucheSpeciale(KeySym keysym);
#endif
};

#endif // PERIPH_H
