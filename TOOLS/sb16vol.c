#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <conio.h>
#include <pc.h>

int main(int argc, char *argv[]) {
    uint16_t sb_port = 0x220;
    int vol_percent = 100;

    printf("\nSound Blaster 16 Hardware Mixer Utility v1.0\n");
    printf("===========================================\n");

    // 1. Parse BLASTER environment variable
    char *blaster = getenv("BLASTER");
    if (blaster) {
        char *p = blaster;
        while (*p) {
            if (*p == 'A' || *p == 'a') {
                sb_port = strtol(p + 1, NULL, 16);
                break;
            }
            p++;
        }
    }

    // 2. Parse volume command line argument
    if (argc > 1) {
        int val = atoi(argv[1]);
        if (val >= 0 && val <= 100) {
            vol_percent = val;
        } else {
            printf("Usage: sb16vol [volume_percent]\n");
            printf("Volume percentage must be between 0 and 100. Default is 100.\n");
            return 1;
        }
    }

    // 3. Map 0-100% to 5-bit register range (0 to 31)
    int sb_vol = (vol_percent * 31) / 100;
    // Shift left by 3 bits to align with high 5 bits of SB16 mixer register layout (bits 7-3)
    uint8_t reg_val = (sb_vol << 3) & 0xF8;

    printf("Detected Sound Blaster Port: 0x%03X\n", sb_port);
    printf("Setting Mixer Master and DAC Volumes to: %d%% (Mixer raw value: 0x%02X)\n", vol_percent, reg_val);

    // 4. Write to SB16 Mixer Ports
    // Mixer registers:
    // 0x30: Master Volume Left
    // 0x31: Master Volume Right
    // 0x32: Voice/DAC Volume Left
    // 0x33: Voice/DAC Volume Right
    
    // Master Volume Left
    outportb(sb_port + 4, 0x30);
    outportb(sb_port + 5, reg_val);
    
    // Master Volume Right
    outportb(sb_port + 4, 0x31);
    outportb(sb_port + 5, reg_val);
    
    // Voice/DAC Volume Left
    outportb(sb_port + 4, 0x32);
    outportb(sb_port + 5, reg_val);
    
    // Voice/DAC Volume Right
    outportb(sb_port + 4, 0x33);
    outportb(sb_port + 5, reg_val);

    printf("Hardware Mixer configuration successfully updated!\n\n");
    return 0;
}
