/** @file Defines the PPIs to let PEIMs call SBI

Copyright (c) 2020, Hewlett Packard Development LP. All rights reserved.<BR>

SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef RISCV_SBI_LIB_H_
#define RISCV_SBI_LIB_H_

#include <Uefi.h>
#include <IndustryStandard/RiscVOpensbi.h>
#include <sbi/sbi_scratch.h>
#include <sbi/sbi_platform.h>

//
// EDK2 OpenSBI Firmware extension.
//
#define SBI_EDK2_FW_EXT (SBI_EXT_FIRMWARE_CODE_BASE_START | SBI_OPENSBI_IMPID)
//
// EDK2 OpenSBI Firmware extension functions.
//
#define SBI_EXT_FW_MSCRATCH_FUNC        0
#define SBI_EXT_FW_MSCRATCH_HARTID_FUNC 1

//
// EDK2 OpenSBI firmware extension return status.
//
struct sbiret {
  long error;   ///< SBI status code
  long value;   ///< Value returned
};

#define SbiCall0(ext_id, func_id) \
  SbiCall(ext_id, func_id, 0, 0, 0, 0, 0, 0)
#define SbiCall1(ext_id, func_id, arg0) \
  SbiCall(ext_id, func_id, arg0, 0, 0, 0, 0, 0)
#define SbiCall2(ext_id, func_id, arg0, arg1) \
  SbiCall(ext_id, func_id, arg0, arg1, 0, 0, 0, 0)
#define SbiCall3(ext_id, func_id, arg0, arg1, arg2) \
  SbiCall(ext_id, func_id, arg0, arg1, arg2, 0, 0, 0)
#define SbiCall4(ext_id, func_id, arg0, arg1, arg2, arg3) \
  SbiCall(ext_id, func_id, arg0, arg1, arg2, arg3, 0, 0)
#define SbiCall5(ext_id, func_id, arg0, arg1, arg2, arg3, arg4) \
  SbiCall(ext_id, func_id, arg0, arg1, arg2, arg3, arg4, 0)
#define SbiCall6(ext_id, func_id, arg0, arg1, arg2, arg3, arg4, arg5) \
  SbiCall(ext_id, func_id, arg0, arg1, arg2, arg3, arg4, arg5)

/**
  EDK2 SbiCall to invoke SBI extensions.

  @param[in] ext_id   Sbi extension ID.
  @param[in] func_id  Sbi functions ID.
  @param[in] arg0     Arg0 to function.
  @param[in] arg1     Arg1 to function.
  @param[in] arg2     Arg2 to function.
  @param[in] arg3     Arg3 to function.
  @param[in] arg4     Arg4 to function.
  @param[in] arg5     Arg5 to function.

  @retval  Returns sbiret structure.

**/
inline
EFIAPI
struct sbiret SbiCall(UINTN ext_id, UINTN func_id, UINTN arg0, UINTN arg1,
                           UINTN arg2, UINTN arg3, UINTN arg4, UINTN arg5)
__attribute__((always_inline));

/**
  EDK2 SbiCall to invoke SBI extensions.

  @param[in] ext_id   Sbi extension ID.
  @param[in] func_id  Sbi functions ID.
  @param[in] arg0     Arg0 to function.
  @param[in] arg1     Arg1 to function.
  @param[in] arg2     Arg2 to function.
  @param[in] arg3     Arg3 to function.
  @param[in] arg4     Arg4 to function.
  @param[in] arg5     Arg5 to function.

  @retval  Returns sbiret structure.

**/
inline
EFIAPI
struct sbiret SbiCall(UINTN ext_id, UINTN func_id, UINTN arg0, UINTN arg1,
                           UINTN arg2, UINTN arg3, UINTN arg4, UINTN arg5) {
    register uintptr_t a0 asm ("a0") = (uintptr_t)(arg0);
    register uintptr_t a1 asm ("a1") = (uintptr_t)(arg1);
    register uintptr_t a2 asm ("a2") = (uintptr_t)(arg2);
    register uintptr_t a3 asm ("a3") = (uintptr_t)(arg3);
    register uintptr_t a4 asm ("a4") = (uintptr_t)(arg4);
    register uintptr_t a5 asm ("a5") = (uintptr_t)(arg5);
    register uintptr_t a6 asm ("a6") = (uintptr_t)(func_id);
    register uintptr_t a7 asm ("a7") = (uintptr_t)(ext_id);
    asm volatile ("ecall" \
         : "+r" (a0) \
         : "r" (a1), "r" (a2), "r" (a3), "r" (a4), "r" (a5), "r" (a6), "r" (a7) \
         : "memory"); \
    struct sbiret ret = { a0, a1 };
    return ret;
}

