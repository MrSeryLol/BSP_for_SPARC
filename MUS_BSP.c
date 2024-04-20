#include "MUS_BSP.h"

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


/* 
   ----------------------------------------------------------------------
    Функция SetASR
    Назначение.
        Записать значение в необходимый регистр ASR в зависимости
        от переданного номера:
        ASR16 - %asr16
        ASR17 - %asr17
        ASR18 - %asr18
        Для ASR16 определены 2 макроса для настройки SEU защиты
        (HammcodeInFPU = 0x80000000 и HamcodeInIU)
    Условия.
        Должна вызываться в режиме "supervisor"
    Обращение.
   ----------------------------------------------------------------------
*/
void MUS_BSP_SetASR(int asrNum, int data) {
    switch (asrNum) {
    case ASR16:
        asm
        (
            "wr %i1, %g0, %asr16\n\t"
        );
        break;
    default:
        break;
    }
}

/* 
   ----------------------------------------------------------------------
    Функция GetASR
    Назначение.
        Считать значение необходимого регистра ASR в зависимости
        от переданного номера:
        ASR16 - %asr16
        ASR17 - %asr17
        ASR18 - %asr18
    Условия.
        Должна вызываться в режиме "supervisor"
    Обращение.
   ----------------------------------------------------------------------
*/
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

/*
   --------------------------------------------------------------------
    Системная функция ЧИТАТЬ РСН
    Назначение.
        Чтение текущего значения заданного регистра специального назначения 
        процессора.
    Вход.
        Параметр: 
        - адрес регистра специального назначения (РСН), который необходимо
        прочитать.
    Выход.
        Результат:
        - значение считанное с заданного РСН или 0xDEADDEAD, если адрес РСН
        задан не верно.
    Действия.
        1. Если задан адрес для регистра: ASR16, ASR17 или ASR18,
        считывает значение соответствующего регистра и возвращает его 
        пользователю. 
        2. Если задано иное значение адреса регистра, возвращает 
        пользователю 0хDEADDEAD. 
    Условия.
        Работает с запретом прерываний в режиме "supervisor".
        Обращается к: GetASR16, GetASR17, GetASR18.
 --------------------------------------------------------------------*)
*/
int MUS_BSP_GetSPR(int aReg)
{
    int result;

    switch (aReg) {
        case REGISTER_ASR16_ADDRESS:
            result = MUS_BSP_GetASR(ASR16);
            break;
        case REGISTER_ASR17_ADDRESS:
            result = MUS_BSP_GetASR(ASR17);
            break;
        case REGISTER_ASR18_ADDRESS:
            result = MUS_BSP_GetASR(ASR18);
            break;
        default:
            result = INCORRECT_REGISTER_ADDRESS;
            break;
    }

    return result;
}

void MUS_BSP_Set_r_RGU(int data) {
    *r_RGU = data;
}


int MUS_BSP_Get_r_RGU() {
    return *r_RGU;
}

void MUS_BSP_Set_r_BUK(int data) {
    *r_BUK = data;
}

int MUS_BSP_Get_r_BUK() {
    return *r_BUK;
}

void MUS_BSP_Set_r_UKMVP1(int data) {
    *r_UKMVP1 = data;
}

int MUS_BSP_Get_r_UKMVP1() {
    return *r_UKMVP1;
}

void MUS_BSP_Set_r_UKMVP2(int data) {
    *r_UKMVP2 = data;
}

int MUS_BSP_Get_r_UKMVP2()
{
    return *r_UKMVP2;
}
