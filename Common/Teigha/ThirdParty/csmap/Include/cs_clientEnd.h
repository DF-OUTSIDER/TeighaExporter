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

/******************************************************************************

	A file with this name is included by "cs_map.h", the main header file
	for the CS-MAP library as the last thing it does.  Use this file to
	modify any of the standard settings used in CS-MAP.

	Below please find several macro definitions which are commented out.
	The definitions indicated are the default definitions used in the
	CS-MAP source code if these macros are left undefined.  If you need
	a definition which is different, please supply the definition in this
	file.

	It is intended that application developers simplay maintain their
	own version of this file, and ignore the distribution copy of this
	file.  In this manner, different environments can be supported without
	resorting to modification of CS-MAP distribution code.

	Macro definitions wehich can be replaced are:

#define _csMalloc(_n)         malloc(_n)
#define _csCalloc(_n,_s)      calloc(_n,_s)
#define _csRealloc(_m,_n)     realloc(_m,_n)
#define _csFree(_m)           {if (_m) {free(_m); _m = 0;}}
#define _csClose(_m)          {if (_m >= 0) {close(_m);}}

******************************************************************************/
