CALL vendor\bin\premake5.exe gmake
IF %ERRORLEVEL% NEQ 0 (
  PAUSE
)