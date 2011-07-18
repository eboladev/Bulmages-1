/****************************************************************************
** Meta object code from reading C++ file 'barcode.h'
**
** Created: Fri Jul 1 00:22:31 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../plugins/report/items/barcode/barcode.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'barcode.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Barcode[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
      27,   14, // properties
      16,   95, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // properties: name, type, flags
      19,    8, 0x0009500b,
      48,   37, 0x0009510b,
      71,   58, 0x0009510b,
      87,   83, 0x02095103,
     101,   83, 0x02095103,
     124,   83, 0x02095103,
     149,  142, 0x43095103,
     172,  142, 0x43095103,
     205,  195, 0x0009510b,
     215,   83, 0x02095103,
     236,  228, 0x0a095103,
     243,  228, 0x0a095103,
     252,   83, 0x02095103,
     266,   83, 0x02095003,
     287,  279, 0x0009510b,
     306,  295, 0x0009510b,
     329,  317, 0x0009510b,
     358,  341, 0x0009510b,
     396,  375, 0x0009510b,
     417,  228, 0x0a095103,
     455,  432, 0x0009510b,
     496,  478, 0x0009510b,
     523,  514, 0x0009510b,
     546,  532, 0x0009510b,
     583,  560, 0x0009510b,
     630,  606, 0x0009510b,
     675,  654, 0x0009510b,

 // enums: name, flags, count, data
     606, 0x0,   22,  159,
     654, 0x0,   31,  203,
     560, 0x0,    6,  265,
     514, 0x0,   45,  277,
     532, 0x0,    8,  367,
     478, 0x0,   11,  383,
     432, 0x0,    7,  405,
     375, 0x0,    4,  419,
     341, 0x0,    2,  427,
     195, 0x0,    5,  431,
     279, 0x0,   10,  441,
     317, 0x0,   41,  461,
     295, 0x0,    5,  543,
      37, 0x0,    3,  553,
       8, 0x0,    3,  559,
      58, 0x0,   83,  565,

 // enum data: key, value
     696, uint(Barcode::NONECC200SIZE_AUTOMATIC),
     720, uint(Barcode::NONECC200SIZE_9x9),
     738, uint(Barcode::NONECC200SIZE_11x11),
     758, uint(Barcode::NONECC200SIZE_13x13),
     778, uint(Barcode::NONECC200SIZE_15x15),
     798, uint(Barcode::NONECC200SIZE_17x17),
     818, uint(Barcode::NONECC200SIZE_19x19),
     838, uint(Barcode::NONECC200SIZE_21x21),
     858, uint(Barcode::NONECC200SIZE_23x23),
     878, uint(Barcode::NONECC200SIZE_25x25),
     898, uint(Barcode::NONECC200SIZE_27x27),
     918, uint(Barcode::NONECC200SIZE_29x29),
     938, uint(Barcode::NONECC200SIZE_31x31),
     958, uint(Barcode::NONECC200SIZE_33x33),
     978, uint(Barcode::NONECC200SIZE_35x35),
     998, uint(Barcode::NONECC200SIZE_37x37),
    1018, uint(Barcode::NONECC200SIZE_39x39),
    1038, uint(Barcode::NONECC200SIZE_41x41),
    1058, uint(Barcode::NONECC200SIZE_43x43),
    1078, uint(Barcode::NONECC200SIZE_45x45),
    1098, uint(Barcode::NONECC200SIZE_47x47),
    1118, uint(Barcode::NONECC200SIZE_49x49),
    1138, uint(Barcode::ECC200SIZE_AUTOMATIC),
    1159, uint(Barcode::ECC200SIZE_10x10),
    1176, uint(Barcode::ECC200SIZE_12x12),
    1193, uint(Barcode::ECC200SIZE_14x14),
    1210, uint(Barcode::ECC200SIZE_16x16),
    1227, uint(Barcode::ECC200SIZE_18x18),
    1244, uint(Barcode::ECC200SIZE_20x20),
    1261, uint(Barcode::ECC200SIZE_22x22),
    1278, uint(Barcode::ECC200SIZE_24x24),
    1295, uint(Barcode::ECC200SIZE_26x26),
    1312, uint(Barcode::ECC200SIZE_32x32),
    1329, uint(Barcode::ECC200SIZE_36x36),
    1346, uint(Barcode::ECC200SIZE_40x40),
    1363, uint(Barcode::ECC200SIZE_44x44),
    1380, uint(Barcode::ECC200SIZE_48x48),
    1397, uint(Barcode::ECC200SIZE_52x52),
    1414, uint(Barcode::ECC200SIZE_64x64),
    1431, uint(Barcode::ECC200SIZE_72x72),
    1448, uint(Barcode::ECC200SIZE_80x80),
    1465, uint(Barcode::ECC200SIZE_88x88),
    1482, uint(Barcode::ECC200SIZE_96x96),
    1499, uint(Barcode::ECC200SIZE_104x104),
    1518, uint(Barcode::ECC200SIZE_120x120),
    1537, uint(Barcode::ECC200SIZE_132x132),
    1556, uint(Barcode::ECC200SIZE_144x144),
    1575, uint(Barcode::ECC200SIZE_8x18),
    1591, uint(Barcode::ECC200SIZE_8x32),
    1607, uint(Barcode::ECC200SIZE_12x26),
    1624, uint(Barcode::ECC200SIZE_12x36),
    1641, uint(Barcode::ECC200SIZE_16x36),
    1658, uint(Barcode::ECC200SIZE_16x48),
    1675, uint(Barcode::ECC_200_RECOMMENDED),
    1695, uint(Barcode::ECC_000),
    1703, uint(Barcode::ECC_050),
    1711, uint(Barcode::ECC_080),
    1719, uint(Barcode::ECC_100),
    1727, uint(Barcode::ECC_140),
    1735, uint(Barcode::QR_AUTO_RESIZE),
    1750, uint(Barcode::VERSION_1_21X21),
    1766, uint(Barcode::VERSION_2_25X25),
    1782, uint(Barcode::VERSION_3_29X29),
    1798, uint(Barcode::VERSION_4_33X33),
    1814, uint(Barcode::VERSION_5_37X37),
    1830, uint(Barcode::VERSION_6_41X41),
    1846, uint(Barcode::VERSION_7_45X45),
    1862, uint(Barcode::VERSION_8_49X49),
    1878, uint(Barcode::VERSION_9_53X53),
    1894, uint(Barcode::VERSION_10_57X57),
    1911, uint(Barcode::VERSION_11_61X61),
    1928, uint(Barcode::VERSION_12_65X65),
    1945, uint(Barcode::VERSION_13_69X69),
    1962, uint(Barcode::VERSION_14_73X73),
    1979, uint(Barcode::VERSION_15_77X77),
    1996, uint(Barcode::VERSION_16_81X81),
    2013, uint(Barcode::VERSION_17_85X85),
    2030, uint(Barcode::VERSION_18_89X89),
    2047, uint(Barcode::VERSION_19_93X93),
    2064, uint(Barcode::VERSION_20_97X97),
    2081, uint(Barcode::VERSION_21_101X101),
    2100, uint(Barcode::VERSION_22_105X105),
    2119, uint(Barcode::VERSION_23_109X109),
    2138, uint(Barcode::VERSION_24_113X113),
    2157, uint(Barcode::VERSION_25_117X117),
    2176, uint(Barcode::VERSION_26_121X121),
    2195, uint(Barcode::VERSION_27_125X125),
    2214, uint(Barcode::VERSION_28_129X129),
    2233, uint(Barcode::VERSION_29_133X133),
    2252, uint(Barcode::VERSION_30_137X137),
    2271, uint(Barcode::VERSION_31_141X141),
    2290, uint(Barcode::VERSION_32_145X145),
    2309, uint(Barcode::VERSION_33_149X149),
    2328, uint(Barcode::VERSION_34_153X153),
    2347, uint(Barcode::VERSION_35_157X157),
    2366, uint(Barcode::VERSION_36_161X161),
    2385, uint(Barcode::VERSION_37_165X165),
    2404, uint(Barcode::VERSION_38_169X169),
    2423, uint(Barcode::VERSION_39_173X173),
    2442, uint(Barcode::VERSION_40_177X177),
    2461, uint(Barcode::LEVEL_L_20),
    2472, uint(Barcode::LEVEL_M_37),
    2483, uint(Barcode::LEVEL_Q_55),
    2494, uint(Barcode::LEVEL_H_65),
    2505, uint(Barcode::MQR_AUTO_RESIZE),
    2521, uint(Barcode::VERSION_M1_11X11),
    2538, uint(Barcode::VERSION_M2_13X13),
    2555, uint(Barcode::VERSION_M3_15X15),
    2572, uint(Barcode::VERSION_M4_17X17),
    2589, uint(Barcode::MQR_LEVEL_L_20),
    2604, uint(Barcode::MQR_LEVEL_M_37),
    2619, uint(Barcode::MQR_LEVEL_Q_55),
    2634, uint(Barcode::AUTOMATIC_SYMBOL_SIZE),
    2656, uint(Barcode::VERSION_A_16X18),
    2672, uint(Barcode::VERSION_B_22X22),
    2688, uint(Barcode::VERSION_C_28X32),
    2704, uint(Barcode::VERSION_D_40X42),
    2720, uint(Barcode::VERSION_E_52X54),
    2736, uint(Barcode::VERSION_F_70X76),
    2752, uint(Barcode::VERSION_G_104X98),
    2769, uint(Barcode::VERSION_H_148X134),
    2787, uint(Barcode::VERSION_S_8X_HEIGHT),
    2807, uint(Barcode::VERSION_T_16X_HEIGHT),
    2828, uint(Barcode::AUTO),
    2833, uint(Barcode::CH_3),
    2838, uint(Barcode::CH_4),
    2843, uint(Barcode::CH_5),
    2848, uint(Barcode::CH_6),
    2853, uint(Barcode::CH_7),
    2858, uint(Barcode::CH_8),
    2863, uint(Barcode::MODE_2_OR_3),
    2875, uint(Barcode::MODE_4),
    2882, uint(Barcode::MODE_5),
    2889, uint(Barcode::MODE_6),
    2896, uint(Barcode::NO_CHECK_DIGIT),
    2911, uint(Barcode::MOD_43__CHECK_DIGIT),
    2931, uint(Barcode::DATA),
    2936, uint(Barcode::UNICODE),
    2944, uint(Barcode::GS1),
    2948, uint(Barcode::KANJI),
    2954, uint(Barcode::SJIS),
    2959, uint(Barcode::ECC_AUTO),
    2968, uint(Barcode::ECC_2_WORDS),
    2980, uint(Barcode::ECC_4_WORDS),
    2992, uint(Barcode::ECC_8_WORDS),
    3004, uint(Barcode::ECC_16_WORDS),
    3017, uint(Barcode::ECC_32_WORDS),
    3030, uint(Barcode::ECC_64_WORDS),
    3043, uint(Barcode::ECC_128_WORDS),
    3057, uint(Barcode::ECC_256_WORDS),
    3071, uint(Barcode::ECC_512_WORDS),
    3085, uint(Barcode::AUTOMATIC_RESIZE),
    3102, uint(Barcode::ADJUST_SIZE_TO_15x15_COMPACT),
    3131, uint(Barcode::ADJUST_SIZE_TO_19x19_COMPACT),
    3160, uint(Barcode::ADJUST_SIZE_TO_23x23_COMPACT),
    3189, uint(Barcode::ADJUST_SIZE_TO_27x27_COMPACT),
    3218, uint(Barcode::ADJUST_SIZE_TO_19x19),
    3239, uint(Barcode::ADJUST_SIZE_TO_23x23),
    3260, uint(Barcode::ADJUST_SIZE_TO_27x27),
    3281, uint(Barcode::ADJUST_SIZE_TO_31x31),
    3302, uint(Barcode::ADJUST_SIZE_TO_37x37),
    3323, uint(Barcode::ADJUST_SIZE_TO_41x41),
    3344, uint(Barcode::ADJUST_SIZE_TO_45x45),
    3365, uint(Barcode::ADJUST_SIZE_TO_49x49),
    3386, uint(Barcode::ADJUST_SIZE_TO_53x53),
    3407, uint(Barcode::ADJUST_SIZE_TO_57x57),
    3428, uint(Barcode::ADJUST_SIZE_TO_61x61),
    3449, uint(Barcode::ADJUST_SIZE_TO_67x67),
    3470, uint(Barcode::ADJUST_SIZE_TO_71x71),
    3491, uint(Barcode::ADJUST_SIZE_TO_75x75),
    3512, uint(Barcode::ADJUST_SIZE_TO_79x79),
    3533, uint(Barcode::ADJUST_SIZE_TO_83x83),
    3554, uint(Barcode::ADJUST_SIZE_TO_87x87),
    3575, uint(Barcode::ADJUST_SIZE_TO_91x91),
    3596, uint(Barcode::ADJUST_SIZE_TO_95x95),
    3617, uint(Barcode::ADJUST_SIZE_TO_101x101),
    3640, uint(Barcode::ADJUST_SIZE_TO_105x105),
    3663, uint(Barcode::ADJUST_SIZE_TO_109x109),
    3686, uint(Barcode::ADJUST_SIZE_TO_113x113),
    3709, uint(Barcode::ADJUST_SIZE_TO_117x117),
    3732, uint(Barcode::ADJUST_SIZE_TO_121x121),
    3755, uint(Barcode::ADJUST_SIZE_TO_125x125),
    3778, uint(Barcode::ADJUST_SIZE_TO_131x131),
    3801, uint(Barcode::ADJUST_SIZE_TO_135x135),
    3824, uint(Barcode::ADJUST_SIZE_TO_139x139),
    3847, uint(Barcode::ADJUST_SIZE_TO_143x143),
    3870, uint(Barcode::ADJUST_SIZE_TO_147x147),
    3893, uint(Barcode::ADJUST_SIZE_TO_151x151),
    3916, uint(Barcode::ECC_10_3_words),
    3931, uint(Barcode::ECC_23_3_words),
    3946, uint(Barcode::ECC_36_3_words),
    3961, uint(Barcode::ECC_50_3_words),
    3976, uint(Barcode::MSI_PLESSEY_),
    3989, uint(Barcode::MSI_10),
    3996, uint(Barcode::MSI_10_10),
    4006, uint(Barcode::MSI_11),
    4013, uint(Barcode::MSI_11_10),
    4023, uint(Barcode::NO_FRAME),
    4032, uint(Barcode::BARCODE_BIND),
    4045, uint(Barcode::BARCODE_BOX),
    4057, uint(Barcode::IgnoreAspectRatio),
    4075, uint(Barcode::KeepAspectRatio),
    4091, uint(Barcode::CenterBarCode),
    4105, uint(Barcode::AUSPOST),
    4113, uint(Barcode::AUSREPLY),
    4122, uint(Barcode::AUSROUTE),
    4131, uint(Barcode::AUSREDIRECT),
    4143, uint(Barcode::AZTEC),
    4149, uint(Barcode::AZRUNE),
    4156, uint(Barcode::CHANNEL),
    4164, uint(Barcode::CODABAR),
    4172, uint(Barcode::CODABLOCKF),
    4183, uint(Barcode::HIBC_BLOCKF),
    4195, uint(Barcode::CODEONE),
    4203, uint(Barcode::CODE11),
    4210, uint(Barcode::CODE128),
    4218, uint(Barcode::CODE128B),
    4227, uint(Barcode::HIBC_128),
    4236, uint(Barcode::CODE16K),
    4244, uint(Barcode::C25LOGIC),
    4253, uint(Barcode::C25IATA),
    4261, uint(Barcode::C25IND),
    4268, uint(Barcode::C25INTER),
    4277, uint(Barcode::C25MATRIX),
    4287, uint(Barcode::CODE32),
    4294, uint(Barcode::CODE39),
    4301, uint(Barcode::EXCODE39),
    4310, uint(Barcode::HIBC_39),
    4318, uint(Barcode::CODE49),
    4325, uint(Barcode::CODE93),
    4332, uint(Barcode::CODE_ONE),
    4341, uint(Barcode::RSS14),
    4347, uint(Barcode::RSS14_CC),
    4356, uint(Barcode::RSS_EXP),
    4364, uint(Barcode::RSS_EXP_CC),
    4375, uint(Barcode::RSS_EXPSTACK),
    4388, uint(Barcode::RSS_EXPSTACK_CC),
    4404, uint(Barcode::RSS_LTD),
    4412, uint(Barcode::RSS_LTD_CC),
    4423, uint(Barcode::RSS14STACK),
    4434, uint(Barcode::RSS14STACK_CC),
    4448, uint(Barcode::RSS14STACK_OMNI),
    4464, uint(Barcode::RSS14_OMNI_CC),
    4478, uint(Barcode::DATAMATRIX),
    4489, uint(Barcode::DPIDENT),
    4497, uint(Barcode::DPLEIT),
    4504, uint(Barcode::KIX),
    4508, uint(Barcode::EAN128),
    4515, uint(Barcode::EAN128_CC),
    4525, uint(Barcode::EAN14),
    4531, uint(Barcode::EANX),
    4536, uint(Barcode::EANX_CC),
    4544, uint(Barcode::FIM),
    4548, uint(Barcode::FLAT),
    4553, uint(Barcode::ITF14),
    4559, uint(Barcode::ISBNX),
    4565, uint(Barcode::JAPANPOST),
    4575, uint(Barcode::KOREAPOST),
    4585, uint(Barcode::LOGMARS),
    4593, uint(Barcode::MAXICODE),
    4602, uint(Barcode::MSI_PLESSEY),
    4614, uint(Barcode::NVE18),
    4620, uint(Barcode::PDF417),
    4627, uint(Barcode::PDF417TRUNC),
    4639, uint(Barcode::HIBC_PDF),
    4648, uint(Barcode::MICROPDF417),
    4660, uint(Barcode::HIBC_MICPDF),
    4672, uint(Barcode::PHARMA),
    4679, uint(Barcode::PHARMA_TWO),
    4690, uint(Barcode::PZN),
    4694, uint(Barcode::PLANET),
    4701, uint(Barcode::POSTNET),
    4709, uint(Barcode::QRCODE),
    4716, uint(Barcode::MICROQR),
    4724, uint(Barcode::HIBC_QR),
    4732, uint(Barcode::RM4SCC),
    4739, uint(Barcode::TELEPEN),
    4747, uint(Barcode::TELEPEN_NUM),
    4759, uint(Barcode::PLESSEY),
    4767, uint(Barcode::UPCA),
    4772, uint(Barcode::UPCA_CC),
    4780, uint(Barcode::UPCE),
    4785, uint(Barcode::UPCE_CC),
    4793, uint(Barcode::ONECODE),
    4801, uint(Barcode::DAFT),
    4806, uint(Barcode::HIBC_DM),

       0        // eod
};

static const char qt_meta_stringdata_Barcode[] = {
    "Barcode\0PaintTypes\0howToPaintBarcode\0"
    "FrameTypes\0frameType\0BarcodeTypes\0"
    "barcodeType\0int\0barcodeHeight\0"
    "barcodeWhitespaceWidth\0barcodeBoderWidth\0"
    "QColor\0barcodeForegroundColor\0"
    "barcodeBackgroundColor\0InputMode\0"
    "inputMode\0barcodeWidth\0QString\0script\0"
    "testText\0pdfDataColmns\0pdfCodewords\0"
    "Pdf_ECC\0pdf_ECC\0MsiPlessey\0msiPlessey\0"
    "AztecResize\0aztecResize\0Code39CheckDigit\0"
    "code39CheckDigit\0MaxiCodeEncodingMode\0"
    "maxiCodeEncodingMode\0primaryMessage\0"
    "BarcodeChannelChannels\0barcodeChannelChannels\0"
    "CodeoneSymbolSize\0codeoneSymbolSize\0"
    "QrResize\0qrResize\0MicroQrResize\0"
    "microQrResize\0DataMatrixEncodingMode\0"
    "dataMatrixEncodingMode\0DataMatrixNonECC200Size\0"
    "dataMatrixNonECC200Size\0DataMatrixECC200Size\0"
    "dataMatrixECC200Size\0NONECC200SIZE_AUTOMATIC\0"
    "NONECC200SIZE_9x9\0NONECC200SIZE_11x11\0"
    "NONECC200SIZE_13x13\0NONECC200SIZE_15x15\0"
    "NONECC200SIZE_17x17\0NONECC200SIZE_19x19\0"
    "NONECC200SIZE_21x21\0NONECC200SIZE_23x23\0"
    "NONECC200SIZE_25x25\0NONECC200SIZE_27x27\0"
    "NONECC200SIZE_29x29\0NONECC200SIZE_31x31\0"
    "NONECC200SIZE_33x33\0NONECC200SIZE_35x35\0"
    "NONECC200SIZE_37x37\0NONECC200SIZE_39x39\0"
    "NONECC200SIZE_41x41\0NONECC200SIZE_43x43\0"
    "NONECC200SIZE_45x45\0NONECC200SIZE_47x47\0"
    "NONECC200SIZE_49x49\0ECC200SIZE_AUTOMATIC\0"
    "ECC200SIZE_10x10\0ECC200SIZE_12x12\0"
    "ECC200SIZE_14x14\0ECC200SIZE_16x16\0"
    "ECC200SIZE_18x18\0ECC200SIZE_20x20\0"
    "ECC200SIZE_22x22\0ECC200SIZE_24x24\0"
    "ECC200SIZE_26x26\0ECC200SIZE_32x32\0"
    "ECC200SIZE_36x36\0ECC200SIZE_40x40\0"
    "ECC200SIZE_44x44\0ECC200SIZE_48x48\0"
    "ECC200SIZE_52x52\0ECC200SIZE_64x64\0"
    "ECC200SIZE_72x72\0ECC200SIZE_80x80\0"
    "ECC200SIZE_88x88\0ECC200SIZE_96x96\0"
    "ECC200SIZE_104x104\0ECC200SIZE_120x120\0"
    "ECC200SIZE_132x132\0ECC200SIZE_144x144\0"
    "ECC200SIZE_8x18\0ECC200SIZE_8x32\0"
    "ECC200SIZE_12x26\0ECC200SIZE_12x36\0"
    "ECC200SIZE_16x36\0ECC200SIZE_16x48\0"
    "ECC_200_RECOMMENDED\0ECC_000\0ECC_050\0"
    "ECC_080\0ECC_100\0ECC_140\0QR_AUTO_RESIZE\0"
    "VERSION_1_21X21\0VERSION_2_25X25\0"
    "VERSION_3_29X29\0VERSION_4_33X33\0"
    "VERSION_5_37X37\0VERSION_6_41X41\0"
    "VERSION_7_45X45\0VERSION_8_49X49\0"
    "VERSION_9_53X53\0VERSION_10_57X57\0"
    "VERSION_11_61X61\0VERSION_12_65X65\0"
    "VERSION_13_69X69\0VERSION_14_73X73\0"
    "VERSION_15_77X77\0VERSION_16_81X81\0"
    "VERSION_17_85X85\0VERSION_18_89X89\0"
    "VERSION_19_93X93\0VERSION_20_97X97\0"
    "VERSION_21_101X101\0VERSION_22_105X105\0"
    "VERSION_23_109X109\0VERSION_24_113X113\0"
    "VERSION_25_117X117\0VERSION_26_121X121\0"
    "VERSION_27_125X125\0VERSION_28_129X129\0"
    "VERSION_29_133X133\0VERSION_30_137X137\0"
    "VERSION_31_141X141\0VERSION_32_145X145\0"
    "VERSION_33_149X149\0VERSION_34_153X153\0"
    "VERSION_35_157X157\0VERSION_36_161X161\0"
    "VERSION_37_165X165\0VERSION_38_169X169\0"
    "VERSION_39_173X173\0VERSION_40_177X177\0"
    "LEVEL_L_20\0LEVEL_M_37\0LEVEL_Q_55\0"
    "LEVEL_H_65\0MQR_AUTO_RESIZE\0VERSION_M1_11X11\0"
    "VERSION_M2_13X13\0VERSION_M3_15X15\0"
    "VERSION_M4_17X17\0MQR_LEVEL_L_20\0"
    "MQR_LEVEL_M_37\0MQR_LEVEL_Q_55\0"
    "AUTOMATIC_SYMBOL_SIZE\0VERSION_A_16X18\0"
    "VERSION_B_22X22\0VERSION_C_28X32\0"
    "VERSION_D_40X42\0VERSION_E_52X54\0"
    "VERSION_F_70X76\0VERSION_G_104X98\0"
    "VERSION_H_148X134\0VERSION_S_8X_HEIGHT\0"
    "VERSION_T_16X_HEIGHT\0AUTO\0CH_3\0CH_4\0"
    "CH_5\0CH_6\0CH_7\0CH_8\0MODE_2_OR_3\0MODE_4\0"
    "MODE_5\0MODE_6\0NO_CHECK_DIGIT\0"
    "MOD_43__CHECK_DIGIT\0DATA\0UNICODE\0GS1\0"
    "KANJI\0SJIS\0ECC_AUTO\0ECC_2_WORDS\0"
    "ECC_4_WORDS\0ECC_8_WORDS\0ECC_16_WORDS\0"
    "ECC_32_WORDS\0ECC_64_WORDS\0ECC_128_WORDS\0"
    "ECC_256_WORDS\0ECC_512_WORDS\0"
    "AUTOMATIC_RESIZE\0ADJUST_SIZE_TO_15x15_COMPACT\0"
    "ADJUST_SIZE_TO_19x19_COMPACT\0"
    "ADJUST_SIZE_TO_23x23_COMPACT\0"
    "ADJUST_SIZE_TO_27x27_COMPACT\0"
    "ADJUST_SIZE_TO_19x19\0ADJUST_SIZE_TO_23x23\0"
    "ADJUST_SIZE_TO_27x27\0ADJUST_SIZE_TO_31x31\0"
    "ADJUST_SIZE_TO_37x37\0ADJUST_SIZE_TO_41x41\0"
    "ADJUST_SIZE_TO_45x45\0ADJUST_SIZE_TO_49x49\0"
    "ADJUST_SIZE_TO_53x53\0ADJUST_SIZE_TO_57x57\0"
    "ADJUST_SIZE_TO_61x61\0ADJUST_SIZE_TO_67x67\0"
    "ADJUST_SIZE_TO_71x71\0ADJUST_SIZE_TO_75x75\0"
    "ADJUST_SIZE_TO_79x79\0ADJUST_SIZE_TO_83x83\0"
    "ADJUST_SIZE_TO_87x87\0ADJUST_SIZE_TO_91x91\0"
    "ADJUST_SIZE_TO_95x95\0ADJUST_SIZE_TO_101x101\0"
    "ADJUST_SIZE_TO_105x105\0ADJUST_SIZE_TO_109x109\0"
    "ADJUST_SIZE_TO_113x113\0ADJUST_SIZE_TO_117x117\0"
    "ADJUST_SIZE_TO_121x121\0ADJUST_SIZE_TO_125x125\0"
    "ADJUST_SIZE_TO_131x131\0ADJUST_SIZE_TO_135x135\0"
    "ADJUST_SIZE_TO_139x139\0ADJUST_SIZE_TO_143x143\0"
    "ADJUST_SIZE_TO_147x147\0ADJUST_SIZE_TO_151x151\0"
    "ECC_10_3_words\0ECC_23_3_words\0"
    "ECC_36_3_words\0ECC_50_3_words\0"
    "MSI_PLESSEY_\0MSI_10\0MSI_10_10\0MSI_11\0"
    "MSI_11_10\0NO_FRAME\0BARCODE_BIND\0"
    "BARCODE_BOX\0IgnoreAspectRatio\0"
    "KeepAspectRatio\0CenterBarCode\0AUSPOST\0"
    "AUSREPLY\0AUSROUTE\0AUSREDIRECT\0AZTEC\0"
    "AZRUNE\0CHANNEL\0CODABAR\0CODABLOCKF\0"
    "HIBC_BLOCKF\0CODEONE\0CODE11\0CODE128\0"
    "CODE128B\0HIBC_128\0CODE16K\0C25LOGIC\0"
    "C25IATA\0C25IND\0C25INTER\0C25MATRIX\0"
    "CODE32\0CODE39\0EXCODE39\0HIBC_39\0CODE49\0"
    "CODE93\0CODE_ONE\0RSS14\0RSS14_CC\0RSS_EXP\0"
    "RSS_EXP_CC\0RSS_EXPSTACK\0RSS_EXPSTACK_CC\0"
    "RSS_LTD\0RSS_LTD_CC\0RSS14STACK\0"
    "RSS14STACK_CC\0RSS14STACK_OMNI\0"
    "RSS14_OMNI_CC\0DATAMATRIX\0DPIDENT\0"
    "DPLEIT\0KIX\0EAN128\0EAN128_CC\0EAN14\0"
    "EANX\0EANX_CC\0FIM\0FLAT\0ITF14\0ISBNX\0"
    "JAPANPOST\0KOREAPOST\0LOGMARS\0MAXICODE\0"
    "MSI_PLESSEY\0NVE18\0PDF417\0PDF417TRUNC\0"
    "HIBC_PDF\0MICROPDF417\0HIBC_MICPDF\0"
    "PHARMA\0PHARMA_TWO\0PZN\0PLANET\0POSTNET\0"
    "QRCODE\0MICROQR\0HIBC_QR\0RM4SCC\0TELEPEN\0"
    "TELEPEN_NUM\0PLESSEY\0UPCA\0UPCA_CC\0UPCE\0"
    "UPCE_CC\0ONECODE\0DAFT\0HIBC_DM\0"
};

const QMetaObject Barcode::staticMetaObject = {
    { &Report::ItemInterface::staticMetaObject, qt_meta_stringdata_Barcode,
      qt_meta_data_Barcode, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Barcode::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Barcode::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Barcode::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Barcode))
        return static_cast<void*>(const_cast< Barcode*>(this));
    if (!strcmp(_clname, "ro.bigendian.Report.ItemInterface/1.0"))
        return static_cast< Report::ItemInterface*>(const_cast< Barcode*>(this));
    typedef Report::ItemInterface QMocSuperClass;
    return QMocSuperClass::qt_metacast(_clname);
}

int Barcode::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    typedef Report::ItemInterface QMocSuperClass;
    _id = QMocSuperClass::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    
#ifndef QT_NO_PROPERTIES
     if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< PaintTypes*>(_v) = paintType(); break;
        case 1: *reinterpret_cast< FrameTypes*>(_v) = frameType(); break;
        case 2: *reinterpret_cast< BarcodeTypes*>(_v) = barcodeType(); break;
        case 3: *reinterpret_cast< int*>(_v) = barcodeHeight(); break;
        case 4: *reinterpret_cast< int*>(_v) = barcodeWhitespaceWidth(); break;
        case 5: *reinterpret_cast< int*>(_v) = barcodeBoderWidth(); break;
        case 6: *reinterpret_cast< QColor*>(_v) = barcodeForegroundColor(); break;
        case 7: *reinterpret_cast< QColor*>(_v) = barcodeBackgroundColor(); break;
        case 8: *reinterpret_cast< InputMode*>(_v) = inputMode(); break;
        case 9: *reinterpret_cast< int*>(_v) = barcodeWidth(); break;
        case 10: *reinterpret_cast< QString*>(_v) = script(); break;
        case 11: *reinterpret_cast< QString*>(_v) = testText(); break;
        case 12: *reinterpret_cast< int*>(_v) = pdfDataColmns(); break;
        case 13: *reinterpret_cast< int*>(_v) = pdf417Max(); break;
        case 14: *reinterpret_cast< Pdf_ECC*>(_v) = pdf_ECC(); break;
        case 15: *reinterpret_cast< MsiPlessey*>(_v) = msiPlessey(); break;
        case 16: *reinterpret_cast< AztecResize*>(_v) = aztecResize(); break;
        case 17: *reinterpret_cast< Code39CheckDigit*>(_v) = code39CheckDigit(); break;
        case 18: *reinterpret_cast< MaxiCodeEncodingMode*>(_v) = maxiCodeEncodingMode(); break;
        case 19: *reinterpret_cast< QString*>(_v) = primaryMessage(); break;
        case 20: *reinterpret_cast< BarcodeChannelChannels*>(_v) = barcodeChannelChannels(); break;
        case 21: *reinterpret_cast< CodeoneSymbolSize*>(_v) = codeoneSymbolSize(); break;
        case 22: *reinterpret_cast< QrResize*>(_v) = qrResize(); break;
        case 23: *reinterpret_cast< MicroQrResize*>(_v) = microQrResize(); break;
        case 24: *reinterpret_cast< DataMatrixEncodingMode*>(_v) = dataMatrixEncodingMode(); break;
        case 25: *reinterpret_cast< DataMatrixNonECC200Size*>(_v) = dataMatrixNonECC200Size(); break;
        case 26: *reinterpret_cast< DataMatrixECC200Size*>(_v) = dataMatrixECC200Size(); break;
        }
        _id -= 27;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: setPaintType(*reinterpret_cast< PaintTypes*>(_v)); break;
        case 1: setFrameType(*reinterpret_cast< FrameTypes*>(_v)); break;
        case 2: setBarcodeType(*reinterpret_cast< BarcodeTypes*>(_v)); break;
        case 3: setBarcodeHeight(*reinterpret_cast< int*>(_v)); break;
        case 4: setBarcodeWhitespaceWidth(*reinterpret_cast< int*>(_v)); break;
        case 5: setBarcodeBoderWidth(*reinterpret_cast< int*>(_v)); break;
        case 6: setBarcodeForegroundColor(*reinterpret_cast< QColor*>(_v)); break;
        case 7: setBarcodeBackgroundColor(*reinterpret_cast< QColor*>(_v)); break;
        case 8: setInputMode(*reinterpret_cast< InputMode*>(_v)); break;
        case 9: setBarcodeWidth(*reinterpret_cast< int*>(_v)); break;
        case 10: setScript(*reinterpret_cast< QString*>(_v)); break;
        case 11: setTestText(*reinterpret_cast< QString*>(_v)); break;
        case 12: setPdfDataColmns(*reinterpret_cast< int*>(_v)); break;
        case 13: setPdf417Max(*reinterpret_cast< int*>(_v)); break;
        case 14: setPdf_ECC(*reinterpret_cast< Pdf_ECC*>(_v)); break;
        case 15: setMsiPlessey(*reinterpret_cast< MsiPlessey*>(_v)); break;
        case 16: setAztecResize(*reinterpret_cast< AztecResize*>(_v)); break;
        case 17: setCode39CheckDigit(*reinterpret_cast< Code39CheckDigit*>(_v)); break;
        case 18: setMaxiCodeEncodingMode(*reinterpret_cast< MaxiCodeEncodingMode*>(_v)); break;
        case 19: setPrimaryMessage(*reinterpret_cast< QString*>(_v)); break;
        case 20: setBarcodeChannelChannels(*reinterpret_cast< BarcodeChannelChannels*>(_v)); break;
        case 21: setCodeoneSymbolSize(*reinterpret_cast< CodeoneSymbolSize*>(_v)); break;
        case 22: setQrResize(*reinterpret_cast< QrResize*>(_v)); break;
        case 23: setMicroQrResize(*reinterpret_cast< MicroQrResize*>(_v)); break;
        case 24: setDataMatrixEncodingMode(*reinterpret_cast< DataMatrixEncodingMode*>(_v)); break;
        case 25: setDataMatrixNonECC200Size(*reinterpret_cast< DataMatrixNonECC200Size*>(_v)); break;
        case 26: setDataMatrixECC200Size(*reinterpret_cast< DataMatrixECC200Size*>(_v)); break;
        }
        _id -= 27;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 27;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 27;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 27;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 27;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 27;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 27;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}
QT_END_MOC_NAMESPACE
