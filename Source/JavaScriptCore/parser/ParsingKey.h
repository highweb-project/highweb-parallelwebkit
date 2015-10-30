/*
 * Copyright (C) 2013, 2014 Apple Inc. All rights reserved.
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
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
 */

#ifndef ParsingKey_h
#define ParsingKey_h

#if ENABLE(VMOLAB)

#include "ParserModes.h"
#include <wtf/HashMap.h>

namespace JSC {

class ParsingKey {
public:
    ParsingKey()
        : m_provider(0)
        , m_startOffset(0)
    {
    }
    
    ParsingKey(WTF::HashTableDeletedValueType)
        : m_provider(0)
        , m_startOffset(0)
    {
    }
    
    ParsingKey(SourceProvider* provider, int startOffset)
        : m_provider(provider)
        , m_startOffset(startOffset)
    {
    }
    
    bool operator!() const
    {
        return !m_provider && !m_startOffset;
    }
    
    bool isHashTableDeletedValue() const
    {
        return !m_provider && m_startOffset;
    }
    
    SourceProvider* provider() const { return m_provider; }
    int startOffset() const { return m_startOffset; }
    
    bool operator==(const ParsingKey& other) const
    {
        return m_provider == other.m_provider
            && m_startOffset == other.m_startOffset;
    }
    
    unsigned hash() const
    {
        return WTF::pairIntHash(WTF::PtrHash<SourceProvider*>::hash(m_provider), m_startOffset);
    }
    

private:
    SourceProvider* m_provider;
    int m_startOffset;
};

struct ParsingKeyHash {
    static unsigned hash(const ParsingKey& key) { return key.hash(); }
    static bool equal(const ParsingKey& a, const ParsingKey& b) { return a == b; }
    static const bool safeToCompareToEmptyOrDeleted = true;
};

}  // namespace JSC

namespace WTF {

template<typename T> struct DefaultHash;
template<> struct DefaultHash<JSC::ParsingKey> {
    typedef JSC::ParsingKeyHash Hash;
};

template<typename T> struct HashTraits;
template<> struct HashTraits<JSC::ParsingKey> : SimpleClassHashTraits<JSC::ParsingKey> { };

} // namespace WTF

#endif // ENABLE(VMOLAB)

#endif // ParsingKey_h

