#include "mbed.h"
DigitalOut neo(PA_0);
typedef struct
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
} RGB;

// 動的に扱うための変数と配列ポインタ
static uint32_t g_neoPixelCount = 0;
static RGB *g_neoPixels = nullptr;

void InitNeoPixels(uint32_t count)
{
    if (g_neoPixels)
    {
        delete[] g_neoPixels;
        g_neoPixels = nullptr;
    }
    if (count == 0)
    {
        g_neoPixelCount = 0;
        return;
    }
    g_neoPixels = new RGB[count];
    g_neoPixelCount = count;
    // 初期化（全てオフ）
    for (uint32_t i = 0; i < g_neoPixelCount; ++i)
    {
        g_neoPixels[i].r = 0;
        g_neoPixels[i].g = 0;
        g_neoPixels[i].b = 0;
    }
}

void FreeNeoPixels()
{
    if (g_neoPixels)
    {
        delete[] g_neoPixels;
        g_neoPixels = nullptr;
    }
    g_neoPixelCount = 0;
}

void UpdateNeoPixelBuffer(uint32_t pixelIndex, RGB newValue)
{
    if (pixelIndex >= g_neoPixelCount)
    {
        return;
    }
    g_neoPixels[pixelIndex].r = newValue.r;
    g_neoPixels[pixelIndex].g = newValue.g;
    g_neoPixels[pixelIndex].b = newValue.b;
}
void TurnOffNeoPixelBuffer(uint32_t pixelIndex)
{
    if (pixelIndex >= g_neoPixelCount)
    {
        return;
    }
    g_neoPixels[pixelIndex].r = 0;
    g_neoPixels[pixelIndex].g = 0;
    g_neoPixels[pixelIndex].b = 0;
}
void sendBit(bool bit)
{
    if (bit)
    {
        neo = 1;
        wait_us(1);
        neo = 0;
        wait_us(0);
    }
    else
    {
        neo = 1;
        wait_us(0);
        neo = 0;
        wait_us(1);
    }
}
void sendByte(uint8_t byte)
{
    for (int i = 7; i >= 0; i--)
    {
        sendBit(byte & (1 << i));
    }
}
void ResetNeoPixel()
{
    neo = 0;
    wait_us(50);
}
void UpdateNeoPixel()
{
    if (g_neoPixelCount == 0 || g_neoPixels == nullptr)
        return;
    ResetNeoPixel();
    for (uint32_t i = 0; i < g_neoPixelCount; i++)
    {
        sendByte(g_neoPixels[i].r);
        sendByte(g_neoPixels[i].g);
        sendByte(g_neoPixels[i].b);
    }
}
int main()
{

    InitNeoPixels(100);
    RGB red = {0, 50, 0};
    RGB green = {50, 0, 0};
    RGB blue = {0, 0, 50};
    UpdateNeoPixelBuffer(0, {0, 0, 0});
    UpdateNeoPixel();
    while (1)
    {
        for (int i = 0; i < 25; i++)
        {
            UpdateNeoPixelBuffer(i, red);
            UpdateNeoPixel();
            ThisThread::sleep_for(100ms);
        }
        ThisThread::sleep_for(100ms);
        for (int i = 0; i < 25; i++)
        {
            UpdateNeoPixelBuffer(i, green);
            UpdateNeoPixel();
            ThisThread::sleep_for(100ms);
        }
        ThisThread::sleep_for(100ms);
        for (int i = 0; i < 25; i++)
        {
            UpdateNeoPixelBuffer(i, blue);
            UpdateNeoPixel();
            ThisThread::sleep_for(100ms);
        }
        ThisThread::sleep_for(100ms);
    }

    FreeNeoPixels();
    return 0;
}