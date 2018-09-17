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

#ifndef __CS_IO_UTIL_DIR_ITERATOR_HPP__
#define __CS_IO_UTIL_DIR_ITERATOR_HPP__

#include <string.h>

class CsdDictionaryIterator
{
private:
	CSDictionarySwitch m_dictionarySwitch;
	std::vector<std::pair<char const*, csFILE*> > m_allFiles;
	int m_currentIndex;
	csFILE* (*m_pOpenFunc)(const char* mode);

public:
	CsdDictionaryIterator(csFILE* (*pOpenFunc)(const char* mode))
		: m_currentIndex(-1), m_pOpenFunc(pOpenFunc)
	{
		//the paths we're going to look for CSD files
		//look into the directory first where we're storing the custom CSD files in
		char targetPaths[2][MAXPATH] = { {'\0'}, {'\0' } };
		CS_stncp(targetPaths[0], cs_UserDir, sizeof(targetPaths[0]));
		CS_stncp(targetPaths[1], m_dictionarySwitch.GetCurrentDir(), sizeof(targetPaths[1]));

		//go through all directories we've
		for (size_t i = 0; i < (sizeof(targetPaths) / sizeof(targetPaths[0])); ++i)
		{
			/* Search the file for the requested coordinate system definition. */
			const char* const pTargetPath = targetPaths[i];
			if ('\0' == *pTargetPath)
				continue;

			//switch [cs_dir] and [cs_DirP] to whatever the current target directory is
			if (CS_setdr(pTargetPath, NULL))
			{
				continue;
				//return NULL;
			}

			csFILE* dictionaryFile = this->m_pOpenFunc(_STRM_BINRD);
			if (NULL == dictionaryFile)
				continue;

			char* directoryName = strdup(pTargetPath);
			this->m_allFiles.push_back(std::pair<char const*, csFILE*>(directoryName, dictionaryFile));
		}
	};

	~CsdDictionaryIterator()
	{
		for(std::vector<std::pair<char const*, csFILE*> >::const_iterator fileIterator = this->m_allFiles.begin();
			fileIterator != this->m_allFiles.end(); ++fileIterator)
		{
			free((char*)fileIterator->first); //free the directory name...
			CS_fclose(fileIterator->second); //...and close the file
		}

		this->m_allFiles.clear();
	}

	bool MoveNext()
	{
		if ((m_currentIndex + 1) >= (int)this->m_allFiles.size())
			return false;

		++m_currentIndex;
		return true;
	}

	csFILE *const GetCurrentFile()
	{
		if (this->m_currentIndex < 0 || this->m_currentIndex >= (int) this->m_allFiles.size())
			return NULL;

		return this->m_allFiles[this->m_currentIndex].second;
	}

	char const* GetCurrentDir()
	{
		if (this->m_currentIndex < 0 || this->m_currentIndex >= (int) this->m_allFiles.size())
			return NULL;

		return this->m_allFiles[this->m_currentIndex].first;
	}
};

#endif

