rem MIRACL - IBM PC/MS-DOS Version 4.0
rem This batch files creates miracl.lib from its component parts
rem using the Microsoft 32-bit compiler, (as used in Windows '95
rem and Windows NT), and the Microsoft LIB librarian utility
rem Also included are the commands to create some of the example programs
rem
rem Read your compiler documentation for further information
rem 
rem Invoke as "ms32doit". It is assumed that paths have been correctly set up
rem to the compiler, librarian and linker.
rem
rem Provided mainly as a guide for creating a batch file tailored
rem specifically to your own configuration.
rem
rem Note - the module mrmuldv.c is not needed if MR_NOASM is defined
rem
rem NOTE - the flag /MT should be used for multithreaded applications
rem
rem Compile MIRACL modules
cd include
copy mirdef.h32 mirdef.h
cd ..

cd source
cl /c /O2 /W3 /I../include mrcore.c
cl /c /O2 /W3 /I../include mrarth0.c
cl /c /O2 /W3 /I../include mrarth1.c
cl /c /O2 /W3 /I../include mrarth2.c
cl /c /O2 /W3 /I../include mralloc.c
cl /c /O2 /W3 /I../include mrsmall.c
cl /c /O2 /W3 /I../include mrio1.c
cl /c /O2 /W3 /I../include mrio2.c
cl /c /O2 /W3 /I../include mrgcd.c
cl /c /O2 /W3 /I../include mrjack.c
cl /c /O2 /W3 /I../include mrxgcd.c
cl /c /O2 /W3 /I../include mrarth3.c
cl /c /O2 /W3 /I../include mrbits.c
cl /c /O2 /W3 /I../include mrrand.c
cl /c /O2 /W3 /I../include mrprime.c
cl /c /O2 /W3 /I../include mrcrt.c
cl /c /O2 /W3 /I../include mrscrt.c
cl /c /O2 /W3 /I../include mrmonty.c
cl /c /O2 /W3 /I../include mrpower.c
cl /c /O2 /W3 /I../include mrsroot.c
cl /c /O2 /W3 /I../include mrcurve.c
cl /c /O2 /W3 /I../include mrfast.c
cl /c /O2 /W3 /I../include mrlucas.c
cl /c /O2 /W3 /I../include mrzzn2.c
cl /c /O2 /W3 /I../include mrzzn2b.c
cl /c /O2 /W3 /I../include mrzzn3.c
cl /c /O2 /W3 /I../include mrzzn4.c
cl /c /O2 /W3 /I../include mrshs.c
cl /c /O2 /W3 /I../include mrshs256.c
cl /c /O2 /W3 /I../include mrshs512.c
cl /c /O2 /W3 /I../include mrsha3.c
cl /c /O2 /W3 /I../include mrfpe.c
cl /c /O2 /W3 /I../include mraes.c
cl /c /O2 /W3 /I../include mrgcm.c
cl /c /O2 /W3 /I../include mrstrong.c
cl /c /O2 /W3 /I../include mrbrick.c
cl /c /O2 /W3 /I../include mrebrick.c
cl /c /O2 /W3 /I../include mrgf2m.c
cl /c /O2 /W3 /I../include mrec2m.c
cl /c /O2 /W3 /I../include mrecn2.c
cl /c /O2 /W3 /I../include mrflash.c
cl /c /O2 /W3 /I../include mrfrnd.c
cl /c /O2 /W3 /I../include mrdouble.c
cl /c /O2 /W3 /I../include mrround.c
cl /c /O2 /W3 /I../include mrbuild.c
cl /c /O2 /W3 /I../include mrflsh1.c
cl /c /O2 /W3 /I../include mrpi.c
cl /c /O2 /W3 /I../include mrflsh2.c
cl /c /O2 /W3 /I../include mrflsh3.c
cl /c /O2 /W3 /I../include mrflsh4.c
copy mrmuldv.c32 mrmuldv.c
cl /c /O2 /W3 /I../include mrmuldv.c

rem
rem Create library 'miracl.lib'
del miracl.lib

lib /OUT:miracl.lib mrflsh4.obj mrflsh3.obj mrflsh2.obj mrpi.obj mrflsh1.obj
lib /OUT:miracl.lib miracl.lib mrdouble.obj mrflash.obj mrfrnd.obj mrround.obj mrbuild.obj
lib /OUT:miracl.lib miracl.lib mrio2.obj mrio1.obj mrrand.obj mrprime.obj mrcrt.obj mrscrt.obj mrfast.obj 
lib /OUT:miracl.lib miracl.lib mrjack.obj mrxgcd.obj mrgcd.obj  mrarth3.obj mrarth2.obj mrpower.obj mrsroot.obj
lib /OUT:miracl.lib miracl.lib mrmonty.obj mralloc.obj mrarth1.obj mrarth0.obj mrsmall.obj mrcore.obj mrmuldv.obj
lib /OUT:miracl.lib miracl.lib mrcurve.obj mrshs.obj mraes.obj mrlucas.obj mrstrong.obj mrbrick.obj mrbits.obj 
lib /OUT:miracl.lib miracl.lib mrshs256.obj mrshs512.obj mrebrick.obj mrgf2m.obj mrec2m.obj mrzzn2.obj mrzzn3.obj mrzzn4.obj
lib /OUT:miracl.lib miracl.lib mrecn2.obj mrzzn2b.obj mrgcm.obj mrfpe.obj mrsha3.obj

del mr*.obj
mkdir ..\release
copy miracl.lib ..\release

rem
cl /c /O2 /W3 /I../include /GX big.cpp
cl /c /O2 /W3 /I../include /GX zzn.cpp
cl /c /O2 /W3 /I../include /GX ecn.cpp
cl /c /O2 /W3 /I../include /GX ec2.cpp

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

cl /c /O2 /W3 /I../include /GX flash.cpp
copy *.exe ..\release
mkdir ..\release\include
copy ..\include\*.* ..\release\include
cd ..
