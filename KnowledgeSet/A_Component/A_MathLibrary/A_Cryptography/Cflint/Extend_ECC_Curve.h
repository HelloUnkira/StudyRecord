#ifndef EXTEND_ECC_CURVE_H
#define EXTEND_ECC_CURVE_H
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 选择合适的椭圆曲线 */
#define ECC_secp160r1  1
#define ECC_secp192r1  2
#define ECC_secp256r1  3
#define ECC_secp256k1  4
#define ECC_secp224r1  5
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
#define ECC_SCEP       ECC_secp256k1
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
#if 0
#elif (Cflint_Byte == CFLINT_BYTE1)
#define ECC_Curve1_Size1   20
#define ECC_Curve2_Size1   24
#define ECC_Curve3_Size1   32
#define ECC_Curve4_Size1   32
#define ECC_Curve5_Size1   28
#define ECC_Curve1_Size2   21
#define ECC_Curve2_Size2   24
#define ECC_Curve3_Size2   32
#define ECC_Curve4_Size2   32
#define ECC_Curve5_Size2   28
#elif (Cflint_Byte == CFLINT_BYTE4)
#define ECC_Curve1_Size1   5
#define ECC_Curve2_Size1   6
#define ECC_Curve3_Size1   8
#define ECC_Curve4_Size1   8
#define ECC_Curve5_Size1   7
#define ECC_Curve1_Size2   6
#define ECC_Curve2_Size2   6
#define ECC_Curve3_Size2   8
#define ECC_Curve4_Size2   8
#define ECC_Curve5_Size2   7
#elif (Cflint_Byte == CFLINT_BYTE8)
#define ECC_Curve1_Size1   3
#define ECC_Curve2_Size1   3
#define ECC_Curve3_Size1   4
#define ECC_Curve4_Size1   4
#define ECC_Curve5_Size1   4
#define ECC_Curve1_Size2   3
#define ECC_Curve2_Size2   3
#define ECC_Curve3_Size2   4
#define ECC_Curve4_Size2   4
#define ECC_Curve5_Size2   4
#else
#error "ECC Curve Cannot Support Current Type"
#endif
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
#if 0
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
#elif (Cflint_Byte == CFLINT_BYTE1)
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
#define Curve_P_1 {0xFF, 0xFF, 0xFF, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, \
                   0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, \
                   0xFF, 0xFF, 0xFF, 0xFF}
#define Curve_P_2 {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, \
                   0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, \
                   0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}
#define Curve_P_3 {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, \
                   0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, \
                   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
                   0x01, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF}
#define Curve_P_4 {0x2F, 0xFC, 0xFF, 0xFF, 0xFE, 0xFF, 0xFF, 0xFF, \
                   0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, \
                   0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, \
                   0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}
#define Curve_P_5 {0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
                   0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, \
                   0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, \
                   0xFF, 0xFF, 0xFF, 0xFF}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
#define Curve_B_1 {0x45, 0xFA, 0x65, 0xC5, 0xAD, 0xD4, 0xD4, 0x81, \
                   0x9F, 0xF8, 0xAC, 0x65, 0x8B, 0x7A, 0xBD, 0x54, \
                   0xFC, 0xBE, 0x97, 0x1C}
#define Curve_B_2 {0xB1, 0xB9, 0x46, 0xC1, 0xEC, 0xDE, 0xB8, 0xFE, \
                   0x49, 0x30, 0x24, 0x72, 0xAB, 0xE9, 0xA7, 0x0F, \
                   0xE7, 0x80, 0x9C, 0xE5, 0x19, 0x05, 0x21, 0x64}
#define Curve_B_3 {0x4B, 0x60, 0xD2, 0x27, 0x3E, 0x3C, 0xCE, 0x3B, \
                   0xF6, 0xB0, 0x53, 0xCC, 0xB0, 0x06, 0x1D, 0x65, \
                   0xBC, 0x86, 0x98, 0x76, 0x55, 0xBD, 0xEB, 0xB3, \
                   0xE7, 0x93, 0x3A, 0xAA, 0xD8, 0x35, 0xC6, 0x5A}
#define Curve_B_4 {0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
                   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
                   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
                   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
