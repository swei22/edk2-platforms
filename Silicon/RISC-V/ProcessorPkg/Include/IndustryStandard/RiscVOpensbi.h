/** @file
  SBI inline function calls.

  Copyright (c) 2020, Hewlett Packard Enterprise Development LP. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef EDK2_SBI_H_
#define EDK2_SBI_H_

#include <include/sbi/riscv_asm.h> // Reference to header file in opensbi
#include <RiscVImpl.h>
#include <sbi/sbi_types.h>  // Reference to header file wrapper

#define SBI_SUCCESS                    0
#define SBI_ERR_FAILED                -1
#define SBI_ERR_NOT_SUPPORTED         -2
#define SBI_ERR_INVALID_PARAM         -3
#define SBI_ERR_DENIED                -4
#define SBI_ERR_INVALID_ADDRESS       -5
#define SBI_ERR_ALREADY_AVAILABLE     -6

#define SBI_BASE_EXT                   0x10
#define SBI_HSM_EXT                    0x48534D
#define SBI_TIME_EXT                   0x54494D45
#define SBI_IPI_EXT                    0x735049
#define SBI_RFNC_EXT                   0x52464E43

//
// Below two definitions should be defined in OpenSBI.
//
#define SBI_EXT_FIRMWARE_CODE_BASE_START 0x0A000000
#define SBI_EXT_FIRMWARE_CODE_BASE_END   0x0AFFFFFF

#define SBI_GET_SPEC_VERSION_FUNC      0
#define SBI_GET_IMPL_ID_FUNC           1
#define SBI_GET_IMPL_VERSION_FUNC      2
#define SBI_PROBE_EXTENSION_FUNC       3
#define SBI_GET_MVENDORID_FUNC         4
#define SBI_GET_MARCHID_FUNC           5
#define SBI_GET_MIMPID_FUNC            6

#define SBI_HART_START_FUNC            0
#define SBI_HART_STOP_FUNC             1
#define SBI_HART_GET_STATUS_FUNC       2

#define RISC_V_MAX_HART_SUPPORTED 16

typedef
VOID
(EFIAPI *RISCV_HART_SWITCH_MODE)(
  IN  UINTN   FuncArg0,
  IN  UINTN   FuncArg1,
  IN  UINTN   NextAddr,
  IN  UINTN   NextMode,
  IN  BOOLEAN NextVirt
  );

//
// Keep the structure member in 64-bit alignment.
//
typedef struct {
    UINT64                 IsaExtensionSupported;  // The ISA extension this core supported.
    RISCV_UINT128          MachineVendorId;        // Machine vendor ID
    RISCV_UINT128          MachineArchId;          // Machine Architecture ID
    RISCV_UINT128          MachineImplId;          // Machine Implementation ID
    RISCV_HART_SWITCH_MODE HartSwitchMode;         // OpenSBI's function to switch the mode of a hart
} EFI_RISCV_FIRMWARE_CONTEXT_HART_SPECIFIC;
#define FIRMWARE_CONTEXT_HART_SPECIFIC_SIZE  (64 * 8) // This is the size of EFI_RISCV_FIRMWARE_CONTEXT_HART_SPECIFIC
                                                      // structure. Referred by both C code and assembly code.

typedef struct {
  VOID            *PeiServiceTable;       // PEI Service table
  EFI_RISCV_FIRMWARE_CONTEXT_HART_SPECIFIC  *HartSpecific[RISC_V_MAX_HART_SUPPORTED];
} EFI_RISCV_OPENSBI_FIRMWARE_CONTEXT;

#endif
