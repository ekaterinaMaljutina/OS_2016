#include "interrupt.h"
#include "SerialPort.h"
#include "memory.h"
#include "ints.h"  // cli and sti
#include "ioport.h"


struct idt_entry idt[256];
struct desc_table_ptr descp;

void idt_install(){
    descp.size = sizeof (struct idt_entry) * 256 - 1 ;
    descp.addr = (uint64_t)idt;

	write_idtr(&descp);
}


void idt_set_gate(uint8_t const idx, uint64_t const base, 
				  uint16_t const sel, uint8_t const flags){ 
    idt[idx].base_lo = (base & 0xFFFF);
    idt[idx].base_hi = (base >> 16) & 0xFFFF;
	idt[idx].base_64 = (base >> 32) & 0xFFFFFFFF;
    // сегмент селектор   
	idt[idx].sel = sel; 
	idt[idx].flags = flags; 
}

extern void handler0();
extern void handler1();
extern void handler2();
extern void handler3();
extern void handler4();
extern void handler5();
extern void handler6();
extern void handler7();
extern void handler8();
extern void handler9();
extern void handler10();
extern void handler11();
extern void handler12();
extern void handler13();
extern void handler14();
extern void handler15();
extern void handler16();
extern void handler17();
extern void handler18();
extern void handler19();
extern void handler20();
extern void handler21();
extern void handler22();
extern void handler23();
extern void handler24();
extern void handler25();
extern void handler26();
extern void handler27();
extern void handler28();
extern void handler29();
extern void handler30();
extern void handler31();

//irq 
extern void handler32();
extern void handler33();
extern void handler34();
extern void handler35();
extern void handler36();
extern void handler37();
extern void handler38();
extern void handler39();
extern void handler40();
extern void handler41();
extern void handler42();
extern void handler43();
extern void handler44();
extern void handler45();
extern void handler46();
extern void handler47();

void remap(){
	disable_ints(); 
	/*
		Взаимодействие устройств PIC осуществляется через шину 
		ввода/вывода. В каждом устройстве есть порт команд и 
		порт данных:

    		-Главное устройство: команда - 0x20, данные - 0x21
    		-Подчиненное устройство: команда - 0xA0, данные - 0xA1


		Master PIC - Command 	0x0020
		Master PIC - Data 	0x0021
		Slave PIC - Command 	0x00A0
		Slave PIC - Data 	0x00A1 

		
		0x01 - different functions
		0x02,0x04 - configuration cascade
		0x11 - init command
		0x20, 0x28 - mapping
	*/
	 // Remap the irq table.
	out8(0x20, 0x11);
	out8(0xA0, 0x11);
	out8(0x21, 0x20);
	out8(0xA1, 0x28);
	out8(0x21, 0x04);
	out8(0xA1, 0x02);
	out8(0x21, 0x01);
	out8(0xA1, 0x01);
	out8(0x21, 0xFE);
	out8(0xA1, 0xFF);


	enable_ints();
}

