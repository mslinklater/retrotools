#include <cstdint>

#include "errorcodes.h"

class Disassembler
{
public:
	Disassembler();
	virtual ~Disassembler();
	
	void 		Init();
	eErrorCode	Disassemble(uint16_t address, uint16_t size, uint16_t org);
	void		DumpToTTY(void);
private:
};