/**
  Get the implemented SBI specification version

  The minor number of the SBI specification is encoded in the low 24 bits,
  with the major number encoded in the next 7 bits.  Bit 32 must be 0 and is
  reserved for future expansion.

  @param[out] SpecVersion          The Version of the SBI specification.
**/
VOID
EFIAPI
SbiGetSpecVersion (
  OUT UINTN                       *SpecVersion
  );

/**
  Get the SBI implementation ID

  This ID is used to idenetify a specific SBI implementation in order to work
  around any quirks it might have.

  @param[out] ImplId               The ID of the SBI implementation.
**/
VOID
EFIAPI
SbiGetImplId (
  OUT UINTN                      *ImplId
  );

/**
  Get the SBI implementation version

  The version of this SBI implementation.
  The encoding of this number is determined by the specific SBI implementation.

  @param[out] ImplVersion          The version of the SBI implementation.
**/
VOID
EFIAPI
SbiGetImplversion (
  OUT UINTN                       *ImplVersion
  );

/**
  Probe whether an SBI extension is available

  ProbeResult is set to 0 if the extension is not available or to an extension
  specified value if it is available.

  @param[in]  ExtensionId          The extension ID.
  @param[out] ProbeResult          The return value of the probe.
**/
VOID
EFIAPI
SbiProbeExtension (
  IN  INTN                         ExtensionId,
  OUT INTN                        *ProbeResult
  );

/**
  Get the CPU's vendor ID

  Reads the mvendorid CSR.

  @param[out] MvendorId            The CPU's vendor ID.
**/
VOID
EFIAPI
SbiGetMvendorId (
  OUT UINTN                       *MvendorId
  );

/**
  Get the CPU's architecture ID

  Reads the marchid CSR.

  @param[out] MarchId              The CPU's architecture ID.
**/
VOID
EFIAPI
SbiGetMarchId (
  OUT UINTN                       *MarchId
  );

/**
  Get the CPU's implementation ID

  Reads the mimpid CSR.

  @param[out] MimpId               The CPU's implementation ID.
**/
VOID
EFIAPI
SbiGetMimpId (
  OUT UINTN                       *Mimpid
  );

/**
  Politely ask the SBI to start a given hart.

  This call may return before the hart has actually started executing, if the
  SBI implementation can guarantee that the hart is actually going to start.

  Before the hart jumps to StartAddr, the hart MUST configure PMP if present
  and switch to S-mode.

  @param[in]  HartId               The id of the hart to start.
  @param[in]  StartAddr            The physical address, where the hart starts
                                   executing from.
  @param[in]  Priv                 An XLEN-bit value, which will be in register
                                   a1 when the hart starts.
  @retval EFI_SUCCESS              Hart was stopped and will start executing from StartAddr.
  @retval EFI_LOAD_ERROR           StartAddr is not valid, possibly due to following reasons:
                                    - It is not a valid physical address.
                                    - The address is prohibited by PMP to run in
                                      supervisor mode.
  @retval EFI_INVALID_PARAMETER    HartId is not a valid hart id
  @retval EFI_ALREADY_STARTED      The hart is already running.
  @retval other                    The start request failed for unknown reasons.
**/
EFI_STATUS
EFIAPI
SbiHartStart (
  IN  UINTN                          HartId,
  IN  UINTN                          StartAddr,
  IN  UINTN                          Priv
  );

/**
  Return execution of the calling hart to SBI.

  MUST be called in S-Mode with user interrupts disabled.
  This call is not expected to return, unless a failure occurs.

  @retval     EFI_SUCCESS          Never occurs. When successful, the call does not return.
  @retval     other                Failed to stop hard for an unknown reason.
**/
EFI_STATUS
EFIAPI
SbiHartStop (
  );

/**
  Get the current status of a hart.

  Since harts can transition between states at any time, the status retrieved
  by this function may already be out of date, once it returns.

  Possible values for HartStatus are:
  0: STARTED
  1: STOPPED
  2: START_REQUEST_PENDING
  3: STOP_REQUEST_PENDING

  @param[out] HartStatus           The pointer in which the hart's status is
                                   stored.
  @retval EFI_SUCCESS              The operation succeeds.
  @retval EFI_INVALID_PARAMETER    A parameter is invalid.
**/
EFI_STATUS
EFIAPI
SbiHartGetStatus (
  IN  UINTN                          HartId,
  OUT UINTN                         *HartStatus
  );

