
#include <stdint.h>

class i2c_device {
	public:
		i2c_device(char* i2c_device, uint8_t i2c_addr);
		~i2c_device();

		void writeSingle(uint8_t reg, uint8_t value);
		uint8_t readSingle(uint8_t reg);
	private:
		int fp;
};


