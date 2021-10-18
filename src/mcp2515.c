#include "mcp2515.h"


bool mcp2515_Initialize (MCP2515_Interface_t *iface, void (*intFunc)(void))
{
  // Все эти функции должны быть обязательно заданы
  if ((iface->onReset == NULL) ||
	  (iface->onChipSelect == NULL) ||
      (iface->onTransaction == NULL))
    return false;

  iface->onInterrupt = intFunc;

  iface->onChipSelect (0);
  iface->onReset (0);
  return true;
}


bool mcp2515_SoftReset (MCP2515_Interface_t *iface)
{
  iface->buffer [0] = 0xC0;

  iface->onChipSelect (1);
  bool res = iface->onTransaction (1);
  iface->onChipSelect (0);
  return res;
}


uint8_t *mcp2515_Read (MCP2515_Interface_t *iface, const uint8_t addr, uint8_t len)
{
  // Нужно, чтобы все помещалось в буфер
  if (len > (MCP2515_BUFFER_SIZE - 2))
    return NULL;

  iface->buffer [0] = 0x03;
  iface->buffer [1] = addr;

  // В даташите сказано, что байты, следующие далее, не имеют значения,
  // поэтому передаем мусор и не тратим процессорное время

  uint8_t *res = NULL;
  iface->onChipSelect (1);
  if (iface->onTransaction (len + 2))
    res = &iface->buffer [2];
  iface->onChipSelect (0);
  return res;
}


uint8_t *mcp2515_ReadRxBuffer (MCP2515_Interface_t *iface, ReadRxBuffer_Type type, uint8_t *len)
{
  *len = ((uint8_t)type & 0x02) ? 8 : 13;
  iface->buffer [0] = (uint8_t)type;

  // В даташите сказано, что байты, следующие далее, не имеют значения,
  // поэтому передаем мусор и не тратим процессорное время

  uint8_t *res = NULL;
  iface->onChipSelect (1);
  if (iface->onTransaction (*len + 1))
    res = &iface->buffer [1];
  iface->onChipSelect (0);
  return res;
}


bool mcp2515_Write (MCP2515_Interface_t *iface, const uint8_t addr, uint8_t *data, uint8_t len)
{
  // Нужно, чтобы все помещалось в буфер
  if (len > (MCP2515_BUFFER_SIZE - 2))
    return false;

  uint8_t *p = &iface->buffer [0];
  uint8_t  l = len;
  *p++ = 0x02;
  *p++ = addr;
  while (l--)
	*p++ = *data++;

  iface->onChipSelect (1);
  bool res = iface->onTransaction (len + 2);
  iface->onChipSelect (0);
  return res;
}


bool mcp2515_LoadTxBuffer (MCP2515_Interface_t *iface, LoadTxBuffer_Type type, uint8_t *data)
{
  uint8_t len = ((uint8_t)type & 0x01) ? 8 : 13;
  iface->buffer [0] = (uint8_t)type;

  uint8_t l = len;
  uint8_t *b = &iface->buffer [1];
  while (l--)
	*b++ = *data++;

  iface->onChipSelect (1);
  bool res = iface->onTransaction (len + 1);
  iface->onChipSelect (0);
  return res;
}


bool mcp2515_RTS (MCP2515_Interface_t *iface, uint8_t instruction)
{
  // Нам нужны только некоторые биты, остальные обнуляем
  instruction &= 0x07;
  instruction |= 0x80;

  iface->buffer [0] = instruction;

  iface->onChipSelect (1);
  bool res = iface->onTransaction (1);
  iface->onChipSelect (0);
  return res;
}


int32_t mcp2515_ReadStatus (MCP2515_Interface_t *iface)
{
  iface->buffer [0] = 0xA0;

  int32_t res = -1;
  iface->onChipSelect (1);
  if (0 == iface->onTransaction (2))
    res = (int32_t)iface->buffer [1];
  iface->onChipSelect (0);
  return res;
}


int32_t mcp2515_RxStatus (MCP2515_Interface_t *iface)
{
  iface->buffer [0] = 0xB0;

  int32_t res = -1;
  iface->onChipSelect (1);
  if (0 == iface->onTransaction (2))
    res = (int32_t)iface->buffer [1];
  iface->onChipSelect (0);
  return res;
}


bool mcp2515_BitModify (MCP2515_Interface_t *iface, uint8_t addr, uint8_t mask, uint8_t data)
{
  iface->buffer [0] = 0x05;
  iface->buffer [1] = addr;
  iface->buffer [2] = mask;
  iface->buffer [3] = data;

  iface->onChipSelect (1);
  bool res = iface->onTransaction (4);
  iface->onChipSelect (0);
  return res;
}
