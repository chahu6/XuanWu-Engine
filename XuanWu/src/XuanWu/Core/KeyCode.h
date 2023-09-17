#pragma once

namespace XuanWu
{
	using KeyCode = uint16_t;

	namespace Key
	{
		enum : KeyCode
		{
			SPACE             = 32,
			APOSTROPHE        = 39,
			COMMA             = 44,
			MINUS             = 45,
			PERIOD            = 46,
			SLASH             = 47,


			D0                = 48,
			D1                = 49,
			D2                = 50,
			D3                = 51,
			D4                = 52,
			D5                = 53,
			D6                = 54,
			D7                = 55,
			D8                = 56,
			D9                = 57,


			SEMICOLON         = 59,
			EQUAL             = 61,


			A                 = 65,
			B                 = 66,
			C                 = 67,
			D                 = 68,
			E                 = 69,
			F                 = 70,
			G                 = 71,
			H                 = 72,
			I                 = 73,
			J                 = 74,
			K                 = 75,
			L                 = 76,
			M                 = 77,
			N                 = 78,
			O                 = 79,
			P                 = 80,
			Q                 = 81,
			R                 = 82,
			S                 = 83,
			T                 = 84,
			U                 = 85,
			V                 = 86,
			W                 = 87,
			X                 = 88,
			Y                 = 89,
			Z                 = 90,


			LEFT_BRACKET      = 91,
			BACKSLASH         = 92,
			RIGHT_BRACKET     = 93,
			GRAVE_ACCENT      = 96,
			WORLD_1           = 161,
			WORLD_2           = 162,
			

			ESCAPE            = 256,
			ENTER             = 257,
			TAB               = 258,
			BACKSPACE         = 259,
			INSERT            = 260,
			DEL	              = 261,
			RIGHT             = 262,
			LEFT              = 263,
			DOWN              = 264,
			UP                = 265,
			PAGE_UP           = 266,
			PAGE_DOWN         = 267,
			HOME              = 268,
			END               = 269,
			CAPS_LOCK         = 280,
			SCROLL_LOCK       = 281,
			NUM_LOCK          = 282,
			PRINT_SCREEN      = 283,
			PAUSE             = 284,


			F1                = 290,
			F2                = 291,
			F3                = 292,
			F4                = 293,
			F5                = 294,
			F6                = 295,
			F7                = 296,
			F8                = 297,
			F9                = 298,
			F10               = 299,
			F11               = 300,
			F12               = 301,
			F13               = 302,
			F14               = 303,
			F15               = 304,
			F16               = 305,
			F17               = 306,
			F18               = 307,
			F19               = 308,
			F20               = 309,
			F21               = 310,
			F22               = 311,
			F23               = 312,
			F24               = 313,
			F25               = 314,


			KP_0              = 320,
			KP_1              = 321,
			KP_2              = 322,
			KP_3              = 323,
			KP_4              = 324,
			KP_5              = 325,
			KP_6              = 326,
			KP_7              = 327,
			KP_8              = 328,
			KP_9              = 329,


			KP_DECIMAL        = 330,
			KP_DIVIDE         = 331,
			KP_MULTIPLY       = 332,
			KP_SUBTRACT       = 333,
			KP_ADD            = 334,
			KP_ENTER          = 335,
			KP_EQUAL          = 336,
			LEFT_SHIFT        = 340,
			LEFT_CONTROL      = 341,
			LEFT_ALT          = 342,
			LEFT_SUPER        = 343,
			RIGHT_SHIFT       = 344,
			RIGHT_CONTROL     = 345,
			RIGHT_ALT         = 346,
			RIGHT_SUPER       = 347,
			MENU              = 348
		};
	}
}

