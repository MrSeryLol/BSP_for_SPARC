#ifndef MUS_BSP_H
#define MUS_BSP_H

/* Константы для работы с регистром RPTM*/
unsigned int *const r_RPTM = (unsigned int *)0x20500504; // r/w, РПТМ, Регистр программной телеметрии
const unsigned int m_RPTM = 0x000003FF;

/* Константы для работы с регистром r_RZ1 */
unsigned int *const r_RZ1 = (unsigned int *)0x20500040;

unsigned int *const r_RGU = (unsigned int *)0x20500500; // r/w, РГУ, Регистр управления
unsigned int *const r_BUK = (unsigned int *)0x20500510; // r/w, РБУК, Регистр блокировки управления коммутаторами
unsigned int *const r_UKMVP1 = (unsigned int *)0x20500514; // r/w, РУК_МВП1,Регистр управления коммутатором МВП1
unsigned int *const r_UKMVP2 = (unsigned int *)0x20500518; // r/w, РУК_МВП2,Регистр управления коммутатором МВП2

/* Константы для регистров узлов диагностики */
#define vklRUK 0x000FFA5 // код включения коммутатора МВП на регистре РУК_МВП1/РУК_МВП2

#define ASI02 0x02
#define ASI16 0x10
#define ASI17 0x11

/*
   ----------------------------------------------------------------------
   Перечисление USED_ASR
   Назначение.
       Константы, которые используются для выбора необходимого регистра ASR:
       1) ASR16;
       2) ASR17;
       3) ASR18.

       1) Регистр ASR16 (Регистр управления защитой регистра %asr16):

       31   30 29  27 26      18  17    16   15  14 13  11 10      3  2     1    0
       ______________________________________________________________________________
       | FPFT | FCNT | RESERVED | FTE | FDI | IUFT | ICNT | TB[7:0] | DP | ITE | IDI|
       ‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾

       [31:30]: FP FT ID - определяемый на условиях FPU тип SEU защиты
       [29:27]: количество FCNT ошибок - количество ошибок, обнаруженных в регистре FP.
       [26:18]: сохранение
       [17]:    запуск отладки FPU RF – запускается модуль отладки регистра FPU. Перед записью
                скорректированного бита (ТВ) в регистр FPU производится проверка на четность
                логической операцией «исключающее ИЛИ»
       [16]:    запрет защиты регистров FP (registerRF) – если после установки запрещена защита
                контролем по четности FP RF
       [15:14]: IU FT ID - определяемый на условиях IU тип SEU защиты
       [13:11]: количество IU ошибок - количество ошибок, обнаруженных в регистре IU
       [10:3]:  откорректированные RF биты (FTB) – данные биты в процессе отладки проходят
                проверку на четность логической операцией «исключающее ИЛИ», после чего
                заносятся в регистр
       [2]:     DP ram select (DP) – используется только в IU или FPU, включает 2 парных
                интерфейса ram
       [1]:     запуск отладки IU RF - запускается модуль отладки регистра. Перед записью
                скорректированного бита (ТВ) в регистр FPU производится проверка на четность
                логической операцией «исключающее ИЛИ»
       [0]:     запрет защиты регистров IU RF (IDI) – если после установки запрещена защита
                контролем по четности IU RF

      2) Регистр ASR17 (Регистр конфигурации процессора SoC2012 %asr17)

      31     28 27      18  17  16  15  14   13   12  11 10  9   8   7   5 4    0
       ___________________________________________________________________________
       | INDEX | RESERVED | CS |  CF  | DW | SV | LD | FPU | M | V8 | NWP | NWIN |
       ‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾
       [31:28]: индекс процессора в мультипроцессорной системе; каждое ядро процессора имеет
                данные о поддержке индексов.
       [17]:    запуск выключателя тактовой частоты. Если произведены соответствующие
                установки, то происходит разблокировка выключателей АНВ и частоты процессора.
       [16:15]: синхронизирующая частота процессора (CF). Ядра процессора работают на частоте
                (CF+1) x AHB ((CF+1) times AHB frequency).
       [14]:    отключение системного прерывания вследствие обнаружения ошибки записи
                (DWT). При заданном значении «0» одно системное прерывание вследствие
                обнаружения ошибки записи (tt=0x2b) будет пропущено. При заданном значении
                «1» системное прерывание вследствие обнаружения ошибки будет разрешено.
                После перезагрузки данный параметр устанавливается в значении «0».
       [13]:    запуск захвата одинарного вектора. При заданном значении «1» происходит запуск
                данного системного прерывания. В случае неиспользования захвата одинарного
                вектора, данный параметр необходимо установить в значении «0». После
                перезагрузки данный параметр устанавливается в значении «0».
       [12]:    задержка загрузки. Если произведены соответствующие установки, то линия потока
                данных использует задержку загрузки в два такта, в противном случае использует
                задержку загрузки в один такт.
       [11:10]: выбор модуля операций с плавающей точкой. «00» = no FPU; «01» = GRFPU; «10» =
                Meiko FPU, «11» = GRFPU-Lite
       [9]:     Если произведены соответствующие установки, то доступна для использования
                опционная команда умножения с накоплением (МАС).
       [8]:     Если произведены соответствующие установки, то доступна для использования
                команда умножения SPARC V8.
       [7:5]:   количество выполнимых точек контроля (0-4).
       [4:0]:   количество выполнимых регистровых окон, равнозначно NWIN+1.


      3) Регистр ASR18 (значение регистра младшей части (разряды 31:0) сумматора для команды UMAC %asr18) 
      Команда UMAC производит умножение без знака 16-битного операнда с 32-битным результатом, который также
      суммируется, и в 40-битном сумматоре, который, в свою очередь, состоит из %y регистра
      младших 8 битов и %asr18 регистра.

   ----------------------------------------------------------------------

*/
#define ASR16 16
#define ASR17 17
#define ASR18 18