void intr_install(){ //with irq
	idt_set_gate(0, (uint64_t) handler0,  KERNEL_CS, 0x8E);
	idt_set_gate(1, (uint64_t) handler1,  KERNEL_CS, 0x8E);   
	idt_set_gate(2, (uint64_t) handler2,  KERNEL_CS, 0x8E); 
	idt_set_gate(3, (uint64_t) handler3,  KERNEL_CS, 0x8E);
	idt_set_gate(4, (uint64_t) handler4,  KERNEL_CS, 0x8E);
	idt_set_gate(5, (uint64_t) handler5,  KERNEL_CS, 0x8E);
	idt_set_gate(6, (uint64_t) handler6,  KERNEL_CS, 0x8E);
	idt_set_gate(7, (uint64_t) handler7,  KERNEL_CS, 0x8E);
	idt_set_gate(8, (uint64_t) handler8,  KERNEL_CS, 0x8E);
	idt_set_gate(9, (uint64_t) handler9,  KERNEL_CS, 0x8E);
	idt_set_gate(10, (uint64_t) handler10,  KERNEL_CS, 0x8E);
	idt_set_gate(11, (uint64_t) handler11,  KERNEL_CS, 0x8E);   
	idt_set_gate(12, (uint64_t) handler12,  KERNEL_CS, 0x8E); 
	idt_set_gate(13, (uint64_t) handler13,  KERNEL_CS, 0x8E);
	idt_set_gate(14, (uint64_t) handler14,  KERNEL_CS, 0x8E);
	idt_set_gate(15, (uint64_t) handler15,  KERNEL_CS, 0x8E);
	idt_set_gate(16, (uint64_t) handler16,  KERNEL_CS, 0x8E);
	idt_set_gate(17, (uint64_t) handler17,  KERNEL_CS, 0x8E);
	idt_set_gate(18, (uint64_t) handler18,  KERNEL_CS, 0x8E);
	idt_set_gate(19, (uint64_t) handler19,  KERNEL_CS, 0x8E);
	idt_set_gate(20, (uint64_t) handler20,  KERNEL_CS, 0x8E);
	idt_set_gate(21, (uint64_t) handler21,  KERNEL_CS, 0x8E);   
	idt_set_gate(22, (uint64_t) handler22,  KERNEL_CS, 0x8E); 
	idt_set_gate(23, (uint64_t) handler23,  KERNEL_CS, 0x8E);
	idt_set_gate(24, (uint64_t) handler24,  KERNEL_CS, 0x8E);
	idt_set_gate(25, (uint64_t) handler25,  KERNEL_CS, 0x8E);
	idt_set_gate(26, (uint64_t) handler26,  KERNEL_CS, 0x8E);
	idt_set_gate(27, (uint64_t) handler27,  KERNEL_CS, 0x8E);
	idt_set_gate(28, (uint64_t) handler28,  KERNEL_CS, 0x8E);
	idt_set_gate(29, (uint64_t) handler29,  KERNEL_CS, 0x8E);
	idt_set_gate(30, (uint64_t) handler30,  KERNEL_CS, 0x8E);
	idt_set_gate(31, (uint64_t) handler31,  KERNEL_CS, 0x8E); 

	//irq
	remap();

	idt_set_gate(32, (uint64_t) handler32,  KERNEL_CS, 0x8E);
	idt_set_gate(33, (uint64_t) handler33,  KERNEL_CS, 0x8E); 
	idt_set_gate(34, (uint64_t) handler34,  KERNEL_CS, 0x8E);
	idt_set_gate(35, (uint64_t) handler35,  KERNEL_CS, 0x8E); 
	idt_set_gate(36, (uint64_t) handler36,  KERNEL_CS, 0x8E);
	idt_set_gate(37, (uint64_t) handler37,  KERNEL_CS, 0x8E); 
	idt_set_gate(38, (uint64_t) handler38,  KERNEL_CS, 0x8E);
	idt_set_gate(39, (uint64_t) handler39,  KERNEL_CS, 0x8E);
	idt_set_gate(40, (uint64_t) handler40,  KERNEL_CS, 0x8E);
	idt_set_gate(41, (uint64_t) handler41,  KERNEL_CS, 0x8E);   
	idt_set_gate(42, (uint64_t) handler42,  KERNEL_CS, 0x8E); 
	idt_set_gate(43, (uint64_t) handler43,  KERNEL_CS, 0x8E);
	idt_set_gate(44, (uint64_t) handler44,  KERNEL_CS, 0x8E);
	idt_set_gate(45, (uint64_t) handler45,  KERNEL_CS, 0x8E);
	idt_set_gate(46, (uint64_t) handler46,  KERNEL_CS, 0x8E);
	idt_set_gate(47, (uint64_t) handler47,  KERNEL_CS, 0x8E); 

}

char const * const error_code_out[32]={
	"Деление на ноль\n", //0
	"прерывание отладки, пошаговое исполнение\n", //1
	"Немаскируемое прерывание\n", // 2
	"Прерывание точки останова\n", //3 
	"Переполнение при выполнении команды Into\n" //4,
	"Прерывание выхода за границы данных\n" // 5 
	"Прерывание неправильного кода операции\n", //6
	"Прерывание отсутствие сопроцессора\n", //7 
	"Прерывание двойной ошибки\n" //8
	"Нарушение сегментации памяти сопроцессором\n",//9
	"Неправильный TSS\n",//10
	"Отсутствие сегмента\n",//11
	"Ошибка стека\n",//12
	"Ошибка общей защиты\n",//13
	"Ошибка системы страничной организации памяти\n",//14
	"Неизвестное прерывание\n",//15
	"Ошибка сопроцессора\n",//16
	"Прерывание контроля выравнивания\n",//17
	"Прерывание, связанное с общей работой процессора\n",//18
	"Зарезервировано\n",//19
	"Зарезервировано\n",//20
	"Зарезервировано\n",//21
	"Зарезервировано\n",//22
	"Зарезервировано\n",//23
	"Зарезервировано\n",//24
	"Зарезервировано\n",//25
	"Зарезервировано\n",//26
	"Зарезервировано\n",//27
	"Зарезервировано\n",//28
	"Зарезервировано\n",//29
	"Зарезервировано\n",//30
	"Зарезервировано\n",//31
	};


void handler(struct registers_t const * reg){
	if (reg->int_code < 32) {
		serial_port_write_char("error with code out:   ");
		serial_port_write_char(error_code_out[reg->int_code]);
	}
}

void irq_set_handler(int n,handler_irq handler)
{
    interrupt_handlers[n] = handler;
}

void irq_handler(struct registers_t const * reg){
	if (reg->int_code >= 40 ){ 
	   /* 
	   		Если к возникновению прерывания причастно 
			подчиненное устройство
		*/
		// Send reset signal to slave.
		out8(0xA0, 0x20);
	}	
    handler_irq  handler = interrupt_handlers[reg->int_code-32];
    if (handler)
    {
        handler(reg);
    }
    /*
	// Посылает сигнал перезагрузки в главное устройство 
	(а также в подчиненное устройство, если это необходимо)

	*/
	out8(0x20, 0x20);
}