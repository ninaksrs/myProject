
#include "note.h"

#define buzzer_pin 2

void initBuzzer()
{
    gpio_init(buzzer_pin);
    gpio_set_dir(buzzer_pin, GPIO_OUT);
}
const int buttonTones[] = {
    NOTE_B3,
    NOTE_C4,
    NOTE_CS4,
    NOTE_D4,
    NOTE_DS4,
    NOTE_E4,
    NOTE_F4,
    NOTE_FS4,
    NOTE_G4,
    NOTE_GS4,
    NOTE_A4,
    NOTE_AS4,
    NOTE_B4,
    NOTE_C5,
    NOTE_CS5,
    NOTE_D5,
    NOTE_DS5,
    NOTE_E5,
    NOTE_F5};

void Play_Note(uint Note, uint NoteLen, uint *Notes_dly)
{
    for (uint nl = 0; nl < (NoteLen / Notes_dly[Note]); nl++)
    {
        gpio_put(buzzer_pin, true);
        sleep_us(Notes_dly[Note]);
        gpio_put(buzzer_pin, false);
        sleep_us(Notes_dly[Note]);
    }
    sleep_ms(10);
}

void PlayMusic(uint play[][2], uint size, uint *Notes_dly)
{
    for (int n = 0; n < size; n++)
        Play_Note(play[n][0], play[n][1], Notes_dly);
    sleep_ms(100);
}

void onSong()
{
    // music box
    const uint ref_time = 200000;
    uint Notes_dly[18] = {0};
    for (int cnt = 0; cnt < 15; cnt++)
    {
        Notes_dly[cnt] = 500000 / buttonTones[cnt]; // focus
    }
    uint Dynamite[37][2] = {
        {5, ref_time - 18000},
        {14, ref_time - 18000},
        {12, ref_time - 18000},
        {10, ref_time},
        {9, ref_time},
        {7, ref_time},
        {5, ref_time * 2},
        // 7
        {5, ref_time - 18000},
        {5, ref_time - 18000},
        {14, ref_time - 18000},
        {12, ref_time - 18000},
        {10, ref_time},
        {9, ref_time},
        {7, ref_time},
        {5, ref_time * 2},
        // 15

        {0, ref_time - 10000},
        {3, ref_time - 100000},
        {5, ref_time - 30000},
        {7, ref_time},
        {9, ref_time},
        {10, ref_time},
        {9, ref_time},
        {10, ref_time},
        {12, ref_time},
        {12, ref_time},
        {12, ref_time},
        {9, ref_time * 2},
        // 27
        {5, ref_time - 18000},
        {14, ref_time - 18000},
        {12, ref_time - 18000},
        {10, ref_time},
        {9, ref_time},
        {7, ref_time},
        {5, ref_time},
        {5, ref_time},
        {0, ref_time - 20000},
        {5, ref_time * 2},
        // 37
    };
    PlayMusic(Dynamite, 37, Notes_dly);
}

void timeSong(int time)
{
    for (int i = 0; i < time; i++)
    {

        for (int j = 0; j < 300; j++)
        {
            gpio_put(buzzer_pin, true);
            sleep_us(965);
            gpio_put(buzzer_pin, false);
            sleep_us(965);
        }
        sleep_ms(500);
        onSong();
    }
}
