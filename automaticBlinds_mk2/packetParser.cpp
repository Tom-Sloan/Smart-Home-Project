#include <string.h>
#include <Arduino.h>
#include <bluefruit.h>


#define PACKET_ACC_LEN                  (5)
#define PACKET_GYRO_LEN                 (5)
#define PACKET_MAG_LEN                  (5)
#define PACKET_QUAT_LEN                 (5)
#define PACKET_BUTTON_LEN               (5)
#define PACKET_COLOR_LEN                (5)
#define PACKET_LOCATION_LEN             (5)

//    READ_BUFSIZE            Size of the read buffer for incoming packets
#define READ_BUFSIZE                    (20)


/* Buffer to hold incoming characters */
uint8_t packetbuffer[READ_BUFSIZE + 1];

/**************************************************************************/
/*!
    @brief  Casts the four bytes at the specified address to a float
*/
/**************************************************************************/
float parsefloat(uint8_t *buffer)
{
  float f;
  memcpy(&f, buffer, 4);
  return f;
}

/**************************************************************************/
/*!
    @brief  Prints a hexadecimal value in plain characters
    @param  data      Pointer to the byte data
    @param  numBytes  Data length in bytes
*/
/**************************************************************************/
void printHex(const uint8_t * data, const uint32_t numBytes)
{
  uint32_t szPos;
  for (szPos = 0; szPos < numBytes; szPos++)
  {
    Serial.print(F("0x"));
    // Append leading 0 for small values
    if (data[szPos] <= 0xF)
    {
      Serial.print(F("0"));
      Serial.print(data[szPos] & 0xf, HEX);
    }
    else
    {
      Serial.print(data[szPos] & 0xff, HEX);
    }
    // Add a trailing space if appropriate
    if ((numBytes > 1) && (szPos != numBytes - 1))
    {
      Serial.print(F(" "));
    }
  }
  Serial.println();
}

