module SecureEngineCustomVMs;

// ******************************************************************************
// Header: SecureEngineCustomVMs32.d
// Description: D macros definitions
//
// Author/s: Oreans Technologies 
// (c) 2021 Oreans Technologies
//
// --- File generated automatically from Oreans VM Generator (14/5/2021) ---
// ******************************************************************************

// ******************************************************************************
//                                 Macros definition
// ******************************************************************************

version (D_InlineAsm_X86)
{

const VM_TIGER_WHITE_START                     = "asm { db 0xEB, 0x10, 0x57, 0x4C, 0x20, 0x20, 0x64, 0x00, 0x00, 0x00,
                                                                   0x00, 0x00, 0x00, 0x00, 0x57, 0x4C, 0x20, 0x20;}";

const VM_TIGER_WHITE_END                       = "asm { db 0xEB, 0x10, 0x57, 0x4C, 0x20, 0x20, 0xF4, 0x01, 0x00, 0x00,
                                                                   0x00, 0x00, 0x00, 0x00, 0x57, 0x4C, 0x20, 0x20;}";

const VM_TIGER_RED_START                       = "asm { db 0xEB, 0x10, 0x57, 0x4C, 0x20, 0x20, 0x65, 0x00, 0x00, 0x00,
                                                                   0x00, 0x00, 0x00, 0x00, 0x57, 0x4C, 0x20, 0x20;}";

const VM_TIGER_RED_END                         = "asm { db 0xEB, 0x10, 0x57, 0x4C, 0x20, 0x20, 0xF5, 0x01, 0x00, 0x00,
                                                                   0x00, 0x00, 0x00, 0x00, 0x57, 0x4C, 0x20, 0x20;}";

const VM_TIGER_BLACK_START                     = "asm { db 0xEB, 0x10, 0x57, 0x4C, 0x20, 0x20, 0x66, 0x00, 0x00, 0x00,
                                                                   0x00, 0x00, 0x00, 0x00, 0x57, 0x4C, 0x20, 0x20;}";

const VM_TIGER_BLACK_END                       = "asm { db 0xEB, 0x10, 0x57, 0x4C, 0x20, 0x20, 0xF6, 0x01, 0x00, 0x00,
                                                                   0x00, 0x00, 0x00, 0x00, 0x57, 0x4C, 0x20, 0x20;}";

const VM_FISH_WHITE_START                      = "asm { db 0xEB, 0x10, 0x57, 0x4C, 0x20, 0x20, 0x6A, 0x00, 0x00, 0x00,
                                                                   0x00, 0x00, 0x00, 0x00, 0x57, 0x4C, 0x20, 0x20;}";

const VM_FISH_WHITE_END                        = "asm { db 0xEB, 0x10, 0x57, 0x4C, 0x20, 0x20, 0xFA, 0x01, 0x00, 0x00,
                                                                   0x00, 0x00, 0x00, 0x00, 0x57, 0x4C, 0x20, 0x20;}";

const VM_FISH_RED_START                        = "asm { db 0xEB, 0x10, 0x57, 0x4C, 0x20, 0x20, 0x6C, 0x00, 0x00, 0x00,
                                                                   0x00, 0x00, 0x00, 0x00, 0x57, 0x4C, 0x20, 0x20;}";

const VM_FISH_RED_END                          = "asm { db 0xEB, 0x10, 0x57, 0x4C, 0x20, 0x20, 0xFC, 0x01, 0x00, 0x00,
                                                                   0x00, 0x00, 0x00, 0x00, 0x57, 0x4C, 0x20, 0x20;}";

const VM_FISH_BLACK_START                      = "asm { db 0xEB, 0x10, 0x57, 0x4C, 0x20, 0x20, 0x6E, 0x00, 0x00, 0x00,
                                                                   0x00, 0x00, 0x00, 0x00, 0x57, 0x4C, 0x20, 0x20;}";

const VM_FISH_BLACK_END                        = "asm { db 0xEB, 0x10, 0x57, 0x4C, 0x20, 0x20, 0xFE, 0x01, 0x00, 0x00,
                                                                   0x00, 0x00, 0x00, 0x00, 0x57, 0x4C, 0x20, 0x20;}";

const VM_PUMA_WHITE_START                      = "asm { db 0xEB, 0x10, 0x57, 0x4C, 0x20, 0x20, 0x70, 0x00, 0x00, 0x00,
                                                                   0x00, 0x00, 0x00, 0x00, 0x57, 0x4C, 0x20, 0x20;}";

const VM_PUMA_WHITE_END                        = "asm { db 0xEB, 0x10, 0x57, 0x4C, 0x20, 0x20, 0x00, 0x02, 0x00, 0x00,
                                                                   0x00, 0x00, 0x00, 0x00, 0x57, 0x4C, 0x20, 0x20;}";

