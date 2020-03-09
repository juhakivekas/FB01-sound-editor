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

#include "block.h"

/*****************************************************************************/
Block::Block(uchar * sysEx, const uint lenSysEx, const uint nbParam, const uint offParam)
{
    this->sysEx    = sysEx;
    this->lenSysEx = lenSysEx;
    this->nbParam  = nbParam;
    this->offParam = offParam;
    envoi = false;
}

Block::~Block()
{
}

/*****************************************************************************/
uchar Block::LireParam(const uchar param)
{
    return 0;
}

void Block::EcrireParam(const uchar param, const uchar valeur)
{
}

/*****************************************************************************/
void Block::AutoriserEnvoi(const bool envoi)
{
    this->envoi = envoi;
}

bool Block::EnvoiAutorise()
{
    return envoi;
}

/*****************************************************************************/
void Block::Envoyer(const uchar param)
{
}

void Block::EnvoyerTout()
{
    if (!envoi) return;
    for (uchar i = 0; i < nbParam; i ++)
        Envoyer(i);
}

void Block::RecevoirTout()
{
}

/*****************************************************************************/
uchar Block::LireParam1Oct(const uchar param)
{
    if (lenSysEx == 0) return 0;
    return sysEx[param + offParam] & 0x7F;
}

void Block::EcrireParam1Oct(const uchar param, const uchar valeur)
{
    if (lenSysEx == 0) return;
    sysEx[param + offParam] = valeur & 0x7F;
    if (envoi) Envoyer(param);
}

/*****************************************************************************/
uchar Block::LireParam2Oct(const uchar param)
{
    uchar data;
    if (lenSysEx == 0) return 0;
    data  = sysEx[param * 2 + offParam] & 0xF;
    data += sysEx[param * 2 + offParam + 1] << 4;
    return data;
}

void Block::EcrireParam2Oct(const uchar param, const uchar valeur)
{
    if (lenSysEx == 0) return;
    sysEx[param * 2 + offParam] = valeur & 0xF;
    sysEx[param * 2 + offParam + 1] = valeur >> 4;
    if (envoi) Envoyer(param);
}

/*****************************************************************************/
void Block::Preparer(const uchar * entete, const uint lenEntete)
{
    if (lenSysEx == 0) return;
    memset(sysEx, 0, lenSysEx);
    memcpy(sysEx, entete, lenEntete);
    sysEx[lenSysEx-1] = 0xF7;
}

/*****************************************************************************/
uchar Block::CalculerCheckSum(const uint debut, const uint longueur)
{
    uchar sum = 0;
    if (lenSysEx == 0) return 0;
    for(uint i = 0; i < longueur; i ++)
        sum += sysEx[i + debut];
    return (-sum) & 0x7F;
}

void Block::VerifierCheckSum(const uint debut, const uint longueur, const uint position)
{
    uchar sum = CalculerCheckSum(debut, longueur);
    if (sum != sysEx[position]) {
    //Erreur de transmission
        char text[71];
        sprintf(text, "Bad checksum received :\nFB01 checksum %x, editor checksum %x",
                sysEx[position], sum);
        throw MIDI_ex(text);
    }
}
