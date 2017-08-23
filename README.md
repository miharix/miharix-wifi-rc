# Miharix WiFi RC
[SLO](https://github.com/miharix/miharix-wifi-rc/README.md#slo) / [ENG](https://github.com/miharix/miharix-wifi-rc/README.md#eng)

[![Alt text](https://img.youtube.com/vi/RT00wBG8huE/0.jpg)](https://www.youtube.com/watch?v=RT00wBG8huE)

[![Alt text](https://img.youtube.com/vi/3g94vfjdDN8/0.jpg)](https://www.youtube.com/watch?v=3g94vfjdDN8)

---

[**SLO**](#slo)

Vezje in programski primeri za izdelavo super poceni RV preko WiFi (pametni telefon,tablica,...)

V kolikor vam vezje koristi, lahko podprete razvoj tega ter bodočih projektov s donacijo preko: https://paypal.me/miharix

Kaj se nahaja v mapah:
* **3Dp** - preprosto ohišje za 3Dp
* **Arduino** - *programski primeri za programiranje modula*
* **DIY** - *pdf za izdelavo tiskanega vezja v domači delavnici*
* **Documentation** - *shema vezave in mere*
* **Gerber** - *datoteke za proizvajalca vezij*
* **Render** - *3D izris vezja*

**Za  izdelavo še potrebujete:**
* 1x ESP-12E *(ESP8266)*
* 1x MCP155ST-3302E 3,3V 300mA
* 2x 10K SMD805 (10 kilo ohm)
* 2x 1µF SMD805 (1 mikro Farad)
* opcijsko 3-44 letev moška enoredna lomljiva raster 2,54mm

Za preprosto programiranje predvidena uporaba "Witty cloud"

Testiran domet na zemlji približno 80m
 [(5s preden se avto ustavi na tem posnetku, se je zgubil signal)](https://youtu.be/WoqIe_oOWTM)

Za naredit:
* Testirat na čim več napravah
* Test dometa pri uporabi zunanje antene, ruterja...
* Navodila(video?) kako izdelat, programirat, nastavljat...
* (6 servo demo)Expo
* (6 servo demo)Izklop samodejnega povratka ročice poljubne osi v središče
* (6 servo demo)Sočasno aktivianje gumba za 5&6 med premikanjem ročic
* ...

---

[**ENG**](#eng)

PCB and Example Software for making super cheap RC control over WiFi (mobile phone,tablet,...)

If you like this project, you are welcome to support this and my further projects over https://paypal.me/miharix

What is in the folders:
* **3Dp** - simple 3Dp case
* **Arduino** - *source code examples for programming the module*
* **DIY** - *pdf for making PCB DIY at home*
* **Documentation** - *shematic and dimensions*
* **Gerber** - *files for a PCB factory*
* **Render** - *3D render of PCB*

**For building you also need:**
* 1x ESP-12E *(ESP8266)*
* 1x MCP155ST-3302E 3,3V 300mA
* 2x 10K SMD805 (10 kilo ohm)
* 2x 1µF SMD805 (1 mikro Farad)
* optional 3-44 pin header male one line breakable raster 2,54mm

For easy programming use "Witty cloud"

Ground range test cca 90m
[(5s before car stops, the signal is lost)](https://youtu.be/WoqIe_oOWTM)

Todo List:
* Testing on more devices
* Range test using external antena, router...
* Manual(video?) how to build, programming, configure...
* (6 servo demo)Expo
* (6 servo demo)Disable nipple retun to center for individual axes 
* (6 servo demo)Simultan button(channel 5&6) press and nipple usage
* ...