const VM_PUMA_RED_START                        = "asm { db 0xEB, 0x10, 0x57, 0x4C, 0x20, 0x20, 0x72, 0x00, 0x00, 0x00,
                                                                   0x00, 0x00, 0x00, 0x00, 0x57, 0x4C, 0x20, 0x20;}";

const VM_PUMA_RED_END                          = "asm { db 0xEB, 0x10, 0x57, 0x4C, 0x20, 0x20, 0x02, 0x02, 0x00, 0x00,
                                                                   0x00, 0x00, 0x00, 0x00, 0x57, 0x4C, 0x20, 0x20;}";

const VM_PUMA_BLACK_START                      = "asm { db 0xEB, 0x10, 0x57, 0x4C, 0x20, 0x20, 0x74, 0x00, 0x00, 0x00,
                                                                   0x00, 0x00, 0x00, 0x00, 0x57, 0x4C, 0x20, 0x20;}";

const VM_PUMA_BLACK_END                        = "asm { db 0xEB, 0x10, 0x57, 0x4C, 0x20, 0x20, 0x04, 0x02, 0x00, 0x00,
                                                                   0x00, 0x00, 0x00, 0x00, 0x57, 0x4C, 0x20, 0x20;}";

const VM_SHARK_WHITE_START                     = "asm { db 0xEB, 0x10, 0x57, 0x4C, 0x20, 0x20, 0x76, 0x00, 0x00, 0x00,
                                                                   0x00, 0x00, 0x00, 0x00, 0x57, 0x4C, 0x20, 0x20;}";

const VM_SHARK_WHITE_END                       = "asm { db 0xEB, 0x10, 0x57, 0x4C, 0x20, 0x20, 0x06, 0x02, 0x00, 0x00,
                                                                   0x00, 0x00, 0x00, 0x00, 0x57, 0x4C, 0x20, 0x20;}";

const VM_SHARK_RED_START                       = "asm { db 0xEB, 0x10, 0x57, 0x4C, 0x20, 0x20, 0x78, 0x00, 0x00, 0x00,
                                                                   0x00, 0x00, 0x00, 0x00, 0x57, 0x4C, 0x20, 0x20;}";

const VM_SHARK_RED_END                         = "asm { db 0xEB, 0x10, 0x57, 0x4C, 0x20, 0x20, 0x08, 0x02, 0x00, 0x00,
                                                                   0x00, 0x00, 0x00, 0x00, 0x57, 0x4C, 0x20, 0x20;}";

const VM_SHARK_BLACK_START                     = "asm { db 0xEB, 0x10, 0x57, 0x4C, 0x20, 0x20, 0x7A, 0x00, 0x00, 0x00,
                                                                   0x00, 0x00, 0x00, 0x00, 0x57, 0x4C, 0x20, 0x20;}";

const VM_SHARK_BLACK_END                       = "asm { db 0xEB, 0x10, 0x57, 0x4C, 0x20, 0x20, 0x0A, 0x02, 0x00, 0x00,
                                                                   0x00, 0x00, 0x00, 0x00, 0x57, 0x4C, 0x20, 0x20;}";

const VM_DOLPHIN_WHITE_START                   = "asm { db 0xEB, 0x10, 0x57, 0x4C, 0x20, 0x20, 0x86, 0x00, 0x00, 0x00,
                                                                   0x00, 0x00, 0x00, 0x00, 0x57, 0x4C, 0x20, 0x20;}";

const VM_DOLPHIN_WHITE_END                     = "asm { db 0xEB, 0x10, 0x57, 0x4C, 0x20, 0x20, 0x16, 0x02, 0x00, 0x00,
                                                                   0x00, 0x00, 0x00, 0x00, 0x57, 0x4C, 0x20, 0x20;}";

const VM_DOLPHIN_RED_START                     = "asm { db 0xEB, 0x10, 0x57, 0x4C, 0x20, 0x20, 0x88, 0x00, 0x00, 0x00,
                                                                   0x00, 0x00, 0x00, 0x00, 0x57, 0x4C, 0x20, 0x20;}";

const VM_DOLPHIN_RED_END                       = "asm { db 0xEB, 0x10, 0x57, 0x4C, 0x20, 0x20, 0x18, 0x02, 0x00, 0x00,
                                                                   0x00, 0x00, 0x00, 0x00, 0x57, 0x4C, 0x20, 0x20;}";

const VM_DOLPHIN_BLACK_START                   = "asm { db 0xEB, 0x10, 0x57, 0x4C, 0x20, 0x20, 0x8A, 0x00, 0x00, 0x00,
                                                                   0x00, 0x00, 0x00, 0x00, 0x57, 0x4C, 0x20, 0x20;}";

