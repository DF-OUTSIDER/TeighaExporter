/*
 * Copyright (c) 2008, Autodesk, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the Autodesk, Inc. nor the names of its
 *       contributors may be used to endorse or promote products derived
 *       from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY Autodesk, Inc. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL Autodesk, Inc. OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/**********************************************************************
**	Defines used to communicate between resource file and compiled
**	.cpp code.
**********************************************************************/

/*	Have we been here before? */

#define ID_ELED_DELSAV                 20123
#define ID_DTSL_HELP                   20124
#define ID_ELSL_HELP                   20125
#define IDC_MPDR_HELP                  20126
#define ID_ELED_HELP                   20127
#define ID_BRWS_HELP                   20128
#define IDD_ELEDITOR                   20129
#define IDD_NEWKEYNM                   20131
#define IDD_DTEDITOR                   20132
#define IDD_DTSL_DIALOG                20144
#define IDD_ELSL_DIALOG                20145
#define IDD_CSTST_DIALOG               20146
#define IDD_MPDR_DIALOG                20147
#define IDD_ABOUT_BOX                  20148
#define IDD_BRWS_CS                    20153
#define IDD_EDCS_IDNT                  20156
#define IDD_EDCS_GENRL                 20157
#define IDD_EDCS_ORGS                  20158
#define IDD_EDCS_PARMS                 20159
#define IDD_EDCS_PARMS2                20160
#define IDD_EDCS_EXTENTS               20162
#define IDD_EDCS_PARMS3                20165
#define IDD_EDCS_PARMS4                20174
#define IDD_GDC_EDIT	               20175
#define IDD_GDC_ENTRY	               20176
#define IDD_GEODETIC_FWD               20177
#define IDD_GEODETIC_INV               20178
#define IDD_GEOCENTRIC                 20179
#define IDD_FTOA_FORMAT                20180

