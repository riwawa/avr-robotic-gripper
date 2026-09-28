#include <avr/io.h>
#include <util/delay.h>

#define GRIPPER_OPEN   2728
#define GRIPPER_CLOSED 4628

void gripper_open(void)
{
    OCR1A = GRIPPER_OPEN;
}

void gripper_close(void)
{
    OCR1A = GRIPPER_CLOSED;
}
void gripper_move_to(uint16_t target)
{
    while (OCR1A < target)
    {
        OCR1A += 10;
        _delay_ms(15);
    }

    while (OCR1A > target)
    {
        OCR1A -= 10;
        _delay_ms(15);
    }
}

void servo_init(void)
{
    DDRB |= (1 << DDB1); // D9 / PB1 / OC1A
    TCCR1A =(1 << COM1A1) |(1 << WGM11);
    TCCR1B =(1 << WGM13) |(1 << WGM12) |(1 << CS11);
    ICR1 = 39999; // 20 ms / 50 Hz
}

int main(void)
{
    servo_init();
    gripper_open();

    while (1)
    {
        gripper_move_to(GRIPPER_CLOSED);
        _delay_ms(500);

        gripper_move_to(GRIPPER_OPEN);
        _delay_ms(500);
    }
}