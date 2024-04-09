#include "BSP.h"

void MUS_BSP_SetASI(int asi, int data) {

    switch (asi) {
        case ASI02:
            asm
            (
                "sta %i1, [%g0] 0x02\n\t"
                "nop\n\t"
            );
            break;
        case ASI16:
            asm
            (
                "sta %i1, [%g0] 0x10\n\t"
                "nop\n\t"
            );
            break;
        case ASI17:
            asm 
            (
                "sta %i1, [%g0] 0x11\n\t"
                "nop\n\t"
            );
            break;
        default:
            break;
    }
}

int MUS_BSP_GetASI(int asi) {
    int infoReg;

    switch (asi) {
        case ASI02:
            asm
            (
                "lda [%%g0] 0x02, %0\n\t"
                "nop\n\t"
                : "=r" (infoReg)
            );
            break;
        case ASI16:
            asm
            (
                "lda [%%g0] 0x10, %0\n\t"
                "nop\n\t"
                : "=r" (infoReg)
            );
            break;
        case ASI17:
            asm 
            (
                "lda [%%g0] 0x11, %0\n\t"
                "nop\n\t"
                : "=r" (infoReg)
            );
            break;
        default:
            break;
    }

    return infoReg;
}

void MUS_BSP_SetASR(int asrNum, int data) {

    // Не реализовано
}

int MUS_BSP_GetASR(int asrNum) {
    int data;

    switch (asrNum) {
        case ASR16:
            asm
            (
                "jmpl %%o7+8, %%g0\n\t"
                "rd %%asr16, %%o0\n\t"
                : "=r" (data)
            );
            break;
        case ASR17:
            asm
            (
                "jmpl %%o7+8, %%g0\n\t"
                "rd %%asr17, %%o0\n\t"
                : "=r" (data)
            );
            break;
        case ASR18:
            asm
            (
                "jmpl %%o7+8, %%g0\n\t"
                "rd %%asr18, %%0\n\t"
                : "=r" (data)
            );
            break;
        default:
            break;
    }

    return data;
}

int MUS_BSP_GetRPTM() {
    return *r_RPTM && m_RPTM;
}

void MUS_BSP_SetRPTM(int reg) {
    *r_RPTM = reg && m_RPTM;
}
