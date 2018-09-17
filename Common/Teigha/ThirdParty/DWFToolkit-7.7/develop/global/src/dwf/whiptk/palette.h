//
//  Copyright (c) 1996-2006 by Autodesk, Inc.
//
//  By using this code, you are agreeing to the terms and conditions of
//  the License Agreement included in the documentation for this code.
//
//  AUTODESK MAKES NO WARRANTIES, EXPRESS OR IMPLIED, AS TO THE CORRECTNESS
//  OF THIS CODE OR ANY DERIVATIVE WORKS WHICH INCORPORATE IT. AUTODESK
//  PROVIDES THE CODE ON AN "AS-IS" BASIS AND EXPLICITLY DISCLAIMS ANY
//  LIABILITY, INCLUDING CONSEQUENTIAL AND INCIDENTAL DAMAGES FOR ERRORS,
//  OMISSIONS, AND OTHER PROBLEMS IN THE CODE.
//
//  Use, duplication, or disclosure by the U.S. Government is subject to
//  restrictions set forth in FAR 52.227-19 (Commercial Computer Software
//  Restricted Rights) and DFAR 252.227-7013(c)(1)(ii) (Rights in Technical
//  Data and Computer Software), as applicable.
//

///
///\file        dwf/whiptk/palette.h
///

#if !defined PALETTE_HEADER
#define PALETTE_HEADER

