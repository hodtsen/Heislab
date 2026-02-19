#pragma once

// Globale variabler (defineres i én .c-fil, f.eks. main.c)
extern int orderSpace[10];
extern int orders;

// Hall down
void input_floor_4D(void);
void input_floor_3D(void);
void input_floor_2D(void);

// Hall up
void input_floor_3U(void);
void input_floor_2U(void);
void input_floor_1U(void);

// Cab
void input_cab_4(void);
void input_cab_3(void);
void input_cab_2(void);
void input_cab_1(void);
