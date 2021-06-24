cd source

rem
cl /c /O2 /W3 /I../include /GX big.cpp
cl /c /O2 /W3 /I../include /GX zzn.cpp
cl /c /O2 /W3 /I../include /GX ecn.cpp
cl /c /O2 /W3 /I../include /GX ec2.cpp
cl /c /O2 /W3 /I../include /GX flash.cpp

rem Compile and link example programs

cl /c /O2 /W3 /I../include /GX hail.cpp
link hail.obj big.obj miracl.lib

cl /c /O2 /W3 /I../include /GX palin.cpp
link palin.obj big.obj miracl.lib

cl /c /O2 /W3 /I../include /GX mersenne.cpp
link mersenne.obj big.obj miracl.lib

cl /c /O2 /W3 /I../include /GX brute.cpp
link brute.obj big.obj miracl.lib

cl /c /O2 /W3 /I../include /GX brent.cpp
link brent.obj big.obj zzn.obj miracl.lib

cl /c /O2 /W3 /I../include /GX pollard.cpp
link pollard.obj big.obj zzn.obj miracl.lib

cl /c /O2 /W3 /I../include /GX williams.cpp
link williams.obj big.obj zzn.obj miracl.lib

cl /c /O2 /W3 /I../include /GX lenstra.cpp
link lenstra.obj big.obj zzn.obj miracl.lib

cl /c /O2 /W3 /I../include /GX crt.cpp
cl /c /O2 /W3 /I../include /GX qsieve.cpp
link qsieve.obj crt.obj big.obj zzn.obj miracl.lib

cl /c /O2 /W3 /I../include /GX factor.c
link factor.obj big.obj zzn.obj miracl.lib

cl /c /O2 /W3 /I../include /GX kangaroo.cpp
link kangaroo.obj big.obj zzn.obj miracl.lib

cl /c /O2 /W3 /I../include /GX genprime.cpp
link genprime.obj big.obj zzn.obj miracl.lib

cl /c /O2 /W3 /I../include /GX index.cpp
link index.obj crt.obj big.obj zzn.obj miracl.lib

cl /c /O2 /W3 /I../include /GX pk-demo.cpp
link pk-demo.obj big.obj ecn.obj miracl.lib

cl /c /O2 /W3 /I../include bmark.c
link bmark.obj miracl.lib

cl /c /O2 /W3 /I../include /GX genkey.cpp
link genkey.obj big.obj zzn.obj miracl.lib

cl /c /O2 /W3 /I../include /GX encode.cpp
link encode.obj big.obj zzn.obj miracl.lib

cl /c /O2 /W3 /I../include /GX decode.cpp
link decode.obj big.obj zzn.obj miracl.lib

cl /c /O2 /W3 /I../include /GX enciph.cpp
link enciph.obj big.obj zzn.obj miracl.lib

cl /c /O2 /W3 /I../include /GX deciph.cpp
link deciph.obj big.obj zzn.obj miracl.lib

cl /c /O2 /W3 /I../include /GX dssetup.cpp
link dssetup.obj big.obj zzn.obj miracl.lib

cl /c /O2 /W3 /I../include /GX limlee.cpp
link limlee.obj big.obj zzn.obj miracl.lib

cl /c /O2 /W3 /I../include /GX dssgen.cpp
link dssgen.obj big.obj zzn.obj miracl.lib

cl /c /O2 /W3 /I../include /GX dssign.cpp
link dssign.obj big.obj zzn.obj miracl.lib

cl /c /O2 /W3 /I../include /GX dssver.cpp
link dssver.obj big.obj zzn.obj miracl.lib

cl /c /O2 /W3 /I../include /GX ecsgen.cpp
link ecsgen.obj ecn.obj big.obj zzn.obj miracl.lib

cl /c /O2 /W3 /I../include /GX ecsign.cpp
link ecsign.obj ecn.obj big.obj zzn.obj miracl.lib

cl /c /O2 /W3 /I../include /GX ecsver.cpp
link ecsver.obj ecn.obj big.obj zzn.obj miracl.lib

cl /c /O2 /W3 /I../include /GX ecsgen2.cpp
link ecsgen2.obj ecn.obj ec2.obj big.obj zzn.obj miracl.lib

cl /c /O2 /W3 /I../include /GX ecsign2.cpp
link ecsign2.obj ecn.obj ec2.obj big.obj zzn.obj miracl.lib

cl /c /O2 /W3 /I../include /GX ecsver2.cpp
link ecsver2.obj ecn.obj ec2.obj big.obj zzn.obj miracl.lib

cl /c /O2 /W3 /I../include /GX curve/cm.cpp
cl /c /O2 /W3 /I../include /GX curve/poly.cpp 
cl /c /O2 /W3 /I../include /GX curve/flpoly.cpp 
cl /c /O2 /W3 /I../include /GX curve/complex.cpp
cl /c /O2 /W3 /I../include /GX floating.cpp
link cm.obj poly.obj flpoly.obj complex.obj floating.obj big.obj zzn.obj ecn.obj miracl.lib

cl /c /O2 /W3 /I../include /GX curve/schoof.cpp
cl /c /O2 /W3 /I../include /GX curve/polymod.cpp
link schoof.obj crt.obj poly.obj polymod.obj flpoly.obj complex.obj floating.obj big.obj zzn.obj ecn.obj miracl.lib

cl /c /O2 /W3 /I../include /GX gf2m.cpp
cl /c /O2 /W3 /I../include /GX curve/schoof2.cpp
cl /c /O2 /W3 /I../include /GX curve/poly2.cpp 
cl /c /O2 /W3 /I../include /GX curve/poly2mod.cpp 
link schoof2.obj crt.obj gf2m.obj poly2.obj poly2mod.obj flpoly.obj complex.obj floating.obj big.obj zzn.obj ec2.obj miracl.lib

cl /c /O2 /W3 /I../include /GX curve/ps_big.cpp
cl /c /O2 /W3 /I../include /GX curve/mueller.cpp
link mueller.obj ps_big.obj big.obj miracl.lib

cl /c /O2 /W3 /I../include /GX curve/modpol.cpp
cl /c /O2 /W3 /I../include /GX curve/ps_zzn.cpp
link modpol.obj ps_zzn.obj zzn.obj big.obj miracl.lib

cl /c /O2 /W3 /I../include /GX curve/process.cpp
link process.obj crt.obj poly.obj polymod.obj flpoly.obj complex.obj floating.obj big.obj zzn.obj ecn.obj miracl.lib

cl /c /O2 /W3 /I../include /GX curve/sea.cpp
cl /c /O2 /W3 /I../include /GX curve/polyxy.cpp
link sea.obj crt.obj poly.obj polymod.obj polyxy.obj flpoly.obj complex.obj floating.obj big.obj zzn.obj ecn.obj miracl.lib


copy *.exe ..\release
mkdir ..\release\include
copy ..\include\*.* ..\release\include
cd ..
