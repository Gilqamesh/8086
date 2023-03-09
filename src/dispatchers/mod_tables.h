#pragma once

#include "../defs.h"

void mod__no_ea__reg(int w, int reg, int r_m);
void mod__no_ea__rm(int w, int reg, int r_m);

void mod__ea__reg(int w, int reg, int r_m, int ea);
void mod__ea__rm(int w, int reg, int r_m, int ea);

void mod__register_mode_no_ea__reg(int w, int reg, int r_m);
void mod__register_mode_no_ea__rm(int w, int reg, int r_m);

void mod__no_ea_immediate__reg(int w, int reg, int r_m, word immediate);
void mod__no_ea_immediate__rm(int w, int reg, int r_m, word immediate);

void mod__ea_immediate__reg(int w, int reg, int r_m, word immediate, int ea);
void mod__ea_immediate__rm(int w, int reg, int r_m, word immediate, int ea);

void mod__register_mode_no_ea_immediate__reg(int w, int reg, int r_m, word immediate);
void mod__register_mode_no_ea_immediate__rm(int w, int reg, int r_m, word immediate);

static void (*const mod__no_ea_handlers[2])(int w, int reg, int r_m) = {
    mod__no_ea__rm,
    mod__no_ea__reg
};

static void (*const mod__ea_handlers[2])(int w, int reg, int r_m, int ea) = {
    mod__ea__rm,
    mod__ea__reg
};

static void (*const mod__register_mode_no_ea_handlers[2])(int w, int reg, int r_m) = {
    mod__register_mode_no_ea__rm,
    mod__register_mode_no_ea__reg
};

static void (*const mod__no_ea_immediate_handlers[2])(int w, int reg, int r_m, word immediate) = {
    mod__no_ea_immediate__rm,
    mod__no_ea_immediate__reg
};

static void (*const mod__ea_immediate_handlers[2])(int w, int reg, int r_m, word immediate, int ea) = {
    mod__ea_immediate__rm,
    mod__ea_immediate__reg
};

static void (*const mod__register_mode_no_ea_immediate_handlers[2])(int w, int reg, int r_m, word immediate) = {
    mod__register_mode_no_ea_immediate__rm,
    mod__register_mode_no_ea_immediate__reg
};
