/*-
 * SPDX-License-Identifier: BSD-2-Clause-FreeBSD
 *
 * Copyright (c) 2013 The FreeBSD Foundation
 *
 * This software was developed by Aleksandr Rybalko under sponsorship from the
 * FreeBSD Foundation.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <sys/cdefs.h>
__FBSDID("$FreeBSD$");

#include <sys/param.h>
#include <sys/kernel.h>
#include <sys/libkern.h>
#include <sys/fbio.h>

#include <dev/vt/colors/vt_termcolors.h>

static struct {
	unsigned char r;	/* Red percentage value. */
	unsigned char g;	/* Green percentage value. */
	unsigned char b;	/* Blue percentage value. */
} color_def[NCOLORS] = {
	{0,	0,	0},	/* black */
	{50,	0,	0},	/* dark red */
	{0,	50,	0},	/* dark green */
	{77,	63,	0},	/* dark yellow */
	{20,	40,	64},	/* dark blue */
	{50,	0,	50},	/* dark magenta */
	{0,	50,	50},	/* dark cyan */
	{75,	75,	75},	/* light gray */

	{18,	20,	21},	/* dark gray */
	{100,	0,	0},	/* light red */
	{0,	100,	0},	/* light green */
	{100,	100,	0},	/* light yellow */
	{45,	62,	81},	/* light blue */
	{100,	0,	100},	/* light magenta */
	{0,	100,	100},	/* light cyan */
	{100,	100,	100},	/* white */

	{0,	0,	0},	/* 16 */
	{7,	7,	7},	/* 17 */
	{12,	12,	12},	/* 18 */
	{17,	17,	17},	/* 19 */
	{21,	21,	21},	/* 20 */
	{27,	27,	27},	/* 21 */
	{31,	31,	31},	/* 22 */
	{38,	38,	38},	/* 23 */
	{44,	44,	44},	/* 24 */
	{50,	50,	50},	/* 25 */
	{57,	57,	57},	/* 26 */
	{63,	63,	63},	/* 27 */
	{71,	71,	71},	/* 28 */
	{79,	79,	79},	/* 29 */
	{89,	89,	89},	/* 30 */
	{100,	100,	100},	/* 31 */
	{0,	0,	100},	/* 32 */
	{25,	0,	100},	/* 33 */
	{49,	0,	100},	/* 34 */
	{74,	0,	100},	/* 35 */
	{100,	0,	100},	/* 36 */
	{100,	0,	74},	/* 37 */
	{100,	0,	49},	/* 38 */
	{100,	0,	25},	/* 39 */
	{100,	0,	0},	/* 40 */
	{100,	25,	0},	/* 41 */
	{100,	49,	0},	/* 42 */
	{100,	74,	0},	/* 43 */
	{100,	100,	0},	/* 44 */
	{74,	100,	0},	/* 45 */
	{49,	100,	0},	/* 46 */
	{25,	100,	0},	/* 47 */
	{0,	100,	0},	/* 48 */
	{0,	100,	25},	/* 49 */
	{0,	100,	49},	/* 50 */
	{0,	100,	74},	/* 51 */
	{0,	100,	100},	/* 52 */
	{0,	74,	100},	/* 53 */
	{0,	49,	100},	/* 54 */
	{0,	25,	100},	/* 55 */
	{49,	49,	100},	/* 56 */
	{61,	49,	100},	/* 57 */
	{74,	49,	100},	/* 58 */
	{87,	49,	100},	/* 59 */
	{100,	49,	100},	/* 60 */
	{100,	49,	87},	/* 61 */
	{100,	49,	74},	/* 62 */
	{100,	49,	61},	/* 63 */
	{100,	49,	49},	/* 64 */
	{100,	61,	49},	/* 65 */
	{100,	74,	49},	/* 66 */
	{100,	87,	49},	/* 67 */
	{100,	100,	49},	/* 68 */
	{87,	100,	49},	/* 69 */
	{74,	100,	49},	/* 70 */
	{61,	100,	49},	/* 71 */
	{49,	100,	49},	/* 72 */
	{49,	100,	61},	/* 73 */
	{49,	100,	74},	/* 74 */
	{49,	100,	87},	/* 75 */
	{49,	100,	100},	/* 76 */
	{49,	87,	100},	/* 77 */
	{49,	74,	100},	/* 78 */
	{49,	61,	100},	/* 79 */
	{71,	71,	100},	/* 80 */
	{78,	71,	100},	/* 81 */
	{85,	71,	100},	/* 82 */
	{92,	71,	100},	/* 83 */
	{100,	71,	100},	/* 84 */
	{100,	71,	92},	/* 85 */
	{100,	71,	85},	/* 86 */
	{100,	71,	78},	/* 87 */
	{100,	71,	71},	/* 88 */
	{100,	78,	71},	/* 89 */
	{100,	85,	71},	/* 90 */
	{100,	92,	71},	/* 91 */
	{100,	100,	71},	/* 92 */
	{92,	100,	71},	/* 93 */
	{85,	100,	71},	/* 94 */
	{78,	100,	71},	/* 95 */
	{71,	100,	71},	/* 96 */
	{71,	100,	78},	/* 97 */
	{71,	100,	85},	/* 98 */
	{71,	100,	92},	/* 99 */
	{71,	100,	100},	/* 100 */
	{71,	92,	100},	/* 101 */
	{71,	85,	100},	/* 102 */
	{71,	78,	100},	/* 103 */
	{0,	0,	44},	/* 104 */
	{10,	0,	44},	/* 105 */
	{21,	0,	44},	/* 106 */
	{33,	0,	44},	/* 107 */
	{44,	0,	44},	/* 108 */
	{44,	0,	33},	/* 109 */
	{44,	0,	21},	/* 110 */
	{44,	0,	10},	/* 111 */
	{44,	0,	0},	/* 112 */
	{44,	10,	0},	/* 113 */
	{44,	21,	0},	/* 114 */
	{44,	33,	0},	/* 115 */
	{44,	44,	0},	/* 116 */
	{33,	44,	0},	/* 117 */
	{21,	44,	0},	/* 118 */
	{10,	44,	0},	/* 119 */
	{0,	44,	0},	/* 120 */
	{0,	44,	10},	/* 121 */
	{0,	44,	21},	/* 122 */
	{0,	44,	33},	/* 123 */
	{0,	44,	44},	/* 124 */
	{0,	33,	44},	/* 125 */
	{0,	21,	44},	/* 126 */
	{0,	10,	44},	/* 127 */
	{21,	21,	44},	/* 128 */
	{27,	21,	44},	/* 129 */
	{33,	21,	44},	/* 130 */
	{38,	21,	44},	/* 131 */
	{44,	21,	44},	/* 132 */
	{44,	21,	38},	/* 133 */
	{44,	21,	33},	/* 134 */
	{44,	21,	27},	/* 135 */
	{44,	21,	21},	/* 136 */
	{44,	27,	21},	/* 137 */
	{44,	33,	21},	/* 138 */
	{44,	38,	21},	/* 139 */
	{44,	44,	21},	/* 140 */
	{38,	44,	21},	/* 141 */
	{33,	44,	21},	/* 142 */
	{27,	44,	21},	/* 143 */
	{21,	44,	21},	/* 144 */
	{21,	44,	27},	/* 145 */
	{21,	44,	33},	/* 146 */
	{21,	44,	38},	/* 147 */
	{21,	44,	44},	/* 148 */
	{21,	38,	44},	/* 149 */
	{21,	33,	44},	/* 150 */
	{21,	27,	44},	/* 151 */
	{31,	31,	44},	/* 152 */
	{34,	31,	44},	/* 153 */
	{38,	31,	44},	/* 154 */
	{41,	31,	44},	/* 155 */
	{44,	31,	44},	/* 156 */
	{44,	31,	41},	/* 157 */
	{44,	31,	38},	/* 158 */
	{44,	31,	34},	/* 159 */
	{44,	31,	31},	/* 160 */
	{44,	34,	31},	/* 161 */
	{44,	38,	31},	/* 162 */
	{44,	41,	31},	/* 163 */
	{44,	44,	31},	/* 164 */
	{41,	44,	31},	/* 165 */
	{38,	44,	31},	/* 166 */
	{34,	44,	31},	/* 167 */
	{31,	44,	31},	/* 168 */
	{31,	44,	34},	/* 169 */
	{31,	44,	38},	/* 170 */
	{31,	44,	41},	/* 171 */
	{31,	44,	44},	/* 172 */
	{31,	41,	44},	/* 173 */
	{31,	38,	44},	/* 174 */
	{31,	34,	44},	/* 175 */
	{0,	0,	25},	/* 176 */
	{6,	0,	25},	/* 177 */
	{12,	0,	25},	/* 178 */
	{18,	0,	25},	/* 179 */
	{25,	0,	25},	/* 180 */
	{25,	0,	18},	/* 181 */
	{25,	0,	12},	/* 182 */
	{25,	0,	6},	/* 183 */
	{25,	0,	0},	/* 184 */
	{25,	6,	0},	/* 185 */
	{25,	12,	0},	/* 186 */
	{25,	18,	0},	/* 187 */
	{25,	25,	0},	/* 188 */
	{18,	25,	0},	/* 189 */
	{12,	25,	0},	/* 190 */
	{6,	25,	0},	/* 191 */
	{0,	25,	0},	/* 192 */
	{0,	25,	6},	/* 193 */
	{0,	25,	12},	/* 194 */
	{0,	25,	18},	/* 195 */
	{0,	25,	25},	/* 196 */
	{0,	18,	25},	/* 197 */
	{0,	12,	25},	/* 198 */
	{0,	6,	25},	/* 199 */
	{12,	12,	25},	/* 200 */
	{15,	12,	25},	/* 201 */
	{18,	12,	25},	/* 202 */
	{21,	12,	25},	/* 203 */
	{25,	12,	25},	/* 204 */
	{25,	12,	21},	/* 205 */
	{25,	12,	18},	/* 206 */
	{25,	12,	15},	/* 207 */
	{25,	12,	12},	/* 208 */
	{25,	15,	12},	/* 209 */
	{25,	18,	12},	/* 210 */
	{25,	21,	12},	/* 211 */
	{25,	25,	12},	/* 212 */
	{21,	25,	12},	/* 213 */
	{18,	25,	12},	/* 214 */
	{15,	25,	12},	/* 215 */
	{12,	25,	12},	/* 216 */
	{12,	25,	15},	/* 217 */
	{12,	25,	18},	/* 218 */
	{12,	25,	21},	/* 219 */
	{12,	25,	25},	/* 220 */
	{12,	21,	25},	/* 221 */
	{12,	18,	25},	/* 222 */
	{12,	15,	25},	/* 223 */
	{17,	17,	25},	/* 224 */
	{18,	17,	25},	/* 225 */
	{20,	17,	25},	/* 226 */
	{23,	17,	25},	/* 227 */
	{25,	17,	25},	/* 228 */
	{25,	17,	23},	/* 229 */
	{25,	17,	20},	/* 230 */
	{25,	17,	18},	/* 231 */
	{25,	17,	17},	/* 232 */
	{25,	18,	17},	/* 233 */
	{25,	20,	17},	/* 234 */
	{25,	23,	17},	/* 235 */
	{25,	25,	17},	/* 236 */
	{23,	25,	17},	/* 237 */
	{20,	25,	17},	/* 238 */
	{18,	25,	17},	/* 239 */
	{17,	25,	17},	/* 240 */
	{17,	25,	18},	/* 241 */
	{17,	25,	20},	/* 242 */
	{17,	25,	23},	/* 243 */
	{17,	25,	25},	/* 244 */
	{17,	23,	25},	/* 245 */
	{17,	20,	25},	/* 246 */
	{17,	18,	25},	/* 247 */
	{0,	0,	0},	/* 248 */
	{0,	0,	0},	/* 249 */
	{0,	0,	0},	/* 250 */
	{0,	0,	0},	/* 251 */
	{0,	0,	0},	/* 252 */
	{0,	0,	0},	/* 253 */
	{0,	0,	0},	/* 254 */
	{0,	0,	0},	/* 255 */
};

