========================================================================
    ACTIVE TEMPLATE LIBRARY : MTDatePlugin Project Overview
========================================================================

AppWizard has created this MTDatePlugin project for you to use as the starting point for
writing your Dynamic Link Library (DLL).
This project is implemented with Visual C++ attributes.

This file contains a summary of what you will find in each of the files that
make up your project.

MTDatePlugin.vcproj
    This is the main project file for VC++ projects generated using an Application Wizard. 
    It contains information about the version of Visual C++ that generated the file, and 
    information about the platforms, configurations, and project features selected with the
    Application Wizard.

_MTDatePlugin.idl
    This file will be generated by the compiler when the project is built. It will contain the IDL 
    definitions of the type library, the interfaces and co-classes defined in your project.
    This file will be processed by the MIDL compiler to generate:
        C++ interface definitions and GUID declarations (_MTDatePlugin.h)
        GUID definitions                                (_MTDatePlugin_i.c)
        A type library                                  (_MTDatePlugin.tlb)
        Marshaling code                                 (_MTDatePlugin_p.c and dlldata.c)
MTDatePlugin.cpp
    This file contains the object map and the implementation of your DLL's exports.
MTDatePlugin.rc
    This is a listing of all of the Microsoft Windows resources that the
    program uses.

MTDatePlugin.def
    This module-definition file provides the linker with information about the exports
    required by your DLL. It contains exports for:
        DllGetClassObject  
        DllCanUnloadNow    
        GetProxyDllInfo    
        DllRegisterServer	
        DllUnregisterServer

/////////////////////////////////////////////////////////////////////////////
Other standard files:

StdAfx.h, StdAfx.cpp
    These files are used to build a precompiled header (PCH) file
    named MTDatePlugin.pch and a precompiled types file named StdAfx.obj.

Resource.h
    This is the standard header file that defines resource IDs.

/////////////////////////////////////////////////////////////////////////////
Proxy/stub DLL project and module definition file:

MTDatePluginps.vcproj
    This file is the project file for building a proxy/stub DLL if necessary.
	The IDL file in the main project must contain at least one interface and you must 
	first compile the IDL file before building the proxy/stub DLL.	This process generates
	dlldata.c, MTDatePlugin_i.c and MTDatePlugin_p.c which are required
	to build the proxy/stub DLL.

MTDatePluginps.def
    This module definition file provides the linker with information about the exports
    required by the proxy/stub.
/////////////////////////////////////////////////////////////////////////////
