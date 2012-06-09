#!/bin/bash
############################################################################
## Name:        premake_gen.sh
## Purpose:     Premake 4 build script.
## Author:      Andrea Zanellato
## Modified by: 
## Created:     2011/10/22
## Revision:    $Hash$
## Copyright:   (c) Andrea Zanellato
## Licence:     GNU General Public License Version 2
############################################################################

# These works only on wxWidgets 2.8.10+
#wxcharset=`wx-config --query-chartype`
#wxversion=`wx-config --query-version`

# Parse command line options
wxmedia="yes"
wxroot=
wxstatic="no"
wxunicode="yes"
wxversion=

pmg_show_help()
{
    cmd=`wx-config --exec-prefix`
    if [ ! $cmd ]; then
        cmd="wx-config not found."
    fi
    echo "Available options:"
    echo
    echo " --wxmedia      Disable wxMedia library (wxMediaCtrl)."
    echo "                Example: --wxmedia=no to disable it."
    echo "                Default: $wxmedia"
    echo
    echo " --wxroot       Specify the wxWidgets build path,"
    echo "                useful for wxWidgets builds not installed"
    echo "                in your system (alternate/custom builds)"
    echo "                Example: --wxroot=/home/devel/wx/3.0/buildgtk"
    echo "                Current: $cmd"
    echo
    echo " --wxstatic     Whether to use a static build."
    echo "                Example: --wxstatic=no produces a DLL build."
    echo "                Default: $wxstatic"
    echo
    echo " --wxunicode    Whether to use an Unicode or an ANSI build."
    echo "                Ignored in wxWidgets 2.9 and later."
    echo "                Example: --wxunicode=no produces an ANSI build."
    echo "                Default: $wxunicode"
    echo
    exit $1
}

chk_yn()
{
    case "$2" in
        yes|no)
            return 0;;
        *)
            echo "$1: wrong value $2, use yes or no (lower case)."
            exit 1;;
    esac
}

haveroot=false
for args in "$@"
do
    param=${args%=*}
    value=${args#*=}

    if ( [ $param = "--help" ] || [ $param = "-h" ] );
        then
        pmg_show_help
    elif [ $param = "--wxmedia" ];
        then
        chk_yn $param $value
        wxmedia=$value
    elif [ $param = "--wxstatic" ];
        then
        chk_yn $param $value
        wxstatic=$value
    elif [ $param = "--wxunicode" ];
        then
        chk_yn $param $value
        wxunicode=$value
    elif [ $param = "--wxroot" ];
        then
        cmd=`$value/wx-config --prefix`
        if [ ! $cmd ]; then
            echo "error while executing wx-config from $value."
            exit 1
        fi
        wxroot=$value
        haveroot=true
    fi
done

# Autodetect wxWidgets version
cmdver=
if ( $haveroot ); then
    cmdver=`$wxroot/wx-config --version`
else
    cmdver=`wx-config --version`
fi

if [ ! $cmdver ]; then
    echo "error while executing wx-config."
    exit 1
fi
wxversion=`expr substr $cmdver 1 3`

# Autodetect OS
isbsd=`expr "$unamestr" : '.*BSD'`
platform="unknown"
unamestr=$(uname)

if ( [ "$isbsd" -gt "0" ] ); then
    platform="bsd"
elif [ "$unamestr" = "Linux" ]; then
    platform="linux"
elif [ "$unamestr" = "Darwin" ]; then
    platform="macosx"
fi

# Build premake
cd build
make CONFIG=Release -C./premake/$platform

./premake/$platform/bin/release/premake4 --file=./premake/solution.lua --wxmedia=$wxmedia --wxroot=$wxroot --wxstatic=$wxstatic --wxunicode=$wxunicode --wxversion=$wxversion codelite
./premake/$platform/bin/release/premake4 --file=./premake/solution.lua --wxmedia=$wxmedia --wxroot=$wxroot --wxstatic=$wxstatic --wxunicode=$wxunicode --wxversion=$wxversion codeblocks
./premake/$platform/bin/release/premake4 --file=./premake/solution.lua --wxmedia=$wxmedia --wxroot=$wxroot --wxstatic=$wxstatic --wxunicode=$wxunicode --wxversion=$wxversion gmake

#if [ "$platform" = "macosx" ]; then
#   ./premake4/macosx/bin/release/premake4 --file=./premake4/solution.lua xcode3
#fi

exit 0
