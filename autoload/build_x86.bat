@if "%WINHOOK_BUILD_ONCE%"=="" (
	@echo *** first time setup vc environment, run vcvarsamd64_x86.bat ***
	call "%ProgramFiles%\Microsoft Visual Studio 14.0\VC\bin\amd64_x86\vcvarsamd64_x86.bat"
	@echo done
	@echo.
	@set WINHOOK_BUILD_ONCE=1
)

cl src/hook.cpp src/iathook.cpp /Fehook_x86.dll /LD /EHsc /DEBUG /O2
cl src/injector.cpp /Feinjector_x86.dll /LD /EHsc /DEBUG /O2

@pause