#define Curve_B_5 {0xB4, 0xFF, 0x55, 0x23, 0x43, 0x39, 0x0B, 0x27, \
                   0xBA, 0xD8, 0xBF, 0xD7, 0xB7, 0xB0, 0x44, 0x50, \
                   0x56, 0x32, 0x41, 0xF5, 0xAB, 0xB3, 0x04, 0x0C, \
                   0x85, 0x0A, 0x05, 0xB4}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
#define Curve_G_1 {{0x82, 0xFC, 0xCB, 0x13, 0xB9, 0x8B, 0xC3, 0x68, \
                    0x89, 0x69, 0x64, 0x46, 0x28, 0x73, 0xF5, 0x8E, \
                    0x68, 0xB5, 0x96, 0x4A}, \
                   {0x32, 0xFB, 0xC5, 0x7A, 0x37, 0x51, 0x23, 0x04, \
                    0x12, 0xC9, 0xDC, 0x59, 0x7D, 0x94, 0x68, 0x31, \
                    0x55, 0x28, 0xA6, 0x23}}
#define Curve_G_2 {{0x12, 0x10, 0xFF, 0x82, 0xFD, 0x0A, 0xFF, 0xF4, \
                    0x00, 0x88, 0xA1, 0x43, 0xEB, 0x20, 0xBF, 0x7C, \
                    0xF6, 0x90, 0x30, 0xB0, 0x0E, 0xA8, 0x8D, 0x18},\
                   {0x11, 0x48, 0x79, 0x1E, 0xA1, 0x77, 0xF9, 0x73, \
                    0xD5, 0xCD, 0x24, 0x6B, 0xED, 0x11, 0x10, 0x63, \
                    0x78, 0xDA, 0xC8, 0xFF, 0x95, 0x2B, 0x19, 0x07}}
#define Curve_G_3 {{0x96, 0xC2, 0x98, 0xD8, 0x45, 0x39, 0xA1, 0xF4, \
                    0xA0, 0x33, 0xEB, 0x2D, 0x81, 0x7D, 0x03, 0x77, \
                    0xF2, 0x40, 0xA4, 0x63, 0xE5, 0xE6, 0xBC, 0xF8, \
                    0x47, 0x42, 0x2C, 0xE1, 0xF2, 0xD1, 0x17, 0x6B},\
                   {0xF5, 0x51, 0xBF, 0x37, 0x68, 0x40, 0xB6, 0xCB, \
                    0xCE, 0x5E, 0x31, 0x6B, 0x57, 0x33, 0xCE, 0x2B, \
                    0x16, 0x9E, 0x0F, 0x7C, 0x4A, 0xEB, 0xE7, 0x8E, \
                    0x9B, 0x7F, 0x1A, 0xFE, 0xE2, 0x42, 0xE3, 0x4F}}
#define Curve_G_4 {{0x98, 0x17, 0xF8, 0x16, 0x5B, 0x81, 0xF2, 0x59, \
                    0xD9, 0x28, 0xCE, 0x2D, 0xDB, 0xFC, 0x9B, 0x02, \
                    0x07, 0x0B, 0x87, 0xCE, 0x95, 0x62, 0xA0, 0x55, \
                    0xAC, 0xBB, 0xDC, 0xF9, 0x7E, 0x66, 0xBE, 0x79},\
                   {0xB8, 0xD4, 0x10, 0xFB, 0x8F, 0xD0, 0x47, 0x9C, \
                    0x19, 0x54, 0x85, 0xA6, 0x48, 0xB4, 0x17, 0xFD, \
                    0xA8, 0x08, 0x11, 0x0E, 0xFC, 0xFB, 0xA4, 0x5D, \
                    0x65, 0xC4, 0xA3, 0x26, 0x77, 0xDA, 0x3A, 0x48}}
#define Curve_G_5 {{0x21, 0x1D, 0x5C, 0x11, 0xD6, 0x80, 0x32, 0x34, \
                    0x22, 0x11, 0xC2, 0x56, 0xD3, 0xC1, 0x03, 0x4A, \
                    0xB9, 0x90, 0x13, 0x32, 0x7F, 0xBF, 0xB4, 0x6B, \
                    0xBD, 0x0C, 0x0E, 0xB7}, \
                   {0x34, 0x7E, 0x00, 0x85, 0x99, 0x81, 0xD5, 0x44, \
                    0x64, 0x47, 0x07, 0x5A, 0xA0, 0x75, 0x43, 0xCD, \
                    0xE6, 0xDF, 0x22, 0x4C, 0xFB, 0x23, 0xF7, 0xB5, \
                    0x88, 0x63, 0x37, 0xBD}}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