/* Printable keys */
#define XW_KEY_SPACE              32
#define XW_KEY_APOSTROPHE         39  /* ' */
#define XW_KEY_COMMA              44  /* , */
#define XW_KEY_MINUS              45  /* - */
#define XW_KEY_PERIOD             46  /* . */
#define XW_KEY_SLASH              47  /* / */
#define XW_KEY_0                  48
#define XW_KEY_1                  49
#define XW_KEY_2                  50
#define XW_KEY_3                  51
#define XW_KEY_4                  52
#define XW_KEY_5                  53
#define XW_KEY_6                  54
#define XW_KEY_7                  55
#define XW_KEY_8                  56
#define XW_KEY_9                  57
#define XW_KEY_SEMICOLON          59  /* ; */
#define XW_KEY_EQUAL              61  /* = */
#define XW_KEY_A                  65
#define XW_KEY_B                  66
#define XW_KEY_C                  67
#define XW_KEY_D                  68
#define XW_KEY_E                  69
#define XW_KEY_F                  70
#define XW_KEY_G                  71
#define XW_KEY_H                  72
#define XW_KEY_I                  73
#define XW_KEY_J                  74
#define XW_KEY_K                  75
#define XW_KEY_L                  76
#define XW_KEY_M                  77
#define XW_KEY_N                  78
#define XW_KEY_O                  79
#define XW_KEY_P                  80
#define XW_KEY_Q                  81
#define XW_KEY_R                  82
#define XW_KEY_S                  83
#define XW_KEY_T                  84
#define XW_KEY_U                  85
#define XW_KEY_V                  86
#define XW_KEY_W                  87
#define XW_KEY_X                  88
#define XW_KEY_Y                  89
#define XW_KEY_Z                  90
#define XW_KEY_LEFT_BRACKET       91  /* [ */
#define XW_KEY_BACKSLASH          92  /* \ */
#define XW_KEY_RIGHT_BRACKET      93  /* ] */
#define XW_KEY_GRAVE_ACCENT       96  /* ` */
#define XW_KEY_WORLD_1            161 /* non-US #1 */
#define XW_KEY_WORLD_2            162 /* non-US #2 */

/* Function keys */
#define XW_KEY_ESCAPE             256
#define XW_KEY_ENTER              257
#define XW_KEY_TAB                258
#define XW_KEY_BACKSPACE          259
#define XW_KEY_INSERT             260
#define XW_KEY_DELETE             261
#define XW_KEY_RIGHT              262
#define XW_KEY_LEFT               263
#define XW_KEY_DOWN               264
#define XW_KEY_UP                 265
#define XW_KEY_PAGE_UP            266
#define XW_KEY_PAGE_DOWN          267
#define XW_KEY_HOME               268
#define XW_KEY_END                269
#define XW_KEY_CAPS_LOCK          280
#define XW_KEY_SCROLL_LOCK        281
#define XW_KEY_NUM_LOCK           282
#define XW_KEY_PRINT_SCREEN       283
#define XW_KEY_PAUSE              284
#define XW_KEY_F1                 290
#define XW_KEY_F2                 291
#define XW_KEY_F3                 292
#define XW_KEY_F4                 293
#define XW_KEY_F5                 294
#define XW_KEY_F6                 295
#define XW_KEY_F7                 296
#define XW_KEY_F8                 297
#define XW_KEY_F9                 298
#define XW_KEY_F10                299
#define XW_KEY_F11                300
#define XW_KEY_F12                301
#define XW_KEY_F13                302
#define XW_KEY_F14                303
#define XW_KEY_F15                304
#define XW_KEY_F16                305
#define XW_KEY_F17                306
#define XW_KEY_F18                307
#define XW_KEY_F19                308
#define XW_KEY_F20                309
#define XW_KEY_F21                310
#define XW_KEY_F22                311
#define XW_KEY_F23                312
#define XW_KEY_F24                313
#define XW_KEY_F25                314
#define XW_KEY_KP_0               320
#define XW_KEY_KP_1               321
#define XW_KEY_KP_2               322
#define XW_KEY_KP_3               323
#define XW_KEY_KP_4               324
#define XW_KEY_KP_5               325
#define XW_KEY_KP_6               326
#define XW_KEY_KP_7               327
#define XW_KEY_KP_8               328
#define XW_KEY_KP_9               329
#define XW_KEY_KP_DECIMAL         330
#define XW_KEY_KP_DIVIDE          331
#define XW_KEY_KP_MULTIPLY        332
#define XW_KEY_KP_SUBTRACT        333
#define XW_KEY_KP_ADD             334
#define XW_KEY_KP_ENTER           335
#define XW_KEY_KP_EQUAL           336
#define XW_KEY_LEFT_SHIFT         340
#define XW_KEY_LEFT_CONTROL       341
#define XW_KEY_LEFT_ALT           342
#define XW_KEY_LEFT_SUPER         343
#define XW_KEY_RIGHT_SHIFT        344
#define XW_KEY_RIGHT_CONTROL      345
#define XW_KEY_RIGHT_ALT          346
#define XW_KEY_RIGHT_SUPER        347
#define XW_KEY_MENU               348