extern const WT_RGBA32 WD_New_Default_Palette[256] = {
   WT_RGBA32(0,0,0, WD_Opaque)         /* 0 */
  ,WT_RGBA32(128,0,0, WD_Opaque)       /* 1 */
  ,WT_RGBA32(0,128,0, WD_Opaque)       /* 2 */
  ,WT_RGBA32(128,128,0, WD_Opaque)     /* 3 */
  ,WT_RGBA32(0,0,128, WD_Opaque)       /* 4 */
  ,WT_RGBA32(128,0,128, WD_Opaque)     /* 5 */
  ,WT_RGBA32(0,128,128, WD_Opaque)     /* 6 */
  ,WT_RGBA32(192,192,192, WD_Opaque)   /* 7   */
  ,WT_RGBA32(192,220,192, WD_Opaque)   /* 8   */
  ,WT_RGBA32(166,202,240, WD_Opaque)   /* 9   */
  ,WT_RGBA32(0,0,0, WD_Opaque)         /* 10  */
  ,WT_RGBA32(0,0,51, WD_Opaque)        /* 11  */
  ,WT_RGBA32(0,0,102, WD_Opaque)       /* 12  */
  ,WT_RGBA32(0,0,153, WD_Opaque)       /* 13  */
  ,WT_RGBA32(0,0,204, WD_Opaque)       /* 14  */
  ,WT_RGBA32(0,0,255, WD_Opaque)       /* 15  */
  ,WT_RGBA32(0,51,0, WD_Opaque)        /* 16  */
  ,WT_RGBA32(0,51,51, WD_Opaque)       /* 17  */
  ,WT_RGBA32(0,51,102, WD_Opaque)      /* 18  */
  ,WT_RGBA32(0,51,153, WD_Opaque)      /* 19  */
  ,WT_RGBA32(0,51,204, WD_Opaque)      /* 20  */
  ,WT_RGBA32(0,51,255, WD_Opaque)      /* 21  */
  ,WT_RGBA32(0,102,0, WD_Opaque)       /* 22  */
  ,WT_RGBA32(0,102,51, WD_Opaque)      /* 23  */
  ,WT_RGBA32(0,102,102, WD_Opaque)     /* 24  */
  ,WT_RGBA32(0,102,153, WD_Opaque)     /* 25  */
  ,WT_RGBA32(0,102,204, WD_Opaque)     /* 26  */
  ,WT_RGBA32(0,102,255, WD_Opaque)     /* 27  */
  ,WT_RGBA32(0,153,0, WD_Opaque)       /* 28  */
  ,WT_RGBA32(0,153,51, WD_Opaque)      /* 29  */
  ,WT_RGBA32(0,153,102, WD_Opaque)     /* 30  */
  ,WT_RGBA32(0,153,153, WD_Opaque)     /* 31  */
  ,WT_RGBA32(0,153,204, WD_Opaque)     /* 32  */
  ,WT_RGBA32(0,153,255, WD_Opaque)     /* 33  */
  ,WT_RGBA32(0,204,0, WD_Opaque)       /* 34  */
  ,WT_RGBA32(0,204,51, WD_Opaque)      /* 35  */
  ,WT_RGBA32(0,204,102, WD_Opaque)     /* 36  */
  ,WT_RGBA32(0,204,153, WD_Opaque)     /* 37  */
  ,WT_RGBA32(0,204,204, WD_Opaque)     /* 38  */
  ,WT_RGBA32(0,204,255, WD_Opaque)     /* 39  */
  ,WT_RGBA32(0,255,0, WD_Opaque)       /* 40  */
  ,WT_RGBA32(0,255,51, WD_Opaque)      /* 41  */
  ,WT_RGBA32(0,255,102, WD_Opaque)     /* 42  */
  ,WT_RGBA32(0,255,153, WD_Opaque)     /* 43  */
  ,WT_RGBA32(0,255,204, WD_Opaque)     /* 44  */
  ,WT_RGBA32(0,255,255, WD_Opaque)     /* 45  */
  ,WT_RGBA32(51,0,0, WD_Opaque)        /* 46  */
  ,WT_RGBA32(51,0,51, WD_Opaque)       /* 47  */
  ,WT_RGBA32(51,0,102, WD_Opaque)      /* 48  */
  ,WT_RGBA32(51,0,153, WD_Opaque)      /* 49  */
  ,WT_RGBA32(51,0,204, WD_Opaque)      /* 50  */
  ,WT_RGBA32(51,0,255, WD_Opaque)      /* 51  */
  ,WT_RGBA32(51,51,0, WD_Opaque)       /* 52  */
  ,WT_RGBA32(51,51,51, WD_Opaque)      /* 53  */
  ,WT_RGBA32(51,51,102, WD_Opaque)     /* 54  */
  ,WT_RGBA32(51,51,153, WD_Opaque)     /* 55  */
  ,WT_RGBA32(51,51,204, WD_Opaque)     /* 56  */
  ,WT_RGBA32(51,51,255, WD_Opaque)     /* 57  */
  ,WT_RGBA32(51,102,0, WD_Opaque)      /* 58  */
  ,WT_RGBA32(51,102,51, WD_Opaque)     /* 59  */
  ,WT_RGBA32(51,102,102, WD_Opaque)    /* 60  */
  ,WT_RGBA32(51,102,153, WD_Opaque)    /* 61  */
  ,WT_RGBA32(51,102,204, WD_Opaque)    /* 62  */
  ,WT_RGBA32(51,102,255, WD_Opaque)    /* 63  */
  ,WT_RGBA32(51,153,0, WD_Opaque)      /* 64  */
  ,WT_RGBA32(51,153,51, WD_Opaque)     /* 65  */
  ,WT_RGBA32(51,153,102, WD_Opaque)    /* 66  */
  ,WT_RGBA32(51,153,153, WD_Opaque)    /* 67  */
  ,WT_RGBA32(51,153,204, WD_Opaque)    /* 68  */
  ,WT_RGBA32(51,153,255, WD_Opaque)    /* 69  */
  ,WT_RGBA32(51,204,0, WD_Opaque)      /* 70  */
  ,WT_RGBA32(51,204,51, WD_Opaque)     /* 71  */
  ,WT_RGBA32(51,204,102, WD_Opaque)    /* 72  */
  ,WT_RGBA32(51,204,153, WD_Opaque)    /* 73  */
  ,WT_RGBA32(51,204,204, WD_Opaque)    /* 74  */
  ,WT_RGBA32(51,204,255, WD_Opaque)    /* 75  */
  ,WT_RGBA32(51,255,0, WD_Opaque)      /* 76  */
  ,WT_RGBA32(51,255,51, WD_Opaque)     /* 77  */
  ,WT_RGBA32(51,255,102, WD_Opaque)    /* 78  */
  ,WT_RGBA32(51,255,153, WD_Opaque)    /* 79  */
  ,WT_RGBA32(51,255,204, WD_Opaque)    /* 80  */
  ,WT_RGBA32(51,255,255, WD_Opaque)    /* 81  */
  ,WT_RGBA32(102,0,0, WD_Opaque)       /* 82  */
  ,WT_RGBA32(102,0,51, WD_Opaque)      /* 83  */
  ,WT_RGBA32(102,0,102, WD_Opaque)     /* 84  */
  ,WT_RGBA32(102,0,153, WD_Opaque)     /* 85  */
  ,WT_RGBA32(102,0,204, WD_Opaque)     /* 86  */
  ,WT_RGBA32(102,0,255, WD_Opaque)     /* 87  */
  ,WT_RGBA32(102,51,0, WD_Opaque)      /* 88  */
  ,WT_RGBA32(102,51,51, WD_Opaque)     /* 89  */
  ,WT_RGBA32(102,51,102, WD_Opaque)    /* 90  */
  ,WT_RGBA32(102,51,153, WD_Opaque)    /* 91  */
  ,WT_RGBA32(102,51,204, WD_Opaque)    /* 92  */
  ,WT_RGBA32(102,51,255, WD_Opaque)    /* 93  */
  ,WT_RGBA32(102,102,0, WD_Opaque)     /* 94  */
  ,WT_RGBA32(102,102,51, WD_Opaque)    /* 95  */
  ,WT_RGBA32(102,102,102, WD_Opaque)   /* 96  */
  ,WT_RGBA32(102,102,153, WD_Opaque)   /* 97  */
  ,WT_RGBA32(102,102,204, WD_Opaque)   /* 98  */
  ,WT_RGBA32(102,102,255, WD_Opaque)   /* 99  */
  ,WT_RGBA32(102,153,0, WD_Opaque)     /* 100 */
  ,WT_RGBA32(102,153,51, WD_Opaque)    /* 101 */
  ,WT_RGBA32(102,153,102, WD_Opaque)   /* 102 */
  ,WT_RGBA32(102,153,153, WD_Opaque)   /* 103 */
  ,WT_RGBA32(102,153,204, WD_Opaque)   /* 104 */
  ,WT_RGBA32(102,153,255, WD_Opaque)   /* 105 */
  ,WT_RGBA32(102,204,0, WD_Opaque)     /* 106 */
  ,WT_RGBA32(102,204,51, WD_Opaque)    /* 107 */
  ,WT_RGBA32(102,204,102, WD_Opaque)   /* 108 */
  ,WT_RGBA32(102,204,153, WD_Opaque)   /* 109 */
  ,WT_RGBA32(102,204,204, WD_Opaque)   /* 110 */
  ,WT_RGBA32(102,204,255, WD_Opaque)   /* 111 */
  ,WT_RGBA32(102,255,0, WD_Opaque)     /* 112 */
  ,WT_RGBA32(102,255,51, WD_Opaque)    /* 113 */
  ,WT_RGBA32(102,255,102, WD_Opaque)   /* 114 */
  ,WT_RGBA32(102,255,153, WD_Opaque)   /* 115 */
  ,WT_RGBA32(102,255,204, WD_Opaque)   /* 116 */
  ,WT_RGBA32(102,255,255, WD_Opaque)   /* 117 */
  ,WT_RGBA32(153,0,0, WD_Opaque)       /* 118 */
  ,WT_RGBA32(153,0,51, WD_Opaque)      /* 119 */
  ,WT_RGBA32(153,0,102, WD_Opaque)     /* 120 */
  ,WT_RGBA32(153,0,153, WD_Opaque)     /* 121 */
  ,WT_RGBA32(153,0,204, WD_Opaque)     /* 122 */
  ,WT_RGBA32(153,0,255, WD_Opaque)     /* 123 */
  ,WT_RGBA32(153,51,0, WD_Opaque)      /* 124 */
  ,WT_RGBA32(153,51,51, WD_Opaque)     /* 125 */
  ,WT_RGBA32(153,51,102, WD_Opaque)    /* 126 */
  ,WT_RGBA32(153,51,153, WD_Opaque)    /* 127 */
  ,WT_RGBA32(153,51,204, WD_Opaque)    /* 128 */
  ,WT_RGBA32(153,51,255, WD_Opaque)    /* 129 */
  ,WT_RGBA32(153,102,0, WD_Opaque)     /* 130 */
  ,WT_RGBA32(153,102,51, WD_Opaque)    /* 131 */
  ,WT_RGBA32(153,102,102, WD_Opaque)   /* 132 */
  ,WT_RGBA32(153,102,153, WD_Opaque)   /* 133 */
  ,WT_RGBA32(153,102,204, WD_Opaque)   /* 134 */
  ,WT_RGBA32(153,102,255, WD_Opaque)   /* 135 */
  ,WT_RGBA32(153,153,0, WD_Opaque)     /* 136 */
  ,WT_RGBA32(153,153,51, WD_Opaque)    /* 137 */
  ,WT_RGBA32(153,153,102, WD_Opaque)   /* 138 */
  ,WT_RGBA32(153,153,153, WD_Opaque)   /* 139 */
  ,WT_RGBA32(153,153,204, WD_Opaque)   /* 140 */
  ,WT_RGBA32(153,153,255, WD_Opaque)   /* 141 */
  ,WT_RGBA32(153,204,0, WD_Opaque)     /* 142 */
  ,WT_RGBA32(153,204,51, WD_Opaque)    /* 143 */
  ,WT_RGBA32(153,204,102, WD_Opaque)   /* 144 */
  ,WT_RGBA32(153,204,153, WD_Opaque)   /* 145 */
  ,WT_RGBA32(153,204,204, WD_Opaque)   /* 146 */
  ,WT_RGBA32(153,204,255, WD_Opaque)   /* 147 */
  ,WT_RGBA32(153,255,0, WD_Opaque)     /* 148 */
  ,WT_RGBA32(153,255,51, WD_Opaque)    /* 149 */
  ,WT_RGBA32(153,255,102, WD_Opaque)   /* 150 */
  ,WT_RGBA32(153,255,153, WD_Opaque)   /* 151 */
  ,WT_RGBA32(153,255,204, WD_Opaque)   /* 152 */
  ,WT_RGBA32(153,255,255, WD_Opaque)   /* 153 */
  ,WT_RGBA32(204,0,0, WD_Opaque)       /* 154 */
  ,WT_RGBA32(204,0,51, WD_Opaque)      /* 155 */
  ,WT_RGBA32(204,0,102, WD_Opaque)     /* 156 */
  ,WT_RGBA32(204,0,153, WD_Opaque)     /* 157 */
  ,WT_RGBA32(204,0,204, WD_Opaque)     /* 158 */
  ,WT_RGBA32(204,0,255, WD_Opaque)     /* 159 */
  ,WT_RGBA32(204,51,0, WD_Opaque)      /* 160 */
  ,WT_RGBA32(204,51,51, WD_Opaque)     /* 161 */
  ,WT_RGBA32(204,51,102, WD_Opaque)    /* 162 */
  ,WT_RGBA32(204,51,153, WD_Opaque)    /* 163 */
  ,WT_RGBA32(204,51,204, WD_Opaque)    /* 164 */
  ,WT_RGBA32(204,51,255, WD_Opaque)    /* 165 */
  ,WT_RGBA32(204,102,0, WD_Opaque)     /* 166 */
  ,WT_RGBA32(204,102,51, WD_Opaque)    /* 167 */
  ,WT_RGBA32(204,102,102, WD_Opaque)   /* 168 */
  ,WT_RGBA32(204,102,153, WD_Opaque)   /* 169 */
  ,WT_RGBA32(204,102,204, WD_Opaque)   /* 170 */
  ,WT_RGBA32(204,102,255, WD_Opaque)   /* 171 */
  ,WT_RGBA32(204,153,0, WD_Opaque)     /* 172 */
  ,WT_RGBA32(204,153,51, WD_Opaque)    /* 173 */
  ,WT_RGBA32(204,153,102, WD_Opaque)   /* 174 */
  ,WT_RGBA32(204,153,153, WD_Opaque)   /* 175 */
  ,WT_RGBA32(204,153,204, WD_Opaque)   /* 176 */
  ,WT_RGBA32(204,153,255, WD_Opaque)   /* 177 */
  ,WT_RGBA32(204,204,0, WD_Opaque)     /* 178 */
  ,WT_RGBA32(204,204,51, WD_Opaque)    /* 179 */
  ,WT_RGBA32(204,204,102, WD_Opaque)   /* 180 */
  ,WT_RGBA32(204,204,153, WD_Opaque)   /* 181 */
  ,WT_RGBA32(204,204,204, WD_Opaque)   /* 182 */
  ,WT_RGBA32(204,204,255, WD_Opaque)   /* 183 */
  ,WT_RGBA32(204,255,0, WD_Opaque)     /* 184 */
  ,WT_RGBA32(204,255,51, WD_Opaque)    /* 185 */
  ,WT_RGBA32(204,255,102, WD_Opaque)   /* 186 */
  ,WT_RGBA32(204,255,153, WD_Opaque)   /* 187 */
  ,WT_RGBA32(204,255,204, WD_Opaque)   /* 188 */
  ,WT_RGBA32(204,255,255, WD_Opaque)   /* 189 */
  ,WT_RGBA32(255,0,0, WD_Opaque)       /* 190 */
  ,WT_RGBA32(255,0,51, WD_Opaque)      /* 191 */
  ,WT_RGBA32(255,0,102, WD_Opaque)     /* 192 */
  ,WT_RGBA32(255,0,153, WD_Opaque)     /* 193 */
  ,WT_RGBA32(255,0,204, WD_Opaque)     /* 194 */
  ,WT_RGBA32(255,0,255, WD_Opaque)     /* 195 */
  ,WT_RGBA32(255,51,0, WD_Opaque)      /* 196 */
  ,WT_RGBA32(255,51,51, WD_Opaque)     /* 197 */
  ,WT_RGBA32(255,51,102, WD_Opaque)    /* 198 */
  ,WT_RGBA32(255,51,153, WD_Opaque)    /* 199 */
  ,WT_RGBA32(255,51,204, WD_Opaque)    /* 200 */
  ,WT_RGBA32(255,51,255, WD_Opaque)    /* 201 */
  ,WT_RGBA32(255,102,0, WD_Opaque)     /* 202 */
  ,WT_RGBA32(255,102,51, WD_Opaque)    /* 203 */
  ,WT_RGBA32(255,102,102, WD_Opaque)   /* 204 */
  ,WT_RGBA32(255,102,153, WD_Opaque)   /* 205 */
  ,WT_RGBA32(255,102,204, WD_Opaque)   /* 206 */
  ,WT_RGBA32(255,102,255, WD_Opaque)   /* 207 */
  ,WT_RGBA32(255,153,0, WD_Opaque)     /* 208 */
  ,WT_RGBA32(255,153,51, WD_Opaque)    /* 209 */
  ,WT_RGBA32(255,153,102, WD_Opaque)   /* 210 */
  ,WT_RGBA32(255,153,153, WD_Opaque)   /* 211 */
  ,WT_RGBA32(255,153,204, WD_Opaque)   /* 212 */
  ,WT_RGBA32(255,153,255, WD_Opaque)   /* 213 */
  ,WT_RGBA32(255,204,0, WD_Opaque)     /* 214 */
  ,WT_RGBA32(255,204,51, WD_Opaque)    /* 215 */
  ,WT_RGBA32(255,204,102, WD_Opaque)   /* 216 */
  ,WT_RGBA32(255,204,153, WD_Opaque)   /* 217 */
  ,WT_RGBA32(255,204,204, WD_Opaque)   /* 218 */
  ,WT_RGBA32(255,204,255, WD_Opaque)   /* 219 */
  ,WT_RGBA32(255,255,0, WD_Opaque)     /* 220 */
  ,WT_RGBA32(255,255,51, WD_Opaque)    /* 221 */
  ,WT_RGBA32(255,255,102, WD_Opaque)   /* 222 */
  ,WT_RGBA32(255,255,153, WD_Opaque)   /* 223 */
  ,WT_RGBA32(255,255,204, WD_Opaque)   /* 224 */
  ,WT_RGBA32(255,255,255, WD_Opaque)   /* 225 */
  ,WT_RGBA32(0,0,0, WD_Opaque)         /* 226 */
  ,WT_RGBA32(13,13,13, WD_Opaque)      /* 227 */
  ,WT_RGBA32(26,26,26, WD_Opaque)      /* 228 */
  ,WT_RGBA32(40,40,40, WD_Opaque)      /* 229 */
  ,WT_RGBA32(53,53,53, WD_Opaque)      /* 230 */
  ,WT_RGBA32(67,67,67, WD_Opaque)      /* 231 */
  ,WT_RGBA32(80,80,80, WD_Opaque)      /* 232 */
  ,WT_RGBA32(93,93,93, WD_Opaque)      /* 233 */
  ,WT_RGBA32(107,107,107, WD_Opaque)   /* 234 */
  ,WT_RGBA32(120,120,120, WD_Opaque)   /* 235 */
  ,WT_RGBA32(134,134,134, WD_Opaque)   /* 236 */
  ,WT_RGBA32(147,147,147, WD_Opaque)   /* 237 */
  ,WT_RGBA32(161,161,161, WD_Opaque)   /* 238 */
  ,WT_RGBA32(174,174,174, WD_Opaque)   /* 239 */
  ,WT_RGBA32(187,187,187, WD_Opaque)   /* 240 */
  ,WT_RGBA32(201,201,201, WD_Opaque)   /* 241 */
  ,WT_RGBA32(214,214,214, WD_Opaque)   /* 242 */
  ,WT_RGBA32(228,228,228, WD_Opaque)   /* 243 */
  ,WT_RGBA32(241,241,241, WD_Opaque)   /* 244 */
  ,WT_RGBA32(255,255,255, WD_Opaque)   /* 245 */
  ,WT_RGBA32(255,251,240, WD_Opaque)   /* 246 */
  ,WT_RGBA32(160,160,164, WD_Opaque)   /* 247 */
  ,WT_RGBA32(128,128,128, WD_Opaque)   /* 248 */
  ,WT_RGBA32(255,0,0, WD_Opaque)       /* 249 */
  ,WT_RGBA32(0,255,0, WD_Opaque)       /* 250 */
  ,WT_RGBA32(255,255,0, WD_Opaque)     /* 251 */
  ,WT_RGBA32(0,0,255, WD_Opaque)       /* 252 */
  ,WT_RGBA32(255,0,255, WD_Opaque)     /* 253 */
  ,WT_RGBA32(0,255,255, WD_Opaque)     /* 254 */
  ,WT_RGBA32(255,255,255, WD_Opaque)   /* 255 */
 };