///
/// Timer extension
///

/**
  Clear pending timer interrupt bit and set timer for next event after StimeValue.

  To clear the timer without scheduling a timer event, set StimeValue to a
  practically infinite value or mask the timer interrupt by clearing sie.STIE.

  @param[in]  StimeValue           The time offset to the next scheduled timer interrupt.
**/
VOID
EFIAPI
SbiSetTimer (
  IN  UINT64                         StimeValue
  );

///
/// IPI extension
///

/**
  Send IPI to all harts specified in the mask.

  The interrupts are registered as supervisor software interrupts at the
  receiving hart.

  @param[in]  HartMask             Scalar bit-vector containing hart ids
  @param[in]  HartMaskBase         The starting hartid from which the bit-vector
                                   must be computed. If set to -1, HartMask is
                                   ignored and all harts are considered.
  @retval EFI_SUCCESS              IPI was sent to all the targeted harts.
  @retval EFI_INVALID_PARAMETER    Either hart_mask_base or any of the hartid
                                   from hart_mask is not valid i.e. either the
                                   hartid is not enabled by the platform or is
                                   not available to the supervisor.
**/
EFI_STATUS
EFIAPI
SbiSendIpi (
  IN  UINTN                         *HartMask,
  IN  UINTN                          HartMaskBase
  );

///
/// Remote fence extension
///

/**
  Instructs remote harts to execute a FENCE.I instruction.

  @param[in]  HartMask             Scalar bit-vector containing hart ids
  @param[in]  HartMaskBase         The starting hartid from which the bit-vector
                                   must be computed. If set to -1, HartMask is
                                   ignored and all harts are considered.
  @retval EFI_SUCCESS              IPI was sent to all the targeted harts.
  @retval EFI_INVALID_PARAMETER    Either hart_mask_base or any of the hartid
                                   from hart_mask is not valid i.e. either the
                                   hartid is not enabled by the platform or is
                                   not available to the supervisor.
**/
EFI_STATUS
EFIAPI
SbiRemoteFenceI (
  IN  UINTN                         *HartMask,
  IN  UINTN                          HartMaskBase
  );

/**
  Instructs the remote harts to execute one or more SFENCE.VMA instructions.

  The SFENCE.VMA covers the range of virtual addresses between StartAaddr and Size.

  The remote fence function acts as a full tlb flush if * StartAddr and size
  are both 0 * size is equal to 2^XLEN-1

  @param[in]  HartMask             Scalar bit-vector containing hart ids
  @param[in]  HartMaskBase         The starting hartid from which the bit-vector
                                   must be computed. If set to -1, HartMask is
                                   ignored and all harts are considered.
  @param[in]  StartAddr            The first address of the affected range.
  @param[in]  Size                 How many addresses are affected.
  @retval EFI_SUCCESS              IPI was sent to all the targeted harts.
  @retval EFI_LOAD_ERROR           StartAddr or Size is not valid.
  @retval EFI_INVALID_PARAMETER    Either hart_mask_base or any of the hartid
                                   from hart_mask is not valid i.e. either the
                                   hartid is not enabled by the platform or is
                                   not available to the supervisor.
**/
EFI_STATUS
EFIAPI
SbiRemoteSfenceVma (
  IN  UINTN                         *HartMask,
  IN  UINTN                          HartMaskBase,
  IN  UINTN                          StartAddr,
  IN  UINTN                          Size
  );

/**
  Instructs the remote harts to execute one or more SFENCE.VMA instructions.

  The SFENCE.VMA covers the range of virtual addresses between StartAaddr and Size.
  Covers only the given ASID.

  The remote fence function acts as a full tlb flush if * StartAddr and size
  are both 0 * size is equal to 2^XLEN-1

  @param[in]  HartMask             Scalar bit-vector containing hart ids
  @param[in]  HartMaskBase         The starting hartid from which the bit-vector
                                   must be computed. If set to -1, HartMask is
                                   ignored and all harts are considered.
  @param[in]  StartAddr            The first address of the affected range.
  @param[in]  Size                 How many addresses are affected.
  @retval EFI_SUCCESS              IPI was sent to all the targeted harts.
  @retval EFI_LOAD_ERROR           StartAddr or Size is not valid.
  @retval EFI_INVALID_PARAMETER    Either hart_mask_base or any of the hartid
                                   from hart_mask is not valid i.e. either the
                                   hartid is not enabled by the platform or is
                                   not available to the supervisor.
**/
EFI_STATUS
EFIAPI
SbiRemoteSfenceVmaAsid (
  IN  UINTN                         *HartMask,
  IN  UINTN                          HartMaskBase,
  IN  UINTN                          StartAddr,
  IN  UINTN                          Size,
  IN  UINTN                          Asid
  );