#define IDC_ELED_KEYNAME               21000
#define ID_ELED_NEWCNCL                21001
#define ID_ELED_CLOSE                  21002
#define IDC_ELED_DESCR                 21003
#define IDC_ELED_SOURCE                21004
#define IDC_ELED_EQRAD                 21005
#define IDC_ELED_PLRAD                 21007
#define IDC_ELED_FLTDISP               21008
#define IDC_AGR_VIEW                   21008
#define IDC_ELED_ESQDISP               21009
#define IDC_NEWKEYNAME                 21010
#define IDCHELP                        21011
#define IDC_PRESERVE                   21013
#define IDC_DTED_DELSAV                21014
#define IDC_EDCS_YOFF                  21014
#define IDC_EDCS_PRJ_PRM4              21014
#define IDC_DTED_HELP                  21015
#define IDC_EDCS_MINNZX                21015
#define IDC_EDCS_PRJ_PRM5              21015
#define IDC_DTED_KEYNAME               21016
#define IDC_EDCS_MINNZY                21016
#define IDC_EDCS_PRJ_PRM6              21016
#define IDC_EDCS_MAXY                  21016
#define IDC_DTED_DESCR                 21017
#define IDC_DTED_SOURCE                21018
#define IDC_DTED_284VIA                21020
#define IDC_DTED_DELTAX                21021
#define IDC_DTED_DELTAY                21022
#define IDC_DTED_DELTAZ                21023
#define IDC_DTED_ROTX                  21024
#define IDC_DTED_ROTY                  21025
#define IDC_DTED_ROTZ                  21026
#define IDC_DTED_SCALE                 21027
#define ID_DTED_CLOSE                  21028
#define IDC_DTED_NEWCNCL               21029
#define IDC_BRCS_GROUP                 21050
#define IDC_BRCS_KEYNM                 21051
#define IDC_BRCS_DSCR                  21052
#define IDC_CSID_KEYNAME               21053
#define IDC_BRCS_SOURCE                21053
#define IDC_BRCS_REFTO                 21054
#define IDC_BRCS_UNIT                  21055
#define IDC_DTSL_KEYNAME               21059
#define IDC_DTSL_DESCR                 21060
#define IDC_DTSL_SOURCE                21061
#define IDC_DTSL_ELLIP                 21063
#define IDC_DTSL_TECHNIQUE             21064
#define IDC_ELSL_KEYNAME               21065
#define IDC_ELSL_DESCR                 21066
#define IDC_ELSL_SOURCE                21067
#define IDC_CSGN_REFKYNM               21070
#define IDC_CSGN_DATBTN                21072
#define IDC_CSGN_ELPBTN                21073
#define IDC_CSGN_REFTYP                21077
#define IDC_CSGN_REDRATIO              21078
#define IDC_CSGN_MAPRATIO              21079
#define IDC_DTED_ELIPDSCR              21081
#define IDC_DTED_ELIPBTN               21082
#define IDC_ELSL_NUMBERS               21083
#define IDC_CSID_GROUP                 21084
#define ID_CSTST_CALCULATE             21084
#define IDC_CSID_DESCR                 21085
#define ID_CSTST_REVERSE               21085
#define IDC_CSID_SOURCE                21086
#define IDC_CSTST_SRCKEYNM             21086
#define IDC_CSID_CNTRYST               21087
#define IDC_CSTST_SRCCSBTN             21087
#define IDC_CSID_LOCATN                21088
#define IDC_CSTST_SRCXLAT              21088
#define IDC_CSGN_PRJCTN                21089
#define IDC_CSTST_SRCYLNG              21089
#define IDC_CSTST_SRCGRIDSCL           21090
#define IDC_CSTST_SRCCNVRG             21091
#define IDC_CSTST_TRGKEYNM             21092
#define IDC_CSGN_UNIT                  21093
#define IDC_CSTST_TRGCSBTN             21093
#define IDC_EDCS_ORGLAT                21094
#define IDC_CSTST_TRGXLAT              21094
#define IDC_EDCS_ORGLNG                21095
#define IDC_CSTST_TRGYLNG              21095
#define IDC_EDCS_XOFF                  21096
#define IDC_CSTST_TRGGRIDSCL           21096
#define IDC_EDCS_PP_LBL1               21097
#define IDC_CSTST_TRGCNVRG             21097
#define IDC_EDCS_PP_LBL2               21098
#define IDC_EDCS_PP_LBL3               21099
#define IDC_EDCS_PP_LBL4               21100
#define IDC_EDCS_PP_LBL5               21101
#define IDC_EDCS_PP_LBL6               21102
#define IDC_EDCS_PRJ_PRM1              21103
#define IDC_CSTST_DECIMAL              21103
#define IDC_EDCS_PRJ_PRM2              21104
#define IDC_CSTST_HIGHPREC             21104
#define IDC_EDCS_PRJ_PRM3              21105
#define IDC_EDCS_PP_LBL7               21106
#define IDC_CSTST_SRCDESCR             21106
#define IDC_EDCS_PRJ_PRM7              21107
#define IDC_CSTST_SRCSRC               21107
#define IDC_CSGN_QUAD                  21108
#define IDC_EDCS_PP_LBL8               21108
#define IDC_CSTST_SRCDATLBL            21108
#define IDC_EDCS_PRJ_PRM8              21109
#define IDC_EDCS_MINLAT                21109
#define IDC_CSTST_SRCDATKYNM           21109
#define IDC_EDCS_PP_LBL9               21110
#define IDC_EDCS_MAXLAT                21110
#define IDC_CSTST_SRCUNIT              21110
#define IDC_EDCS_PRJ_PRM9              21111
#define IDC_EDCS_MINX                  21111
#define IDC_CSTST_SRCXLATLBL           21111
#define IDC_EDCS_PP_LBL10              21112
#define IDC_EDCS_MAXX                  21112
#define IDC_CSTST_SRCYLNGLBL           21112
#define IDC_EDCS_PRJ_PRM10             21113
#define IDC_CSTST_TRGDESCR             21113
#define IDC_EDCS_PP_LBL11              21114
#define IDC_CSTST_TRGSRC               21114
#define IDC_EDCS_PRJ_PRM11             21115
#define IDC_CSTST_TRGDATLBL            21115
#define IDC_EDCS_PP_LBL12              21116
#define IDC_CSTST_TRGDTKY              21116
#define IDC_CSGN_MAPSCL                21117
#define IDC_EDCS_PRJ_PRM12             21117
#define IDC_CSTST_TRGUNIT              21117
#define IDC_EDCS_PP_LBL13              21118
#define IDC_CSTST_TRGXLATLBL           21118
#define IDC_EDCS_PRJ_PRM13             21119
#define IDC_CSTST_TRGYLNGLBL           21119
#define IDC_EDCS_PP_LBL14              21120
#define IDC_CSTST_SRCSTAT              21120
#define IDC_EDCS_PRJ_PRM14             21121
#define IDC_CSTST_TRGSTAT              21121
#define IDC_EDCS_PP_LBL15              21122
#define IDC_CSTST_SRCSP83              21122
#define IDC_EDCS_PRJ_PRM15             21123
#define IDC_EDCS_PRJ_PRM19             21123
#define IDC_CSTST_TRGSP83              21123
#define IDC_MPDR_CSNAME                21123
#define IDC_MPDR_DTNAME                21124
#define IDC_MPDR_ELNAME                21125
#define IDC_EDCS_PP_LBL16              21126
#define IDC_EDCS_PRJ_PRM20             21126
#define IDC_MPDR_DIRECTORY             21126
#define IDC_EDCS_PRJ_PRM16             21127
#define IDC_EDCS_PRJ_PRM21             21127
#define IDC_MPDR_SVREGISTRY            21127
#define IDC_EDCS_PP_LBL17              21128
#define IDC_EDCS_PRJ_PRM22             21128
#define IDC_MPDR_SVINI                 21128
#define IDC_EDCS_PRJ_PRM17             21129
#define IDC_EDCS_PRJ_PRM24             21129
#define ID_CSTST_HELP                  21129
#define IDC_EDCS_PP_LBL18              21130
#define IDC_EDCS_PRJ_PRM23             21130
#define IDC_ABT_RELEASE                21130
#define IDC_EDCS_PRJ_PRM18             21131
#define IDC_EDCS_PP_LBL19              21131
#define IDC_ABT_LICNBR                 21131
#define IDC_ABT_LICTO                  21132
#define IDC_EDCS_PP_LBL20              21133
#define IDC_EDCS_PP_LBL21              21134
#define IDC_EDCS_PP_LBL22              21135
#define IDC_EDCS_PP_LBL23              21136
#define IDC_EDCS_PP_LBL24              21137
#define IDC_EDCS_MINLNG                21140
#define IDC_EDCS_MAXLNG                21141
#define IDC_EDCS_MINY                  21142
#define IDC_CSGN_SCLRED                21156
#define IDC_EDCS_PRM1_NOTE             21172
#define IDC_CSGN_SELDSCR               21173
#define IDC_CSOR_SELDSCR               21174
#define IDC_EDCS_SELDSCR               21175
#define IDC_CSPR_SELDSCR               21176
#define IDC_CSID_BRWSE                 21177
#define IDC_CSGN_DEFLT_CHK             21178
#define IDC_UNIT_DEFLT_CHK             21179
#define IDC_EDCS_PRM2_NOTE             21184
#define IDC_CSTST_3DCHECK              21185
#define IDC_CSTST_SRCZHGT              21186
#define IDC_CSTST_TRGZHGT              21187
#define IDC_CSTST_SRCZHGTLBL           21188
#define IDC_CSTST_TRGZHGTLBL           21189

