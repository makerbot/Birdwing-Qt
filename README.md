Birdwing-Qt
===========

Qt source modifications for the Birdwing Machine

Note
----

The macro definition `QT_LOCALE_USES_FCVT` allows for proper compilation on ARM, but uses a block of code in /src/corelib/tools/qlocale.cpp that is noted to not be thread safe, and may require modification. This code block follows the line:

`#ifdef QT_QLOCALE_USES_FCVT`