static int
vt_parse_rgb_triplet(const char *rgb, unsigned char *r,
    unsigned char *g, unsigned char *b)
{
	unsigned long v;
	const char *ptr;
	char *endptr;

	ptr = rgb;

	/* Handle #rrggbb case */
	if (*ptr == '#') {
		if (strlen(ptr) != 7)
			return (-1);
		v = strtoul(ptr + 1, &endptr, 16);
		if (*endptr != '\0')
			return (-1);

		*r = (v >> 16) & 0xff;
		*g = (v >>  8) & 0xff;
		*b = v & 0xff;

		return (0);
	}

	/* "r, g, b" case */
	v = strtoul(ptr, &endptr, 10);
	if (ptr == endptr)
		return (-1);
	if (v > 255)
		return (-1);
	*r = v & 0xff;
	ptr = endptr;

	/* skip separator */
	while (*ptr == ',' || *ptr == ' ')
		ptr++;

	v = strtoul(ptr, &endptr, 10);
	if (ptr == endptr)
		return (-1);
	if (v > 255)
		return (-1);
	*g = v & 0xff;
	ptr = endptr;

	/* skip separator */
	while (*ptr == ',' || *ptr == ' ')
		ptr++;

	v = strtoul(ptr, &endptr, 10);
	if (ptr == endptr)
		return (-1);
	if (v > 255)
		return (-1);
	*b = v & 0xff;
	ptr = endptr;

	/* skip trailing spaces */
	while (*ptr == ' ')
		ptr++;

	/* unexpected characters at the end of the string */
	if (*ptr != 0)
		return (-1);

	return (0);
}

