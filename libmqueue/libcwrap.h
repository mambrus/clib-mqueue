/***************************************************************************
 *   Copyright (C) 2013 by Michael Ambrus                                  *
 *   ambrmi09@gmail.com                                             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#if defined(_WIN32) &&  defined(_MSC_VER)
   #define CHAINPATH C:\Program Files\Microsoft Visual Studio\VC98\Include

#elif defined(__BORLANDC__) || defined(__BCPLUSPLUS__)
   #define CHAINPATH C:/BC5/INCLUDE

#elif defined( __C166__ )
   #define CHAINPATH C:\Keil\C166\INC

#elif defined(__GNUC__) /*|| defined(__CYGWIN32__)  || defined(__CYGWIN__)*/
   #ifndef CHAINPATH
      #define CHAINPATH /usr/include
   #endif

#else
   #error "Can\'t determine the target you're building for"
#endif

#define DEFSTR( x ) \
   #x

#define FNAME( path, file ) \
   DEFSTR( path/file )
   
#define BUILDCHAIN( file ) \
   FNAME( CHAINPATH, file ) \

