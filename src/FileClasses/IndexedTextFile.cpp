/*
 *  This file is part of Dune Legacy.
 *
 *  Dune Legacy is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  Dune Legacy is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Dune Legacy.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <FileClasses/IndexedTextFile.h>

#include <misc/string_util.h>
#include <misc/exceptions.h>

#include <SDL_endian.h>
#include <stdio.h>
#include <string>
#include <algorithm>

IndexedTextFile::IndexedTextFile(SDL_RWops* rwop, bool bDecode) {

    if(rwop == nullptr) {
        THROW(std::invalid_argument, "IndexedTextFile:IndexedTextFile(): rwop == nullptr!");
    }

    Sint64 endOffset = SDL_RWsize(rwop);
    if(endOffset < 0) {
        THROW(std::runtime_error, "IndexedTextFile:IndexedTextFile(): Cannot determine size of this file!");
    }

	size_t indexedTextFilesize = static_cast<size_t>(endOffset);

    if(indexedTextFilesize < 2) {
        THROW(std::runtime_error, "IndexedTextFile:IndexedTextFile(): No valid indexed textfile: File too small!");
    }

    unsigned char* pFiledata;
    if( (pFiledata = (unsigned char*) malloc(indexedTextFilesize)) == nullptr) {
        throw std::bad_alloc();
    }

    if(SDL_RWread(rwop, pFiledata, indexedTextFilesize, 1) != 1) {
        free(pFiledata);
        THROW(std::runtime_error, "IndexedTextFile:IndexedTextFile(): Reading this indexed textfile failed!");
    }

    int numIndexedStrings = (SDL_SwapLE16(((Uint16*) pFiledata)[0]))/2 - 1;

    Uint16* pIndex = (Uint16*) pFiledata;
    for(int i=0; i <= numIndexedStrings; i++) {
        pIndex[i] = SDL_SwapLE16(pIndex[i]);
    }

    try {
        for(int i=0; i < numIndexedStrings; i++) {
            std::string text((const char*) (pFiledata+pIndex[i]));

            if(bDecode) {
                indexedStrings.push_back(convertCP850ToISO8859_1(decodeString(text)));
            } else {
                indexedStrings.push_back( convertCP850ToISO8859_1(text) );
            }
        }
    } catch(std::exception&) {
        delete [] pFiledata;
        throw;
    }

    free(pFiledata);
}

IndexedTextFile::~IndexedTextFile() {
}
