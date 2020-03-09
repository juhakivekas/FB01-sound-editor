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

#ifndef MIDI_H
#define MIDI_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../types.h"
#include "../win32.h"
#include "../linux.h"

#include "automation.h"

#include "../excep/midi_ex.h"
#include "../excep/memory_ex.h"

class MIDI
{
public:
//Enumération des drivers
    static void EnumererDrivers();
    static void LibererDrivers();
//Récupération du nom
    static const char * DriverIn(const int index);
    static const char * DriverOut(const int index);
    static uint NbDriversIn();
    static uint NbDriversOut();
//Activation des drivers
    static void ActiverIn(const int index);
    static void ActiverOut(const int index);
    static void ActiverCtrl(const int index);
//Etat des drivers
    static bool InOk();
    static bool OutOk();
    static bool CtrlOk();
//Envoi de messages
    static void EnvMsg(uchar * msg);
    static void EnvSysEx(uchar * sysEx, const int taille, const bool reponse = false);
    static void RecSysEx(uchar * sysEx, const int taille);
//Paramêtres de transmission
    static void  ChoisirMidiChannel(const uchar channel);
    static uchar MidiChannel();
    static void  ChoisirVelocity(const uchar velocity);
    static uchar Velocity();
    static void  ChoisirSysChannel(const uchar channel);
    static uchar SysChannel();
//Relaye les messages courts reçus vers la sortie
    static void ActiverINToOUT(const bool active);
    static void ActiverCTRLToOUT(const bool active);
//Envoi de notes
    static void NoteOn(const uchar note);
    static void NoteOff(const uchar note);
    static void AllNotesOff();
//Backup du tampon de réception
    static bool BackupTampon(char * Chemin);
private:
//Constantes communication
    #define MIDI_LEN_TAMPON 0x2000
    #define MIDI_ATTENTE 10000
    #define MIDI_PAUSE_MESSAGE 10
    #define MIDI_PAUSE_THREAD 1
//Constantes drivers
    #define MIDI_MAX_DRIVERS 32
    #define MIDI_LEN_CHEMIN 40
//Structures des drivers
    typedef struct {
        void * descr;
        char   chemin[MIDI_LEN_CHEMIN];
        union {
            uint   handleNum;
            void * handleStruct;
        };
    }DriversStr;
//Objets de la communication
    static DriversStr ins[MIDI_MAX_DRIVERS];
    static DriversStr outs[MIDI_MAX_DRIVERS];
    static uint nbIns, nbOuts;
    static int indIn, indOut, indCtrl;
//Paramêtres de communication
    static uchar midiChannel;
    static uchar velocity;
    static uchar sysChannel;
//Paramêtres de relai
    static bool relaiIN;
    static bool relaiCTRL;
//Tampon de réception
    static uchar tampon[MIDI_LEN_TAMPON];
    static bool  prepare;
    static bool  attente;
//Gestion interne des drivers
    static void AjouterDriverIn(void * descr, const char * chemin);
    static void AjouterDriverOut(void * descr, const char * chemin);
//Déclarations spécifiques windows
#ifdef WIN32
//Statut des tampons
    static MIDIHDR header;
//Gestion des tampons
    static void PreparerTampon();
    static void DePreparerTampon();
//Callbacks MIDI
    static void WINAPI CallbackIn(uint hmi, uint msg, uint instance, uint param1, uint param2);
    static void WINAPI CallbackCtrl(uint hmi, uint msg, uint instance, uint param1, uint param2);
#endif
//Déclarations spécifiques linux
#ifdef LINUX
//Structure de réception
    typedef struct {
        uchar tamponSysEx[MIDI_LEN_TAMPON];
        uchar tamponNormal[4];
        bool recSysEx, recNormal;
        uint posSysEx, posNormal;
    }ReceptionStr;
//Réception des données
    static void RecevoirMessages(snd_rawmidi_t * handle, ReceptionStr * rec, bool * okNormal, bool * okSysEx);
//Threads MIDI
    static void * CallbackIn(void * param);
    static void * CallbackCtrl(void * param);
    static pthread_t threadIn,threadCtrl;
    static bool runIn, runCtrl;
#endif
};


#endif // MIDI_H
