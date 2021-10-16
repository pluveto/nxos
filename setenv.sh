echo Set environment for NXOS.
export NXOS_NAME=NXOS
export NXOS_ROOT="$( cd "$( dirname "$0"  )" && pwd  )"
export NXOS_EXEC_PATH=$NXOS_ROOT/Tools/x86_64-elf-gcc/bin
export PATH=$NXOS_ROOT:$PATH
echo [ROOT DIR] $NXOS_ROOT
echo [EXEC PATH] $NXOS_EXEC_PATH
