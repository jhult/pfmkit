
Pismo File Mount Development Kit build 190
Pismo Technic Inc. Copyright 2006-2017 Joe Lowe
2017.11.16
http://www.pismotechnic.com/


Full documentation for the developer kit is included in the file
index.html .


Overview
--------

The Pismo File Mount Devoper Kit is used by developers in
conjunction with the Pismo File Mount Audit Package or Core package.
Using this kit developers integrate Pismo File Mount technology with
their applications, or implement Pismo File Mount based file
systems.

Features
--------

Release History
---------------

-- Build 190 - 2017.11.13

Added pfmMountFlagForceDriveLetter mount option.

-- Build 185 - 2017.06.23

Added PFM marshaller support for file name conversions between
windows and mac/linux.

-- Build 184 - 2016.01.05

Fixed java API shim, not loading JNI DLL correctly.

-- Build 183 - 2016.06.30

Fixed endName handling in rename and delete marshaller methods.

-- Build 182 - 2016.04.20

Changed C# API shim DLL name to *\pfmshim16_xxx.dll .

Changed java API reverse domain namespace from com.pismotechnic to
com.pismotec .

Mount "report" API added for diagnostic information.

-- Build 181 - 2016.03.29

No significant changes.

-- Build 180 - 2015.12.02

No changes.

-- Build 179 - 2015.08.02

Dev kit now includes direct support of more programming languages
and environments, including:
- C
- C++
- C# and other .Net and Mono hosted languages (VB, F#)
- Java and other JVM hosted languages

The non-concurrent marshaller interfaces are now deprecated.
All samples converted to use the concurrent marshaller,

-- Build 177 - 2015.05.07

No changes.

-- Build 176 - 2015.05.05

Minor API changes. Restored and updated CLR (C#) API shim and
samples.

-- Build 175 - 2015.03.29

No changes.

-- Build 174 - 2015.03.25

Dropped virtual mount point and alerter support on Windows.

-- Build 171 - 2014.07.11

Minor changes.

-- Build 170 - 2013.04.09

Minor changes.

-- Build 169 - 2013.04.09

Minor changes.

-- Build 167 - 2012.12.03

Minor API changes.

-- Build 165 - 2012.05.08

Impoved support for OSX and Linux.

New PfmApi::FileMountCreate API to allow applications
to directly utilize the PFM formatters such as the ISO
reader or Private Folder file system.

Removed PfmReadOnlyFormatterOps and converted
related samples to use PfmFormatterOps.

New mounter samples to demonstrate use of the new
PfmApi::FileMountCreate API.

Converted all samples and documentation to focus on
standalone application development, not PFM Audit Package
formatter development.

Various other documentation updates.

-- Build 163 - 2012.01.09

Impoved support for OSX and Linux.

-- Build 162 - 2011.10.26

Improved support for OSX and Linux.

-- Build 157 - 2010.05.18

First public release with Mac and Linux support.

Added support for asynchronous mount cancellation to marshaller
and formatter interfaces.

-- Build 154 - 2009.10.14

Bumped build number to avoid user confusion caused by leading
zero in previous builds.

Changed inline API DLL loading logic so no longer necessary
to include windows.h.

-- Build 051 - 2009.06.04

Updated documentation to match interface changes.

Added missing hellofs C# application sample.

Added flags to allow formatter to force buffered/unbuffered I/O.

-- Build 050 - 2009.03.05

New concurrent marshaller interface for C++ formatters and file
systems, undocumented but defined in headers.

Protocol enhancements for fastpipe support, to eliminate
extra buffering of file data.

-- Build 045 - 2008.06.20

No significant changes.

-- Build 044 - 2008.05.14

Added support for CLR/.NET development, including samples
and documentation.

-- Build 043 - 2008.05.01

First release of Pismo File Mount Developer Kit. Previously the
kit files were included with Pismo File Mount Audit Package.
