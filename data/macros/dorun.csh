#!/bin/tcsh

setenv filecatalog $1
@ index = $2
echo "file catalog: ${filecatalog} | index number: ${index} | number of events: 0"
@ indexp1 = ${index} + 1

setenv run `head -$indexp1 ${filecatalog} | tail -1 | awk '{print $1}'`
echo "run number: ${run}"
setenv seg `head -$indexp1 ${filecatalog} | tail -1 | awk '{print $2}'`
echo "seg number: ${seg}"

env ODBCINI=/opt/phenix/etc/odbc.ini.mirror root.exe -b -q RunMyMacro.C\(1,0,${run},${seg},\"${filecatalog}\",\"outputall\"\)
exit