extern const WT_RGBA32 WD_Old_Default_Palette[256] = {
  WT_RGBA32(0  , 0  , 0,    WD_Opaque),                    /* 0 */
  WT_RGBA32(255, 0  , 0,    WD_Opaque),                    /* 1 */
  WT_RGBA32(255, 255, 0,    WD_Opaque),                    /* 2 */
  WT_RGBA32(0  , 255, 0,    WD_Opaque),                    /* 3 */
  WT_RGBA32(0  , 255, 255,  WD_Opaque),                    /* 4 */
  WT_RGBA32(0  , 0  , 255,  WD_Opaque),                    /* 5 */
  WT_RGBA32(255, 0  , 255,  WD_Opaque),                    /* 6 */
  WT_RGBA32(0  , 0  , 0  ,  WD_Opaque),                    /* 7   */
  WT_RGBA32(128, 128, 128,  WD_Opaque),                    /* 8   */
  WT_RGBA32(220, 220, 220,  WD_Opaque),                    /* 9   */
  WT_RGBA32(255, 0  , 0  ,  WD_Opaque),                    /* 10  */
  WT_RGBA32(255, 127, 127,  WD_Opaque),                    /* 11  */
  WT_RGBA32(221, 0  , 0  ,  WD_Opaque),                    /* 12  */
  WT_RGBA32(221, 110, 110,  WD_Opaque),                    /* 13  */
  WT_RGBA32(184, 0  , 0  ,  WD_Opaque),                    /* 14  */
  WT_RGBA32(184, 92 , 92 ,  WD_Opaque),                    /* 15  */
  WT_RGBA32(149, 0  , 0  ,  WD_Opaque),                    /* 16  */
  WT_RGBA32(149, 74 , 74 ,  WD_Opaque),                    /* 17  */
  WT_RGBA32(114, 0  , 0  ,  WD_Opaque),                    /* 18  */
  WT_RGBA32(114, 57 , 57 ,  WD_Opaque),                    /* 19  */
  WT_RGBA32(255, 63 , 0  ,  WD_Opaque),                    /* 20  */
  WT_RGBA32(255, 159, 127,  WD_Opaque),                    /* 21  */
  WT_RGBA32(221, 55 , 0  ,  WD_Opaque),                    /* 22  */
  WT_RGBA32(221, 138, 110,  WD_Opaque),                    /* 23  */
  WT_RGBA32(184, 46 , 0  ,  WD_Opaque),                    /* 24  */
  WT_RGBA32(184, 115, 92 ,  WD_Opaque),                    /* 25  */
  WT_RGBA32(149, 37 , 0  ,  WD_Opaque),                    /* 26  */
  WT_RGBA32(149, 93 , 74 ,  WD_Opaque),                    /* 27  */
  WT_RGBA32(114, 28 , 0  ,  WD_Opaque),                    /* 28  */
  WT_RGBA32(114, 71 , 57 ,  WD_Opaque),                    /* 29  */
  WT_RGBA32(255, 127, 0  ,  WD_Opaque),                    /* 30  */
  WT_RGBA32(255, 191, 127,  WD_Opaque),                    /* 31  */
  WT_RGBA32(221, 110, 0  ,  WD_Opaque),                    /* 32  */
  WT_RGBA32(221, 165, 110,  WD_Opaque),                    /* 33  */
  WT_RGBA32(184, 92 , 0  ,  WD_Opaque),                    /* 34  */
  WT_RGBA32(184, 138, 92 ,  WD_Opaque),                    /* 35  */
  WT_RGBA32(149, 74 , 0  ,  WD_Opaque),                    /* 36  */
  WT_RGBA32(149, 112, 74 ,  WD_Opaque),                    /* 37  */
  WT_RGBA32(114, 57 , 0  ,  WD_Opaque),                    /* 38  */
  WT_RGBA32(114, 86 , 57 ,  WD_Opaque),                    /* 39  */
  WT_RGBA32(255, 191, 0  ,  WD_Opaque),                    /* 40  */
  WT_RGBA32(255, 223, 127,  WD_Opaque),                    /* 41  */
  WT_RGBA32(221, 165, 0  ,  WD_Opaque),                    /* 42  */
  WT_RGBA32(221, 193, 110,  WD_Opaque),                    /* 43  */
  WT_RGBA32(184, 138, 0  ,  WD_Opaque),                    /* 44  */
  WT_RGBA32(184, 161, 92 ,  WD_Opaque),                    /* 45  */
  WT_RGBA32(149, 112, 0  ,  WD_Opaque),                    /* 46  */
  WT_RGBA32(149, 131, 74 ,  WD_Opaque),                    /* 47  */
  WT_RGBA32(114, 86 , 0  ,  WD_Opaque),                    /* 48  */
  WT_RGBA32(114, 100, 57 ,  WD_Opaque),                    /* 49  */
  WT_RGBA32(255, 255, 0  ,  WD_Opaque),                    /* 50  */
  WT_RGBA32(255, 255, 127,  WD_Opaque),                    /* 51  */
  WT_RGBA32(221, 221, 0  ,  WD_Opaque),                    /* 52  */
  WT_RGBA32(221, 221, 110,  WD_Opaque),                    /* 53  */
  WT_RGBA32(184, 184, 0  ,  WD_Opaque),                    /* 54  */
  WT_RGBA32(184, 184, 92 ,  WD_Opaque),                    /* 55  */
  WT_RGBA32(149, 149, 0  ,  WD_Opaque),                    /* 56  */
  WT_RGBA32(149, 149, 74 ,  WD_Opaque),                    /* 57  */
  WT_RGBA32(114, 114, 0  ,  WD_Opaque),                    /* 58  */
  WT_RGBA32(114, 114, 57 ,  WD_Opaque),                    /* 59  */
  WT_RGBA32(191, 255, 0  ,  WD_Opaque),                    /* 60  */
  WT_RGBA32(223, 255, 127,  WD_Opaque),                    /* 61  */
  WT_RGBA32(165, 221, 0  ,  WD_Opaque),                    /* 62  */
  WT_RGBA32(193, 221, 110,  WD_Opaque),                    /* 63  */
  WT_RGBA32(138, 184, 0  ,  WD_Opaque),                    /* 64  */
  WT_RGBA32(161, 184, 92 ,  WD_Opaque),                    /* 65  */
  WT_RGBA32(112, 149, 0  ,  WD_Opaque),                    /* 66  */
  WT_RGBA32(131, 149, 74 ,  WD_Opaque),                    /* 67  */
  WT_RGBA32(86 , 114, 0  ,  WD_Opaque),                    /* 68  */
  WT_RGBA32(100, 114, 57 ,  WD_Opaque),                    /* 69  */
  WT_RGBA32(127, 255, 0  ,  WD_Opaque),                    /* 70  */
  WT_RGBA32(191, 255, 127,  WD_Opaque),                    /* 71  */
  WT_RGBA32(110, 221, 0  ,  WD_Opaque),                    /* 72  */
  WT_RGBA32(165, 221, 110,  WD_Opaque),                    /* 73  */
  WT_RGBA32(92 , 184, 0  ,  WD_Opaque),                    /* 74  */
  WT_RGBA32(138, 184, 92 ,  WD_Opaque),                    /* 75  */
  WT_RGBA32(74 , 149, 0  ,  WD_Opaque),                    /* 76  */
  WT_RGBA32(112, 149, 74 ,  WD_Opaque),                    /* 77  */
  WT_RGBA32(57 , 114, 0  ,  WD_Opaque),                    /* 78  */
  WT_RGBA32(86 , 114, 57 ,  WD_Opaque),                    /* 79  */
  WT_RGBA32(63 , 255, 0  ,  WD_Opaque),                    /* 80  */
  WT_RGBA32(159, 255, 127,  WD_Opaque),                    /* 81  */
  WT_RGBA32(55 , 221, 0  ,  WD_Opaque),                    /* 82  */
  WT_RGBA32(138, 221, 110,  WD_Opaque),                    /* 83  */
  WT_RGBA32(46 , 184, 0  ,  WD_Opaque),                    /* 84  */
  WT_RGBA32(115, 184, 92 ,  WD_Opaque),                    /* 85  */
  WT_RGBA32(37 , 149, 0  ,  WD_Opaque),                    /* 86  */
  WT_RGBA32(93 , 149, 74 ,  WD_Opaque),                    /* 87  */
  WT_RGBA32(28 , 114, 0  ,  WD_Opaque),                    /* 88  */
  WT_RGBA32(71 , 114, 57 ,  WD_Opaque),                    /* 89  */
  WT_RGBA32(0  , 255, 0  ,  WD_Opaque),                    /* 90  */
  WT_RGBA32(127, 255, 127,  WD_Opaque),                    /* 91  */
  WT_RGBA32(0  , 221, 0  ,  WD_Opaque),                    /* 92  */
  WT_RGBA32(110, 221, 110,  WD_Opaque),                    /* 93  */
  WT_RGBA32(0  , 184, 0  ,  WD_Opaque),                    /* 94  */
  WT_RGBA32(92 , 184, 92 ,  WD_Opaque),                    /* 95  */
  WT_RGBA32(0  , 149, 0  ,  WD_Opaque),                    /* 96  */
  WT_RGBA32(74 , 149, 74 ,  WD_Opaque),                    /* 97  */
  WT_RGBA32(0  , 114, 0  ,  WD_Opaque),                    /* 98  */
  WT_RGBA32(57 , 114, 57 ,  WD_Opaque),                    /* 99  */
  WT_RGBA32(0  , 255, 63 ,  WD_Opaque),                    /* 100 */
  WT_RGBA32(127, 255, 159,  WD_Opaque),                    /* 101 */
  WT_RGBA32(0  , 221, 55 ,  WD_Opaque),                    /* 102 */
  WT_RGBA32(110, 221, 138,  WD_Opaque),                    /* 103 */
  WT_RGBA32(0  , 184, 46 ,  WD_Opaque),                    /* 104 */
  WT_RGBA32(92 , 184, 115,  WD_Opaque),                    /* 105 */
  WT_RGBA32(0  , 149, 37 ,  WD_Opaque),                    /* 106 */
  WT_RGBA32(74 , 149, 93 ,  WD_Opaque),                    /* 107 */
  WT_RGBA32(0  , 114, 28 ,  WD_Opaque),                    /* 108 */
  WT_RGBA32(57 , 114, 71 ,  WD_Opaque),                    /* 109 */
  WT_RGBA32(0  , 255, 127,  WD_Opaque),                    /* 110 */
  WT_RGBA32(127, 255, 191,  WD_Opaque),                    /* 111 */
  WT_RGBA32(0  , 221, 110,  WD_Opaque),                    /* 112 */
  WT_RGBA32(110, 221, 165,  WD_Opaque),                    /* 113 */
  WT_RGBA32(0  , 184, 92 ,  WD_Opaque),                    /* 114 */
  WT_RGBA32(92 , 184, 138,  WD_Opaque),                    /* 115 */
  WT_RGBA32(0  , 149, 74 ,  WD_Opaque),                    /* 116 */
  WT_RGBA32(74 , 149, 112,  WD_Opaque),                    /* 117 */
  WT_RGBA32(0  , 114, 57 ,  WD_Opaque),                    /* 118 */
  WT_RGBA32(57 , 114, 86 ,  WD_Opaque),                    /* 119 */
  WT_RGBA32(0  , 255, 191,  WD_Opaque),                    /* 120 */
  WT_RGBA32(127, 255, 223,  WD_Opaque),                    /* 121 */
  WT_RGBA32(0  , 221, 165,  WD_Opaque),                    /* 122 */
  WT_RGBA32(110, 221, 193,  WD_Opaque),                    /* 123 */
  WT_RGBA32(0  , 184, 138,  WD_Opaque),                    /* 124 */
  WT_RGBA32(92 , 184, 161,  WD_Opaque),                    /* 125 */
  WT_RGBA32(0  , 149, 112,  WD_Opaque),                    /* 126 */
  WT_RGBA32(74 , 149, 131,  WD_Opaque),                    /* 127 */
  WT_RGBA32(0  , 114, 86 ,  WD_Opaque),                    /* 128 */
  WT_RGBA32(57 , 114, 100,  WD_Opaque),                    /* 129 */
  WT_RGBA32(0  , 255, 255,  WD_Opaque),                    /* 130 */
  WT_RGBA32(127, 255, 255,  WD_Opaque),                    /* 131 */
  WT_RGBA32(0  , 221, 221,  WD_Opaque),                    /* 132 */
  WT_RGBA32(110, 221, 221,  WD_Opaque),                    /* 133 */
  WT_RGBA32(0  , 184, 184,  WD_Opaque),                    /* 134 */
  WT_RGBA32(92 , 184, 184,  WD_Opaque),                    /* 135 */
  WT_RGBA32(0  , 149, 149,  WD_Opaque),                    /* 136 */
  WT_RGBA32(74 , 149, 149,  WD_Opaque),                    /* 137 */
  WT_RGBA32(0  , 114, 114,  WD_Opaque),                    /* 138 */
  WT_RGBA32(57 , 114, 114,  WD_Opaque),                    /* 139 */
  WT_RGBA32(0  , 191, 255,  WD_Opaque),                    /* 140 */
  WT_RGBA32(127, 223, 255,  WD_Opaque),                    /* 141 */
  WT_RGBA32(0  , 165, 221,  WD_Opaque),                    /* 142 */
  WT_RGBA32(110, 193, 221,  WD_Opaque),                    /* 143 */
  WT_RGBA32(0  , 138, 184,  WD_Opaque),                    /* 144 */
  WT_RGBA32(92 , 161, 184,  WD_Opaque),                    /* 145 */
  WT_RGBA32(0  , 112, 149,  WD_Opaque),                    /* 146 */
  WT_RGBA32(74 , 131, 149,  WD_Opaque),                    /* 147 */
  WT_RGBA32(0  , 86 , 114,  WD_Opaque),                    /* 148 */
  WT_RGBA32(57 , 100, 114,  WD_Opaque),                    /* 149 */
  WT_RGBA32(0  , 127, 255,  WD_Opaque),                    /* 150 */
  WT_RGBA32(127, 191, 255,  WD_Opaque),                    /* 151 */
  WT_RGBA32(0  , 110, 221,  WD_Opaque),                    /* 152 */
  WT_RGBA32(110, 165, 221,  WD_Opaque),                    /* 153 */
  WT_RGBA32(0  , 92 , 184,  WD_Opaque),                    /* 154 */
  WT_RGBA32(92 , 138, 184,  WD_Opaque),                    /* 155 */
  WT_RGBA32(0  , 74 , 149,  WD_Opaque),                    /* 156 */
  WT_RGBA32(74 , 112, 149,  WD_Opaque),                    /* 157 */
  WT_RGBA32(0  , 57 , 114,  WD_Opaque),                    /* 158 */
  WT_RGBA32(57 , 86 , 114,  WD_Opaque),                    /* 159 */
  WT_RGBA32(0  , 63 , 255,  WD_Opaque),                    /* 160 */
  WT_RGBA32(127, 159, 255,  WD_Opaque),                    /* 161 */
  WT_RGBA32(0  , 55 , 221,  WD_Opaque),                    /* 162 */
  WT_RGBA32(110, 138, 221,  WD_Opaque),                    /* 163 */
  WT_RGBA32(0  , 46 , 184,  WD_Opaque),                    /* 164 */
  WT_RGBA32(92 , 115, 184,  WD_Opaque),                    /* 165 */
  WT_RGBA32(0  , 37 , 149,  WD_Opaque),                    /* 166 */
  WT_RGBA32(74 , 93 , 149,  WD_Opaque),                    /* 167 */
  WT_RGBA32(0  , 28 , 114,  WD_Opaque),                    /* 168 */
  WT_RGBA32(57 , 71 , 114,  WD_Opaque),                    /* 169 */
  WT_RGBA32(0  , 0  , 255,  WD_Opaque),                    /* 170 */
  WT_RGBA32(127, 127, 255,  WD_Opaque),                    /* 171 */
  WT_RGBA32(0  , 0  , 221,  WD_Opaque),                    /* 172 */
  WT_RGBA32(110, 110, 221,  WD_Opaque),                    /* 173 */
  WT_RGBA32(0  , 0  , 184,  WD_Opaque),                    /* 174 */
  WT_RGBA32(92 , 92 , 184,  WD_Opaque),                    /* 175 */
  WT_RGBA32(0  , 0  , 149,  WD_Opaque),                    /* 176 */
  WT_RGBA32(74 , 74 , 149,  WD_Opaque),                    /* 177 */
  WT_RGBA32(0  , 0  , 114,  WD_Opaque),                    /* 178 */
  WT_RGBA32(57 , 57 , 114,  WD_Opaque),                    /* 179 */
  WT_RGBA32(63 , 0  , 255,  WD_Opaque),                    /* 180 */
  WT_RGBA32(159, 127, 255,  WD_Opaque),                    /* 181 */
  WT_RGBA32(55 , 0  , 221,  WD_Opaque),                    /* 182 */
  WT_RGBA32(138, 110, 221,  WD_Opaque),                    /* 183 */
  WT_RGBA32(46 , 0  , 184,  WD_Opaque),                    /* 184 */
  WT_RGBA32(115, 92 , 184,  WD_Opaque),                    /* 185 */
  WT_RGBA32(37 , 0  , 149,  WD_Opaque),                    /* 186 */
  WT_RGBA32(93 , 74 , 149,  WD_Opaque),                    /* 187 */
  WT_RGBA32(28 , 0  , 114,  WD_Opaque),                    /* 188 */
  WT_RGBA32(71 , 57 , 114,  WD_Opaque),                    /* 189 */
  WT_RGBA32(127, 0  , 255,  WD_Opaque),                    /* 190 */
  WT_RGBA32(191, 127, 255,  WD_Opaque),                    /* 191 */
  WT_RGBA32(110, 0  , 221,  WD_Opaque),                    /* 192 */
  WT_RGBA32(165, 110, 221,  WD_Opaque),                    /* 193 */
  WT_RGBA32(92 , 0  , 184,  WD_Opaque),                    /* 194 */
  WT_RGBA32(138, 92 , 184,  WD_Opaque),                    /* 195 */
  WT_RGBA32(74 , 0  , 149,  WD_Opaque),                    /* 196 */
  WT_RGBA32(112, 74 , 149,  WD_Opaque),                    /* 197 */
  WT_RGBA32(57 , 0  , 114,  WD_Opaque),                    /* 198 */
  WT_RGBA32(86 , 57 , 114,  WD_Opaque),                    /* 199 */
  WT_RGBA32(191, 0  , 255,  WD_Opaque),                    /* 200 */
  WT_RGBA32(223, 127, 255,  WD_Opaque),                    /* 201 */
  WT_RGBA32(165, 0  , 221,  WD_Opaque),                    /* 202 */
  WT_RGBA32(193, 110, 221,  WD_Opaque),                    /* 203 */
  WT_RGBA32(138, 0  , 184,  WD_Opaque),                    /* 204 */
  WT_RGBA32(161, 92 , 184,  WD_Opaque),                    /* 205 */
  WT_RGBA32(112, 0  , 149,  WD_Opaque),                    /* 206 */
  WT_RGBA32(131, 74 , 149,  WD_Opaque),                    /* 207 */
  WT_RGBA32(86 , 0  , 114,  WD_Opaque),                    /* 208 */
  WT_RGBA32(100, 57 , 114,  WD_Opaque),                    /* 209 */
  WT_RGBA32(255, 0  , 255,  WD_Opaque),                    /* 210 */
  WT_RGBA32(255, 127, 255,  WD_Opaque),                    /* 211 */
  WT_RGBA32(221, 0  , 221,  WD_Opaque),                    /* 212 */
  WT_RGBA32(221, 110, 221,  WD_Opaque),                    /* 213 */
  WT_RGBA32(184, 0  , 184,  WD_Opaque),                    /* 214 */
  WT_RGBA32(184, 92 , 184,  WD_Opaque),                    /* 215 */
  WT_RGBA32(149, 0  , 149,  WD_Opaque),                    /* 216 */
  WT_RGBA32(149, 74 , 149,  WD_Opaque),                    /* 217 */
  WT_RGBA32(114, 0  , 114,  WD_Opaque),                    /* 218 */
  WT_RGBA32(114, 57 , 114,  WD_Opaque),                    /* 219 */
  WT_RGBA32(255, 0  , 191,  WD_Opaque),                    /* 220 */
  WT_RGBA32(255, 127, 223,  WD_Opaque),                    /* 221 */
  WT_RGBA32(221, 0  , 165,  WD_Opaque),                    /* 222 */
  WT_RGBA32(221, 110, 193,  WD_Opaque),                    /* 223 */
  WT_RGBA32(184, 0  , 138,  WD_Opaque),                    /* 224 */
  WT_RGBA32(184, 92 , 161,  WD_Opaque),                    /* 225 */
  WT_RGBA32(149, 0  , 112,  WD_Opaque),                    /* 226 */
  WT_RGBA32(149, 74 , 131,  WD_Opaque),                    /* 227 */
  WT_RGBA32(114, 0  , 86 ,  WD_Opaque),                    /* 228 */
  WT_RGBA32(114, 57 , 100,  WD_Opaque),                    /* 229 */
  WT_RGBA32(255, 0  , 127,  WD_Opaque),                    /* 230 */
  WT_RGBA32(255, 127, 191,  WD_Opaque),                    /* 231 */
  WT_RGBA32(221, 0  , 110,  WD_Opaque),                    /* 232 */
  WT_RGBA32(221, 110, 165,  WD_Opaque),                    /* 233 */
  WT_RGBA32(184, 0  , 92 ,  WD_Opaque),                    /* 234 */
  WT_RGBA32(184, 92 , 138,  WD_Opaque),                    /* 235 */
  WT_RGBA32(149, 0  , 74 ,  WD_Opaque),                    /* 236 */
  WT_RGBA32(149, 74 , 112,  WD_Opaque),                    /* 237 */
  WT_RGBA32(114, 0  , 57 ,  WD_Opaque),                    /* 238 */
  WT_RGBA32(114, 57 , 86 ,  WD_Opaque),                    /* 239 */
  WT_RGBA32(255, 0  , 63 ,  WD_Opaque),                    /* 240 */
  WT_RGBA32(255, 127, 159,  WD_Opaque),                    /* 241 */
  WT_RGBA32(221, 0  , 55 ,  WD_Opaque),                    /* 242 */
  WT_RGBA32(221, 110, 138,  WD_Opaque),                    /* 243 */
  WT_RGBA32(184, 0  , 46 ,  WD_Opaque),                    /* 244 */
  WT_RGBA32(184, 92 , 115,  WD_Opaque),                    /* 245 */
  WT_RGBA32(149, 0  , 37 ,  WD_Opaque),                    /* 246 */
  WT_RGBA32(149, 74 , 93 ,  WD_Opaque),                    /* 247 */
  WT_RGBA32(114, 0  , 28 ,  WD_Opaque),                    /* 248 */
  WT_RGBA32(114, 57 , 71 ,  WD_Opaque),                    /* 249 */
  WT_RGBA32(84 , 84 , 84 ,  WD_Opaque),                    /* 250 */
  WT_RGBA32(118, 118, 118,  WD_Opaque),                    /* 251 */
  WT_RGBA32(152, 152, 152,  WD_Opaque),                    /* 252 */
  WT_RGBA32(186, 186, 186,  WD_Opaque),                    /* 253 */
  WT_RGBA32(220, 220, 220,  WD_Opaque),                    /* 254 */
  WT_RGBA32(255, 255, 255,  WD_Opaque)                     /* 255 */
 };

#endif // PALETTE_HEADER
