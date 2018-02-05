#include "stm32f10x_gpio.h"

#ifndef bool

#define false 0x00;
#define true 0x01;
#define bool uint8_t

#endif

#ifndef DS18B20_H_
#define DS18B20_H_

#ifdef __cplusplus
 extern "C" {
#endif

typedef struct OneWire_TypeDef
{
  GPIO_TypeDef* GPIOx;

  uint16_t GPIO_Pin;

  bool (*Initialize)(struct OneWire_TypeDef* OneWire);
  void (*Read)(struct OneWire_TypeDef* OneWire, uint8_t length, uint8_t* Data);
  void (*WriteByte)(struct OneWire_TypeDef* OneWire, uint8_t data);
  void (*_Delay)(__IO uint32_t nTime);
  uint8_t (*_CRC8)(uint8_t crc, uint8_t data);

}OneWire_TypeDef;

void OneWireStructInit(OneWire_TypeDef *OneWire);
bool DS18D20_ReadTemperature(OneWire_TypeDef* OneWire, uint8_t *Temperature);
void OneWire_Delay(__IO uint32_t nTime);

#ifdef __cplusplus
}
#endif

#endif /* DS18B20_H_ */
