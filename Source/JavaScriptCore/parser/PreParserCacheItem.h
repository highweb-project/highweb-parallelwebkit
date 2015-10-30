/*
 * Copyright (C) 2011 Apple Inc. All rights reserved.
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
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. AND ITS CONTRIBUTORS ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL APPLE INC. OR ITS CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef PreParserCacheItem_h
#define PreParserCacheItem_h

#if ENABLE(VMOLAB)
#include <wtf/PassOwnPtr.h>
#include "ParserModes.h"

namespace JSC {

class Identifier;

struct PreParserCacheItemCreationParameters {
    int startOffset;
    int endOffset;
    int firstLine;
    int startColumn; 

    JSParserStrictness strictness;

    const Identifier* name;
};

class PreParserCacheItem {
    WTF_MAKE_FAST_ALLOCATED;
public:
    static std::unique_ptr<PreParserCacheItem> create(const PreParserCacheItemCreationParameters&);
    ~PreParserCacheItem() {}

    void saveItemToFile(FILE*);

    int startOffset;
    int endOffset;
    int firstLine;
    int startColumn; 

    JSParserStrictness strictness;

    Identifier name;

private:
    PreParserCacheItem(const PreParserCacheItemCreationParameters&);

};

inline std::unique_ptr<PreParserCacheItem> PreParserCacheItem::create(const PreParserCacheItemCreationParameters& parameters)
{
    size_t objectSize = sizeof(PreParserCacheItem);
    void* slot = fastMalloc(objectSize);
    return std::unique_ptr<PreParserCacheItem>(new (slot) PreParserCacheItem(parameters));
}

inline PreParserCacheItem::PreParserCacheItem(const PreParserCacheItemCreationParameters& parameters)
    : startOffset(parameters.startOffset)
    , endOffset(parameters.endOffset)
    , firstLine(parameters.firstLine)
    , startColumn(parameters.startColumn)
    , strictness(parameters.strictness)
    , name(*parameters.name)
{
}

inline void PreParserCacheItem::saveItemToFile(FILE* file)
{
    fprintf(file, "%d\n%d\n%d\n%d\n%d\n", startOffset, endOffset, firstLine, startColumn, strictness);
    fprintf(file, "%s\n", name.ascii().data());
}
    
}

#endif // VMOLAB

#endif // PreParserCacheItem_h