/* Маски для настройки FPU и IU регистров в ASR16*/
#define HamcodeInFPU 0x80000000
#define HamcodeInIU 0x8000

/* Константы адресов для регистров, связанных с SPR */
#define REGISTER_Y_ADDRESS 0x90400000
#define REGISTER_FSR_ADDRESS 0x90400018
#define REGISTER_ASR16_ADDRESS 0x90400040
#define REGISTER_ASR17_ADDRESS 0x90400044
#define REGISTER_ASR18_ADDRESS 0x90400048
#define INCORRECT_REGISTER_ADDRESS 0xDEADDEAD


unsigned int *const r_RZC1 = (unsigned int *)0x20500030; //r  , РЗС1, Чтение регистра запросов 1 со сбросом всех заявок
unsigned int *const r_RZC2 = (unsigned int *)0x20500034; //r  , РЗС2, Чтение регистра запросов 2 со сбросом всех заявок
unsigned int *const r_RZC3 = (unsigned int *)0x20500038; //r  , РЗС3, Чтение регистра запросов 3 со сбросом всех заявок 

/* ---------------------- Interupt Registers ---------------------------- */
unsigned int *const r_ITC = (unsigned int *)0x8000020C;; //w, Interrupt Clear Register

/* ----------------------- AHB Registers ------------------------------- */
unsigned int *const r_StatusAHB = (unsigned int *)0x80000F00; //r/w, AHB Status register
unsigned int *const r_Failar = (unsigned int *)0x80000F04; //r/ , AHB Failing address register


unsigned int *const r_RAO = (unsigned int *)0x20500530; //r,   РАО, Регистр адреса одиночной ошибки
unsigned int *const r_RSO = (unsigned int *)0x20500534; //r/w, РCO, Регистр счетчика одиночных ошибок
unsigned int *const r_RAD = (unsigned int *)0x20500538; //r,   РАД, Регистр адреса двойной ошибки
unsigned int *const r_RSD = (unsigned int *)0x2050053C; //r/w, РСД, Регистр счетчика двойных ошибок
unsigned int *const r_RUE = (unsigned int *)0x20500540; //r/w, РУЕ, Регистр управления EDAC


void MUS_BSP_SetASI(int asi, int data);
int MUS_BSP_GetASI(int asi);