/**
  Instructs the remote harts to execute one or more SFENCE.GVMA instructions.

  The SFENCE.GVMA covers the range of virtual addresses between StartAaddr and Size.
  Covers only the given VMID.
  This function call is only valid for harts implementing the hypervisor extension.

  The remote fence function acts as a full tlb flush if * StartAddr and size
  are both 0 * size is equal to 2^XLEN-1

  @param[in]  HartMask             Scalar bit-vector containing hart ids
  @param[in]  HartMaskBase         The starting hartid from which the bit-vector
                                   must be computed. If set to -1, HartMask is
                                   ignored and all harts are considered.
  @param[in]  StartAddr            The first address of the affected range.
  @param[in]  Size                 How many addresses are affected.
  @retval EFI_SUCCESS              IPI was sent to all the targeted harts.
  @retval EFI_LOAD_ERROR           StartAddr or Size is not valid.
  @retval EFI_UNSUPPORTED          SBI does not implement this function or one
                                   of the target harts does not support the
                                   hypervisor extension.
  @retval EFI_INVALID_PARAMETER    Either hart_mask_base or any of the hartid
                                   from hart_mask is not valid i.e. either the
                                   hartid is not enabled by the platform or is
                                   not available to the supervisor.
**/
EFI_STATUS
EFIAPI
SbiRemoteHfenceGvmaVmid (
  IN  UINTN                         *HartMask,
  IN  UINTN                          HartMaskBase,
  IN  UINTN                          StartAddr,
  IN  UINTN                          Size,
  IN  UINTN                          Vmid
  );

/**
  Instructs the remote harts to execute one or more SFENCE.GVMA instructions.

  The SFENCE.GVMA covers the range of virtual addresses between StartAaddr and Size.
  This function call is only valid for harts implementing the hypervisor extension.

  The remote fence function acts as a full tlb flush if * StartAddr and size
  are both 0 * size is equal to 2^XLEN-1

  @param[in]  HartMask             Scalar bit-vector containing hart ids
  @param[in]  HartMaskBase         The starting hartid from which the bit-vector
                                   must be computed. If set to -1, HartMask is
                                   ignored and all harts are considered.
  @param[in]  StartAddr            The first address of the affected range.
  @param[in]  Size                 How many addresses are affected.
  @retval EFI_SUCCESS              IPI was sent to all the targeted harts.
  @retval EFI_LOAD_ERROR           StartAddr or Size is not valid.
  @retval EFI_UNSUPPORTED          SBI does not implement this function or one
                                   of the target harts does not support the
                                   hypervisor extension.
  @retval EFI_INVALID_PARAMETER    Either hart_mask_base or any of the hartid
                                   from hart_mask is not valid i.e. either the
                                   hartid is not enabled by the platform or is
                                   not available to the supervisor.
**/
EFI_STATUS
EFIAPI
SbiRemoteHfenceGvma (
  IN  UINTN                         *HartMask,
  IN  UINTN                          HartMaskBase,
  IN  UINTN                          StartAddr,
  IN  UINTN                          Size
  );

/**
  Instructs the remote harts to execute one or more SFENCE.VVMA instructions.

  The SFENCE.GVMA covers the range of virtual addresses between StartAaddr and Size.
  Covers only the given ASID.
  This function call is only valid for harts implementing the hypervisor extension.

  The remote fence function acts as a full tlb flush if * StartAddr and size
  are both 0 * size is equal to 2^XLEN-1

  @param[in]  HartMask             Scalar bit-vector containing hart ids
  @param[in]  HartMaskBase         The starting hartid from which the bit-vector
                                   must be computed. If set to -1, HartMask is
                                   ignored and all harts are considered.
  @param[in]  StartAddr            The first address of the affected range.
  @param[in]  Size                 How many addresses are affected.
  @retval EFI_SUCCESS              IPI was sent to all the targeted harts.
  @retval EFI_LOAD_ERROR           StartAddr or Size is not valid.
  @retval EFI_UNSUPPORTED          SBI does not implement this function or one
                                   of the target harts does not support the
                                   hypervisor extension.
  @retval EFI_INVALID_PARAMETER    Either hart_mask_base or any of the hartid
                                   from hart_mask is not valid i.e. either the
                                   hartid is not enabled by the platform or is
                                   not available to the supervisor.
**/
EFI_STATUS
EFIAPI
SbiRemoteHfenceVvmaAsid (
  IN  UINTN                         *HartMask,
  IN  UINTN                          HartMaskBase,
  IN  UINTN                          StartAddr,
  IN  UINTN                          Size,
  IN  UINTN                          Asid
  );

