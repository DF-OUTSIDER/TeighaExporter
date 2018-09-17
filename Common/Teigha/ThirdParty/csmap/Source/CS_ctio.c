/*
 * Copyright (c) 2012, Autodesk, Inc.
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

#include "cs_map.h"
#include "cs_ioUtil.h"

struct cs_Ctdef_* EXP_LVL3 CS_ctdef (Const char *ct_nam)
{
    return CS_ctDefinition(ct_nam);
}

int EXP_LVL3 CS_ctdefAll (struct cs_Ctdef_ **pDefArray[])
{
    return CS_ctDefinitionAll(pDefArray);
}

int EXP_LVL3 CS_ctdel (struct cs_Ctdef_ *ctdef)
{
    return CS_ctDelete(ctdef);
}

int EXP_LVL3 CS_ctupd (Const struct cs_Ctdef_ *ctdef)
{
    return CS_ctUpdate(ctdef);
}

int EXP_LVL3 CS_ctrd (csFILE *strm, struct cs_Ctdef_ *ct_def)
{
    return CS_ctRead(strm, ct_def);
}

int EXP_LVL3 CS_ctwr (csFILE *strm, Const struct cs_Ctdef_ *ct_def)
{
    return CS_ctWrite(strm, ct_def);
}
