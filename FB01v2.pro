#############################################################################
# FB01 Sound Editor V2.1
# Makefile LINUX
# Copyright Meslin Frédéric 2009 - 2010
# This file is part of FB01 SE
#
# FB01 SE is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# FB01 SE is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with FB01 SE.  If not, see <http://www.gnu.org/licenses/>.
#############################################################################

OTHER_FILES += FB01.rc
HEADERS += types.h \
    linux.h \
    win32.h \
    editeur.h \
    core/midi.h \
    core/block.h \
    core/edit.h \
    core/set.h \
    core/operateur.h \
    core/instrument.h \
    core/voice.h \
    core/bank.h \
    core/config.h \
    core/bank_voice.h \
    core/automation.h \
    core/automated.h \
    core/periph.h \
    interface/qoperateur.h \
    interface/qbutton.h \
    interface/qclavier.h \
    interface/qvoice.h \
    interface/qinstrument.h \
    interface/qenveloppe.h \
    interface/mainwindow.h \
    interface/qbank.h \
    interface/qautomation.h \
    interface/qconfig.h \
    excep/memory_ex.h \
    excep/midi_ex.h \
    excep/ex.h \
    excep/automation_ex.h
SOURCES += editeur.cpp \
    core/set.cpp \
    core/midi.cpp \
    core/block.cpp \
    core/voice.cpp \
    core/operateur.cpp \
    core/instrument.cpp \
    core/bank.cpp \
    core/config.cpp \
    core/edit.cpp \
    core/bank_voice.cpp \
    core/automation.cpp \
    core/periph.cpp \
    interface/qoperateur.cpp \
    interface/qbutton.cpp \
    interface/qclavier.cpp \
    interface/qvoice.cpp \
    interface/qinstrument.cpp \
    interface/qenveloppe.cpp \
    interface/qbank.cpp \
    interface/mainwindow.cpp \
    interface/qautomation.cpp \
    interface/qconfig.cpp \
    excep/memory_ex.cpp \
    excep/midi_ex.cpp \
    excep/ex.cpp \
    excep/automation_ex.cpp
RESOURCES += FB01.qrc
FORMS += interface/qoperateur.ui \
    interface/qinstrument.ui \
    interface/qvoice.ui \
    interface/mainwindow.ui \
    interface/qbank.ui \
    interface/qconfig.ui \
    interface/qautomation.ui
winnt { 
    DEFINES += WIN32
    LIBS    += -luser32 -lwinmm
    RC_FILE  = FB01.rc
}
win32 { 
    DEFINES += WIN32
    LIBS    += -luser32 -lwinmm
    RC_FILE  = FB01.rc
}
linux {
    DEFINES += LINUX
    LIBS += -lasound
}
unix { 
    DEFINES += LINUX
    LIBS += -lasound
}