/**************************************************************************/
/*!
    @brief  Waits for incoming data and parses it
*/
/**************************************************************************/
uint8_t readPacket(BLEUart *ble_uart, uint16_t timeout)
{
  uint16_t origtimeout = timeout, replyidx = 0;

  memset(packetbuffer, 0, READ_BUFSIZE);

  while (timeout--) {

    if (replyidx >= 20) break;
    //    Serial.print("Packet buffer ");
    //    Serial.print(0);
    //    Serial.print(": ");
    //    Serial.write(packetbuffer[0]);
    //    Serial.println("");
    //
    //    Serial.print("Packet buffer ");
    //    Serial.print(1);
    //    Serial.print(": ");
    //    Serial.write(packetbuffer[1]);
    //    Serial.println("");
    //
    //    Serial.print("Packet buffer ");
    //    Serial.print(2);
    //    Serial.print(": ");
    //    Serial.write(packetbuffer[2]);
    //    Serial.println("");
    //
    //    Serial.print("Packet buffer ");
    //    Serial.print(3);
    //    Serial.print(": ");
    //    Serial.write(packetbuffer[3]);
    //    Serial.println("");
    //
    //    Serial.print("Packet buffer ");
    //    Serial.print(4);
    //    Serial.print(": ");
    //    Serial.write(packetbuffer[4]);
    //    Serial.println("");
    //
    //    Serial.print("Packet buffer ");
    //    Serial.print(5);
    //    Serial.print(": ");
    //    Serial.write(packetbuffer[5]);
    //    Serial.println("");



    if ((packetbuffer[1] == 'B') && (replyidx == PACKET_BUTTON_LEN)) {
      //      Serial.print("Packet buffer ");
      //      Serial.print(0);
      //      Serial.print(": ");
      //      Serial.write(packetbuffer[0]);
      //      Serial.println("");
      //
      //      Serial.print("Packet buffer ");
      //      Serial.print(1);
      //      Serial.print(": ");
      //      Serial.write(packetbuffer[1]);
      //      Serial.println("");
      //
      //      Serial.print("Packet buffer ");
      //      Serial.print(2);
      //      Serial.print(": ");
      //      Serial.write(packetbuffer[2]);
      //      Serial.println("");
      //
      //      Serial.print("Packet buffer ");
      //      Serial.print(3);
      //      Serial.print(": ");
      //      Serial.write(packetbuffer[3]);
      //      Serial.println("");
      //      Serial.print("Packet buffer DECIMAL ");
      //      Serial.print(3);
      //      Serial.print(": ");
      //      Serial.print(packetbuffer[3], DEC);
      //      Serial.println("");
      //
      //      Serial.print("Packet buffer ");
      //      Serial.print(4);
      //      Serial.print(": ");
      //      Serial.write(packetbuffer[4]);
      //      Serial.println("");
      //      Serial.print("Packet buffer DECIMAL ");
      //      Serial.print(4);
      //      Serial.print(": ");
      //      Serial.print(packetbuffer[4], DEC);
      //      Serial.println("");
      //
      //      Serial.print("Packet buffer ");
      //      Serial.print(5);
      //      Serial.print(": ");
      //      Serial.write(packetbuffer[5]);
      //      Serial.println("");
      //
      //      Serial.print("Packet buffer DECIMAL ");
      //      Serial.print(5);
      //      Serial.print(": ");
      //      Serial.print(packetbuffer[5], DEC);
      //      Serial.println("");
      break;
    }
    if ((packetbuffer[1] == 'C') && (replyidx == PACKET_COLOR_LEN)) {
      //      Serial.print("Packet buffer ");
      //      Serial.print(0);
      //      Serial.print(": ");
      //      Serial.println(packetbuffer[0]);
      //
      //      Serial.print("Packet buffer ");
      //      Serial.print(1);
      //      Serial.print(": ");
      //      Serial.write(packetbuffer[1]);
      //      Serial.println("");
      //
      //      Serial.print("Packet buffer ");
      //      Serial.print(2);
      //      Serial.print(": ");
      //      Serial.write(packetbuffer[2]);
      //      Serial.println("");
      //
      //      Serial.print("Packet buffer ");
      //      Serial.print(3);
      //      Serial.print(": ");
      //      Serial.write(packetbuffer[3]);
      //      Serial.println("");
      //
      //      Serial.print("Packet buffer ");
      //      Serial.print(4);
      //      Serial.print(": ");
      //      Serial.write(packetbuffer[4]);
      //      Serial.println("");
      //
      //      Serial.print("Packet buffer ");
      //      Serial.print(5);
      //      Serial.print(": ");
      //      Serial.write(packetbuffer[5]);
      //      Serial.println("");
      break;
    }
    if ((packetbuffer[1] == 'D') && (replyidx == PACKET_COLOR_LEN)) {
      break;
    }

    while (ble_uart->available()) {
      char c =  ble_uart->read();
      if (c == '!') {
        replyidx = 0;
      }
      packetbuffer[replyidx] = c;
      replyidx++;
      timeout = origtimeout;
    }

    if (timeout == 0) break;
    delay(1);
  }

  packetbuffer[replyidx] = 0;  // null term

  if (!replyidx)  // no data or timeout
    return 0;
  if (packetbuffer[0] != '!')  // doesn't start with '!' packet beginning
    return 0;

//ADAfruit added this but i don't get what it does and after testing couldn't find a error that this would stop. If this is important then we are screwed.

  // check checksum!
//  uint8_t xsum = 0;
//  uint8_t checksum = packetbuffer[replyidx - 1];
//
//  for (uint8_t i = 0; i < replyidx - 1; i++) {
//    xsum += packetbuffer[i];
//  }
//  xsum = ~xsum;
//
//  // Throw an error message if the checksum's don't match
//  if (xsum != checksum)
//  {
//    Serial.print("Checksum mismatch in packet : ");
//    printHex(packetbuffer, replyidx + 1);
//    return 0;
//  }

  // checksum passed!
  return replyidx;
}
