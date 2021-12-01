echo "Set environment for NXOS."
export NXOS_NAME="NXOS"
export NXOS_ROOT_DIR=$(pwd)
export NXOS_SRC_DIR=$NXOS_ROOT_DIR/Src
export NXOS_EXEC_PATH=$NXOS_ROOT_DIR/Tools/x86_64-elf-gcc/bin
export PATH=$NXOS_EXEC_PATH:$PATH
echo "[ROOT DIR ]" $NXOS_ROOT_DIR
echo "[SRC DIR  ]" $NXOS_SRC_DIR
echo "[EXEC PATH]" $NXOS_EXEC_PATH
cp Configs/Platform-Riscv64-Qemu.mk Platform.mk
echo "[PLAFORM  ]" Riscv64-Qemu
