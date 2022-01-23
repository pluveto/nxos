echo "Set environment for NXOS."
export NXOS_NAME="NXOS"
export NXOS_ROOT_DIR=$(pwd)
export NXOS_SRC_DIR=$NXOS_ROOT_DIR/src
export NXOS_EXEC_PATH=$NXOS_ROOT_DIR/tools/gcc/x86_64-elf-gcc/bin
export PATH=$NXOS_EXEC_PATH:$PATH
echo "[ROOT DIR ]" $NXOS_ROOT_DIR
echo "[SRC DIR  ]" $NXOS_SRC_DIR
echo "[EXEC PATH]" $NXOS_EXEC_PATH
cp configs/platform-x86-i386.mk platform.mk
echo "[PLAFORM  ]" x86-i386