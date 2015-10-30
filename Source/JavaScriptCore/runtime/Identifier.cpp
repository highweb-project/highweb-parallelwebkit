/*
 *  Copyright (C) 2003, 2004, 2005, 2006, 2007, 2008, 2012 Apple Inc. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public License
 *  along with this library; see the file COPYING.LIB.  If not, write to
 *  the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 *  Boston, MA 02110-1301, USA.
 *
 */

#include "config.h"
#include "Identifier.h"

#include "CallFrame.h"
#include "JSObject.h"
#include "JSScope.h"
#include "NumericStrings.h"
#include "JSCInlines.h"
#include <new>
#include <string.h>
#include <wtf/Assertions.h>
#include <wtf/FastMalloc.h>
#include <wtf/HashSet.h>
#include <wtf/text/ASCIIFastPath.h>
#include <wtf/text/StringHash.h>

using WTF::ThreadSpecific;

namespace JSC {

#if ENABLE(VMOLAB)
    /*
struct IdentifierASCIIStringTranslator {
    static unsigned hash(const LChar* c)
    {   
        return StringHasher::computeHashAndMaskTop8Bits(c);
    }   

    static bool equal(StringImpl* r, const LChar* s)
    {   
        return Identifier::equal(r, s); 
    }   

    static void translate(StringImpl*& location, const LChar* c, unsigned hash)
    {   
        size_t length = strlen(reinterpret_cast<const char*>(c));
        location = StringImpl::createFromLiteral(reinterpret_cast<const char*>(c), length).leakRef();
        location->setHash(hash);
        location->setIsAtomic(true);
    }
};

struct IdentifierLCharFromUCharTranslator {
    static unsigned hash(const CharBuffer<UChar>& buf)
    {   
        return StringHasher::computeHashAndMaskTop8Bits(buf.s, buf.length);
    }   
            
    static bool equal(StringImpl* str, const CharBuffer<UChar>& buf)
    {   
        return Identifier::equal(str, buf.s, buf.length);
    }   
               
    static void translate(StringImpl*& location, const CharBuffer<UChar>& buf, unsigned hash)
    {   
        LChar* d;
        StringImpl* r = StringImpl::createUninitialized(buf.length, d).leakRef();
        WTF::copyLCharsFromUCharSource(d, buf.s, buf.length);
        r->setHash(hash);
        r->setIsAtomic(true);
        location = r;  
    }   
};
*/
#endif

PassRef<StringImpl> Identifier::add(VM* vm, const char* c)
{
    ASSERT(c);
    ASSERT(c[0]);
    if (!c[1])
        return *vm->smallStrings.singleCharacterStringRep(c[0]);

#if ENABLE(VMOLAB)
    /*
    HashSet<StringImpl*>::AddResult addResult = vm->atomicStringTable()->table().add<const LChar*, IdentifierASCIIStringTranslator>(reinterpret_cast<const LChar*>(c));

    return addResult.isNewEntry ? adoptRef(*addResult.iterator) : *addResult.iterator;
    */
#if ENABLE(VMOLAB_TIME)
    //TimeCounter::count();
#endif
    return *AtomicString::addIdentifier(*vm, c);
#else
    return *AtomicString::add(c);
#endif
}

PassRef<StringImpl> Identifier::add(ExecState* exec, const char* c)
{
    return add(&exec->vm(), c);
}

PassRef<StringImpl> Identifier::add8(VM* vm, const UChar* s, int length)
{
    if (length == 1) {
        UChar c = s[0];
        ASSERT(c <= 0xff);
        if (canUseSingleCharacterString(c))
            return *vm->smallStrings.singleCharacterStringRep(c);
    }
    if (!length)
        return *StringImpl::empty();

#if ENABLE(VMOLAB)
    /*
    CharBuffer<UChar> buf = { s, static_cast<unsigned>(length) };
    HashSet<StringImpl*>::AddResult addResult = vm->atomicStringTable()->table().add<CharBuffer<UChar>, IdentifierLCharFromUCharTranslator >(buf);

    return addResult.isNewEntry ? adoptRef(*addResult.iterator) : *addResult.iterator;
    */
#if ENABLE(VMOLAB_TIME)
    //TimeCounter::count();
#endif
    return *AtomicString::addIdentifier(*vm, s, length);
#else
    return *AtomicString::add(s, length);
#endif
}

Identifier Identifier::from(ExecState* exec, unsigned value)
{
    return Identifier(exec, exec->vm().numericStrings.add(value));
}

Identifier Identifier::from(ExecState* exec, int value)
{
    return Identifier(exec, exec->vm().numericStrings.add(value));
}

Identifier Identifier::from(ExecState* exec, double value)
{
    return Identifier(exec, exec->vm().numericStrings.add(value));
}

Identifier Identifier::from(VM* vm, unsigned value)
{
    return Identifier(vm, vm->numericStrings.add(value));
}

Identifier Identifier::from(VM* vm, int value)
{
    return Identifier(vm, vm->numericStrings.add(value));
}

Identifier Identifier::from(VM* vm, double value)
{
    return Identifier(vm, vm->numericStrings.add(value));
}

void Identifier::dump(PrintStream& out) const
{
    if (impl())
        out.print(impl());
    else
        out.print("<null identifier>");
}

#ifndef NDEBUG

void Identifier::checkCurrentAtomicStringTable(VM* vm)
{
    // Check the identifier table accessible through the threadspecific matches the
    // vm's identifier table.
    ASSERT_UNUSED(vm, vm->atomicStringTable() == wtfThreadData().atomicStringTable());
}

void Identifier::checkCurrentAtomicStringTable(ExecState* exec)
{
    checkCurrentAtomicStringTable(&exec->vm());
}

#else

// These only exists so that our exports are the same for debug and release builds.
// This would be an RELEASE_ASSERT_NOT_REACHED(), but we're in NDEBUG only code here!
NO_RETURN_DUE_TO_CRASH void Identifier::checkCurrentAtomicStringTable(VM*) { CRASH(); }
NO_RETURN_DUE_TO_CRASH void Identifier::checkCurrentAtomicStringTable(ExecState*) { CRASH(); }

#endif

} // namespace JSC
