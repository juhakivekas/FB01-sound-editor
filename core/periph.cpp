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

#include "periph.h"

//Spécificités Linux
#ifdef LINUX
    Display * Periph::display = NULL;
#endif

/*****************************************************************************/
void Periph::Initialiser(void * X11display)
{
#ifdef LINUX
    display = (Display *) X11display;
#endif
}

void Periph::Terminer()
{
}

/*****************************************************************************/
bool Periph::ToucheASCII(const char code)
{
#ifdef WIN32
    short key = VkKeyScan(code);
    if (key == 0xFFFF) return false;
    return (GetAsyncKeyState(key & 0xFF) & 0x8000) > 0;
#endif
#ifdef LINUX
    char chaine[2] = {0, 0}, keys[32];
//Convertit le code ASCII en code touche
    chaine[0] = code;
    KeySym keysym = XStringToKeysym(chaine);
    if (keysym == NoSymbol) return false;
    KeyCode keycode = XKeysymToKeycode(display, keysym);
    if (keycode == 0) return false;
//Vérifie la touche
    XQueryKeymap(display, keys);
    char k = keys[keycode / 8];
    return (k >> (keycode % 8)) & 1;
#endif
}

/*****************************************************************************/
bool Periph::ToucheShift()
{
#ifdef WIN32
    return (GetAsyncKeyState(VK_SHIFT) & 0x8000) > 0;
#endif
#ifdef LINUX
    return EtatToucheSpeciale(XK_Shift_L) || EtatToucheSpeciale(XK_Shift_R);
#endif
}

bool Periph::ToucheCtrl()
{
#ifdef WIN32
    return (GetAsyncKeyState(VK_CONTROL) & 0x8000) > 0;
#endif
#ifdef LINUX
    return EtatToucheSpeciale(XK_Control_L) || EtatToucheSpeciale(XK_Control_R);
#endif
}

bool Periph::ToucheAlt()
{
#ifdef WIN32
    return (GetAsyncKeyState(VK_MENU) & 0x8000) > 0;
#endif
#ifdef LINUX
    return EtatToucheSpeciale(XK_Alt_L) || EtatToucheSpeciale(XK_Alt_R);
#endif
}

/*****************************************************************************/
#ifdef LINUX
bool Periph::EtatToucheSpeciale(KeySym keysym)
{
    char keys[32];
//Récupère le code touche
    KeyCode keycode = XKeysymToKeycode(display, keysym);
    if (keycode == 0) return false;
//Vérifie la touche
    XQueryKeymap(display, keys);
    char k = keys[keycode / 8];
    return(k >> (keycode % 8)) & 1;
}
#endif
