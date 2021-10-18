#ifndef INC_MCP2515_H_
#define INC_MCP2515_H_

#include "stdint.h"
#include "stddef.h"
#include "stdbool.h"


/// @brief Размер массива для хранения передаваемых и принимаемых данных
#define MCP2515_BUFFER_SIZE 32


/// @defgroup MCP2515_RTS_TYPE Варианты запросов на передачу
/// @{
#define RTS_TYPE_BUFFER0 0U  ///< Отправить данные из буфера 0
#define RTS_TYPE_BUFFER1 2U  ///< Отправить данные из буфера 1
#define RTS_TYPE_BUFFER2 4U  ///< Отправить данные из буфера 2
/// @}


/// @defgroup MCP2515_Status Поля статуса
/// @{
#define MCP2515_STATUS_RX0IF  0x01U ///< Прерывание по приему буфера 0
#define MCP2515_STATUS_RX1IF  0x02U ///< Прерывание по приему буфера 1
#define MCP2515_STATUS_TX0REQ 0x04U ///< Запрос на передачу буфера 0
#define MCP2515_STATUS_TX0IF  0x08U ///< Прерывание по окончанию передачи буфера 0
#define MCP2515_STATUS_TX1REQ 0x10U ///< Запрос на передачу буфера 1
#define MCP2515_STATUS_TX1IF  0x20U ///< Прерывание по окончанию передачи буфера 1
#define MCP2515_STATUS_TX2REQ 0x40U ///< Запрос на передачу буфера 2
#define MCP2515_STATUS_TX2IF  0x80U ///< Прерывание по окончанию передачи буфера 2
/// @}


/// @defgroup MCP2515_RxStatus Поля статуса приема
/// @{
#define MCP2515_RXSTATUS_RECMSG_MASK       0xC0U ///< Маска принятых сообщений
#define MCP2515_RXSTATUS_RECMSG_NONE       0x00U ///< Нет принятых сообщений
#define MCP2515_RXSTATUS_RECMSG_RXB0       0x40U ///< Сообщение принято в буфер 0
#define MCP2515_RXSTATUS_RECMSG_RXB1       0x80U ///< Сообщение принято в буфер 1
#define MCP2515_RXSTATUS_RECMSG_BOTH       0xC0U ///< Сообщение принято в оба буфера

#define MCP2515_RXSTATUS_MSGTYPE_MASK      0x18U ///< Маска типа сообщения
#define MCP2515_RXSTATUS_MSGTYPE_STD       0x00U ///< Стандартный фрейм
#define MCP2515_RXSTATUS_MSGTYPE_STDREMOTE 0x08U ///< Стандартный RTR фрейм
#define MCP2515_RXSTATUS_MSGTYPE_EXT       0x10U ///< Расширенный фрейм
#define MCP2515_RXSTATUS_MSGTYPE_EXTREMOTE 0x18U ///< Расширенный RTR фрейм

#define MCP2515_RXSTATUS_FILTER_MASK       0x07U ///< Маска совпадения фильтров
#define MCP2515_RXSTATUS_FILTER_RXF0       0x00U ///<
#define MCP2515_RXSTATUS_FILTER_RXF1       0x01U ///<
#define MCP2515_RXSTATUS_FILTER_RXF2       0x02U ///<
#define MCP2515_RXSTATUS_FILTER_RXF3       0x03U ///<
#define MCP2515_RXSTATUS_FILTER_RXF4       0x04U ///<
#define MCP2515_RXSTATUS_FILTER_RXF5       0x05U ///<
#define MCP2515_RXSTATUS_FILTER_RXF0ROLL   0x06U ///< RXF0 rollover RXB1
#define MCP2515_RXSTATUS_FILTER_RXF1ROLL   0x07U ///< RXF1 rollover RXB1
/// @}


/// @brief Структура, описывающая интерфейс передачи данных для MCP2515
typedef struct
{
  /// @brief Функция обратного вызова для сигнала сброса
  /// @param reset 1 - переводит микросхему в режим сброса (низкий лог. уровень);
  /// 0 - микросхема в обычном режиме.
  void (*onReset) (uint32_t reset);

  /// @brief Функция обратного вызова для сигнала ChipSelect SPI
  /// @param select 1 - микросхема выбрана для обмена (низкий лог. уровень);
  /// 0 - микросхема в высокоимпедансном состоянии (высокий лог. уровень)
  void (*onChipSelect) (uint32_t select);

  /// @brief Функция обратного вызова для передачи данных по SPI
  /// @details Сами данные перед вызовом функции хранятся в массиве @ref buffer
  /// @param len количество передаваемых байтов
  /// @returns true, если передача данных прошла успешно
  bool (*onTransaction) (size_t len);

  /// @brief Функция обратного вызова, сигнализирующая о возникновении прерывания
  /// @details Под прерыванием подразумевается низкий уровень сигнала INT
  void (*onInterrupt) (void);

  /// @brief Массив для хранения передаваемых и принимаемых данных
  uint8_t buffer [MCP2515_BUFFER_SIZE];

} MCP2515_Interface_t;


/// @brief Перечисление, описывающее варианты чтения приемного буфера
typedef enum
{
  READ_RX_BUFFER_RXB0SIDH = 0x90U,  ///< Читать 13 байт, начиная с RXB0SIDH
  READ_RX_BUFFER_RXB0D0   = 0x92U,  ///< Читать 8 байт, начиная с RXB0D0
  READ_RX_BUFFER_RXB1SIDH = 0x94U,  ///< Читать 13 байт, начиная с RXB1SIDH
  READ_RX_BUFFER_RXB1D0   = 0x96U   ///< Читать 8 байт, начиная с RXB1D0
} ReadRxBuffer_Type;