static void
vt_palette_init(void)
{
	int i;
	char rgb[32];
	char tunable[32];
	unsigned char r, g, b;

	for (i = 0; i < NCOLORS; i++) {
		snprintf(tunable, sizeof(tunable),
		    "kern.vt.color.%d.rgb", i);
		if (TUNABLE_STR_FETCH(tunable, rgb, sizeof(rgb))) {
			if (vt_parse_rgb_triplet(rgb, &r, &g, &b) == 0) {
				/* convert to percentages */
				color_def[i].r = r*100/255;
				color_def[i].g = g*100/255;
				color_def[i].b = b*100/255;
			}
		}
	}
}

static int
vt_generate_cons_palette(uint32_t *palette, int format, uint32_t rmax,
    int roffset, uint32_t gmax, int goffset, uint32_t bmax, int boffset)
{
	int i;

	switch (format) {
	case COLOR_FORMAT_VGA:
		for (i = 0; i < NCOLORS; i++)
			palette[i] = cons_to_vga_colors[i];
		break;
	case COLOR_FORMAT_RGB:
		vt_palette_init();
#define	CF(_f, _i) ((_f ## max * color_def[(_i)]._f / 100) << _f ## offset)
		for (i = 0; i < NCOLORS; i++)
			palette[i] = CF(r, i) | CF(g, i) | CF(b, i);
#undef	CF
		break;
	default:
		return (ENODEV);
	}

	return (0);
}

int
vt_config_cons_colors(struct fb_info *info, int format, uint32_t rmax,
    int roffset, uint32_t gmax, int goffset, uint32_t bmax, int boffset)
{
	if (format == COLOR_FORMAT_RGB) {
		info->fb_rgboffs.red = roffset;
		info->fb_rgboffs.green = goffset;
		info->fb_rgboffs.blue = boffset;
	} else
		memset(&info->fb_rgboffs, 0, sizeof(info->fb_rgboffs));

	return (vt_generate_cons_palette(info->fb_cmap, format, rmax,
	    roffset, gmax, goffset, bmax, boffset));
}
