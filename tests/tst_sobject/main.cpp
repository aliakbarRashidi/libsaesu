/*
 * Copyright (C) 2011 Robin Burchell <viroteck@viroteck.net>
 *
 * This software, and all associated material(s), including but not limited
 * to documentation are protected by copyright. All rights reserved.
 * Copying, including reproducing, storing, adapting, translating, or
 * reverse-engineering any or all of this material requires prior written
 * consent. This material also contains confidential information which
 * may not be disclosed in any form without prior written consent.
 */

// Qt
#include <QTest>
#include <QSignalSpy>

// Us
#include "main.h"

void tst_SObject::construct() const
{
    SObject obj;
}

QTEST_MAIN(tst_SObject)
