@ECHO OFF
:: "c:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\vcvarsall.bat"

:: To use cl.exe directly uncomment below
:: cl /LD src/*.cpp /Ic:\Python27\include c:\Python27\libs\python27.lib /link/out:astar.pyd

:: To use python setup.py build option you need to have either VS2008 or if using any other VS, set a envir var
:: PS: If your'e using VS2008 you don't need to set envir var.

:: For VS2013 uncomment below
SET VS90COMNTOOLS=%VS120COMNTOOLS%

:: For VS2012
:: SET VS90COMNTOOLS=%VS110COMNTOOLS%

:: For VS2010
:: SET VS90COMNTOOLS=%VS100COMNTOOLS%


@ECHO ON
python setup.py build_ext -i
@ECHO OFF
if exist "astar.pyd" (
    del astar.pyd
)

copy build\lib.win32-2.7\astar.pyd .
@ECHO ON
