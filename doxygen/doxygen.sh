#!/bin/bash
#
# Name:        doxygen.sh
# Purpose:     Doxygen manual generator script
# Revision:    $Hash$
# Licence:     wxWindows licence
#
# remember current folder and then cd to the docs/doxygen one
me=$(basename $0)
path=${0%%/$me}        # path from which the script has been launched
current=$(pwd)
cd $path
export WXGUIDESIGNER=`cd .. && pwd`

cfgfile="Doxyfile"

doxygen $cfgfile

# Doxygen has the annoying habit to put the full path of the
# affected files in the log file; remove it to make the log
# more readable
currpath=`pwd`/
interfacepath=`cd ../interface && pwd`/
cat doxygen.log | sed -e "s|$currpath||g" -e "s|$interfacepath||g" > temp
cat temp > doxygen.log
rm temp

# return to the original folder from which this script was launched
cd $current