/* 
   ----------------------------------------------------------------------
    Функция SetASR
    Записать значение в необходимый регистр ASR
   ----------------------------------------------------------------------
*/
void MUS_BSP_SetASR(int asrNum, int data);

/* 
   ----------------------------------------------------------------------
    Функция GetASR
    Считать значение необходимого регистра ASR
   ----------------------------------------------------------------------
*/
int MUS_BSP_GetASR(int asrNum);

/*
   ----------------------------------------------------------------------
    Функция ЧИТАТЬ РПТМ  
    Назначение.  
        Для чтения регистра РПТМ.  
    Вход.  
        Регистр РПТМ.  
    Выход.  
        Результат функции - значащая часть регистра РПТМ.  
    Действия.  
        Читает значение регистр РПТМ, накладывает на него маску значащих 
        битов РПТМ, возвращает полученное значение пользователю.  
    Условия.
        Должна работать с запретом прерываний.
        Использует регистр РПТМ. 
    Обращение.
   ----------------------------------------------------------------------
*/
int MUS_BSP_GetRPTM();

/*
   ----------------------------------------------------------------------
    Функция ЗАПИСАТЬ РПТМ  
    Назначение.  
        Для записи в регистр РПТМ.  
    Вход.  
        Параметр: 
        - записываемое значение в регистр РПМТ.
    Выход.  
        Обновлённое значение значащих битов регистра РПТМ.  
    Действия.  
        Записывает в значащие биты регистры РПТМ соответствующие биты из 
        значения указанного пользователем. 
    Условия.
        Должна работать с запретом прерываний.
        Использует регистр РПТМ. 
    Обращение.
   ----------------------------------------------------------------------
*/
void MUS_BSP_SetRPTM(int reg);

/*
   --------------------------------------------------------------------
    Системная функция ЧИТАТЬ РСН
    Чтение текущего значения заданного регистра специального назначения 
    процессора.
   --------------------------------------------------------------------*)
*/
int MUS_BSP_GetSPR(int aReg);

/* 
   ----------------------------------------------------------------------
    Функция Set_r_RGU
    Записать значение в регистр РГУ, Регистр управления
   ----------------------------------------------------------------------
*/
void MUS_BSP_Set_r_RGU(int data);

/* 
   ----------------------------------------------------------------------
    Функция Get_r_RGU
    Считать значение регистр РГУ, Регистр управления
   ----------------------------------------------------------------------
*/
int MUS_BSP_Get_r_RGU();

/* 
   ----------------------------------------------------------------------
    Функция Set_r_BUK
    Записать значение в регистр РБУК, Регистр блокировки управления коммутаторами
   ----------------------------------------------------------------------
*/
void MUS_BSP_Set_r_BUK(int data);

/* 
   ----------------------------------------------------------------------
    Функция Get_r_BUK
    Считать значение регистра РБУК, Регистр блокировки управления коммутаторами
   ----------------------------------------------------------------------
*/
int MUS_BSP_Get_r_BUK(); 

/* 
   ----------------------------------------------------------------------
    Функция Set_r_UKMVP1
    Записать значение в регистр РУК_МВП1,Регистр управления коммутатором МВП1
   ----------------------------------------------------------------------
*/
void MUS_BSP_Set_r_UKMVP1(int data);

/* 
   ----------------------------------------------------------------------
    Функция Get_r_UKMVP1
    Считать значение регистра РУК_МВП1,Регистр управления коммутатором МВП1
   ----------------------------------------------------------------------
*/
int MUS_BSP_Get_r_UKMVP1();

/* 
   ----------------------------------------------------------------------
    Функция Set_r_UKMVP2
    Записать значение в регистр РУК_МВП2,Регистр управления коммутатором МВП2
   ----------------------------------------------------------------------
*/
void MUS_BSP_Set_r_UKMVP2(int data);

/* 
   ----------------------------------------------------------------------
    Функция Get_r_UKMVP2
    Считать значение регистра РУК_МВП2, Регистр управления коммутатором МВП2
   ----------------------------------------------------------------------
*/
int MUS_BSP_Get_r_UKMVP2();

#endif