#define IDD_MGTST_DIALOG               21190
#define IDC_MGTST_ELLIPSOID            21191
#define IDC_MGTST_MGRS                 21192
#define IDC_MGTST_MGRSBTN              21193
#define IDC_MGTST_UTMBTN               21194
#define IDC_MGTST_ZONECOMBO            21195
#define IDC_MGTST_LAT                  21196
#define IDC_MGTST_LONG                 21197
#define IDC_MGTST_UTMX                 21198
#define IDC_MGTST_UTMY                 21199
#define IDC_MGTST_LATLNGBTN            21200
#define ID_MGTST_HELP                  21201
#define IDC_MGTST_BESSEL               21202
#define IDC_MGTST_PREC                 21203
#define IDC_MGTST_PRECSPIN             21204
#define IDC_MGTST_ELSELECT             21205

#define IDD_BRWS_CS2                   21210
#define ID_CSBR2_HELP                  21211
#define IDC_APPROXMSG                  21212
#define IDC_CSBR2_SRCGRP               21213
#define IDC_CSBR2_SRCKEYNM             21214
#define IDC_CSBR2_SRCREFLBL            21215
#define IDC_CSBR2_SREFLBL2             21216
#define IDC_CSBR2_TRGGRP               21217
#define IDC_CSBR2_TRGKEYNM             21218
#define IDC_CSBR2_TRGREFLBL            21219
#define IDC_CSBR2_SREFLBL4             21220
#define IDC_CSBR2_SRCDESCR             21221
#define IDC_CSBR2_SRCSRC               21222
#define IDC_CSBR2_SRCREF               21223
#define IDC_CSBR2_SRCUNIT              21224
#define IDC_CSBR2_TRGDESCR             21225
#define IDC_CSBR2_TRGSRC               21226
#define IDC_CSBR2_TRGREF               21227
#define IDC_CSBR2_TRGUNIT              21228

