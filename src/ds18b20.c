#include "ds18b20.h"

/**
 * Initialization sequence:
 * Pull line --> wait for 490 us --> release line --> wait for 60 us --> check if line is pulled down
 */
bool _OneWire_Initialize(OneWire_TypeDef* OneWire)
{
	// Initialization sequence
	OneWire->GPIOx->BRR = OneWire->GPIO_Pin;
	OneWire->_Delay(480);
	OneWire->GPIOx->BSRR = OneWire->GPIO_Pin;
	OneWire->_Delay(65);

	if ((OneWire->GPIOx->IDR & OneWire->GPIO_Pin) != 0x00)
	{
		// no one-wire devices found
		return false;
	}
	OneWire->_Delay(405);

	return true;
}

/**
 * Sending logic "1" to device:
 * pull line --> wait for 1 us --> release line
 *
 * Sending logic "0" to device:
 * pull line --> wait for 90 us --> release line
 */
void _OneWire_WriteByte(OneWire_TypeDef* OneWire, uint8_t data)
{
	for (uint8_t i = 0x00; i < 0x08; i++)
	{
		if ((data & 0x01) == 0x01) {
			// Send 1
			OneWire->GPIOx->BRR = OneWire->GPIO_Pin;
			OneWire->_Delay(1);
			OneWire->GPIOx->BSRR = OneWire->GPIO_Pin;
			OneWire->_Delay(60);
		} else {
			// Send "0"
			OneWire->GPIOx->BRR = OneWire->GPIO_Pin;
			OneWire->_Delay(60);
			OneWire->GPIOx->BSRR = OneWire->GPIO_Pin;
			OneWire->_Delay(10);
		}

		data = data >> 1;
	}
}

void _OneWire_Read(OneWire_TypeDef* OneWire, uint8_t length, uint8_t* Data)
{
	uint8_t bit = 0x00, byte = 0x00;
	for (byte = 0x00; byte < length; byte++)
	{
		Data[byte] = 0x00;
		for (bit = 0x00; bit < 0x08; bit++)
		{
			OneWire->GPIOx->BRR = OneWire->GPIO_Pin;
			OneWire->_Delay(2);
			OneWire->GPIOx->BSRR = OneWire->GPIO_Pin;
			OneWire->_Delay(20);
			if ((OneWire->GPIOx->ODR & OneWire->GPIO_Pin) != 0x00)
			{
				Data[byte] ^= 1 << bit;
			}
			OneWire->_Delay(45);
		}
	}
}

uint8_t _OneWire_CRC8(uint8_t crc, uint8_t data)
{
	uint8_t i;

	crc = crc ^ data;
	for (i = 0; i < 8; i++)
	{
		if (crc & 0x01) {
			crc = (crc >> 1) ^ 0x8C;
		} else {
			crc >>= 1;
		}
	}

	return crc;
}

bool DS18D20_ReadTemperature(OneWire_TypeDef* OneWire, uint8_t *Temperature)
{
	Temperature = 0x00;
	if (!OneWire->Initialize(OneWire))
	{
		return false;
	}

	OneWire->WriteByte(OneWire, 0xCC);
	OneWire->WriteByte(OneWire, 0x44);
	OneWire->GPIOx->BRR = OneWire->GPIO_Pin;
	OneWire->_Delay(755);
	OneWire->GPIOx->BSRR = OneWire->GPIO_Pin;
	if (!OneWire->Initialize(OneWire))
	{
		return false;
	}
	OneWire->WriteByte(OneWire, 0xCC);
	OneWire->WriteByte(OneWire, 0xBE);
	uint8_t Data[9] = { 0x00 };
	OneWire->Read(OneWire, 9, Data);
	uint8_t crc = 0x00;
	for (uint8_t i = 0x00; i < 9; i++) {
		crc = OneWire->_CRC8(crc, Data[i]);
	}

	if (crc != Data[8]) {
		return false;
	}
	Temperature[0] = Data[0];
	Temperature[1] = Data[1];

	return true;
}

void OneWireStructInit(OneWire_TypeDef *OneWire)
{
	OneWire->Initialize = &_OneWire_Initialize;
	OneWire->Read = &_OneWire_Read;
	OneWire->WriteByte = &_OneWire_WriteByte;
	OneWire->_Delay = &OneWire_Delay;
	OneWire->_CRC8 = &_OneWire_CRC8;
}
