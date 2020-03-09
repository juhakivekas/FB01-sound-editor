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

#ifndef LINUX_H
#define LINUX_H
#ifdef LINUX

//Inclusions génériques
extern "C"
{
    #include "X11/Xlib.h"
    #include "X11/keysym.h"
    #include <unistd.h>
    #define stdSleep(t) usleep(t * 1000)
}

//Inclusions relatives au MIDI
extern "C"
{
    #include "pthread.h"
    #include "alsa/asoundlib.h"
}

#endif
#endif // LINUX_H