#define Curve_N_1 {0x57, 0x22, 0x75, 0xCA, 0xD3, 0xAE, 0x27, 0xF9, \
                   0xC8, 0xF4, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, \
                   0x00, 0x00, 0x00, 0x00, 0x01}
#define Curve_N_2 {0x31, 0x28, 0xD2, 0xB4, 0xB1, 0xC9, 0x6B, 0x14, \
                   0x36, 0xF8, 0xDE, 0x99, 0xFF, 0xFF, 0xFF, 0xFF, \
                   0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}
#define Curve_N_3 {0x51, 0x25, 0x63, 0xFC, 0xC2, 0xCA, 0xB9, 0xF3, \
                   0x84, 0x9E, 0x17, 0xA7, 0xAD, 0xFA, 0xE6, 0xBC, \
                   0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, \
                   0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF}
#define Curve_N_4 {0x41, 0x41, 0x36, 0xD0, 0x8C, 0x5E, 0xD2, 0xBF, \
                   0x3B, 0xA0, 0x48, 0xAF, 0xE6, 0xDC, 0xAE, 0xBA, \
                   0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, \
                   0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}
#define Curve_N_5 {0x3D, 0x2A, 0x5C, 0x5C, 0x45, 0x29, 0xDD, 0x13, \
                   0x3E, 0xF0, 0xB8, 0xE0, 0xA2, 0x16, 0xFF, 0xFF, \
                   0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, \
                   0xFF, 0xFF, 0xFF, 0xFF}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
#elif (Cflint_Byte == CFLINT_BYTE4)
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
#define Curve_P_1 {0x7FFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, \
                   0xFFFFFFFF}
#define Curve_P_2 {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFE, 0xFFFFFFFF, \
                   0xFFFFFFFF, 0xFFFFFFFF}
#define Curve_P_3 {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000, \
                   0x00000000, 0x00000000, 0x00000001, 0xFFFFFFFF}
#define Curve_P_4 {0xFFFFFC2F, 0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, \
                   0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF}
#define Curve_P_5 {0x00000001, 0x00000000, 0x00000000, 0xFFFFFFFF, \
                   0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
#define Curve_B_1 {0xC565FA45, 0x81D4D4AD, 0x65ACF89F, 0x54BD7A8B, \
                   0x1C97BEFC}
#define Curve_B_2 {0xC146B9B1, 0xFEB8DEEC, 0x72243049, 0x0FA7E9AB, \
                   0xE59C80E7, 0x64210519}
#define Curve_B_3 {0x27D2604B, 0x3BCE3C3E, 0xCC53B0F6, 0x651D06B0, \
                   0x769886BC, 0xB3EBBD55, 0xAA3A93E7, 0x5AC635D8}
#define Curve_B_4 {0x00000007, 0x00000000, 0x00000000, 0x00000000, \
                   0x00000000, 0x00000000, 0x00000000, 0x00000000}
#define Curve_B_5 {0x2355FFB4, 0x270B3943, 0xD7BFD8BA, 0x5044B0B7, \
                   0xF5413256, 0x0C04B3AB, 0xB4050A85}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
#define Curve_G_1 {{0x13CBFC82, 0x68C38BB9, 0x46646989, 0x8EF57328, \
                    0x4A96B568}, \
                   {0x7AC5FB32, 0x04235137, 0x59DCC912, 0x3168947D, \
                    0x23A62855}}
#define Curve_G_2 {{0x82FF1012, 0xF4FF0AFD, 0x43A18800, 0x7CBF20EB, \
                    0xB03090F6, 0x188DA80E}, \
                   {0x1E794811, 0x73F977A1, 0x6B24CDD5, 0x631011ED, \
                    0xFFC8DA78, 0x07192B95}}
