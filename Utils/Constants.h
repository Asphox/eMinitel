//
// Created by asphox on 12/26/23.
//

#ifndef CONSTANTS_H
#define CONSTANTS_H

enum VDTX_CODE_CONTROL
{
    __VC_MIN    ,
    VC_NUL      = 0x00,
    VC_SOH      = 0x01,
    VC_EOT      = 0x04,
    VC_ENQ      = 0x05,
    VC_BEL      = 0x07,
    VC_BS       = 0x08,
    VC_HT       = 0x09,
    VC_LF       = 0x0A,
    VC_VT       = 0x0B,
    VC_FF       = 0x0C,
    VC_CR       = 0x0D,
    VC_SO       = 0x0E,
    VC_SI       = 0x0F,
    VC_DLE      = 0x10,
    VC_CON      = 0x11,
    VC_REP      = 0x12,
    VC_SEP      = 0x13,
    VC_COFF     = 0x14,
    VC_NACK     = 0x15,
    VC_SYN      = 0x16,
    VC_CAN      = 0x18,
    VC_SS2      = 0x19,
    VC_SUB      = 0x1A,
    VC_ESC      = 0x1B,
    VC_SS3      = 0x1D,
    VC_RS       = 0x1E,
    VC_US       = 0x1F,
    __VC_MAX    = VC_US,
};

#endif //CONSTANTS_H
