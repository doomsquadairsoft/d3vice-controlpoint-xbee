# d3vice-controlpoint-xbee
Arduino Fio / XBee based D3VICE control point for Airsoft games

## Feedback

If you have used and enjoy this code base, I'd love to hear from you!

[![Say Thanks!](https://img.shields.io/badge/Say%20Thanks-!-1EAEDB.svg)](https://saythanks.io/to/insanity54)


## Development setup process

  * Download, Install, Open Arduino IDE >=1.8.5 from https://arduino.cc/
  * Set board in `Tools > Board(...) > Arduino Fio`
  * Install contributed libraries `Sketch > Include Libraries > Manage Libraries`
    * Adafruit NeoPixel library
    * XBee-Arduino library
  * Add your user account to tty and dialout groups (May or may not be required for your OS)
    * `sudo usermod -aG tty,dialout <YOUR_USERNAME_HERE>`

## Enclosure Ideas

  * [MTM Survivor Dry Box with O-Ring Seal, Large](https://www.amazon.com/MTM-Survivor-Ring-Orange-Large/dp/B002KENWZY/ref=pd_rhf_ee_s_cp_0_3?_encoding=UTF8&pd_rd_i=B002KENWZY&pd_rd_r=DREY47809SR4JA75TENW&pd_rd_w=tL9qp&pd_rd_wg=YghNF&psc=1&refRID=DREY47809SR4JA75TENW)
  * [Seahorse SE-300 Protective Case Without Foam](https://www.amazon.com/Seahorse-Protective-Case-without-Orange/dp/B001A1TR1Q/ref=sr_1_14?ie=UTF8&qid=1509563510&sr=8-14&keywords=waterproof+case)


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

## Special Thanks

The following people have helped this project in some way, and deserve a mention. Thank you all for helping this project become a reality!

  * [BulldogLowell](https://forum.arduino.cc/index.php?action=profile;u=258955) for helping me with code when I got stuck
  * [gfvalvo](https://forum.arduino.cc/index.php?action=profile;u=557197) for helping me with code when I got stuck
  * [Jiggy-Ninja](https://forum.arduino.cc/index.php?action=profile;u=53253) a.k.a. [MarkRD](https://hackaday.io/MarkRD) for helping me with code when I got stuck

## Todo list

  * [x] Fix phase 3 issue where ttw is ???
  * [ ] Implement XBee
  * [ ] Implement debugging via XBee wireless link
  * [x] Fix button debounce issue
  * [x] Fix endless beep on phase >=1
  * [x] Add button hold logic
  * [x] Implement ButtonManager
