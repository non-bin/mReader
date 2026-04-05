# mReader

Welcome to mReader!  
Just plug me into a computer, drag and drop a text file on to the 'MREADER' drive, and start reading!

 - Alice <3

## Parts

- [Waveshare 1.54inch V2 e-Paper with Module](https://www.waveshare.com/wiki/1.54inch_e-Paper_Module_Manual)
- Any RP2040 based board, I use the [Waveshare RP2040-Zero](https://www.waveshare.com/wiki/RP2040-Zero)
- 4 buttons
- Battery and charger board

Wiring can be inferred from [config.h](./src/config.h)

## Licence

Generally, look at the comment at the top of each file, but here's an overview:

### TinyUSB

[MIT](https://opensource.org/license/MIT)  
Copyright 2019 by Ha Thach <https://tinyusb.org>

    Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

### FatFS

[FatFS-License](https://elm-chan.org/fsw/ff/doc/appnote.html#license) (Basically [BSD-1-Clause](https://opensource.org/license/BSD-1-Clause))  
Copyright 20xx by ChaN <https://elm-chan.org/fsw/ff/>  
Copyright 2026 by Alice Jacka <https://github.com/non-bin/mReader>

    FatFs module is an open source software. Redistribution and use of FatFs in
    source and binary forms, with or without modification, are permitted provided
    that the following condition is met:

    1. Redistributions of source code must retain the above copyright notice,
       this condition and the following disclaimer.

    This software is provided by the copyright holder and contributors "AS IS"
    and any warranties related to this software are DISCLAIMED.
    The copyright owner or contributors be NOT LIABLE for any damages caused
    by use of this software.

### Dev-container Setup

Copyright 2025 by va1da5 <https://github.com/va1da5/pico-vscode-dev-environment>  
Copyright 2026 by Alice Jacka <https://github.com/non-bin/mReader>

Technically unlicensed, but I believe it was shared with the intent of being FOSS. I have opened [an issue](https://github.com/va1da5/pico-vscode-dev-environment/issues/1) requesting a formal license.

### WS2812 RGB LED

[BSD-3-Clause](https://opensource.org/license/BSD-3-Clause)  
Copyright 2020 by Raspberry Pi (Trading) Ltd [https://github.com/raspberrypi/pico-examples](https://github.com/raspberrypi/pico-examples/tree/master/pio/ws2812)  
Copyright 2026 by Alice Jacka <https://github.com/non-bin/mReader>

    Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

    2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

    3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS “AS IS” AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

### ePaper and GUI

[MIT](https://opensource.org/license/MIT)  
Copyright 2019 by the Waveshare team <https://github.com/waveshare/Pico_ePaper_Code>  
Copyright 2022 by hammadrauf <https://github.com/hammadrauf/pico_epd_1in54>  
Copyright 2026 by Alice Jacka <https://github.com/non-bin/mReader>

    Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

### Fonts 8-24

[BSD-3-Clause](https://opensource.org/license/BSD-3-Clause)  
Copyright 2026 by The Monotype Corporation <https://learn.microsoft.com/en-us/typography/font-list/courier-new>  
Copyright 2014 by STMicroelectronics  
Copyright 2019 by the Waveshare team <https://github.com/waveshare/Pico_ePaper_Code>  
Copyright 2022 by hammadrauf <https://github.com/hammadrauf/pico_epd_1in54>  
Copyright 2026 by Alice Jacka <https://github.com/non-bin/mReader>

    Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

    2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

    3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS “AS IS” AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

### Pico SDK

[BSD-3-Clause](https://opensource.org/license/BSD-3-Clause)  
Copyright 2020 by Raspberry Pi (Trading) Ltd. <https://github.com/raspberrypi/pico-sdk/>

    Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
    following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following
      disclaimer.

    2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
      disclaimer in the documentation and/or other materials provided with the distribution.

    3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
    INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
    SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
    SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
    WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
    THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

### Everything Else

[agpl-3-0](https://opensource.org/license/agpl-3-0)  
Copyright 2026 by Alice Jacka <https://github.com/non-bin/mReader>

    mReader is a micro e-reader
    Copyright (C) 2026  Alice Jacka

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>  .
