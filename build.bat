@echo off
SET SourceFile=.\py_test\displayclass.cp37-win32.pyd
if exist %SourceFile% (
del %SourceFile%
) else (
    echo %SourceFile% is not exist!
)
python setup.py build --build-lib .\py_test\
pause
python .\py_test\test.py
pause