const VM_DOLPHIN_BLACK_END                     = "asm { db 0xEB, 0x10, 0x57, 0x4C, 0x20, 0x20, 0x1A, 0x02, 0x00, 0x00,
                                                                   0x00, 0x00, 0x00, 0x00, 0x57, 0x4C, 0x20, 0x20;}";

const VM_EAGLE_WHITE_START                     = "asm { db 0xEB, 0x10, 0x57, 0x4C, 0x20, 0x20, 0x92, 0x00, 0x00, 0x00,
                                                                   0x00, 0x00, 0x00, 0x00, 0x57, 0x4C, 0x20, 0x20;}";

const VM_EAGLE_WHITE_END                       = "asm { db 0xEB, 0x10, 0x57, 0x4C, 0x20, 0x20, 0x22, 0x02, 0x00, 0x00,
                                                                   0x00, 0x00, 0x00, 0x00, 0x57, 0x4C, 0x20, 0x20;}";

const VM_EAGLE_RED_START                       = "asm { db 0xEB, 0x10, 0x57, 0x4C, 0x20, 0x20, 0x94, 0x00, 0x00, 0x00,
                                                                   0x00, 0x00, 0x00, 0x00, 0x57, 0x4C, 0x20, 0x20;}";

const VM_EAGLE_RED_END                         = "asm { db 0xEB, 0x10, 0x57, 0x4C, 0x20, 0x20, 0x24, 0x02, 0x00, 0x00,
                                                                   0x00, 0x00, 0x00, 0x00, 0x57, 0x4C, 0x20, 0x20;}";

const VM_EAGLE_BLACK_START                     = "asm { db 0xEB, 0x10, 0x57, 0x4C, 0x20, 0x20, 0x96, 0x00, 0x00, 0x00,
                                                                   0x00, 0x00, 0x00, 0x00, 0x57, 0x4C, 0x20, 0x20;}";

const VM_EAGLE_BLACK_END                       = "asm { db 0xEB, 0x10, 0x57, 0x4C, 0x20, 0x20, 0x26, 0x02, 0x00, 0x00,
                                                                   0x00, 0x00, 0x00, 0x00, 0x57, 0x4C, 0x20, 0x20;}";

const VM_LION_WHITE_START                      = "asm { db 0xEB, 0x10, 0x57, 0x4C, 0x20, 0x20, 0xA0, 0x00, 0x00, 0x00,
                                                                   0x00, 0x00, 0x00, 0x00, 0x57, 0x4C, 0x20, 0x20;}";

const VM_LION_WHITE_END                        = "asm { db 0xEB, 0x10, 0x57, 0x4C, 0x20, 0x20, 0x30, 0x02, 0x00, 0x00,
                                                                   0x00, 0x00, 0x00, 0x00, 0x57, 0x4C, 0x20, 0x20;}";

const VM_LION_RED_START                        = "asm { db 0xEB, 0x10, 0x57, 0x4C, 0x20, 0x20, 0xA2, 0x00, 0x00, 0x00,
                                                                   0x00, 0x00, 0x00, 0x00, 0x57, 0x4C, 0x20, 0x20;}";

const VM_LION_RED_END                          = "asm { db 0xEB, 0x10, 0x57, 0x4C, 0x20, 0x20, 0x32, 0x02, 0x00, 0x00,
                                                                   0x00, 0x00, 0x00, 0x00, 0x57, 0x4C, 0x20, 0x20;}";

const VM_LION_BLACK_START                      = "asm { db 0xEB, 0x10, 0x57, 0x4C, 0x20, 0x20, 0xA4, 0x00, 0x00, 0x00,
                                                                   0x00, 0x00, 0x00, 0x00, 0x57, 0x4C, 0x20, 0x20;}";

const VM_LION_BLACK_END                        = "asm { db 0xEB, 0x10, 0x57, 0x4C, 0x20, 0x20, 0x34, 0x02, 0x00, 0x00,
                                                                   0x00, 0x00, 0x00, 0x00, 0x57, 0x4C, 0x20, 0x20;}";

const VM_MUTATE_ONLY_START                     = "asm { db 0xEB, 0x10, 0x57, 0x4C, 0x20, 0x20, 0x10, 0x00, 0x00, 0x00,
                                                                   0x00, 0x00, 0x00, 0x00, 0x57, 0x4C, 0x20, 0x20;}";

const VM_MUTATE_ONLY_END                       = "asm { db 0xEB, 0x10, 0x57, 0x4C, 0x20, 0x20, 0x11, 0x00, 0x00, 0x00,
                                                                   0x00, 0x00, 0x00, 0x00, 0x57, 0x4C, 0x20, 0x20;}";

}