#define Curve_G_3 {{0xD898C296, 0xF4A13945, 0x2DEB33A0, 0x77037D81, \
                    0x63A440F2, 0xF8BCE6E5, 0xE12C4247, 0x6B17D1F2},\
                   {0x37BF51F5, 0xCBB64068, 0x6B315ECE, 0x2BCE3357, \
                    0x7C0F9E16, 0x8EE7EB4A, 0xFE1A7F9B, 0x4FE342E2}}
#define Curve_G_4 {{0x16F81798, 0x59F2815B, 0x2DCE28D9, 0x029BFCDB, \
                    0xCE870B07, 0x55A06295, 0xF9DCBBAC, 0x79BE667E},\
                   {0xFB10D4B8, 0x9C47D08F, 0xA6855419, 0xFD17B448, \
                    0x0E1108A8, 0x5DA4FBFC, 0x26A3C465, 0x483ADA77}}
#define Curve_G_5 {{0x115C1D21, 0x343280D6, 0x56C21122, 0x4A03C1D3, \
                    0x321390B9, 0x6BB4BF7F, 0xB70E0CBD}, \
                   {0x85007E34, 0x44D58199, 0x5A074764, 0xCD4375A0, \
                    0x4C22DFE6, 0xB5F723FB, 0xBD376388}}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
#define Curve_N_1 {0xCA752257, 0xF927AED3, 0x0001F4C8, 0x00000000, \
                   0x00000000, 0x00000001}
#define Curve_N_2 {0xB4D22831, 0x146BC9B1, 0x99DEF836, 0xFFFFFFFF, \
                   0xFFFFFFFF, 0xFFFFFFFF}
#define Curve_N_3 {0xFC632551, 0xF3B9CAC2, 0xA7179E84, 0xBCE6FAAD, \
                   0xFFFFFFFF, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF}
#define Curve_N_4 {0xD0364141, 0xBFD25E8C, 0xAF48A03B, 0xBAAEDCE6, \
                   0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF}
#define Curve_N_5 {0x5C5C2A3D, 0x13DD2945, 0xE0B8F03E, 0xFFFF16A2, \
                   0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
#elif (Cflint_Byte == CFLINT_BYTE8)
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
#define Curve_P_1 {0xFFFFFFFF7FFFFFFFull, 0xFFFFFFFFFFFFFFFFull, \
                   0x00000000FFFFFFFFull}
#define Curve_P_2 {0xFFFFFFFFFFFFFFFFull, 0xFFFFFFFFFFFFFFFEull, \
                   0xFFFFFFFFFFFFFFFFull}
#define Curve_P_3 {0xFFFFFFFFFFFFFFFFull, 0x00000000FFFFFFFFull, \
                   0x0000000000000000ull, 0xFFFFFFFF00000001ull}
#define Curve_P_4 {0xFFFFFFFEFFFFFC2Full, 0xFFFFFFFFFFFFFFFFull, \
                   0xFFFFFFFFFFFFFFFFull, 0xFFFFFFFFFFFFFFFFull}
#define Curve_P_5 {0x0000000000000001ull, 0xFFFFFFFF00000000ull, \
                   0xFFFFFFFFFFFFFFFFull, 0x00000000FFFFFFFFull}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
#define Curve_B_1 {0x81D4D4ADC565FA45ull, 0x54BD7A8B65ACF89Full, \
                   0x000000001C97BEFCull}
#define Curve_B_2 {0xFEB8DEECC146B9B1ull, 0x0FA7E9AB72243049ull, \
                   0x64210519E59C80E7ull}
#define Curve_B_3 {0x3BCE3C3E27D2604Bull, 0x651D06B0CC53B0F6ull, \
                   0xB3EBBD55769886BCull, 0x5AC635D8AA3A93E7ull}
#define Curve_B_4 {0x0000000000000007ull, 0x0000000000000000ull, \
                   0x0000000000000000ull, 0x0000000000000000ull}
#define Curve_B_5 {0x270B39432355FFB4ull, 0x5044B0B7D7BFD8BAull, \
                   0x0C04B3ABF5413256ull, 0x00000000B4050A85ull}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
#define Curve_G_1 {{0x68C38BB913CBFC82ull, 0x8EF5732846646989ull, \
                    0x000000004A96B568ull}, \
                   {0x042351377AC5FB32ull, 0x3168947D59DCC912ull, \
                    0x0000000023A62855ull}}
