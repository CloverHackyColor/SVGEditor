# SVGEditor
Cross-platform application to browse and edit SVG with CONIC GRADIENT support.
It is made based on Qt5 so can be compiled in any systems supporting Qt.

##Compilation
In windows. You have to install Qt5.15 into Visual Studio Community.
Open SVGEditor.sln and go.
Or may be install Visual Studio Code and choose qbs plugin.
The application requires standard set of dll libraries for Qt5.15.

In macOS. Install Qt5 by MacPort or by Homebrew.
In the SVGEditor folder type
```bash
qmake
make
```

The created application requires Framework inside that can be taken from any application
already compiled with Qt5. 

##History
The base of the application origin from CloverBootloader project which use NanoSVG project 
https://github.com/memononen/nanosvg. Copyright (c) 2013-14 Mikko Mononen memon@inside.org
but with huge rewriting
Thanks also to:
 * aksdfauytv - support for recursive images
 * porglezomb - handle visibility
 * tpecholt, tesch1 - feature groups
 * boris-ulyanov, rzaumseil - style processing
 * jamislike - basic text parsing
 * darealshinji - multiple improvements
 * technosaurus - independent x-y scaling
 * olivierchatry - added suppport for stylesheets and other small improvements
 * JaimeIvanCervantes - idea for <use href>
 * MalcolmMcLean - forwards differencing to flatten Bezier, binary search color name - improve speed
 * poke1024 - fix handling of <defs> after <g>, add basic support for clip paths
 * SergeySlice for all these improvements and further developement
