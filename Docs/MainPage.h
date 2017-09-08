/*! 
 * @mainpage  
 * 
 *  <A NAME="Contents"></A>
 * @section contents_sec Table of Contents
 * <A HREF="#Introduction">Introduction</A><br>
 * <A HREF="#VersionLog">Version Log</A><br>
 * <A HREF="#Acronyms">Acronyms</A><br>
 * <A HREF="#API">API's</A><br>
 * <A HREF="#CodingStandard">Coding Standard</A><br>
 * <A HREF="#Documentation">Documentation</A><br>
 * <A HREF="#Requirements">Requirements</A><br>
 * <A HREF="#Testing">Testing</A><br>
 * <A HREF="#Tools">Tools</A><br>
 *
 * <HR>
 *  
 *  <A NAME="Introduction"></A>
 * @section intro_sec Introduction
 *
 *  This document serves the purpose of providing the background and details of the embedded software that was developed for ........
 *
 *  Provide an introduction to the project, its purpose, and basic overview of what the system does.  
 *
 * @image html blockdiagram.jpg
 *
 * <br><A HREF="#Contents">Table of Contents</A><br>
 *   
 *
 * <hr>
 *
 *
 * <A NAME="VersionLog"></A>
 * 
 * @section Version Version Log
 *
 * @subsection Release1 Release 1 
 *  - added Initial Program Structure
 *  - added Spi Driver
 *  - added Dio Driver
 *  - added Task Scheduler
 *  - added Adc driver
 *  - added Canbus Driver
 *  - added CAN Application Layer
 *  
 *  Final build capable of detecting an event through Adc and storing the event to RAM.  RAM data can be retrieved through the CAN bus.
 *	
 *
 *
 * <br><A HREF="#Contents">Table of Contents</A><br>
 * <hr>
 *
 * <A NAME="Acronyms"></A>
 *
 * @section Acronyms Acronyms and Abbreviations
 * Any acronyms used within this documentation are defined here.<br>
 * 
 * A2D - Analog to Digital Converter <br>
 * ADC - Analog to Digital Converter <br>
 * ATD - Analog To Digital converter <br>
 * CAN - Controller Area Network <br>
 * DIO - Digital Input / Output  <br>
 * I2C - Inter-Integrated Circuit <br>
 * MCU - Micro Controller Unit    <br>
 * MEM - Memory <br>
 * RAM - Random Access Memory <br>
 * ROM - Read Only Memory <br>
 * RTC - Real Time Clock  <br>
 * SPI - Serial Peripheral Interface <br>
 * SYS - System <br>
 * TMR - Timer <br>
 * TSK - Task
 * WDT - Watchdog Timer <br><br>
 *
 * Any abbreviations used within this documentation are defined here. <br>
 *
 * Rq - Request <br>
 * Rs - Response <br>
 * Sm - State Machine <br>
 * St - State <br>
 *
 * <br><A HREF="#Contents">Table of Contents</A><br>
 * <hr>
 *
 *
 * <A NAME="API"></A>
 *  
 * @section API API
 * The Application Programming Interface (API) describes the functions that are available to the application for accessing the peripherals and library routines.
 * 
 * "Update this section with an API or links to the API's used in the software.  "
 *
 * @subsection Controller Area Network (CAN)
 * Some useful functions for controlling CAN
 * @see Can_Transmit - Transmit data over CANbus. <br>
 * @see Can_Receive - Receive data from CANbus. <br>
 *
 * <br><A HREF="#Contents">Table of Contents</A><br> 
 * <hr>
 *
 *
 * <A NAME="CodingStandard"></A>
 * 
 * @section Standard Coding Standard Overview
 * The XXXXX Coding Standard was used to develop the embedded firmware for this project.  While the standard 
 * itself is extensive, a general overview is presented here so that a basic understanding of how the code
 * is organized and developed can be understood.  <br><br>
 *
 * "Update this section with a description of the Coding Standard used, where it is located and some highlights"
 * "Some basic examples are below"
 *
 * @subsection CNC Code Naming Convention
 * Function names should conform to the following standard, <br>
 * - Function names should start with a three letter sub-system name followed by an 
 *   under-score. <br>
 * - The first character of the sub-system name shall be capitalized.    <br>
 * - The rest of the function name should not have any under-scores.     <br>
 * - The rest of the function name should describe what the function does.<br>
 * - The first character of each word should be capitalized. <br>
 * - Global variables and variables that are static to a module should have their 
 *   names conform to the same standard as function names. <br>
 * - Sub-system names aren't required, but use them if you can.<br><br>
 * 
 * For example,
 * - Spi_Init(&Spi_Config), where "Spi_" is the sub-system and "Init" describes the 
 * action to be performed.<br><br>
 * 
 * Common first words after the sub-system name,
 * - Init
 * - Read
 * - Set
 * - Write
 * 
 * For function prototype parameters,
 * - The sub-system shall not be used.
 * - The first character of each word shall be capitalized.
 * - There should be no under-scores in the name.
 * 
 * For local variables (to a function),
 * - The sub-system shall not be used.
 * - The first character of the first word shall not be capitalized.
 * - If the name contains more than one word, each word after the first shall
 *   have its first character capitalized.
 * 
 * For example,<br><br>
 * 
 * @code
 * uint16 Spi_Read(uint8 *Charptr) 
 * {
 *   uint8 i;
 *   uint8 myVariable;
 *   
 *   // Some code
 *   statements;
 * }
 * @endcode<br>
 * 
 * - The parameter "Charptr" has the first character capitalized and does not start
 *   with a sub-system name because it is a parameter.
 * - The variable "i" is not capitalized and does not start with a sub-system name 
 *   because it is a local variable.
 * - The variable "myVariable" has it's first word un-capitalized, it's second word
 *   capitalized, and does not start with a sub-system name because it is a local 
 *   variable.
 * 
 * @subsection SSN Sub-System Names
 * Here is a list of sub-system names. More can be added as needed. <br>
 * 
 * Adc   - Analog to Digital Conversion <br>
 * Dio   - Digital Input/Output <br>
 * Eep   - EEPROM memory <br>
 * Flash - Flash memory <br>
 * Icu   - Input Capture Unit <br>
 * Mcu   - Micro-Controller Unit <br>
 * Pwm   - Pulse Width Modulation <br>
 * Rtc   - Real-Time Counter <br>
 * Spi   - Serial Peripheral Interface <br>
 * Sys   - System
 * Swu   - Software UART <br>
 * Tmr   - Timer
 * Test  - Testing functionality (anything using this sub-system name should be
 *         conditionally compiled so that it does not end up in production code). <br>
 * Wdt   - Watchdog Timer <br>
 * 
 * @subsection CF Code Formatting/Style
 * Code formatting and style shall conform to the following guidelines.
 * 
 * - Indentation shall be 2 spaces (not tabs).
 * - Opening braces shall be placed on separate lines.
 * - Closing braces shall be placed on separate lines.
 * - If, for, switch, and while statements shall always use braces.
 * - A comment block shall be placed before every function definition describing
 *   what the function does, any paramaters passed to it, any return value, and
 *   anything else that would be relevant or useful to someone that has to 
 *   maintain it.
 * 
 * For example,		<br>
 * @code 
 * // This is correct. 
 * if (condition) 
 * { 
 *   statement; 
 * } 
 * else 
 * { 
 *   statement2; 
 * } 
 * 
 * // This is NOT.
 * if (condition) { 
 *   statement;     
 * } else {	    
 *   statement2;    	
 * }		    	
 * 
 * // Neither is this.
 * if (condition) 
 *   statement; 
 * else		
 *   statement2; 
 * @endcode		
 * 
 * <br><A HREF="#Contents">Table of Contents</A><br>
 * <hr>
 *
 *
 * <A NAME="Documentation"></A>
 * 
 * @section Docs Documentation
 * 
 * 
 * How To Add An Image To This Documentation:
 * You can include images in the documentation by writing "(\image html filename)".  
 * If you put it alone on a line it will be embedded in place.
 * Doxygen supports gif, jpg, jpeg, png, and bmp files.
 * Place the images in the "Doc/Images" directory.
 * 
 * @subsection Gimp Gimp
 * Application "Gimp" can convert a PDF file into a jpeg, png, or bmp.
 * http://www.gimp.org
 * 
 * - Start Gimp. 
 * - Open your PDF file. 
 * - Click "Import" to import the PDF file.
 * - From the Menu click "Tools -> Selection Tools -> Rectangle Select".
 * - Make a box around the area that you want to create an image of using the 
 *   rectangle select tool. 
 * - From the Menu, click "Image -> Fit Canvas To Selection".
 * - Again from the Menu, click "Image -> Crop to Selection".
 * - Finally, from the Menu click "Image -> Autocrop Image".
 * - If this is the image that you want we just need to save it.
 * - From the Menu, click "File -> Save As".
 * - Click on the plus sign next to "Select File Type (By Extension)".
 * - Go down the list and click on "PNG Image".
 * - Select the directory (Doc/Images) to save the file to.
 * - Edit the filename if you want.
 * - Click "Save" to save the image as a png.
 * 
 * To combine images that are on 2 different pdf pages:
 * - Import each page (same as above).
 * - Use the rectangle tool to remove stuff from the first page.
 * - Use Image -> Canvas Size to change just the height.
 *   Make it big enough (double) so that you will be able to paste page 2 onto it.
 * - Click on Image -> Flatten Image to change the background of the new part on 
 *   the bottom to the same as the original page (white from some kind of 
 *   checkered pattern).
 * - Use the rectangle tool to select and copy (CTRL-C) the image from the second page.
 * - Move the rectangle tool cursor back to the first page and paste it. 
 * 
 * When adding pictures from a camara, the resolution is usaully too big.
 * I like to scale it down to 800x600.
 * - Click Image -> Scale Image...
 * - Enter 800 for the Width and then hit the Tab key. The Height should
 *   scale down automatically (600 for my pictures).
 * - Click Scale
 * - Then Save the image.  
 * 
 * @subsection PDFS PDF Split And Merge:
 * Application "PDF Split and Merge" can split a PDF document into single pages.
 * http://www.pdfsam.org
 * 
 * @subsection PDFE PDF Escape:
 * PDF Escape can be used to edit PDF files. It is web based (nothing to download). 
 * You upload a file and can then edit it. I just use it for copying text from 
 * datasheets and then pasting it into my documentation.
 * http://www.pdfescape.com
 * 
 * @subsection Edraw Edraw
 * Edraw is used to generate the architecture and design documents.
 * 
 * Edraw can be used to draw many 
 * different kinds of diagrams. It currently has special objects to help draw 
 * entity relationship diagrams, UML diagrams, flowcharts, network diagrams, and 
 * many other diagrams. 
 * 
 *
 * @subsection Doxygen Doxygen
 * Doxygen is used to generate all the HTML related documentation directly from the source.  Information on how  to install it and how to properly document your
 * code can be found at http://www.stack.nl/~dimitri/doxygen/index.html
 *
 * <br><A HREF="#Contents">Table of Contents</A><br> 
 * <hr>
 * 
 * <A NAME="Requirements"></A>
 * 
 * @section Requirements Requirements
 * Refer to the Software Requirements Specification for a detailed view of the requirements for the embedded software.  Refer to the quick spec 2 for details on the
 * system requirements.
 * 
 * <br><A HREF="#Contents">Table of Contents</A><br>
 * <hr>
 *
 * <A NAME="Testing"></A>
 * 
 * @section Validation System Testing and Validation
 * This section is where testing and validation is demonstrated and explained.  How test cases were developed.  How test cases were determined.  Estimated number of
 * bugs per 1k of code.  Bug tracking.  Things to do.
 * 
 * @subsection Bugs Bug Tracking
 * Currently any bugs which are found in the software are recorded and logged on the Trac website.  Reports can be generated on tasks by
 * milestone, user, version etc.  Bugs which need to be added to the list can be added by visiting the Trac site at the following link:  <br><br>
 *
 * update link
 *
 * @subsection ToDo Todo
 * Currently any tasks which resulted from software features that need to be added are stored on the Trac website.  Reports can be generated on tasks by
 * milestone, user, version etc.  Features which need to be added to the list can be added by visiting the Trac site at the following link:  <br><br>
 *
 * update link
 *
 * <br><A HREF="#Contents">Table of Contents</A><br> 
 * <hr>
 *
 *
 * <A NAME="Tools"></A>
 * 
 * @section DevEnviron Tools
 * 
 * This section explains a number of software development tools which were instrumental in the development of this firmware.  These tools include a source code
 * version repository, compiler and IDE, code metrics tools and static analysis tools.  Details on each tool can be found below:
 * 
 * @subsection subversion Subversion
 * 
 * The XXX embedded firmware is stored on the XXX SVN server located at UPDATE LINK.  The primary client which was used during development was the 
 * Tortoise SVN client which is an open source SVN client.  
 * 
 * @subsection svn1 Step 1: Install Tortoise SVN
 * Tortoise can be downloaded from http://tortoisesvn.net/downloads.html.  Once downloaded, intstall by double clicking the executable and follow the on-screen instructions.  
 * It will be necessary to restart the computer in order to get the sub menu's available by right clicking in windows explorer.
 * 
 * @subsection svn2 Step 2: Checkout Source
 * In windows explorer, right click in the C:/ path and select SVN Checkout as shown in the image below.
 * @image html svn.checkouta.jpg
 * 
 * Enter the following repository information as shown in the image below:
 *
 * @image html svn.checkoutb.jpg
 *
 * The source can now be accessed from the c:/bears/ directory.
 * 
 * @subsection CodeWarrior Code Warrior
 * 
 * @image html CodeWarrior.jpg
 * 
 * @subsection Compiler Compiler
 * 
 * The compiler that was used was the default compiler included in the S12X version of Code Warrior.  Production compilation was done with the linker for S12.
 * 
 * @subsection RSM RSM Code metrics
 * 
 * Latest Code Metric Report will go here.
 * 
 * @subsection Lint Lint
 * 
 * Lint is a static analysis tool which was used to analyze the code and check for suspicious and non-portable constructs that were likely to cause bugs.  In this case, PCLint
 * from Gimpel.  Static analysis performs additional checks that the compiler does not perform.  A few examples of these are
 * - initialization checking
 * - value tracking
 * - strong type checking
 * - macro analysis
 * 
 * Lint integrates into Code Warrior as a separate linker option.  It also includes additional checks that can be enabled such as MISRA standard checks.  Lint was used primarily
 * to check for inconsistancies which could cause bugs and duplications that could lead to misunderstanding the code.  A complete list of checks that lint performs can be found
 * at <br>
 *
 * http://gimpel-online.com/MsgRef.html
 * 
 * <br><A HREF="#Contents">Table of Contents</A><br>
 * <hr>
 * 
 */
