@if "%WINHOOK_BUILD_ONCE%"=="" (
	@echo *** first time setup vc environment, run vcvars64.bat ***
	call "%ProgramFiles%Microsoft Visual Studio 14.0\VC\bin\amd64\vcvars64.bat"
	@echo done
	@echo.
	@set WINHOOK_BUILD_ONCE=1
)

cl src/hook.cpp src/iathook.cpp /Fehook_x64.dll /LD /EHsc /DEBUG /O2
cl src/injector.cpp /Feinjector_x64.dll /LD /EHsc /DEBUG /O2

@pause