/**
  Instructs the remote harts to execute one or more SFENCE.VVMA instructions.

  The SFENCE.GVMA covers the range of virtual addresses between StartAaddr and Size.
  This function call is only valid for harts implementing the hypervisor extension.

  The remote fence function acts as a full tlb flush if * StartAddr and size
  are both 0 * size is equal to 2^XLEN-1

  @param[in]  HartMask             Scalar bit-vector containing hart ids
  @param[in]  HartMaskBase         The starting hartid from which the bit-vector
                                   must be computed. If set to -1, HartMask is
                                   ignored and all harts are considered.
  @param[in]  StartAddr            The first address of the affected range.
  @param[in]  Size                 How many addresses are affected.
  @retval EFI_SUCCESS              IPI was sent to all the targeted harts.
  @retval EFI_LOAD_ERROR           StartAddr or Size is not valid.
  @retval EFI_UNSUPPORTED          SBI does not implement this function or one
                                   of the target harts does not support the
                                   hypervisor extension.
  @retval EFI_INVALID_PARAMETER    Either hart_mask_base or any of the hartid
                                   from hart_mask is not valid i.e. either the
                                   hartid is not enabled by the platform or is
                                   not available to the supervisor.
**/
EFI_STATUS
EFIAPI
SbiRemoteHfenceVvma (
  IN  UINTN                         *HartMask,
  IN  UINTN                          HartMaskBase,
  IN  UINTN                          StartAddr,
  IN  UINTN                          Size
  );

///
/// Vendor Specific extension space: Extension Ids 0x09000000 through 0x09FFFFFF
///

/**
  Call a function in a vendor defined SBI extension

  ASSERT() if the ExtensionId is not in the designated SBI Vendor Extension
  Space.

  @param[in]  ExtensionId          The SBI vendor extension ID.
  @param[in]  FunctionId           The function ID to call in this extension.
  @param[in]  NumArgs              How many arguments are passed.
  @param[in]  ...                  Actual Arguments to the function.
  @retval EFI_SUCCESS if the SBI function was called and it was successful
  @retval EFI_INVALID_PARAMETER if NumArgs exceeds 6
  @retval others if the called SBI function returns an error
**/
EFI_STATUS
EFIAPI
SbiVendorCall (
  IN  UINTN                          ExtensionId,
  IN  UINTN                          FunctionId,
  IN  UINTN                          NumArgs,
  ...
  );

///
/// Firmware SBI Extension
///
/// This SBI Extension is defined and used by EDK2 only in order to be able to
/// run PI and DXE phase in S-Mode.
///

/**
  Get scratch space of the current hart.

  Please consider using the wrapper SbiGetFirmwareContext if you only need to
  access the firmware context.

  @param[out] ScratchSpace         The scratch space pointer.
  @retval EFI_SUCCESS              The operation succeeds.
**/
EFI_STATUS
EFIAPI
SbiGetMscratch (
  OUT struct sbi_scratch             **ScratchSpace
  );

/**
  Get scratch space of the given hart id.

  @param[in]  HartId               The hart id.
  @param[out] ScratchSpace         The scratch space pointer.
  @retval EFI_SUCCESS              The operation succeeds.
**/
EFI_STATUS
EFIAPI
SbiGetMscratchHartid (
  IN  UINTN                            HartId,
  OUT struct sbi_scratch             **ScratchSpace
  );

/**
  Get firmware context of the calling hart.

  @param[out] FirmwareContext      The firmware context pointer.
  @retval EFI_SUCCESS              The operation succeeds.
**/
EFI_STATUS
EFIAPI
SbiGetFirmwareContext (
  OUT EFI_RISCV_OPENSBI_FIRMWARE_CONTEXT **FirmwareContext
  );

/**
  Set firmware context of the calling hart.

  @param[in] FirmwareContext       The firmware context pointer.
  @retval EFI_SUCCESS              The operation succeeds.
**/
EFI_STATUS
EFIAPI
SbiSetFirmwareContext (
  IN EFI_RISCV_OPENSBI_FIRMWARE_CONTEXT *FirmwareContext
  );

#endif