/// @brief Перечисление, описывающее варианты загрузки данных в передающий буфер
typedef enum
{
  LOAD_TX_BUFFER_TXB0SIDH = 0x40U,  ///< Писать 13 байт, начиная с TXB0SIDH
  LOAD_TX_BUFFER_TXB0D0   = 0x41U,  ///< Писать 8 байт, начиная с TXB0D0
  LOAD_TX_BUFFER_TXB1SIDH = 0x42U,  ///< Писать 13 байт, начиная с TXB1SIDH
  LOAD_TX_BUFFER_TXB1D0   = 0x43U,  ///< Писать 8 байт, начиная с TXB1D0
  LOAD_TX_BUFFER_TXB2SIDH = 0x44U,  ///< Писать 13 байт, начиная с TXB2SIDH
  LOAD_TX_BUFFER_TXB2D0   = 0x45U   ///< Писать 8 байт, начиная с TXB1D0
} LoadTxBuffer_Type;


/// @brief Инициализирует интерфейс для работы с микросхемой
/// @details Перед использованием функции нужно установить знаычения
/// полей в @ref iface.
///
/// Выводит микросхему из режима сброса, устанавливает высокий лог. уровень
/// ChipSelect
/// @param iface конкретный интерфейс для работы с микросхемой
/// @param intFunc указатель на функцию, вызываемую при возниновении прерывания
/// (низкий уровень сигнала INT)
/// @returns true, если инициализация прошла успешно
bool mcp2515_Initialize (MCP2515_Interface_t *iface, void (*intFunc)(void));


/// @brief Передает по SPI последовательность данных для программного сброса
/// @param iface конкретный интерфейс для работы с микросхемой
/// @returns true, если функция выполнена успешно
bool mcp2515_SoftReset (MCP2515_Interface_t *iface);


/// @brief Передает по SPI последовательность данных для чтения регистров
/// @param iface конкретный интерфейс для работы с микросхемой
/// @param addr адрес первого регистра, с которого начнется чтение
/// @param len количество читаемых подряд регистров
/// @returns NULL, если параметры функции заданы не верно, либо не удалось прочитать
/// данные; в противном случае возвращает адрес, где хранятся считанные данные
uint8_t *mcp2515_Read (MCP2515_Interface_t *iface, const uint8_t addr, uint8_t len);


/// @brief Передает по SPI последовательность данных для чтения приемного буфера
/// @param iface конкретный интерфейс для работы с микросхемой
/// @param type задает что конкретно нужно считывать (см. @ref ReadRxBuffer_Type)
/// @param [out] len в данную переменную запишется количество считанных данных
/// @returns NULL, если параметры функции заданы не верно, либо не удалось прочитать
/// данные; в противном случае возвращает адрес, где хранятся считанные данные
uint8_t *mcp2515_ReadRxBuffer (MCP2515_Interface_t *iface, ReadRxBuffer_Type type, uint8_t *len);


/// @brief Передает по SPI последовательность данных для записи регистров
/// @param iface конкретный интерфейс для работы с микросхемой
/// @param addr адрес первого регистра, с которого начнется запись
/// @param [in] data данные, которые нужно записать в регистры
/// @param len количество записываемых подряд регистров
/// @returns true, если функция выполнена успешно
bool mcp2515_Write (MCP2515_Interface_t *iface, const uint8_t addr, uint8_t *data, uint8_t len);


/// @brief Передает по SPI последовательность данных для загрузки в передающий буфер
/// @param iface конкретный интерфейс для работы с микросхемой
/// @param type задает куда конкретно нужно загрузить (см. @ref LoadTxBuffer_Type)
/// @param [in] data данные, которые нужно записать в буфер
/// @returns true, если функция выполнена успешно
bool mcp2515_LoadTxBuffer (MCP2515_Interface_t *iface, LoadTxBuffer_Type type, uint8_t *data);


/// @brief Передает по SPI последовательность для запроса отправки данных из передающих буферов
/// @param iface конкретный интерфейс для работы с микросхемой
/// @param type комбинация @ref MCP2515_RTS_TYPE
/// @returns true, если функция выполнена успешно
bool mcp2515_RTS (MCP2515_Interface_t *iface, uint8_t type);


/// @brief Передает по SPI последовательность данных для чтения статуса
/// @param iface конкретный интерфейс для работы с микросхемой
/// @returns -1, если произошла ошибка; значение статуса (>= 0), если функция выполнена успешно.
/// См. @ref MCP2515_Status
int32_t mcp2515_ReadStatus (MCP2515_Interface_t *iface);


/// @brief Передает по SPI последовательность данных для чтения статуса приема
/// @param iface конкретный интерфейс для работы с микросхемой
/// @returns -1, если произошла ошибка; значение статуса (>= 0), если функция выполнена успешно.
/// См. @ref MCP2515_RxStatus
int32_t mcp2515_RxStatus (MCP2515_Interface_t *iface);


/// @brief Передает по SPI последовательность данных для модификации бит регистра
/// @details Не все регистры можно модифицировать данной функцией, см. документацию
/// @param iface конкретный интерфейс для работы с микросхемой
/// @param addr адрес регистра, биты которого нужно модифицировать
/// @param mask маска модификации бит
/// @param data данные, к которым применится маска и они запишутся в регистр
/// @returns true, если функция выполнена успешно
bool mcp2515_BitModify (MCP2515_Interface_t *iface, uint8_t addr, uint8_t mask, uint8_t data);

#endif /* INC_MCP2515_H_ */
