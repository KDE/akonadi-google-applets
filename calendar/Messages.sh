#!/bin/bash
$EXTRACTRC `find . -name \*.ui` >> rc.cpp
$XGETTEXT `find . -name \*.cpp` -o $podir/plasma_applet_akonadi_calendars.pot
rm -f rc.cpp