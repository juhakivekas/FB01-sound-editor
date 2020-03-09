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

# General compiling & installing rules
all: release
    
install: FB01v2
	cp FB01v2 /usr/bin
	mkdir /usr/share/fb01-soundeditor
	cp -r ./imgs /usr/share/fb01-soundeditor
	cp -r ./help /usr/share/fb01-soundeditor
	cp fb01-soundeditor.desktop /usr/share/applications
	chmod +rwx -R /usr/share/fb01-soundeditor
	
uninstall: FB01v2
	rm /usr/bin/FB01v2
	rm -r /usr/share/fb01-soundeditor
	rm /usr/share/applications/fb01-soundeditor.desktop
	
# Cleaning rules
clean: clean.release clean.debug
clean.release: Makefile.Release
	make -f Makefile.Release clean
	rm Makefile.Release
	
clean.debug: Makefile.Debug
	make -f Makefile.Debug clean
	rm Makefile.Debug	
	
# Project compiling rules
release:
	qmake-qt4 FB01v2.pro -spec linux-g++ -r -o Makefile.Release
	make -f Makefile.Release
	
debug:
	qmake-qt4 FB01v2.pro -spec linux-g++ -r -o Makefile.Debug CONFIG+=debug
	make -f Makefile.Debug
