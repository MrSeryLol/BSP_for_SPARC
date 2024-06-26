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
    return *r_RPTM & m_RPTM;
}

void MUS_BSP_SetRPTM(int reg) {
    *r_RPTM = reg & m_RPTM;
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

/*
   --------------------------------------------------------------------    
    Функция ЧИТАТЬ РЕГИСТР МАСКИ ПРЕРЫВАНИЙ
    Назначение.    
        Считатывание значения регистра маски контроллера прерываний ОК.
    Вход.
        Параметр:     
        - номер регистра который следует прочитать. 
    Выход.    
        Результат функции - прочитанное значение маски указанного регистра.  
    Действия.    
        В соответствии с номером указанного регистра маски, читает и 
        возвращает пользователю его значение.
    Условия.
       Должна работать с запретом прерываний.
       Использует регистры маски КПр ОК: РМ1, РМ2, РМ3.
    Обращение.
 --------------------------------------------------------------------
*/
int MUS_BSP_getRegIntMaska(TNomRegInt nReg) {
    switch (nReg) {
        case int1:
            return *r_RM1;
        case int2:
            return *r_RM2;
        case int3:
            return *r_RM3;
    }
}

/*
   --------------------------------------------------------------------    
    Функция УСТАНОВИТЬ РЕГИСТР МАСКИ ПРЕРЫВАНИЙ
    Назначение.    
        Запись значения в регистр маски контроллера прерываний ОК.
    Вход.    
        Параметры:
        - номер регистра маски, в который следует записать  маску          
        - устанавливаемое значение регистра маски
    Выход.    
        В указанный регистр маски записанно новое значение.  
    Действия.    
        В указанный регистра маски прерываний записывает новое значение.
    Условия.
        Должна работать с запретом прерываний.
        Использует регистры записи 0 и 1 в регистры маски КПр ОК: 
        Зп0РМ1, Зп0РМ2, Зп0РМ3, Зп1РМ1, Зп1РМ2, Зп1РМ3.
    Обращение.
   --------------------------------------------------------------------
*/
void MUS_BSP_setRegIntMaska(TNomRegInt nReg, int maska) {
    switch (nReg) {
        case int1:
            *r_Write0RM1 = -maska;
            *r_Write1RM1 = maska;
            break;
        case int2:
            *r_Write0RM2 = -maska;
            *r_Write1RM2 = maska;
            break;
        case int3:
            *r_Write0RM3 = -maska;
            *r_Write1RM3 = maska;
            break;
        default:
            break;
    }
}

/*
   --------------------------------------------------------------------    
    Функция ЧИТАТЬ РЕГИСТР ЗАПРОСА ПРЕРЫВАНИЯ
    Назначение.    
        Считывание значения регистра запросов контроллера прерываний ОК.
    Вход.    
        Параметр:
        - номер регистра запросов, который следует прочитать 
    Выход.    
        Результат функции 
        - прочитанное значение значащих битов указанного регистра.  
    Действия.    
        Читает указанный параметром функции регистр запроса прерываний и 
        возвращает пользователю его значащие биты.
    Условия.
        Должна работать с запретом прерываний.
        Использует регистры запросов без сброса заявок КПр ОК: РЗ1, РЗ2, РЗ3.
    Обращение.
   --------------------------------------------------------------------
*/
int MUS_BSP_getReqInt(TNomRegInt nReg) {
    switch (nReg) {
        case irq:
            return *r_ITP & m_ITM_0;
        case int1:
            return *r_RZ1 & m_RZ1;
        case int2:
            return *r_RZ2 & m_RZ2;
        case int3:
            return *r_RZ3 & m_RZ3;
    }
}

/*
   --------------------------------------------------------------------    
    Функция ОЧИСТИТЬ РЕГИСТР ЗАПРОСА ПРЕРЫВАНИЯ
    Назначение.
        Чтение значения регистра запросов контроллера прерываний ОК со 
        сбросом всех заявок.                                            
    Вход.
        Параметр: 
        - номер регистра запросов прерываний который следует очистить.
    Выход.
        Результат функции: прочитанное значение указанного регистра запроса
        прерываний.
        Запросы, указанные в регистре и соответствующие им заявки прерываний
        сброшены.
        Если в момент чтения регистра поступят новые прерывания, то разряды,
        соответствующие этим прерываниям, сброшены не будут.
    Действия.
        Считывает указанный регистр запроса прерываний со сбросом заявок 
        указанного регистра и возвращает считанное значение пользователю.
    Условия.
        Должна работать с запретом прерываний.
        Использует регистры запросов со сбросом всех заявок КПр ОК: 
        РЗС1, РЗС2, РЗС3.
    Обращение.
   --------------------------------------------------------------------
*/
int MUS_BSP_getClearReqInt(TNomRegInt nReg) {
    switch (nReg) {
        case int1:
            return *r_RZC1;
        case int2:
            return *r_RZC2;
        case int3:
            return *r_RZC3;
    }
}

/*
   --------------------------------------------------------------------
    Функция ЧИТАТЬ СТВ
    Назначение.
        Для считывания счётчика текущего времени (СТВ).
    Вход.
        Регистр СТВ1.
    Выход.
        Результат функции - значение СТВ.
    Действия.  
        Снимает значение регистра СТВ1 в глобальную переменную STV. 
        Если переполнение СТВ1 произошло, но не обработано, добавляет к снятому 
        значению величину переполнения.
        Выдаёт результат пользователю.
    Условия.
        Должна работать с запретом прерываний.
        Использует регистры: СТВ1, РЗ1.
    Обращение.
   --------------------------------------------------------------------
*/
uint64_t MUS_BSP_getSTV() {
    STV = *r_RSTV1; // Неявное преобразование из int64_t в uint32_t
    
    if (STV < 10) { //если счётчик < 10 миллисекунд
        // Сдвигаем побитово значение в r_RZ1 и смотрим, если 1, то есть переполнение, если 0, то переполнения нет
        if ((*r_RZ1 >> IRQ_OverflowTime32 - 1) == 1) { //если есть переполенение
            return STV + TimeOverflow; //то добавляем величину переполнения
        }
        else if ((*r_RZ1 >> IRQ_OverflowTime32 - 1) == 1) { // повторно смотрим переполенение и если оно всё таки появилось:
            return STV + TimeOverflow; // то добавляем величину переполнения
        }
    }

    return STV;
}

/*
   --------------------------------------------------------------------
    Функция ЗАПИСАТЬ СТВ
    Назначение.
        Для установки значения счётчика текущего времени.
    Вход.
        Параметр: 
        - новое значение счетчика текущего времени.
    Выход.
        Новое значение регистра счетчика текущего времени (СТВ1).
    Действия.
        1) При заданном значении параметра больше 0хffffffff, берем 0xffffffff
        2) Записать значение time в регистр счетчика СТВ1.
    Условия.
        Должна работать с запретом прерываний.
        Использует регистр СТВ1.
    Обращение.
   --------------------------------------------------------------------
*/
void MUS_BSP_setSTV(uint64_t time) {
    // если значение параметра больше Максимально возможного значения счетчика, 
    // берем максимальное значение.
    if (time >= TimeOverflow) {
        time = TimeOverflow - 1; // max возможное значение счетчика
    }

    *r_RSTV1 = (unsigned int)time;
}

/*
   --------------------------------------------------------------------
    Функция ЗАПИСАТЬ УСТАВКУ
    Назначение.
        Для установки значения в регистр уставки СТВ.
    Вход.
        Параметр: значение регистра уставки СТВ.
    Выход.
        Значение регистра уставки СТВ1 изменено.
    Действия.
        Записывает указанное значение в регистр уставки СТВ1 (РУСТ1).
    Условия.
        Должна работать с запретом прерываний.
        Использует регистр РУСТ1.
    Обращение.
   --------------------------------------------------------------------
*/
void MUS_BSP_setUst(int64_t time) {
    *r_RUST1 = (unsigned int)time;
}

/*
   --------------------------------------------------------------------
    Функция СБРОСИТЬ СТТ
    Назначение.
        Для сброса сторожевого таймера.
    Вход.
        Нет.
    Выход.
        Значение регистра СТТ = 0.
    Действия.
        Заносит нулевое значение в регистр СТТ.      
    Условия.
        Должна работать с запретом прерываний.
        Использует регистр СТТ.
    Обращение.
   --------------------------------------------------------------------
*/
void MUS_BSP_resetSTT() {
    *r_RSTT = 0; // обнуление сторожевого таймера ОК (регистр СТТ)
}

/*
   --------------------------------------------------------------------
    Функция ЧИТАТЬ СТТ
    Назначение.
        Для считывания значения уставки сторожевого таймера.
    Вход.
        Значение регистра уставки СТТ.
    Выход.
        Результат функции: значение регистра уставки СТТ.
    Действия.
        Возвращает пользователю значение регистра уставки СТТ.        
    Условия.
        Должна работать с запретом прерываний.
        Использует регистр хранения уставки СТТ (РХУСТ).
    Обращение.
   --------------------------------------------------------------------
*/
int MUS_BSP_getSTT() {
    return *r_RUSSTT;
}

/*
   --------------------------------------------------------------------
    Функция УСТАНОВИТЬ СТТ
    Назначение.
        Для установки значения уставки сторожевого таймера.
    Вход.
        Параметр: устанавливаемое значение уставки СТТ
    Выход.
        Значение уставки СТТ = указанному в параметре функции значению.
        Разблокирован счёт СТТ со его сбросом (обнулением).
    Действия.
        Записывает в регистр уставки СТТ указанное параметром функции значение. 
    Условия.
        Должна работать с запретом прерываний.
        Использует регистр РХУСТ.
    Обращение.
   --------------------------------------------------------------------
*/
void MUS_BSP_setSTT(int time) {
    *r_RUSSTT = time;
}

/*
   --------------------------------------------------------------------
    Функция БЛОКИРОВАТЬ СТТ
    Назначение.
        Для блокировки работы сторожевого таймера.
    Вход.  
    Выход.
        СТТ заблокирован (в самом деле заблокировано сравнение счетчика СТТ с уставкой)
    Действия.
        Записывает в регистр БлСТТ значение 0хffff 
    Условия.
        Использует регистр БлСТТ.
    Обращение.
   --------------------------------------------------------------------
*/
void MUS_BSP_blockSTT() {
    *r_BlSTT = 0xFFF; // достаточно записать любое значение
}

unsigned int FixRT = 0;
/*
   --------------------------------------------------------------------  
    Функция ЧИТАТЬ RT ТАЙМЕР  
    Назначение.
        Для считывания таймера контроля времени выполнения задач.
    Вход.
        Текущее значение таймера контроля времени выполнения задач.
    Выход.
        Результат функции: значение регистра таймера контроля времени 
        выполнения задач ("RT таймера").
    Действия.
        Возвращает пользователю значение регистра таймера контроля времени 
        выполнения задач в мкс.
    Условия.
        Должна работать с запретом прерываний.
        Использует регистр счетчика таймера №1 процессора.
    Обращение.
   --------------------------------------------------------------------
*/
unsigned int MUS_BSP_getRT() {
    FixRT += 1000;
    return FixRT;
}

/*
   --------------------------------------------------------------------
    Функция ЗАПИСАТЬ RT ТАЙМЕР
    Назначение.
        Для установки значения таймера контроля времени выполнения задач
        с его запуском и сбросом запроса на прерывание.
    Вход.
        Параметр: устанавливаемое значение RT таймера.
    Выход.
        Значение регистра контроля времени выполнения задач обновлено.
    Действия.  
        1. Записывает указанное значение в регистр счетчика таймера 1 ЦП.
        2. Запускает таймер №1 ЦП, разрешает прерывание по потере 
           значимости таймера №1 ЦП.
        3. Сбрасывает запрос на прерывание от таймера №1 ЦП.
    Условия.
        Должна работать с запретом прерываний.
        Использует: регистр счетчика таймера №1 ЦП, регистр управления 
        таймера №1 ЦП, регистр сброса прерываний процессора.
    Обращение.
   --------------------------------------------------------------------
*/
void MUS_BSP_setRT(unsigned int time) {
    *r_TIMC1 = time;
    unsigned int bitset = (1 << LD) | (1 << IP); // Делаем битовый сдвиг для битов LD и IP
    *r_TIMCTR1 = (*r_TIMCTR1 & (~bitset)) | (1 << IE | 1 << EN); // Настраиваем необходимые биты в регистре TIMCTR1
    *r_ITC = 1 << iT1;
}

/*
   --------------------------------------------------------------------
    Функция ЧИТАТЬ ТАЙМЕР ПРОЦЕССОРА
    Назначение.
        Для чтения текущего значения регистра счётчика указанного таймера.
    Вход.
        Параметр: номер таймера процессора.
    Выход.
        Результат функции: значение регистра счётчика указанного таймера в мкс.
    Действия.
        В зависимости от указанного номера таймера, считывает соответствующее
        значение его регистра счётчика и возвращает пользователю. 
    Условия.
        Должна работать с запретом прерываний.
        Использует регистры счетчика таймеров № 1..5.
    Обращение.
   --------------------------------------------------------------------
*/
unsigned int MUS_BSP_getTimer(nomTimer nTimer) {
    switch (nTimer) {
        case timer1:
            return *r_TIMC1;
        case timer2:
            return *r_TIMC2;
        case timer3:
            return *r_TIMC3;
        case timer4:
            return *r_TIMC4;
        case timer5:
            return *r_TIMC5;
    }
}

/*
   --------------------------------------------------------------------
    Функция ПИСАТЬ ТАЙМЕР ПРОЦЕССОРА
    Назначение.
        Для установки значения регистра счётчика указанного таймера
        с запуском таймера и отчисткой запроса на прерывание.
    Вход.
        Параметры: номер таймера и новое значение.
    Выход.
        Значение регистра указанного таймера изменено.
    Действия.
        В зависимости от номера таймера: 
        1. Записывает указанное значение в регистр счетчика соответствующего
           таймера.
        2. Запускает счет таймера, разрешает прерывание по потере значимости 
           таймера. 
        3. Сбрасывает запрос на прерывание от соответствующего таймера.
    Условия.
        Должна работать с запретом прерываний.
        Использует регистры счетчиков таймеров №1..5 ЦП, регистры управления 
        таймеров №1..5 ЦП, регистр сброса прерываний процессора.
    Обращение.
   --------------------------------------------------------------------
*/
void MUS_BSP_setTimer(nomTimer nTimer, unsigned int time) {
    unsigned int bitset = (1 << LD) | (1 << IP); // Делаем битовый сдвиг для битов LD и IP

    switch (nTimer) {
        case timer1:
            *r_TIMC1 = time;
            *r_TIMCTR1 = (*r_TIMCTR1 & ~(bitset)) | (1 << IE | 1 << EN);
            *r_ITC = 1 << iT1;
            break;
        case timer2:
            *r_TIMC2 = time;
            *r_TIMCTR2 = (*r_TIMCTR2 & ~(bitset)) | (1 << IE | 1 << EN);
            *r_ITC = 1 << iT2;
            break;
        case timer3:
            *r_TIMC3 = time;
            *r_TIMCTR3 = (*r_TIMCTR3 & ~(bitset)) | (1 << IE | 1 << EN);
            *r_ITC = 1 << iT3;
            break;
        case timer4:
            *r_TIMC4 = time;
            *r_TIMCTR4 = (*r_TIMCTR4 & ~(bitset)) | (1 << IE | 1 << EN);
            *r_ITC = 1 << iT4;
            break;
        case timer5:
            *r_TIMC5 = time;
            *r_TIMCTR5 = (*r_TIMCTR5 & ~(bitset)) | (1 << IE | 1 << EN);
            *r_ITC = 1 << iT5;
            break;
    }
}

/*
   --------------------------------------------------------------------
    Функция УСТАНОВИТЬ ПЕРЕЗАГРУЗКУ ТАЙМЕРА
    Назначение.
        Для установки значения регистра перезагрузки и счетчика указанного 
        таймера с запуском таймера и включением режима его автоматической 
        перезагрузки и перезапуска.
    Вход.
        Параметры:
        - номер таймера процессора;           
        - значение перезагрузки (и счетчика) таймера в мкс.
    Выход.
        Значение регистров перезагрузки и счетчика указанного таймера изменено.
    Действия.
        В зависимости от номера таймера:
        - записывает указанное значение в регистр перезагрузки таймера; 
        - в счетчик таймера заносит указанное значение;
        - включает режим автоматической перезагрузки и перезапуска таймера
          по потере его значимости;
        - запускает счет таймера.
    Условия.
        Должна работать с запретом прерываний.
        Использует регистры счетчиков таймеров №1..5 ЦП, регистры управления 
        таймеров №1..5 ЦП.
    Обращение.
   --------------------------------------------------------------------
*/
void MUS_BSP_setTimerReload(nomTimer nTimer, unsigned int reload) {
    unsigned int bitset = (1 << IP); // Делаем битовый сдвиг для битов IP

    switch (nTimer) {
        case timer1:
            *r_TIMR1 = reload;
            *r_TIMCTR1 = (*r_TIMCTR1 & ~(bitset)) | (1 << RS | 1 << LD | 1 << EN);
            break;
        case timer2:
            *r_TIMR2 = reload;
            *r_TIMCTR2 = (*r_TIMCTR2 & ~(bitset)) | (1 << RS | 1 << LD | 1 << EN);
            break;
        case timer3:
            *r_TIMR3 = reload;
            *r_TIMCTR3 = (*r_TIMCTR3 & ~(bitset)) | (1 << RS | 1 << LD | 1 << EN);
            break;
        case timer4:
            *r_TIMR4 = reload;
            *r_TIMCTR4 = (*r_TIMCTR4 & ~(bitset)) | (1 << RS | 1 << LD | 1 << EN);
            break;
        case timer5:
            *r_TIMR5 = reload;
            *r_TIMCTR5 = (*r_TIMCTR5 & ~(bitset)) | (1 << RS | 1 << LD | 1 << EN);
            break;
    }
}

/*
   --------------------------------------------------------------------
    Функция УПРАВЛЕНИЕ ТАЙМЕРОМ ПРОЦЕССОРА
    Назначение.
        Для установки значения регистра управления указанного таймера.
    Вход.
        Параметры: 
        - номер таймера процессора;
        - значение регистра управления таймера
    Выход.
        Значение регистра управления указанного таймера изменено.
    Действия.
        В зависимости от номера таймера процессора, записывает указанное 
        значение в соответствующий регистр управления таймером.
    Условия.
        Должна работать с запретом прерываний.
        Использует регистры управления таймеров №1..5 ЦП.
    Обращение.
   --------------------------------------------------------------------
*/
void MUS_BSP_setTimerCtrl(nomTimer nTimer, unsigned int contrl) {
    unsigned int s32;

    s32 = contrl & m_TIMCTR;

    switch (nTimer) {
        case timer1:
            *r_TIMCTR1 = s32;
            break;
        case timer2:
            *r_TIMCTR2 = s32;
            break;
        case timer3:
            *r_TIMCTR3 = s32;
            break;
        case timer4:
            *r_TIMCTR4 = s32;
            break;
        case timer5:
            *r_TIMCTR5 = s32;
            break;
    }
}
