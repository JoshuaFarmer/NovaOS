#include <stdint.h>

uint16_t A = 0;
uint16_t X = 0;
uint16_t Y = 0;
uint16_t IP= 0;
uint16_t IR= 0;

enum {
	// No Operation
	NOP,

	// Arithmetic
	ADX=0x1,
	ADY=0x2,
	SUX,
	SUY,

	ANX,
	ANY,
	ORX,
	ORY,
	XOX,
	XOY,
	NOX,
	NOY,
	SLX,
	SRX,

	LDIA,
	LDIX,
	LDIY,

	// Memory
	STA, // store A in memory address <I>
	LDA, // load A from memory address <I>

	STX, // store X in memory address <I>
	LDX, // load X from memory address <I>

	STO, // store A in memory address X|Y
	LOA, // load A from memory address X|Y

	ACHX, // swap A&X
	ACHY, // swap A&Y
	XCHY, // swap X&Y

	// I/O
	OUTA,
	INA,

	OUTX,
	INX,

	// Built in Functions
	KBDIN, // get a key from terminal and store in A
	KBDOUT, // print to terminal the contents of A

	// Jumps
	JMP,
	JMZ,
	JMC,
	JMX,
} inst;