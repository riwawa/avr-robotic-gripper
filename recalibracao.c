#include <avr/io.h>
#include <stdint.h>

#define GRIPPER_OPEN    2728
#define GRIPPER_CLOSED  4628
#define GRIPPER_HALF ((GRIPPER_OPEN + GRIPPER_CLOSED) / 2)

void uart_init(void)
{
    UBRR0 = 103; // 9600 baud @ 16 MHz
    UCSR0B =(1 << TXEN0) | (1 << RXEN0);

    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void uart_putchar(char c)
{
    while (!(UCSR0A & (1 << UDRE0)))
    {
    }

    UDR0 = c;
}

char uart_getchar(void)
{
    while (!(UCSR0A & (1 << RXC0)))
    {
    }

    return UDR0;
}

void uart_print(const char *texto)
{
    while (*texto != '\0')
    {
        uart_putchar(*texto);
        texto++;
    }
}

void uart_print_uint16(uint16_t valor)
{
    char buffer[6];
    uint8_t i = 0;

    if (valor == 0)
    {
        uart_putchar('0');
        return;
    }

    while (valor > 0)
    {
        buffer[i++] = '0' + (valor % 10);
        valor /= 10;
    }

    while (i > 0)
    {
        uart_putchar(buffer[--i]);
    }
}

void servo_init(void)
{
    DDRB |= (1<<DDB1); // D9 / PB1 / OC1A
    TCCR1A =(1 << COM1A1)|(1 << WGM11);
    TCCR1B =(1 << WGM13) | (1 << WGM12) |(1 << CS11);
    ICR1 = 39999; // 20 ms / 50 Hz
}

uint16_t ticks_para_us(uint16_t ticks)
{
    return ticks / 2;
}

uint16_t ticks_para_angulo(uint16_t ticks)
{
    uint16_t pulso_us = ticks_para_us(ticks);

    if (pulso_us <= 500)
        return 0;

    if (pulso_us >= 2500)
        return 180;

    return ((uint32_t)(pulso_us - 500) * 180) / 2000;
}

void mostrar_status(void)
{
    uint16_t pulso_us = ticks_para_us(OCR1A);
    uint16_t angulo = ticks_para_angulo(OCR1A);

    uart_print("\r\nOCR1A: ");
    uart_print_uint16(OCR1A);

    uart_print("\r\nPulso: ");
    uart_print_uint16(pulso_us);
    uart_print(" us");

    uart_print("\r\nAngulo estimado: ");
    uart_print_uint16(angulo);
    uart_print(" graus\r\n");
}

int main(void)
{
    uart_init();
    servo_init();

    OCR1A = 4778;

    uart_print("\r\nServo calibrator\r\n");
    uart_print("+ aumenta\r\n");
    uart_print("- diminui\r\n");
    uart_print("p mostra status\r\n");

    mostrar_status();

    while (1)
    {
        char comando = uart_getchar();

        if (comando == '+')
        {
            if (OCR1A < 5000)
                OCR1A += 50;

            mostrar_status();
        }

        else if (comando == '-')
        {
            if (OCR1A > 1000)
                OCR1A -= 50;

            mostrar_status();
        }

        else if (comando == 'p')
        {
            mostrar_status();
        }
    }
}