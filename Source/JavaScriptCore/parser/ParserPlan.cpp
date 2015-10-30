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

#include "config.h"
#include "ParserPlan.h"

#if ENABLE(VMOLAB)

#include "Parser.h"
#include "SourceCode.h" 
//#include "JSCInlines.h"

namespace JSC { 

Plan::Plan(VM& passedVM, const SourceCode& passedSourceCode, int* elements, Identifier&& ident)
    : vm(passedVM)
    , strictness((JSParserStrictness)elements[5])
    , name(ident)
    , stage(Preparing)
{
    sourceCode = (const_cast<SourceCode*>(&passedSourceCode))->subExpression(elements[0], elements[1], elements[2], elements[3]);
}

Plan::~Plan()
{
}

void Plan::parseInThread(ThreadData* threadData)
{
    this->threadData = threadData;
    
    bool success = parseInThreadImpl();
    
}

bool Plan::parseInThreadImpl()
{
    ParserError error;
    body = parse<FunctionBodyNode>(&vm, sourceCode, 0, name, strictness, JSParseFunctionCode, error, 0, true, true);

    if (!body) {
        printf("Failed Parallel Parsing!\n");
        return false;
    }

    return true;
}

void Plan::notifyParsing()
{
    stage = Parsing;
}

void Plan::notifyParsed()
{
    stage = Parsed;
}

ParsingKey Plan::key()
{
    return ParsingKey(sourceCode.provider(), sourceCode.startOffset());
}

}  // namespace JSC

#endif // ENABLE(VMOLAB)

