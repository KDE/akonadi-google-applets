#!/bin/bash
$EXTRACTRC `find . -name \*.ui` >> rc.cpp
$XGETTEXT `find . -name \*.cpp` -o $podir/plasma_calendar.pot
rm -f rc.cpp