#define IDC_GDC_CATALOGS               21241
#define IDC_GDC_ENTRYLIST              21242
#define IDC_GDC_FALLBACK               21243
#define IDC_GDC_FILENAME               21244
#define IDC_GDC_PATH                   21245
#define IDC_GDC_DIRREL                 21246
#define IDC_GDC_BUFRSIZE               21248
#define IDC_GDC_GRIDCELL               21251
#define IDC_GDC_UPDATE                 21257
#define IDC_GDC_ADD                    21260
#define IDC_GDC_DELETE                 21262
#define IDC_GDC_DTBRWSE                21263
#define IDC_GDC_FILEBRWSE              21264

#define IDC_GDE_FILENAME               21265
#define IDC_GDE_PATH                   21266
#define IDC_GDE_ABSOLUTE               21267
#define IDC_GDE_BUFRSIZE               21268
#define IDC_GDE_DENSITY                21269
#define IDC_GDE_BRWSE                  21270

#define IDC_GDC_BTNUP                  21271
#define IDC_GDC_BTNDOWN                21272
#define IDC_GDC_BTNFIRST               21273
#define IDC_GDC_BTNLAST                21274

#define	IDC_BTN_DIRBRWSE               21275
#define	IDC_KYNM_LEGEND	               21276

#define IDC_EDT_ELLIPSOID              21277
#define IDC_BTN_ELLIPSOID              21278
#define IDC_EDT_SRCLAT                 21279
#define IDC_EDT_GEOLAT                 21280
#define IDC_EDT_SRCLNG                 21281
#define IDC_EDT_GEOLNG                 21282
#define IDC_SRC_AZM                    21283
#define IDC_EDT_GEOHGT                 21284
#define IDC_SRC_DIST                   21285
#define IDC_EDT_CENXXX                 21286
#define IDC_EDT_UNIT                   21287
#define IDC_BTN_UNIT                   21288
#define IDC_EDT_TRGLAT                 21289
#define IDC_EDT_CENYYY                 21290
#define IDC_EDT_TRGLNG                 21291
#define IDC_EDT_CENZZZ                 21292
#define IDC_BTN_CYCLE                  21293
#define IDC_BTN_TOCENTRIC              21294
#define IDC_EDT_FRMLAT                 21295
#define IDC_BTN_TOGEOGRAPHIC           21296
#define IDC_EDT_FRMLNG                 21297
#define IDC_EDT_TOLAT                  21298
#define IDC_CHECK4                     21299
#define IDC_EDT_TOLNG                  21300
#define IDC_RADIO1                     21301
#define IDC_EDT_TRGDIST                21302
#define IDC_RADIO2                     21303
#define IDC_EDT_TRGAZM                 21304
#define IDC_RADIO3                     21305
#define IDC_CHECK1                     21306
#define IDC_RADIO4                     21307
#define IDC_RADIO5                     21308
#define IDC_RADIO6                     21309

#define IDC_FTOA_ISEAST                21310
#define IDC_FTOA_ISNORTH               21311
#define IDC_FTOA_MODE360               21312
#define IDC_FTOA_EXPONENTIAL           21313
#define IDC_FTOA_3DIGITS               21314
#define IDC_FTOA_LEADSEC0              21315
#define IDC_FTOA_LEADMINSEC0           21316
#define IDC_FTOA_PRECISION             21317
#define IDC_FTOA_PRECAUTO              21318
#define IDC_FTOA_PRECSPIN              21319
#define IDC_FTOA_DECDEG                21320
#define IDC_FTOA_DEGMIN                21321
#define IDC_FTOA_DEGMINSEC             21322
#define IDC_FTOA_NOCOMMA               21323
#define IDC_FTOA_COMMAS                21324
#define IDC_FTOA_RATIO                 21325
#define IDC_FTOA_DIRCHR                21326
#define IDC_FTOA_LEADMIN0              21327
#define IDC_FTOA_LEADDEG0              21328
#define IDC_STAT_ELHGT                 21329
