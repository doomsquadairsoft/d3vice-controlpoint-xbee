# d3vice-controlpoint-xbee
Arduino Fio / XBee based D3VICE control point for Airsoft games

## Feedback

If you have used and enjoy this code base, I'd love to hear from you!

[![Say Thanks!](https://img.shields.io/badge/Say%20Thanks-!-1EAEDB.svg)](https://saythanks.io/to/insanity54)


## Development setup process

  * Download, Install [Atom editor](https://atom.io)
  * Download, Install [Platformio](https://platformio.org/install/ide?install=atom) Atom package
  * Restart atom. Open Atom & PlatformIO then import this repo.

??? @TODO once I figure out the process after this
  * Install contributed libraries `Sketch > Include Libraries > Manage Libraries`
    * Adafruit NeoPixel library
    * XBee-Arduino library
  * Add your user account to tty and dialout groups (May or may not be required for your OS)
    * `sudo usermod -aG tty,dialout <YOUR_USERNAME_HERE>`

## Enclosure Ideas

  * [MTM Survivor Dry Box with O-Ring Seal, Large](https://www.amazon.com/MTM-Survivor-Ring-Orange-Large/dp/B002KENWZY/ref=pd_rhf_ee_s_cp_0_3?_encoding=UTF8&pd_rd_i=B002KENWZY&pd_rd_r=DREY47809SR4JA75TENW&pd_rd_w=tL9qp&pd_rd_wg=YghNF&psc=1&refRID=DREY47809SR4JA75TENW)
  * [Seahorse SE-300 Protective Case Without Foam](https://www.amazon.com/Seahorse-Protective-Case-without-Orange/dp/B001A1TR1Q/ref=sr_1_14?ie=UTF8&qid=1509563510&sr=8-14&keywords=waterproof+case)


## XBee

### Setup

To connect to the XBee, a [SparkFun FTDI Basic Breakout 3.3v](https://www.sparkfun.com/products/9873) is used along with an [Adafruit XBee Adapter kit v1.1](https://www.adafruit.com/product/126). As my Adafruit XBee adapter kit v1.1 is discontinued, the replacment seems to be Adafruit's [USB XBee Adapter](https://www.adafruit.com/product/247) which I haven't tested but should do the trick. A separate FTDI chip isn't necessary with that new model.

XCTU is the program for configuring XBee modules. It is cross-platform.

57600 baudrate

Set the controlpoint as an ZB Router API NOT a ZB Endpoint API! Endpoint will work, but has complications with sleep mode.

#### XCTU Troubleshootinghttps://www.youtube.com/watch?v=ChCHfJoo9CQ

Make sure if using it on linux, you add yourself to the `dialout` group. Example: `sudo usermod -a -G dialout chris`

Make sure Arduino IDE is closed when working with XCTU. Radio discovery can be finnicky, but it works when it works.

XCTU's download links on Digi's website are usually broken. Find the filename from the [download page](https://www.digi.com/products/xbee-rf-solutions/xctu-software/xctu#productsupport-drivers), then manually browse their [ftp server](ftp://ftp1.digi.com/support/utilities/) for the correct version of XCTU.

If you can't get XCTU to communicate with your XBee, the rule of thumb is keep trying. It doesn't always work the first or second time. Try the different connection methods as well, from discovery to manually entering XBee serial/USB configuration.


### Power

The XBee is very sensitive when it comes to power. It needs as close to 3.3V as possible. When using USB, the Fio gives it a clean 3.285V. 3.296 was acheived using the buck converter regulator and that seemed to do the trick. If you are attempting to remotely program the XBee using X-CTU, and the read process seems slow, it may be that the XBee is crashing when it tries to pull power. Check voltages and try again.

### IDs

#### PAN ID

0x73706F6B616E6561 (ASCII spokanea)

#### switzerland-robust

SH 0013A200
SL 40B51A26

#### borders-portals

SH 0013A200
SL 40B774EC

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
  * [ ] Try LoRa radio [Moteino](https://lowpowerlab.com/shop/product/119) looks nice.

## Parts list

  * (2) 220 Ohm resistor (JP3 button LEDs) DEPRECATED -- The buttons have their own capacitor
  * (3) 10K Ohm resistor (x2 JP4 button switches, x1 Battery Voltage Divider)
  * (1) 100K Ohm resistor (Battery voltage divider)
  * (1) 470 Ohm resistor (R1 Level shifter)
  * (1) 100 Ohm resistor (R2 buzzer)
  * (1) [Arduino Fio](https://amzn.to/2vkoAoj)
  * (1) 900MHz series XBee S2B
  * (1) [Digi JF1R6-CR3-4I U.FL fem to RPSMA fem cable](https://www.mouser.com/productdetail/digi-international/jf1r6-cr3-4i?qs=YPg7lQ8MWSd%252bJ21AYb5tjA%3D%3D)
  * (1) [2.4GHz dipole antenna](https://www.mouser.com/ProductDetail/Laird-LS-Research/001-0001?qs=%2fha2pyFaduic7%252bgB%2fLd2y%2fydYaVrig4l0IiQcO85%252biN2Plogftrjhg%3d%3d) (@TODO this doesn't seem right)
  * (1) 5V Buzzer
  * (1) 1000 uF electrolytic capacitor
  * (1) [TI 74AHCT125](http://adafru.it/1787) (NeoPixel 3V to 5V level shifter)
  * (1) [Red Arcade pushbutton](https://amzn.to/2qzc0vI)
  * (1) [Blue Arcade pushbutton](https://amzn.to/2vszEjp)
  * (2) [18650 Cell holder](https://amzn.to/2JN0Lsh)
  * (2) 18650 Li-Ion 3.7V battery
  * (1) [Qunqi MP1584EN DC-DC Buck Converter](https://amzn.to/2EPh9oB)
    * (1) [Alternative Power supply 001](https://www.ebay.com/itm/Durable-Quality-DC-DC-12V-To-3-3V-5V-Buck-Step-down-Power-Supply-Module-RH/113014911452?hash=item1a5036b1dc:g:4ecAAOSwzWpZjrM5)
  * (1) [MTM Survivor Dry Box with O-Ring](https://amzn.to/2JQuaSx)
  * (1) [Medium keyswitch](https://www.sparkfun.com/products/retired/11473)
  * (n) [24 AWG copper wire](https://amzn.to/2FrzB6S)
  * (2) [Chasis handles](https://www.mouser.com/ProductDetail/RAF-Electronic-Hardware/8075-440-A-24?qs=%2fha2pyFadugv9UkFwjFj1mse%2fGe0gQVLL5igE44%2fsfXi69vVZQTh6A%3d%3d)
  * (?) [4-40 bolts](https://www.banggood.com/4-40-UNC-Stainless-Steel-Hex-Socket-Head-Cap-Screws-Nuts-Assortment-Kit-126pcs-p-1147256.html?rmmds=search&cur_warehouse=CN)
  * (?) [LED Strip Housing](https://www.ebay.com/itm/50cm-U-V-YW-Aluminium-Housing-Holder-For-LED-Strip-Light-Bar-Under-Cabinet-Lamp/372358319157?hash=item56b2490c35:m:mi8XunysgHwlc1OMXDJ0slw:rk:29:pf:0)
  * (2) [Arcade Button Quick-Connect Wire Pairs - 0.11"](https://www.adafruit.com/product/1152)
