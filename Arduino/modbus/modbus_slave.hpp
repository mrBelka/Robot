#pragma once
#include <Arduino.h>

namespace protocol
{
	class ModbusSlave
	{
		Stream& m_stream;
		uint32_t m_baudrate;
		byte m_msg[256];
		uint8_t m_addr;
		bool m_isHardware;

	public:
		ModbusSlave(HardwareSerial& stream, uint32_t baudrate, uint8_t addr) :
			m_stream(stream), m_baudrate(baudrate), m_addr(addr) {
			
			m_isHardware = true;
		}

		ModbusSlave(SoftwareSerial& stream, uint32_t baudrate, uint8_t addr) :
			m_stream(stream), m_baudrate(baudrate), m_addr(addr) {
			
			m_isHardware = false;
		}

		void setup() {
			if(m_isHardware)
				static_cast<HardwareSerial&>(m_stream).begin(m_baudrate);
			else
				static_cast<SoftwareSerial&>(m_stream).begin(m_baudrate);
		}

		void run() {
			int len = 0;
			len = m_stream.available();
			delay(10);

			if(len == 0)
				return;
			
			for(uint8_t i = 0; i < len; ++i)
				m_msg[i] = m_stream.read();

			for(uint8_t i = 0; i < len; ++i)
				m_stream.print(m_msg[i]);

		}

	};
}
