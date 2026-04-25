extern void initialise_monitor_handles(void);
#define RCC_APB2ENR   (*(volatile int*)0x40021018)
#define GPIOA_CRH     (*(volatile int*)0x40010804)
#define GPIOA_BSRR     (*(volatile int*)0x40010810)

void delay() {
    for (volatile int i = 0; i < 500000; i++);
}

void led_on(int pin) {
	GPIOA_BSRR = (1 << pin);
}

void led_off(int pin){
	GPIOA_BSRR = (1 << (pin + 16));
}

void select_mode_output(){
	RCC_APB2ENR |= (1 << 2);
//	for(int i = 0 ; i < 17; i += 4){
//		GPIOA_CRH &= ~(0xF << i);
//		GPIOA_CRH |=  (0x2 << i);
//	}
	GPIOA_CRH &= (0xFFF00000);
	GPIOA_CRH |=  (0x00022222);

}

int main() {
	initialise_monitor_handles();
    select_mode_output();

    int a = 5,b = 5, result;
    unsigned int psr;
    // -------------------------
    // CLEAR FLAGS FIRST
    // -------------------------
    __asm volatile(
        "MOV R0, #0\n"
        "MSR APSR_nzcvq, R0\n"
        "MRS %0, APSR\n"
    	:"=r"(psr)
        :
        : "r0","cc"
    );

    // -------------------------
    // Z FLAG TEST
    // -------------------------
    __asm volatile(
        "SUBS %1, %2, %3\n"
        "MRS %0, APSR\n"
        : "=r"(psr), "=&r"(result)
        : "r"(a), "r"(b)
        : "cc"
    );


    if (psr & (1 << 30))
        led_on(9);

    // -------------------------
    // N FLAG TEST
    // -------------------------
    __asm volatile(
        "MOV R0, #5\n"
        "MOV R1, #10\n"
        "SUBS R2, R0, R1\n"   // negative → N=1
        "MRS %0, APSR\n"
        : "=r"(psr)
        :
        : "r0","r1","r2","cc"
    );

    if (psr & (1 << 31))
    	led_on(8);

    // -------------------------
    // C FLAG TEST
    // -------------------------
    __asm volatile(
        "LDR R0, =4294967295\n"
        "ADDS R1, R0, #1\n"   // carry → C=1
        "MRS %0, APSR\n"
        : "=r"(psr)
        :
        : "r0","r1","cc"
    );

    if (psr & (1 << 29))
    	led_on(10);

    // -------------------------
    // Q FLAG TEST (Saturation)
    // -------------------------
    __asm volatile(
        "LDR R0, =64\n"
        "SSAT R1, #7, R0\n"
        "MRS %0, APSR\n"
        : "=r"(psr)
        :
        : "r0","r1","cc"
    );

    if (psr & (1 << 27))
    	led_on(12);

    // -------------------------
	// V FLAG TEST (Overflow)
	// -------------------------
	__asm volatile(
		"LDR R0, =2147483647\n"
		"LDR R1, =1\n"
		"ADDS R0, R0, R1\n"
		"MRS %0, APSR\n"
		: "=r"(psr)
		:
		: "r0","r1","cc"
	);
	if (psr & (1 << 28))
		led_on(11);

    while(1);
}
