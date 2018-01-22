# d3vice-controlpoint-xbee
Arduino Fio / XBee based D3VICE control point for Airsoft games

## Development setup process

  * Download, Install, Open Arduino IDE >=1.8.5 from https://arduino.cc/
  * Set board in `Tools > Board(...) > Arduino Fio`
  * Install contributed libraries `Sketch > Include Libraries > Manage Libraries`
    * Adafruit NeoPixel library
    * XBee-Arduino library
  * Add your user account to tty and dialout groups (May or may not be required for your OS)
    * `sudo usermod -aG tty dialout <YOUR_USERNAME_HERE>`



## Enclosure Ideas

https://www.amazon.com/MTM-Survivor-Ring-Orange-Large/dp/B002KENWZY/ref=pd_rhf_ee_s_cp_0_3?_encoding=UTF8&pd_rd_i=B002KENWZY&pd_rd_r=DREY47809SR4JA75TENW&pd_rd_w=tL9qp&pd_rd_wg=YghNF&psc=1&refRID=DREY47809SR4JA75TENW
https://www.amazon.com/Seahorse-Protective-Case-without-Orange/dp/B001A1TR1Q/ref=sr_1_14?ie=UTF8&qid=1509563510&sr=8-14&keywords=waterproof+case


## XBee

### Power

The XBee is very sensitive when it comes to power. It needs as close to 3.3V as possible. When using USB, the Fio gives it a clean 3.285V. 3.296 was acheived using the buck converter regulator and that seemed to do the trick. If you are attempting to remotely program the XBee using X-CTU, and the read process seems slow, it may be that the XBee is crashing when it tries to pull power. Check voltages and try again.

### IDs

#### PAN ID

0x73706F6B616E6561 (ASCII spokanea)

#### switzerland-robust

SH 13A200
SL 40B51A26

#### borders-portals

SH 13A200
SL 408774EC

