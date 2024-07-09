#pragma once

#define ram_size 4096
#define Z S[0]
#define C S[1]
#define H S[2]

static uint16_t A = 0;
static uint16_t X = 0;
static uint16_t Y = 0;
static uint16_t T = 0;
static uint16_t S[3]; // zero && carry

static uint16_t IP = 0;
static uint16_t ram[ram_size];

enum {
	// No Operation
	NOP,

	// Arithmetic
	ADX,
	ADY,
	ADC, // w carry
	SUX,
	SUY,
	SUC, // w carry

	ANX,
	ANY,
	ORX,
	ORY,
	XOX,
	XOY,
	NOA,
	NOX,
	NOY,
	SLX,
	SRX,
	SLY,
	SRY,

	// Memory
	LDIA,
	LDIX,
	LDIY,

	STA, // store A in memory address <I>
	LDA, // load A from memory address <I>

	STX, // store X in memory address <I>
	LDX, // load X from memory address <I>

	STO, // store A in memory address X
	LDO, // load A from memory address X

	ACHX, // swap A&X
	ACHY, // swap A&Y
	XCHY, // swap X&Y

	// I/O
	OUTA,
	INA,

	// Built in Functions
	KBDIN, // get a key from terminal and store in A
	KBDOUT, // print to terminal the contents of A

	// Jumps
	JMP,
	JMZ,
	JMC,
	JMX,
	JMXZ,
	JMXC,

	// flags
	STF, // store a in the flags
	LDF, // laod a from the flags

	CLC, // set carry low
	STC, // set carry high

	HLT,
	INST_COUNT,
} inst;

uint16_t fetch(uint16_t a) {
	if (a >= ram_size) a = ram_size-1;
	return ram[a];
}

void set(uint16_t a, uint16_t v) {
	if (a >= ram_size) a = ram_size-1;
	ram[a] = v;
}

void flags(int b) {
	Z = !(A&1);
	if((b > 0 && A < T) || (b < 0 && A > T)) {
		C = 1;
	} else C = 0;
}

void execute(uint16_t I) {
	// only six bits because yes
	switch (I & 0b111111) {
		case NOP: break;
		case ADX: T=A; A+=X; flags(X); break;
		case ADY: T=A; A+=Y; flags(Y); break;
		case ADC: T=A; A+=X+C; flags(X+C); break;
		case SUX: T=A; A-=X; flags(X); break;
		case SUY: T=A; A-=Y; flags(Y); break;
		case SUC: T=A; A-=X-C; flags(Y-C); break;

		case ANX: T=A; A&=X; flags(X); break;
		case ANY: T=A; A&=Y; flags(Y); break;
		case ORX: T=A; A|=X; flags(X); break;
		case ORY: T=A; A|=Y; flags(Y); break;
		case XOX: T=A; A^=X; flags(X); break;
		case XOY: T=A; A^=Y; flags(Y); break;

		case NOA: T=A; A=!A; flags(A); break;
		case NOX: T=A; A=!X; flags(X); break;
		case NOY: T=A; A=!Y; flags(Y); break;

		case SLX: T=A; A<<=X; flags(X); break;
		case SRX: T=A; A>>=X; flags(X); break;
		case SLY: T=A; A<<=Y; flags(Y); break;
		case SRY: T=A; A>>=Y; flags(Y); break;

		case LDIA: A=fetch(IP++); break;
		case LDIX: X=fetch(IP++); break;
		case LDIY: Y=fetch(IP++); break;

		case LDA: T = fetch(IP++); A = fetch(T); break;
		case STA: T = fetch(IP++); set(T,A); break;

		case LDX: T = fetch(IP++); X = fetch(T); break;
		case STX: T = fetch(IP++); set(T,X); break;

		case LDO: A = fetch(X); break;
		case STO: set(X,A); break;

		case ACHX: T=A; A=X; X=T; break;
		case ACHY: T=A; A=Y; Y=T; break;
		case XCHY: T=X; X=Y; Y=T; break;

		case OUTA: outw(X,A); break;
		case INA: A = inw(X); break;

		case KBDIN: A = getch(); break;
		case KBDOUT: putc(A); break;

		case JMP: T = fetch(IP++); IP = T; break;
		case JMZ: T = fetch(IP++); IP = (Z == 1) ? T : IP; break;
		case JMC: T = fetch(IP++); IP = (C == 1) ? T : IP; break;
		case JMX: IP = X; break;
		case JMXZ: IP = (Z == 1) ? X : IP; break;
		case JMXC: IP = (C == 1) ? X : IP; break;
		
		case CLC: C = 0; break;
		case STC: C = 1; break;

		case STF: S[0] = A&1; S[1] = A&10; break;
		case LDF: A = S[0] | (S[1] << 1); break;

		case HLT: H = 1; break;
	}
}