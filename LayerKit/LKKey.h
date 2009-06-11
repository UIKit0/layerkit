/*  
 * Copyright (C) 2007 University of South Australia
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 * 
 * Contributors: 
 *   Andrew Cunningham <andrewcunningham@mac.com>
 */

#ifndef LKKey_h
#define LKKey_h

typedef enum {
	/* The keyboard syms have been cleverly chosen to map to ASCII */
	LKKEY_UNKNOWN	= 0,
	LKKEY_FIRST		= 0,
	LKKEY_BACKSPACE	= 8,
	LKKEY_TAB		= 9,
	LKKEY_CLEAR		= 12,
	LKKEY_RETURN	= 13,
	LKKEY_PAUSE		= 19,
	LKKEY_ESCAPE	= 27,
	LKKEY_SPACE		= 32,
	LKKEY_EXCLAIM	= 33,
	LKKEY_QUOTEDBL	= 34,
	LKKEY_HASH		= 35,
	LKKEY_DOLLAR	= 36,
	LKKEY_AMPERSAND	= 38,
	LKKEY_QUOTE		= 39,
	LKKEY_LEFTPAREN	= 40,
	LKKEY_RIGHTPAREN= 41,
	LKKEY_ASTERISK	= 42,
	LKKEY_PLUS		= 43,
	LKKEY_COMMA		= 44,
	LKKEY_MINUS		= 45,
	LKKEY_PERIOD	= 46,
	LKKEY_SLASH		= 47,
	LKKEY_0			= 48,
	LKKEY_1			= 49,
	LKKEY_2			= 50,
	LKKEY_3			= 51,
	LKKEY_4			= 52,
	LKKEY_5			= 53,
	LKKEY_6			= 54,
	LKKEY_7			= 55,
	LKKEY_8			= 56,
	LKKEY_9			= 57,
	LKKEY_COLON		= 58,
	LKKEY_SEMICOLON	= 59,
	LKKEY_LESS		= 60,
	LKKEY_EQUALS	= 61,
	LKKEY_GREATER	= 62,
	LKKEY_QUESTION	= 63,
	LKKEY_AT		= 64,
	/* 
	   Skip uppercase letters
	 */
	LKKEY_LEFTBRACKET= 91,
	LKKEY_BACKSLASH	= 92,
	LKKEY_RIGHTBRACKET	= 93,
	LKKEY_CARET		= 94,
	LKKEY_UNDERSCORE= 95,
	LKKEY_BACKQUOTE	= 96,
	LKKEY_a			= 97,
	LKKEY_b			= 98,
	LKKEY_c			= 99,
	LKKEY_d			= 100,
	LKKEY_e			= 101,
	LKKEY_f			= 102,
	LKKEY_g			= 103,
	LKKEY_h			= 104,
	LKKEY_i			= 105,
	LKKEY_j			= 106,
	LKKEY_k			= 107,
	LKKEY_l			= 108,
	LKKEY_m			= 109,
	LKKEY_n			= 110,
	LKKEY_o			= 111,
	LKKEY_p			= 112,
	LKKEY_q			= 113,
	LKKEY_r			= 114,
	LKKEY_s			= 115,
	LKKEY_t			= 116,
	LKKEY_u			= 117,
	LKKEY_v			= 118,
	LKKEY_w			= 119,
	LKKEY_x			= 120,
	LKKEY_y			= 121,
	LKKEY_z			= 122,
	LKKEY_DELETE		= 127,
	/* End of ASCII mapped keysyms */

	/* International keyboard syms */
	LKKEY_WORLD_0		= 160,		/* 0xA0 */
	LKKEY_WORLD_1		= 161,
	LKKEY_WORLD_2		= 162,
	LKKEY_WORLD_3		= 163,
	LKKEY_WORLD_4		= 164,
	LKKEY_WORLD_5		= 165,
	LKKEY_WORLD_6		= 166,
	LKKEY_WORLD_7		= 167,
	LKKEY_WORLD_8		= 168,
	LKKEY_WORLD_9		= 169,
	LKKEY_WORLD_10		= 170,
	LKKEY_WORLD_11		= 171,
	LKKEY_WORLD_12		= 172,
	LKKEY_WORLD_13		= 173,
	LKKEY_WORLD_14		= 174,
	LKKEY_WORLD_15		= 175,
	LKKEY_WORLD_16		= 176,
	LKKEY_WORLD_17		= 177,
	LKKEY_WORLD_18		= 178,
	LKKEY_WORLD_19		= 179,
	LKKEY_WORLD_20		= 180,
	LKKEY_WORLD_21		= 181,
	LKKEY_WORLD_22		= 182,
	LKKEY_WORLD_23		= 183,
	LKKEY_WORLD_24		= 184,
	LKKEY_WORLD_25		= 185,
	LKKEY_WORLD_26		= 186,
	LKKEY_WORLD_27		= 187,
	LKKEY_WORLD_28		= 188,
	LKKEY_WORLD_29		= 189,
	LKKEY_WORLD_30		= 190,
	LKKEY_WORLD_31		= 191,
	LKKEY_WORLD_32		= 192,
	LKKEY_WORLD_33		= 193,
	LKKEY_WORLD_34		= 194,
	LKKEY_WORLD_35		= 195,
	LKKEY_WORLD_36		= 196,
	LKKEY_WORLD_37		= 197,
	LKKEY_WORLD_38		= 198,
	LKKEY_WORLD_39		= 199,
	LKKEY_WORLD_40		= 200,
	LKKEY_WORLD_41		= 201,
	LKKEY_WORLD_42		= 202,
	LKKEY_WORLD_43		= 203,
	LKKEY_WORLD_44		= 204,
	LKKEY_WORLD_45		= 205,
	LKKEY_WORLD_46		= 206,
	LKKEY_WORLD_47		= 207,
	LKKEY_WORLD_48		= 208,
	LKKEY_WORLD_49		= 209,
	LKKEY_WORLD_50		= 210,
	LKKEY_WORLD_51		= 211,
	LKKEY_WORLD_52		= 212,
	LKKEY_WORLD_53		= 213,
	LKKEY_WORLD_54		= 214,
	LKKEY_WORLD_55		= 215,
	LKKEY_WORLD_56		= 216,
	LKKEY_WORLD_57		= 217,
	LKKEY_WORLD_58		= 218,
	LKKEY_WORLD_59		= 219,
	LKKEY_WORLD_60		= 220,
	LKKEY_WORLD_61		= 221,
	LKKEY_WORLD_62		= 222,
	LKKEY_WORLD_63		= 223,
	LKKEY_WORLD_64		= 224,
	LKKEY_WORLD_65		= 225,
	LKKEY_WORLD_66		= 226,
	LKKEY_WORLD_67		= 227,
	LKKEY_WORLD_68		= 228,
	LKKEY_WORLD_69		= 229,
	LKKEY_WORLD_70		= 230,
	LKKEY_WORLD_71		= 231,
	LKKEY_WORLD_72		= 232,
	LKKEY_WORLD_73		= 233,
	LKKEY_WORLD_74		= 234,
	LKKEY_WORLD_75		= 235,
	LKKEY_WORLD_76		= 236,
	LKKEY_WORLD_77		= 237,
	LKKEY_WORLD_78		= 238,
	LKKEY_WORLD_79		= 239,
	LKKEY_WORLD_80		= 240,
	LKKEY_WORLD_81		= 241,
	LKKEY_WORLD_82		= 242,
	LKKEY_WORLD_83		= 243,
	LKKEY_WORLD_84		= 244,
	LKKEY_WORLD_85		= 245,
	LKKEY_WORLD_86		= 246,
	LKKEY_WORLD_87		= 247,
	LKKEY_WORLD_88		= 248,
	LKKEY_WORLD_89		= 249,
	LKKEY_WORLD_90		= 250,
	LKKEY_WORLD_91		= 251,
	LKKEY_WORLD_92		= 252,
	LKKEY_WORLD_93		= 253,
	LKKEY_WORLD_94		= 254,
	LKKEY_WORLD_95		= 255,		/* 0xFF */

	/* Numeric keypad */
	LKKEY_KP0		= 256,
	LKKEY_KP1		= 257,
	LKKEY_KP2		= 258,
	LKKEY_KP3		= 259,
	LKKEY_KP4		= 260,
	LKKEY_KP5		= 261,
	LKKEY_KP6		= 262,
	LKKEY_KP7		= 263,
	LKKEY_KP8		= 264,
	LKKEY_KP9		= 265,
	LKKEY_KP_PERIOD		= 266,
	LKKEY_KP_DIVIDE		= 267,
	LKKEY_KP_MULTIPLY	= 268,
	LKKEY_KP_MINUS		= 269,
	LKKEY_KP_PLUS		= 270,
	LKKEY_KP_ENTER		= 271,
	LKKEY_KP_EQUALS		= 272,

	/* Arrows + Home/End pad */
	LKKEY_UP			= 273,
	LKKEY_DOWN		= 274,
	LKKEY_RIGHT		= 275,
	LKKEY_LEFT		= 276,
	LKKEY_INSERT		= 277,
	LKKEY_HOME		= 278,
	LKKEY_END		= 279,
	LKKEY_PAGEUP		= 280,
	LKKEY_PAGEDOWN		= 281,

	/* Function keys */
	LKKEY_F1			= 282,
	LKKEY_F2			= 283,
	LKKEY_F3			= 284,
	LKKEY_F4			= 285,
	LKKEY_F5			= 286,
	LKKEY_F6			= 287,
	LKKEY_F7			= 288,
	LKKEY_F8			= 289,
	LKKEY_F9			= 290,
	LKKEY_F10		= 291,
	LKKEY_F11		= 292,
	LKKEY_F12		= 293,
	LKKEY_F13		= 294,
	LKKEY_F14		= 295,
	LKKEY_F15		= 296,

	/* Key state modifier keys */
	LKKEY_NUMLOCK		= 300,
	LKKEY_CAPSLOCK		= 301,
	LKKEY_SCROLLOCK		= 302,
	LKKEY_RSHIFT		= 303,
	LKKEY_LSHIFT		= 304,
	LKKEY_RCTRL		= 305,
	LKKEY_LCTRL		= 306,
	LKKEY_RALT		= 307,
	LKKEY_LALT		= 308,
	LKKEY_RMETA		= 309,
	LKKEY_LMETA		= 310,
	LKKEY_LSUPER		= 311,		/* Left "Windows" key */
	LKKEY_RSUPER		= 312,		/* Right "Windows" key */
	LKKEY_MODE		= 313,		/* "Alt Gr" key */
	LKKEY_COMPOSE		= 314,		/* Multi-key compose key */

	/* Miscellaneous function keys */
	LKKEY_HELP		= 315,
	LKKEY_PRINT		= 316,
	LKKEY_SYSREQ		= 317,
	LKKEY_BREAK		= 318,
	LKKEY_MENU		= 319,
	LKKEY_POWER		= 320,		/* Power Macintosh power key */
	LKKEY_EURO		= 321,		/* Some european keyboards */
	LKKEY_UNDO		= 322,		/* Atari keyboard has Undo */

	/* Add any other keys here */

	LKKEY_LAST
} LKKey;

#endif