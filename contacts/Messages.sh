#!/bin/bash
$EXTRACTRC `find . -name \*.ui` >> rc.cpp
$XGETTEXT `find . -name \*.cpp` -o $podir/plasma_applet_akonadi_contacts.pot
rm -f rc.cpp