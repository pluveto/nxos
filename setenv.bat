@echo Set environment for NXOS.
@set NXOS_NAME=NXOS
@set NXOS_ROOT=%cd%
@set NXOS_EXEC_PATH=%NXOS_ROOT%\Tools\x86_64-elf-gcc\bin
@set Path=%NXOS_EXEC_PATH%;%Path%;
@echo [ROOT DIR] %NXOS_ROOT%
@echo [EXEC PATH] %NXOS_EXEC_PATH%