#define Curve_G_2 {{0xF4FF0AFD82FF1012ull, 0x7CBF20EB43A18800ull, \
                    0x188DA80EB03090F6ull}, \
                   {0x73F977A11E794811ull, 0x631011ED6B24CDD5ull, \
                    0x07192B95FFC8DA78ull}}
#define Curve_G_3 {{0xF4A13945D898C296ull, 0x77037D812DEB33A0ull, \
                    0xF8BCE6E563A440F2ull, 0x6B17D1F2E12C4247ull},\
                   {0xCBB6406837BF51F5ull, 0x2BCE33576B315ECEull, \
                    0x8EE7EB4A7C0F9E16ull, 0x4FE342E2FE1A7F9Bull}}
#define Curve_G_4 {{0x59F2815B16F81798ull, 0x029BFCDB2DCE28D9ull, \
                    0x55A06295CE870B07ull, 0x79BE667EF9DCBBACull}, \
                   {0x9C47D08FFB10D4B8ull, 0xFD17B448A6855419ull, \
                    0x5DA4FBFC0E1108A8ull, 0x483ADA7726A3C465ull}}
#define Curve_G_5 {{0x343280D6115C1D21ull, 0x4A03C1D356C21122ull, \
                    0x6BB4BF7F321390B9ull, 0x00000000B70E0CBDull}, \
                   {0x44D5819985007E34ull, 0xCD4375A05A074764ull, \
                    0xB5F723FB4C22DFE6ull, 0x00000000BD376388ull}}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
#define Curve_N_1 {0xF927AED3CA752257ull, 0x000000000001F4C8ull, \
                   0x0000000100000000ull}
#define Curve_N_2 {0x146BC9B1B4D22831ull, 0xFFFFFFFF99DEF836ull, \
                   0xFFFFFFFFFFFFFFFFull}
#define Curve_N_3 {0xF3B9CAC2FC632551ull, 0xBCE6FAADA7179E84ull, \
                   0xFFFFFFFFFFFFFFFFull, 0xFFFFFFFF00000000ull}
#define Curve_N_4 {0xBFD25E8CD0364141ull, 0xBAAEDCE6AF48A03Bull, \
                   0xFFFFFFFFFFFFFFFEull, 0xFFFFFFFFFFFFFFFFull}
#define Curve_N_5 {0x13DD29455C5C2A3Dull, 0xFFFF16A2E0B8F03Eull, \
                   0xFFFFFFFFFFFFFFFFull, 0x00000000FFFFFFFFull}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
#else
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
#error "ECC Curve Cannot Support Current Type"
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
#endif
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
#define ECC_CONCAT2(A, B)      A##B
#define ECC_CONCAT3(A, B, C)   A##B##C
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
#define ECC_CURVE_SIZE1(ECC_SCEP) ECC_CONCAT3(ECC_Curve, ECC_SCEP, _Size1)
#define ECC_CURVE_SIZE2(ECC_SCEP) ECC_CONCAT3(ECC_Curve, ECC_SCEP, _Size2)
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
#define Ecc_Curve_Size1    ECC_CURVE_SIZE1(ECC_SCEP)
#define Ecc_Curve_Size2    ECC_CURVE_SIZE2(ECC_SCEP)
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
#define ECC_CURVE_P(ECC_SCEP)     ECC_CONCAT2(Curve_P_, ECC_SCEP)
#define ECC_CURVE_B(ECC_SCEP)     ECC_CONCAT2(Curve_B_, ECC_SCEP)
#define ECC_CURVE_G(ECC_SCEP)     ECC_CONCAT2(Curve_G_, ECC_SCEP)
#define ECC_CURVE_N(ECC_SCEP)     ECC_CONCAT2(Curve_N_, ECC_SCEP)
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
#define Ecc_Curve_P    ECC_CURVE_P(ECC_SCEP)
#define Ecc_Curve_B    ECC_CURVE_B(ECC_SCEP)
#define Ecc_Curve_G    ECC_CURVE_G(ECC_SCEP)
#define Ecc_Curve_N    ECC_CURVE_N(ECC_SCEP)
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
#endif
