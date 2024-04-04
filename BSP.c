#include "BSP.h"

void SetASI(int asi, int data) {
    USED_ASI usedASI = asi;

    switch (usedASI) {
        case ASI_0x02:
            asm
            (
                "sta %i1, [%g0] 0x02\n\t"
                "nop\n\t"
            );
            break;
        case ASI_0x10:
            asm
            (
                "sta %i1, [%g0] 0x10\n\t"
                "nop\n\t"
            );
            break;
        case ASI_0x11:
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

int GetASI(int asi) {
    USED_ASI usedASI = asi;
    int infoReg;

    switch (usedASI) {
        case ASI_0x02:
            asm
            (
                "lda [%%g0] 0x02, %0\n\t"
                "nop\n\t"
                : "=r" (infoReg)
            );
            break;
        case ASI_0x10:
            asm
            (
                "lda [%%g0] 0x10, %0\n\t"
                "nop\n\t"
                : "=r" (infoReg)
            );
            break;
        case ASI_0x11:
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

void SetASR(int asrNum, int data) {
    USED_ASI usedASR = asrNum;

    // Не реализовано
}

int GetASR(int asrNum) {
    USED_ASR usedASR = asrNum;
    int data;

    switch (usedASR) {
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

int GetRPTM() {
    return *r_RPTM * m_RPTM;
}

void SetRPTM(int reg) {
    *r_RPTM = reg * m_RPTM;
}
