#include "MCP4725.h"

/**************************************************************************/
/*!
    @brief  Instantiates a new MCP4725 class
*/
/**************************************************************************/
MCP4725::MCP4725() {}

/**************************************************************************/
/*!
    @brief  Setups the hardware and checks the DAC was found
    @param i2c_address The I2C address of the DAC, defaults to 0x62
    @param wire The I2C TwoWire object to use, defaults to &Wire
    @returns True if DAC was found on the I2C address.
*/
/**************************************************************************/
bool MCP4725::begin(uint8_t i2c_address, TwoWire *wire, uint32_t i2c_frequency) {
  if (i2c_dev) {
    delete i2c_dev;
  }

  i2c_dev = new Adafruit_I2CDevice(i2c_address, wire);

  if (!i2c_dev->begin()) {
    return false;
  }

  i2c_dev->setSpeed(i2c_frequency);
  return true;
}

/**************************************************************************/
/*!
    @brief  Sets the output voltage to a fraction of source vref.  (Value
            can be 0..4095)

    @param[in]  output
                The 12-bit value representing the relationship between
                the DAC's input voltage and its output voltage.
    @param[in]  writeEEPROM
                If this value is true, 'output' will also be written
                to the MCP4725's internal non-volatile memory, meaning
                that the DAC will retain the current voltage output
                after power-down or reset.
    @returns True if able to write the value over I2C
*/
/**************************************************************************/
bool MCP4725::setVoltage(uint16_t output, bool writeEEPROM) {
  uint8_t packet[3];

  if (writeEEPROM) {
    packet[0] = MCP4725_CMD_WRITEDACEEPROM;
  } else {
    packet[0] = MCP4725_CMD_WRITEDAC;
  }
  packet[1] = output / 16;        // Upper data bits (D11.D10.D9.D8.D7.D6.D5.D4)
  packet[2] = (output % 16) << 4; // Lower data bits (D3.D2.D1.D0.x.x.x.x)

  if (!i2c_dev->write(packet, 3)) {
    return false;
  }
  return true;
}
