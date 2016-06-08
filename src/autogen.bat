cl /c /O2 /W3 /I. /I../3rd/MIRACL/include base32.c
cl /c /O2 /W3 /I. /I../3rd/MIRACL/include hash.c
cl /c /O2 /W3 /I. /I../3rd/MIRACL/include sign.c
cl /c /O2 /W3 /I. /I../3rd/MIRACL/include keygen.c
cl /c /O2 /W3 /I. /I../3rd/MIRACL/include verifier.c
cl /c /O2 /W3 /I. /I../3rd/MIRACL/include keyver.c

link keygen.obj base32.obj hash.obj sign.obj ../3rd/MIRACL/source/miracl.lib
link keyver.obj verifier.obj base32.obj hash.obj sign.obj ../3rd/MIRACL/source/miracl.lib

del *.obj