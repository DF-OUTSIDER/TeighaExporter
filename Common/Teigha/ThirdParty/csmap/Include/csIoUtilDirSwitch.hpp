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

#ifndef __CS_IO_UTIL_DIR_SWITCH_HPP__
#define __CS_IO_UTIL_DIR_SWITCH_HPP__

class CSDictionarySwitch
{
private:
	char m_currentDir[MAXPATH];

	bool m_bSwitched;
	bool m_bCanSwitch;
	bool m_bInitialized;

private:
	int DoDirSwitch(char* targetDir)
	{
		if (!this->m_bInitialized)
			return -1;

		return CS_setdr(targetDir, NULL);
	}

public:
	CSDictionarySwitch()
		: m_bSwitched(false), m_bCanSwitch(false), m_bInitialized(false)
	{
		memset(this->m_currentDir, '\0', sizeof(this->m_currentDir) / sizeof(char));
		
		this->m_bInitialized = (CS_getdr(this->m_currentDir) >= 0);
		if (this->m_bInitialized)
			this->m_bCanSwitch = ('\0' != cs_UserDir[0]);
	}

	bool CanSwitchToUserDir() const
	{
		return this->m_bCanSwitch;
	}

	const char* const GetCurrentDir() const
	{
		return this->m_currentDir;
	}

	int SwitchToUserDir()
	{
		extern char cs_UserDir[];
		if (!this->m_bCanSwitch)
			return -2;

		return this->DoDirSwitch(cs_UserDir);
	}

	int SwitchToDefaultDir()
	{
		return this->DoDirSwitch(m_currentDir);
	}

	~CSDictionarySwitch()
	{
		if (!this->m_bInitialized)
			return;

		//there's not so much we can here;
		//after all, [CS_setdr] just copies strings around;
		/* int result = */CS_setdr(this->m_currentDir, NULL);
	}
};

#endif
