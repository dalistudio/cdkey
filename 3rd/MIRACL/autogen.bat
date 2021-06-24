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

