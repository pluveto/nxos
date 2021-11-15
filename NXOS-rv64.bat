@echo Set environment for NXOS.
@set NXOS_NAME=NXOS
@set NXOS_ROOT=%cd%
@set NXOS_EXEC_PATH=%NXOS_ROOT%\Tools\RISC-VEmbeddedGCC-8.1.0-2-20181019-0952\bin
@set Path=%NXOS_EXEC_PATH%;%Path%;
@echo [ROOT DIR] %NXOS_ROOT%
@echo [EXEC PATH] %NXOS_EXEC_PATH%
