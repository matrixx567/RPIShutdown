RPIShutdown
===========

[![Documentation Status](https://readthedocs.org/projects/rpishutdown/badge/?version=latest)](https://readthedocs.org/projects/rpishutdown/?badge=latest)


RPIShutdown adds a power switch to the Raspberry Pi. So it is possible to completely 
disconnect the Pi from the electrical power source.

![](https://github.com/matrixx567/RPIShutdown/blob/master/docs/source/_static/images/RPIShutdown.JPG)

How it works?
-------------
It works in the same way as the power button in the desktop computer will do.

Usually the Raspberry Pi isn't powered. If the button is pressed, the Raspberry Pi will be started.
The next press will smoothly shutdown the PI by running the `halt` command.  
If you press the button for about 4 seconds the Raspberry Pi will be hardly shutdown by 
disconnecting the power.


Installation
-------------
More information is available in the [installation notes](http://rpishutdown.readthedocs.org/content/installation-notes.html) in the documentation.


Documentation
--------------
More information are available in the [project documentation](http://rpishutdown.readthedocs.org).


other Projects
--------------

[ATXRaspi](http://lowpowerlab.com/atxraspi/): Has nearly the same functionality and you can buy it. 
But it isn't open source.

License
--------------
The MIT License (MIT)

Copyright (c) 2013 Martin Noehrer

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
